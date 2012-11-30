#include <rudiments/codetree.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

codetree::codetree() {
	error=false;
	indent=0;
	grammartag=NULL;
}

bool codetree::parse(const char *input,
			const char *grammar,
			const char *starttoken,
			xmldomnode *output) {

	// load grammar
	if (!this->grammar.parseString(grammar)) {
		return NULL;
	}
	grammartag=this->grammar.getRootNode()->getFirstTagChild("grammar");
	if (grammartag->isNullNode()) {
		return NULL;
	}

	// re-init the error
	error=false;

	// parse, starting with the specified start token
	const char	*codeposition=input;
	return (parseNonTerminal(starttoken,output,
				&codeposition,NULL) && !error);
}

bool codetree::parseNonTerminal(const char *name,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	//printIndent();
	//printf("looking for \"%s\"\n",name);

	// find the definition
	xmldomnode	*defnode=
			grammartag->getFirstTagChild("definition","name",name);
	if (defnode->isNullNode()) {
		return false;
	}

	// some variables...
	xmldomnode	*codenode=NULL;
	stringbuffer	*localtokenbuffer=NULL;

	// If a token buffer was passed in then we're apparently already
	// building up another token that stores a single value.  In that case
	// we don't need to do anything but pass the current code parent and
	// token buffer forward.
	if (!tokenbuffer) {

		// If no token buffer was passed in...

		// If this nonterminal is a token then we need to create a new
		// node for it that will be appended to the tree if we do,
		// indeed, find an instance of this nonterminal.  If it's not a
		// token then we just need to pass the current code parent
		// forward.  If this is a token that stores a single value,
		// rather than a token that groups other tokens, then we need
		// to create a buffer to build up the value in and pass it
		// forward.
		const char	*token=defnode->getAttributeValue("token");
		if (charstring::length(token)) {
			codenode=new xmldomnode(treeparent->getTree(),
						treeparent->getNullNode(),
						TAG_XMLDOMNODETYPE,
						name,NULL);
		}
		if (!charstring::compare(token,"single")) {
			localtokenbuffer=new stringbuffer;
		}
	}

	// keep track of the current position in the code, if we don't find
	// this nonterminal then we'll need to reset the position in the code
	// to this
	const char	*currentcodeposition=*codeposition;

	// there should be only one child
	if (parseChild(defnode->getFirstTagChild(),
			(codenode)?codenode:treeparent,
			codeposition,
			(tokenbuffer)?tokenbuffer:localtokenbuffer)) {

		//printIndent();
		//printf("found \"%s\"\n",name);

		// we found one of these, attach the node to the
		// code and value to the node, if necessary
		if (codenode) {
			treeparent->appendChild(codenode);
		}
		if (localtokenbuffer) {
			//printIndent();
			//printf("      value: \"%s\"\n",
					//localtokenbuffer->getString());
			codenode->setAttributeValue("value",
					localtokenbuffer->getString());
			delete localtokenbuffer;
		}

		// advance the new code position
		return true;
	}

	// apparently we didn't find one of these, delete the
	// node and buffer and reset the position in the code
	delete codenode;
	*codeposition=currentcodeposition;
	return false;
}

