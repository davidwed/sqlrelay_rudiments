#include <rudiments/codetree.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <stdio.h>

#define DEBUG_MESSAGES 1
#ifdef DEBUG_MESSAGES
	#define debugFunction() printf("%s:%s():%d:\n",__FILE__,__FUNCTION__,__LINE__); fflush(stdout);
	#define debugPrintf(ARGS...) printf(##ARGS); fflush(stdout);
	#define debugSafePrint(str) charstring::safePrint(str)
	#define debugSafePrintLength(str,len) charstring::safePrint(str,(charstring::length(str)>len)?len:charstring::length(str))
	#define debugPrintIndent() for (uint32_t i=0; i<indent; i++) { debugPrintf(" "); }
#else
	#define debugFunction() /* */
	#define debugPrintf(format, ...) /* */
	#define debugSafePrint(str) /* */
	#define debugSafePrintLength(str,len) /* */
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

codetree::codetree() {
	error=false;
	indent=0;
	grammartag=NULL;
	previousparsechildretval=true;
	finalcodeposition=NULL;
}

xmldomnode	*top=NULL;

bool codetree::parse(const char *input,
			const char *grammar,
			const char *starttoken,
			xmldomnode *output,
			const char **codeposition) {

	// load grammar
	if (!this->grammar.parseString(grammar)) {
		return false;
	}
	grammartag=this->grammar.getRootNode()->getFirstTagChild("grammar");
	if (grammartag->isNullNode()) {
		return false;
	}

	// re-init the error
	error=false;

top=output;
	// parse, starting with the specified start token
	const char	*codepos=input;
	finalcodeposition=*codeposition;
	bool	retval=(parseNonTerminal(starttoken,output,
						&codepos,NULL) && !error);
	if (codeposition) {
		*codeposition=finalcodeposition;
	}
	return retval;
}

bool codetree::parseNonTerminal(const char *name,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	debugPrintIndent();
	debugPrintf("nonterminal \"%s\" at \"",name);
        debugSafePrintLength(*codeposition,20);
	debugPrintf("\"\n");

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
	// building up another token that stores a literal value.  In that case
	// we don't need to do anything but pass the current code parent and
	// token buffer forward.
	if (!tokenbuffer) {

		// If no token buffer was passed in...

		// If this nonterminal is a token then we need to create a new
		// node for it that will be appended to the tree if we do,
		// indeed, find an instance of this nonterminal.  If it's not a
		// token then we just need to pass the current code parent
		// forward.  If this is a token that stores a literal value,
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
		if (!charstring::compare(token,"literal")) {
			localtokenbuffer=new stringbuffer;
		}
	}

	// keep track of the current position in the code, if we don't find
	// this nonterminal then we'll need to reset the position in the code
	// to this
	const char	*startcodeposition=*codeposition;

	// there should be only one child
	if (parseChild(defnode->getFirstTagChild(),
			(codenode)?codenode:treeparent,
			codeposition,
			(tokenbuffer)?tokenbuffer:localtokenbuffer)) {

		debugPrintIndent();
		debugPrintf("nonterminal \"%s\" found",name);

		// we found one of these, attach the node to the
		// code and value to the node, if necessary
		if (codenode) {
			treeparent->appendChild(codenode);
		}
		if (localtokenbuffer) {
			debugPrintf(" - value: \"%s\"",
					localtokenbuffer->getString());
			codenode->setAttributeValue("value",
					localtokenbuffer->getString());
			delete localtokenbuffer;
		}

		debugPrintf("\n");

		// advance the new code position
		return true;
	}

	debugPrintIndent();
	debugPrintf("nonterminal \"%s\" not found\n",name);

	// apparently we didn't find one of these, delete the
	// node and buffer and reset the position in the code
	delete codenode;
	*codeposition=startcodeposition;
	return false;
}

