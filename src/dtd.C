// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/dtd.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/dtdinlines.h>
#endif

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

int dtd::parseDtd() {

	// create the new tree
	xmldtd->createRootNode();
	xmldtd->getRootNode()->cascadeOnDelete();

	// step through tags
	xmldomnode	*root=xmld->getRootNode();
	xmldomnode	*currentnode=root->getFirstTagChild();
	while (!currentnode->isNullNode()) {

		if (!strcmp(currentnode->getName(),"!ELEMENT")) {

			// for each element, create a new element node
			if (!newElement(currentnode)) {
				return 0;
			}

		} else if (!strcmp(currentnode->getName(),"!ATTLIST")) {

			// for each attribute, create a new
			// attribute node
			if (!newAttribute(currentnode)) {
				return 0;
			}
		}

		// move on to the next tag
		currentnode=currentnode->getNextTagSibling();
	}

	// success
	return 1;
}

int dtd::newElement(xmldomnode *node) {

	// sanity check
	if (node->getAttributeCount()<2) {
		return 0;
	}

	// add the following to the tree:
	// ...
	// 	<element name="elementname">
	// 		<child name="elementname" count="*">
	// 			...
	// 		</child>
	// 		...
	// 		<attribute name="attributename" default="...">
	// 			<value data="value"/>
	// 			...
	// 		</attribute>
	// 		...
	// 	</element>
	// ...

	xmldomnode	*root=xmldtd->getRootNode();
	root->appendText("\n");
	xmldomnode	*element=new xmldomnode(root->getNullNode(),
					TAG_XMLDOMNODETYPE,"element",NULL);
	element->cascadeOnDelete();
	if (!root->appendChild(element)) {
		delete element;
		return 0;
	}
	if (!element->appendAttribute("name",
					node->getAttribute(0)->getName())) {
		return 0;
	}

	return parseList(node->getAttribute(1)->getName(),element,
							1,1,',',"child");
}

int dtd::parseList(const char *list, xmldomnode *node,
					int checkcount, int indent,
					char delimiter,
					const char *name) {

	if (!list) {

		// return failure for a NULL list
		return 0;


	} else if (!strcmp(list,"EMPTY")) {

		// return success for "EMPTY"
		return 1;

	} else {

		char	*ptr1=(char *)list;
		char	*ptr2;
		int	length;
		char	*word;
		char	count[2];
		count[1]=(char)NULL;
		for (;;) {
	
			// look for the specified delimiter
			ptr2=strchr(ptr1,delimiter);

			// if we don't find the delimiter, go
			// to the end of the string
			if (!ptr2) {
				ptr2=(char *)(list+strlen(list));
			}
	
			// parse out the word, including the * or +
			length=ptr2-ptr1;
			word=new char[length+1];
			word[length]=(char)NULL;
			strncpy(word,ptr1,length);

			// evaluate the trailing * or + and truncate it
			if (checkcount) {
				count[0]=word[strlen(word)-1];
				if (count[0]!='*' && count[0]!='+') {
					count[0]='1';
				} else {
					word[strlen(word)-1]=(char)NULL;
				}
			}

			// create the new tag
			node->appendText("\n");
			for (int i=0; i<indent; i++) {
				node->appendText("	");
			}
			xmldomnode	*newtag=new xmldomnode(
					xmldtd->getRootNode()->getNullNode(),
					TAG_XMLDOMNODETYPE,name,NULL);
			newtag->cascadeOnDelete();
			if (!node->appendChild(newtag)) {
				delete newtag;
				delete[] word;
				return 0;
			}
			if (!newtag->appendAttribute("name",word) ||
				(checkcount &&
				!newtag->appendAttribute("count",count))) {
				return 0;
			}
	
			// clean up
			delete[] word;
	
			// break if we're at the end of the string
			if (!*ptr2) {
				break;
			}
	
			// move on to the next word
			ptr1=ptr2+1;
		}

		// return success
		return 1;
	}
}

int dtd::newAttribute(xmldomnode *node) {

	// sanity check
	if (node->getAttributeCount()<4) {
		return 0;
	}

	// get the appropriate element
	xmldomnode	*element=xmldtd->getRootNode()->
		getChild("element","name",node->getAttribute(0)->getName());
	if (element->isNullNode()) {
		return 0;
	}

	// create a new attribute element
	element->appendText("\n	");
	xmldomnode	*attribute=new xmldomnode(
					xmldtd->getRootNode()->getNullNode(),
					TAG_XMLDOMNODETYPE,"attribute",NULL);
	attribute->cascadeOnDelete();
	if (!element->appendChild(attribute)) {
		return 0;
	}

	// insert the name and default value attributes
	if (!attribute->appendAttribute("name",
				node->getAttribute(1)->getName()) ||
		!attribute->appendAttribute("default",
				node->getAttribute(3)->getName())) {
		return 0;
	}

	// insert the list of valid values or none if CDATA
	if (strcmp(node->getAttribute(2)->getName(),"CDATA")) {
		return parseList(node->getAttribute(2)->getName(),
					attribute,0,2,'|',"value");
	}
	return 1;
}
