// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/dtd.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/dtdinlines.h>
#endif

#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

int dtd::parseDtd() {

	// create the new tree
	xmldtd->createRootNode();
	xmldtd->getRootNode()->cascadeOnDelete();

	xmldomnode	*root=xmld->getRootNode();
	int		tagcount=root->getChildCount();

	// step through tags
	xmldomnode	*currentnode=root->getChild(0);
	while (currentnode!=root->getNullNode()) {

		if (currentnode->getType()==TAG_XMLDOMNODETYPE) {

			if (!strcmp(currentnode->getName(),"!ENTITY")) {

				// for each entity, create a new entity node
				if (!newEntity(currentnode)) {
					return 0;
				}

			} else if (!strcmp(currentnode->getName(),
							"!ATTRIBUTE")) {

				// for each attribute, create a new
				// attribute node
				if (!newAttribute(currentnode)) {
					return 0;
				}
			}
		}

		// move on to the next tag
		currentnode=currentnode->getNextSibling();
	}

	// success
	return 1;
}

int dtd::newEntity(xmldomnode *node) {

	// add the following to the tree:
	// ...
	// 	<entity name="entityname">
	// 		...
	// 		<attribute name="atttributename" count="*">
	// 			...
	// 		</attribute>
	// 		...
	// 	</entity>
	// ...

	xmldomnode	*entity=new xmldomnode(
					xmldtd->getRootNode()->getNullNode(),
					TAG_XMLDOMNODETYPE,
					"entity",
					NULL);
	if (!xmldtd->getRootNode()->appendChild(entity)) {
		delete entity;
		return 0;
	}
	if (!entity->appendAttribute("name",node->getAttribute(0)->getName())) {
		return 0;
	}
	if (!xmldtd->getRootNode()->appendText("\n")) {
		return 0;
	}

	return parseList(node->getAttribute(1)->getName(),entity,
				1,1,"attribute");
}

int dtd::parseList(const char *attributelist, xmldomnode *entity,
					int checkcount, int indent,
					const char *name) {

	if (!attributelist) {

		// return failure for a NULL attribute list
		return 0;

	} else if (attributelist[0]!='(') {

		char	*ptr1=(char *)attributelist+1;
		char	*ptr2;
		int	length;
		char	*word;
		char	count[2];
		for (;;) {
	
			// look for a | or ) as a delimiter, if we find neither
			// then return failure
			if (!(ptr2=strchr(ptr1,'|')) &&
				!(ptr2=strchr(ptr1,')'))) {
				return 0;
			}
	
			// parse out the word, including the * or +
			length=ptr2-ptr1;
			word=new char[length+1];
			strncpy(word,ptr1,length);

			// evaluate the trailing * or + and truncate it
			if (checkcount) {
				count[0]=word[strlen(word)-1];
				if (count[0]!='*' && count[0]!='+') {
					count[0]=(char)NULL;
				} else {
					word[strlen(word)-1]=(char)NULL;
				}
			}

			// create the new attribute tag
			for (int i=0; i<indent; i++) {
				entity->appendText("	");
			}
			xmldomnode	*attribute=new xmldomnode(
					xmldtd->getRootNode()->getNullNode(),
					TAG_XMLDOMNODETYPE,name,NULL);
			if (!entity->appendChild(attribute)) {
				delete attribute;
				delete[] word;
				return 0;
			}
			if (!attribute->appendAttribute("name",word) ||
				(checkcount &&
				!attribute->appendAttribute("count",count))) {
				return 0;
			}
			entity->appendText("\n");
	
			// clean up
			delete[] word;
	
			// move on to the next word
			ptr1=ptr2+1;
	
			// break if we're at the end of the string
			if (!ptr1) {
				break;
			}
		}

		// return success
		return 1;

	} else if (!strcmp(attributelist,"EMPTY")) {

		// return success for "EMPTY"
		return 1;

	}

	// return failure for anything else
	return 0;
}

int dtd::newAttribute(xmldomnode *node) {

	// get the appropriate entity
	xmldomnode	*entity=xmldtd->getRootNode()->
		getChild("entity","name",node->getAttribute(0)->getName());
	if (entity->isNullNode()) {
		return 0;
	}

	// get the appropriate attribute
	xmldomnode	*attribute=entity->getChild(0)->
		getChild("attribute","name",node->getAttribute(1)->getName());
	if (attribute->isNullNode()) {
		return 0;
	}

	// insert the list of valid values and the default
	return (parseList(node->getAttribute(2)->getName(),
				attribute,0,2,"value") &&
		attribute->appendAttribute("default",
				node->getAttribute(3)->getName()));
}
