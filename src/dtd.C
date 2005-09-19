// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/dtd.h>
#include <rudiments/charstring.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class dtdprivate {
	friend class dtd;
	private:
		xmldom	_xmld;
		xmldom	_xmldtd;

		stringbuffer	_err;
};

dtd::dtd() {
	pvt=new dtdprivate;
}

dtd::~dtd() {
	delete pvt;
}

bool dtd::parseFile(const char *filename) {
	if (!pvt->_xmld.parseFile(filename)) {
		pvt->_err.clear();
		pvt->_err.append(pvt->_xmld.getError());
		return false;
	}
	if (!parseDtd()) {
		return false;
	}
	return true;
}

bool dtd::parseString(const char *string) {
	if (!pvt->_xmld.parseString(string)) {
		pvt->_err.clear();
		pvt->_err.append(pvt->_xmld.getError());
		return false;
	}
	if (!parseDtd()) {
		return false;
	}
	return true;
}

bool dtd::parseDtd() {

	// create the new tree
	pvt->_xmldtd.createRootNode();
	pvt->_xmldtd.getRootNode()->cascadeOnDelete();
	pvt->_xmldtd.getRootNode()->appendChild(
		new xmldomnode(pvt->_xmldtd.getRootNode()->getNullNode(),
						TAG_XMLDOMNODETYPE,"dtd",NULL));

	// step through tags
	xmldomnode	*root=pvt->_xmld.getRootNode();
	xmldomnode	*currentnode=root->getFirstTagChild();
	while (!currentnode->isNullNode()) {

		// for each element, create a new element node,
		// for each attribute, create a new attribute node
		if ((!charstring::compare(currentnode->getName(),
							"!ELEMENT") &&
					!newElement(currentnode)) ||
			(!charstring::compare(currentnode->getName(),
							"!ATTLIST") &&
					!newAttribute(currentnode))) {
				return false;
		}

		// move on to the next tag
		currentnode=currentnode->getNextTagSibling();
	}

	// success
	return true;
}

bool dtd::newElement(xmldomnode *node) {

	// sanity check
	if (node->getAttributeCount()<2) {
		nodeError(node);
		return false;
	}

	// append a carriage return just for looks
	xmldomnode	*dtdnode=pvt->_xmldtd.getRootNode()->getChild("dtd");
	dtdnode->appendText("\n");

	// create an "element" element and add it to the tree
	xmldomnode	*element=new xmldomnode(dtdnode->getNullNode(),
					TAG_XMLDOMNODETYPE,"element",NULL);
	element->cascadeOnDelete();
	if (!dtdnode->appendChild(element)) {
		nodeError(node);
		delete element;
		return false;
	}

	// add a name attribute to the element
	const char	*name=node->getAttribute(0)->getName();
	if (!element->appendAttribute("name",name)) {
		nodeError(node);
		return false;
	}

	// add the list of valid child elements to the tree
	const char	*list=node->getAttribute(1)->getName();
	if (parseList(list,element,1,1,',',"child")) {
		return true;
	}
	nodeError(node);
	return false;
}

bool dtd::parseList(const char *list, xmldomnode *node,
					int checkcount, int indent,
					char delimiter,
					const char *name) {

	// return failure for a NULL list and success for "EMPTY"
	if (!list) {
		return false;
	} else if (!charstring::compare(list,"EMPTY")) {
		return true;
	} else {

		// parse the list, it should be "delimiter" seperated values
		const char	*ptr1=list;
		const char	*ptr2;
		int	length;
		char	*value;
		char	count[2];
		count[1]='\0';
		for (;;) {
	
			// look for the specified delimiter
			ptr2=charstring::findFirst(ptr1,delimiter);

			// if we don't find the delimiter,
			// use the end of the string instead 
			if (!ptr2) {
				ptr2=list+charstring::length(list);
			}
	
			// parse out the value, including the * or +
			length=ptr2-ptr1;
			value=new char[length+1];
			value[length]='\0';
			charstring::copy(value,ptr1,length);

			// if specified, evaluate any trailing *'s or +'s and
			// truncate them
			if (checkcount) {
				count[0]=value[charstring::length(value)-1];
				if (count[0]!='*' && count[0]!='+') {
					count[0]='1';
				} else {
					value[charstring::length(value)-1]='\0';
				}
			}


			// append a carriage return just for looks
			node->appendText("\n");

			// indent for looks as well
			for (int i=0; i<indent; i++) {
				node->appendText("	");
			}

			// create a new element and add it to the tree
			xmldomnode	*newtag=new xmldomnode(
				pvt->_xmldtd.getRootNode()->getNullNode(),
					TAG_XMLDOMNODETYPE,name,NULL);
			newtag->cascadeOnDelete();
			if (!node->appendChild(newtag)) {
				delete newtag;
				delete[] value;
				return false;
			}

			// give the new element a name attribute and a count
			// attribute if that was specified
			if (!newtag->appendAttribute("name",value) ||
				(checkcount &&
				!newtag->appendAttribute("count",count))) {
				return false;
			}

			// clean up
			delete[] value;
	
			// break if we're at the end of the string
			if (!*ptr2) {
				break;
			}
	
			// move on to the next value
			ptr1=ptr2+1;
		}

		// return success
		return true;
	}
}

bool dtd::newAttribute(xmldomnode *node) {

	// sanity check
	if (node->getAttributeCount()<4) {
		nodeError(node);
		return false;
	}

	// get the appropriate element to add this attribute to
	xmldomnode	*element=pvt->_xmldtd.getRootNode()->getChild("dtd")->
						getChild("element","name",
							node->getAttribute(0)->
								getName());
	if (element->isNullNode()) {
		nodeError(node);
		return false;
	}

	// create a new "attribute" element and add it to the tree
	element->appendText("\n	");
	xmldomnode	*attribute=new xmldomnode(
					pvt->_xmldtd.getRootNode()->
							getNullNode(),
					TAG_XMLDOMNODETYPE,"attribute",NULL);
	attribute->cascadeOnDelete();
	if (!element->appendChild(attribute)) {
		nodeError(node);
		return false;
	}

	// give the new element name and default value attributes
	if (!attribute->appendAttribute("name",
				node->getAttribute(1)->getName()) ||
		!attribute->appendAttribute("default",
				node->getAttribute(3)->getName())) {
		nodeError(node);
		return false;
	}

	// insert the list of valid values or none if CDATA
	const char	*values=node->getAttribute(2)->getName();
	if (!charstring::compare(values,"CDATA")) {
		return true;
	}
	if (parseList(values,attribute,0,2,'|',"value")) {
		return true;
	}
	nodeError(node);
	return false;
}

xmldomnode *dtd::xml() {
	return pvt->_xmldtd.getRootNode();
}

const char *dtd::getError() {
	return pvt->_err.getString();
}

void dtd::nodeError(xmldomnode *node) {
	stringbuffer	*xml=node->xml();
	pvt->_err.append("error processing:\n");
	pvt->_err.append(xml->getString());
	delete xml;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