bool codetree::parseChild(xmldomnode *grammarnode,
			xmldomnode *treeparent,
			const char **codeposition,
			stringbuffer *tokenbuffer) {

	indent++;

	bool		retval=true;
	const char	*name=grammarnode->getName();
	if (!charstring::compare(name,"concatenation")) {
		if (!parseConcatenation(grammarnode,treeparent,
					codeposition,tokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"alternation")) {
		if (!parseAlternation(grammarnode,treeparent,
					codeposition,tokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"option")) {
		if (!parseOption(grammarnode,treeparent,
					codeposition,tokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"repetition")) {
		if (!parseRepetition(grammarnode,treeparent,
					codeposition,tokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"terminal")) {
		if (!parseTerminal(grammarnode,treeparent,
					codeposition,tokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"nonterminal")) {
		if (!parseNonTerminal(grammarnode,treeparent,
					codeposition,tokenbuffer)) {
			retval=false;
		}
	} else {
		retval=false;
	}

	indent--;
	return retval;
}

bool codetree::parseConcatenation(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	//printIndent();
	//printf("concatenation...\n");

	// all children must parse successfully
	for (xmldomnode *child=grammarnode->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		if (!parseChild(child,treeparent,
				codeposition,tokenbuffer)) {
			return false;
		}
	}
	return true;
}

bool codetree::parseAlternation(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	//printIndent();
	//printf("alternation...\n");

	// one of the children must parse successfully
	for (xmldomnode *child=grammarnode->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		if (parseChild(child,treeparent,
				codeposition,tokenbuffer)) {
			return true;
		}
	}
	return false;
}

bool codetree::parseOption(xmldomnode *grammarnode, 
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	//printIndent();
	//printf("option...\n");

	// there should be only one child and it doesn't
	// matter if it parses successfully or not
	parseChild(grammarnode->getFirstTagChild(),treeparent,
						codeposition,tokenbuffer);
	return true;
}

bool codetree::parseRepetition(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	//printIndent();
	//printf("repetition...\n");

	// there should be only one child, and zero or more instances of it
	xmldomnode	*child=grammarnode->getFirstTagChild();
	for (;;) {
		if (!parseChild(child,treeparent,
				codeposition,tokenbuffer)) {
			return true;
		}
	}
}

bool codetree::parseTerminal(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {

	// get the attributes of this terminal
	const char	*value=grammarnode->getAttributeValue("value");
	size_t		valuelength=charstring::length(value);
	bool		caseinsensitive=!charstring::compare(
					grammarnode->getAttributeValue("case"),
					"false");

	// see the code matches this terminal
	bool	match=false;
	if (caseinsensitive) {
		match=!charstring::compareIgnoringCase(
					value,*codeposition,valuelength);
	} else {
		match=!charstring::compare(value,*codeposition,valuelength);
	}

	// if it matches, append the terminal to
	// the token and advance the code position
	if (match) {
		if (tokenbuffer) {
			tokenbuffer->append(*codeposition,valuelength);
		}
		*codeposition=*codeposition+valuelength;
		//printIndent();
		//printf("\"%s\"\n",value);
		return true;
	}

	// if it didn't match, don't advance the code position
	return false;
}

bool codetree::parseNonTerminal(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	return parseNonTerminal(grammarnode->getAttributeValue("name"),
						treeparent,codeposition,
						tokenbuffer);
}

void codetree::print(xmldomnode *tree) {
	if (!tree || tree->isNullNode()) {
		return;
	}
	stringbuffer	*xmlstr=tree->xml();
	const char	*xml=xmlstr->getString();
	int16_t		indent=0;
	bool		endtag=false;
	for (const char *ptr=xml; *ptr; ptr++) {
		if (*ptr=='<') {
			if (*(ptr+1)=='/') {
				indent=indent-2;
				endtag=true;
			}
			for (uint16_t i=0; i<indent; i++) {
				printf(" ");
			}
		}
		printf("%c",*ptr);
		if (*ptr=='>') {
			printf("\n");
			if (*(ptr-1)!='/' && !endtag) {
				indent=indent+2;
			}
			endtag=false;
		}
	}
	delete xmlstr;
}

void codetree::printIndent() {
	for (uint32_t i=0; i<indent; i++) {
		printf(" ");
	}
}

bool codetree::write(xmldomnode *input,
			const char *grammar,
			stringbuffer *output) {

	// if we have an empty input, just return
	if (!input || input->isNullNode()) {
		return NULL;
	}

	// load grammar
	if (!this->grammar.parseString(grammar)) {
		return NULL;
	}
	grammartag=this->grammar.getRootNode()->getFirstTagChild("grammar");
	if (grammartag->isNullNode()) {
		return NULL;
	}

	// re-init indent
	indent=0;

	// write the nodes
	return writeNode(input,output);
}

bool codetree::writeNode(xmldomnode *node, stringbuffer *output) {

	// we're done
	if (node->isNullNode()) {
		return true;
	}

	// if it's the root node then just write the children and return
	if (node->getType()==ROOT_XMLDOMNODETYPE) {
		for (xmldomnode *child=node->getFirstTagChild();
				!child->isNullNode();
				child=child->getNextTagSibling()) {
			if (!writeNode(child,output)) {
				return false;
			}
		}
		return true;
	}

	// if the node has a value, just write that and return
	const char	*value=node->getAttributeValue("value");
	if (charstring::length(value)) {
		output->append(value);
		return true;
	}

	// if the node doesn't have a value then get its definition
	xmldomnode	*def=grammartag->getFirstTagChild("definition",
							"name",node->getName());
	if (def->isNullNode()) {
		return false;
	}

	// see if this is a group, block or line
	const char	*token=def->getAttributeValue("token");
	bool		block=!charstring::compare(token,"block");
	bool		line=!charstring::compare(token,"line");

	// write the start
	if (block || line) {
		for (uint32_t i=0; i<indent; i++) {
			output->append("\t");
		}
	}
	output->append(def->getAttributeValue("start"));
	if (block) {
		output->append("\n");
	}

	// increase indent
	if (block) {
		indent++;
	}

	// write children
	for (xmldomnode *child=node->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		if (!writeNode(child,output)) {
			return false;
		}
	}

	// decrease indent
	if (block) {
		indent--;
	}

	// write the end
	output->append(def->getAttributeValue("end"));

	// carriage return at the end of line or block
	if (block || line) {
		output->append("\n");
	}
	return true;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
