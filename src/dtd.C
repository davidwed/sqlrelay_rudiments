// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/dtd.h>

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

bool dtd::parseFile(const char *filename) {
	if (!xmld.parseFile(filename)) {
		clearError();
		appendError(xmld.getError());
		return false;
	}
	if (!parseDtd()) {
		clearError();
		appendError(xmldtd.getError());
		return false;
	}
	return true;
}

bool dtd::parseString(const char *string) {
	if (!xmld.parseString(string)) {
		clearError();
		appendError(xmld.getError());
		return false;
	}
	if (!parseDtd()) {
		clearError();
		appendError(xmldtd.getError());
		return false;
	}
	return true;
}

bool dtd::parseDtd() {

	// create the new tree
	xmldtd.createRootNode();
	xmldtd.getRootNode()->cascadeOnDelete();
	xmldtd.getRootNode()->appendChild(
			new xmldomnode(xmldtd.getRootNode()->getNullNode(),
						TAG_XMLDOMNODETYPE,"dtd",NULL));

	// step through tags
	xmldomnode	*root=xmld.getRootNode();
	xmldomnode	*currentnode=root->getFirstTagChild();
	while (!currentnode->isNullNode()) {

		// for each element, create a new element node,
		// for each attribute, create a new attribute node
		if ((!strcmp(currentnode->getName(),"!ELEMENT") &&
					!newElement(currentnode)) ||
			(!strcmp(currentnode->getName(),"!ATTLIST") &&
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
		return false;
	}

	// append a carriage return just for looks
	xmldomnode	*dtdnode=xmldtd.getRootNode()->getChild("dtd");
	dtdnode->appendText("\n");

	// create an "element" element and add it to the tree
	xmldomnode	*element=new xmldomnode(dtdnode->getNullNode(),
					TAG_XMLDOMNODETYPE,"element",NULL);
	element->cascadeOnDelete();
	if (!dtdnode->appendChild(element)) {
		delete element;
		return false;
	}

	// add a name attribute to the element
	char	*name=node->getAttribute(0)->getName();
	if (!element->appendAttribute("name",name)) {
		return false;
	}

	// add the list of valid child elements to the tree
	char	*list=node->getAttribute(1)->getName();
	return parseList(list,element,1,1,',',"child");
}

bool dtd::parseList(const char *list, xmldomnode *node,
					int checkcount, int indent,
					char delimiter,
					const char *name) {

	// return failure for a NULL list and success for "EMPTY"
	if (!list) {
		return false;
	} else if (!strcmp(list,"EMPTY")) {
		return true;
	} else {

		// parse the list, it should be "delimiter" seperated values
		char	*ptr1=(char *)list;
		char	*ptr2;
		int	length;
		char	*value;
		char	count[2];
		count[1]=(char)NULL;
		for (;;) {
	
			// look for the specified delimiter
			ptr2=strchr(ptr1,delimiter);

			// if we don't find the delimiter,
			// use the end of the string instead 
			if (!ptr2) {
				ptr2=(char *)(list+strlen(list));
			}
	
			// parse out the value, including the * or +
			length=ptr2-ptr1;
			value=new char[length+1];
			value[length]=(char)NULL;
			strncpy(value,ptr1,length);

			// if specified, evaluate any trailing *'s or +'s and
			// truncate them
			if (checkcount) {
				count[0]=value[strlen(value)-1];
				if (count[0]!='*' && count[0]!='+') {
					count[0]='1';
				} else {
					value[strlen(value)-1]=(char)NULL;
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
					xmldtd.getRootNode()->getNullNode(),
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
		return false;
	}

	// get the appropriate element to add this attribute to
	xmldomnode	*element=xmldtd.getRootNode()->getChild("dtd")->
		getChild("element","name",node->getAttribute(0)->getName());
	if (element->isNullNode()) {
		return false;
	}

	// create a new "attribute" element and add it to the tree
	element->appendText("\n	");
	xmldomnode	*attribute=new xmldomnode(
					xmldtd.getRootNode()->getNullNode(),
					TAG_XMLDOMNODETYPE,"attribute",NULL);
	attribute->cascadeOnDelete();
	if (!element->appendChild(attribute)) {
		return false;
	}

	// give the new element name and default value attributes
	if (!attribute->appendAttribute("name",
				node->getAttribute(1)->getName()) ||
		!attribute->appendAttribute("default",
				node->getAttribute(3)->getName())) {
		return false;
	}

	// insert the list of valid values or none if CDATA
	char	*values=node->getAttribute(2)->getName();
	return (!strcmp(values,"CDATA"))?
			true:parseList(values,attribute,0,2,'|',"value");
}

xmldom *dtd::xml() {
	return &xmldtd;
}