bool codetree::parseChild(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {

	indent++;

	finalcodeposition=*codeposition;

	// save the initial state
	uint64_t	startchildcount=treeparent->getChildCount();
	const char	*startcodeposition=*codeposition;
	stringbuffer	*localtokenbuffer=NULL;
	if (tokenbuffer) {
		localtokenbuffer=new stringbuffer();
	}

	// handle the standard children
	bool		retval=true;
	const char	*name=grammarnode->getName();
	if (!charstring::compare(name,"concatenation")) {
		if (!parseConcatenation(grammarnode,treeparent,
					codeposition,localtokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"alternation")) {
		if (!parseAlternation(grammarnode,treeparent,
					codeposition,localtokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"option")) {
		if (!parseOption(grammarnode,treeparent,
					codeposition,localtokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"repetition")) {
		if (!parseRepetition(grammarnode,treeparent,
					codeposition,localtokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"terminal")) {
		if (!parseTerminal(grammarnode,treeparent,
					codeposition,localtokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"nonterminal")) {
		if (!parseNonTerminal(grammarnode,treeparent,
					codeposition,localtokenbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"exception")) {
		// ignore exceptions if we encounter them directly
		retval=previousparsechildretval;
	} else {
		retval=false;
	}

	// if parsing the child succeeded, process
	// any exception that might follow
	if (retval) {

		// if the next node is an exception...
		xmldomnode	*sibling=
				grammarnode->getNextTagSibling("exception");
		if (!sibling->isNullNode()) {

			// create some local buffers for parsing the exception
			const char	*exccodeposition=startcodeposition;
			stringbuffer	exctokenbuffer;
			xmldomnode	excnode(treeparent->getTree(),
						treeparent->getNullNode(),
						TAG_XMLDOMNODETYPE,
						"temp",NULL);

			// parse the exception
			if (parseException(sibling,&excnode,
					&exccodeposition,&exctokenbuffer)) {

				// if we've been building up a literal token,
				// then see if the exception that we found
				// matches this last chunk or not
				if (localtokenbuffer &&
					!charstring::compare(
						exctokenbuffer.getString(),
					localtokenbuffer->getString())){
					retval=false;
				}

				// FIXME: if we weren't building up a literal
				// token then we need to compare the tree
				// generated by parsing the exception to the
				// tree built by parsing the previous child
			}
		}
	}

	if (!retval) {
		// reset to the initial state
		if (startcodeposition!=*codeposition) {
			debugPrintIndent();
			debugPrintf("resetting to \"");
			debugSafePrintLength(startcodeposition,20);
			debugPrintf("\"\n");
		}
		*codeposition=startcodeposition;
		uint64_t	childcount=treeparent->getChildCount();
		if (childcount>startchildcount) {
			treeparent->deleteChild(childcount-1);
		}
	} else {
		if (tokenbuffer) {
			tokenbuffer->append(localtokenbuffer->getString());
		}
	}
	delete localtokenbuffer;

	indent--;
	previousparsechildretval=retval;
	return retval;
}

bool codetree::parseConcatenation(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	debugPrintIndent();
	debugPrintf("concatenation...\n");

	// all children must parse successfully
	for (xmldomnode *child=grammarnode->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		if (!parseChild(child,treeparent,codeposition,tokenbuffer)) {
			debugPrintIndent();
			debugPrintf("concatenation failed\n");
			return false;
		}
	}

	debugPrintIndent();
	debugPrintf("concatenation success\n");
	return true;
}

bool codetree::parseAlternation(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	debugPrintIndent();
	debugPrintf("alternation...\n");

	// one of the children must parse successfully
	for (xmldomnode *child=grammarnode->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		if (parseChild(child,treeparent,codeposition,tokenbuffer)) {
			debugPrintIndent();
			debugPrintf("alternation success\n");
			return true;
		}
	}

	debugPrintIndent();
	debugPrintf("alternation failed\n");
	return false;
}

bool codetree::parseOption(xmldomnode *grammarnode, 
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	debugPrintIndent();
	debugPrintf("option...\n");

	// there should be only one child and it doesn't
	// matter if it parses successfully or not
	if (!parseChild(grammarnode->getFirstTagChild(),treeparent,
						codeposition,tokenbuffer)) {
		debugPrintIndent();
		debugPrintf("option failed\n");
	} else {
		debugPrintIndent();
		debugPrintf("option success\n");
	}
	return true;
}

bool codetree::parseRepetition(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	debugPrintIndent();
	debugPrintf("repetition...\n");

	// there should be only one child, and zero or more instances of it
	xmldomnode	*child=grammarnode->getFirstTagChild();
	bool		anyfound=false;
	for (;;) {
		if (!parseChild(child,treeparent,codeposition,tokenbuffer)) {
			if (anyfound) {
				debugPrintIndent();
				debugPrintf("repetition success\n");
			} else {
				debugPrintIndent();
				debugPrintf("repetition failed\n");
			}
			return true;
		}
		anyfound=true;
	}
}

bool codetree::parseException(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	debugPrintIndent();
	debugPrintf("exception...\n");

	// there should be only one child
	if (parseChild(grammarnode->getFirstTagChild(),
				treeparent,codeposition,tokenbuffer)) {
		debugPrintIndent();
		debugPrintf("exception success\n");
		return true;
	}

	debugPrintIndent();
	debugPrintf("exception failed\n");
	return false;
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
		debugPrintIndent();
		debugPrintf("terminal \"");
		debugSafePrint(value);
		debugPrintf("\" found\n");
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

bool codetree::write(xmldomnode *input,
			const char *grammar,
			stringbuffer *output) {

	// if we have an empty input, just return
	if (!input || input->isNullNode()) {
		return false;
	}

	// load grammar
	if (!this->grammar.parseString(grammar)) {
		return false;
	}
	grammartag=this->grammar.getRootNode()->getFirstTagChild("grammar");
	if (grammartag->isNullNode()) {
		return false;
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

	// if the node doesn't have a value then get its definition
	xmldomnode	*def=grammartag->getFirstTagChild("definition",
							"name",node->getName());
	if (def->isNullNode()) {
		return false;
	}

	// see if this is a block, line or inline token
	const char	*token=def->getAttributeValue("token");
	bool		blocktoken=!charstring::compare(token,"block");
	bool		linetoken=!charstring::compare(token,"line");
	//bool		inlinetoken=!charstring::compare(token,"inline");

	// write the start
	// FIXME: tab over for inline tokens if the previous token was a block
	if (blocktoken || linetoken) {
		for (uint32_t i=0; i<indent; i++) {
			output->append("\t");
		}
	}
	output->append(def->getAttributeValue("start"));
	if (blocktoken) {
		output->append("\n");
	}

	// if the node has a value, just write that,
	// otherwise write its children
	const char	*value=node->getAttributeValue("value");
	if (charstring::length(value)) {
		output->append(value);
	} else {

		// increase indent
		if (blocktoken) {
			indent++;
		}

		// write the children
		for (xmldomnode *child=node->getFirstTagChild();
				!child->isNullNode();
				child=child->getNextTagSibling()) {
			if (!writeNode(child,output)) {
				return false;
			}
		}

		// decrease indent
		if (blocktoken) {
			indent--;
		}
	}

	// write the end
	const char	*end=def->getAttributeValue("end");
	if (blocktoken && charstring::length(end)) {
		output->append("\n");
		for (uint32_t i=0; i<indent; i++) {
			output->append("\t");
		}
	}
	output->append(end);
	if (blocktoken || linetoken) {
		output->append("\n");
	}
	return true;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
