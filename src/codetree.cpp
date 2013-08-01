#include <rudiments/codetree.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// FIXME: make methods out of these
#define debugPrintIndent(level) if (pvt->_debuglevel>=level) { for (uint32_t i=0; i<pvt->_indent; i++) { printf(" "); } }
#ifdef _MSC_VER
	#define debugPrintf(level,ARGS,...) if (pvt->_debuglevel>=level) { fprintf(stdout,ARGS,__VA_ARGS__); fflush(stdout); }
#else
	#define debugPrintf(level,ARGS...) if (pvt->_debuglevel>=level) { fprintf(stdout,ARGS); fflush(stdout); }
#endif
#define debugSafePrint(level,string) if (pvt->_debuglevel>=level) { charstring::safePrint(string); }
#define debugSafePrintLength(level,string,length) if (pvt->_debuglevel>=level) { charstring::safePrint(string,length); }

class codetreeprivate {
	friend class codetree;
	private:
		xmldom		_grammar;
		xmldomnode	*_grammartag;
		bool		_error;
		uint32_t	_indent;
		bool		_previousparsechildretval;
		const char	*_finalcodeposition;
		uint8_t		_debuglevel;
};

codetree::codetree() {
	pvt=new codetreeprivate;
	pvt->_error=false;
	pvt->_indent=0;
	pvt->_grammartag=NULL;
	pvt->_previousparsechildretval=true;
	pvt->_finalcodeposition=NULL;
	pvt->_debuglevel=0;
}

codetree::~codetree() {
	delete pvt;
}

void codetree::setDebugLevel(uint8_t debuglevel) {
	pvt->_debuglevel=debuglevel;
}

bool codetree::parse(const char *input,
			const char *grammar,
			const char *starttoken,
			xmldomnode *output,
			const char **codeposition) {

	// load grammar
	if (!pvt->_grammar.parseString(grammar)) {
		return false;
	}
	pvt->_grammartag=pvt->_grammar.getRootNode()->
					getFirstTagChild("grammar");
	if (pvt->_grammartag->isNullNode()) {
		return false;
	}

	// re-init the error
	pvt->_error=false;

	// parse, starting with the specified start token
	const char	*codepos=input;
	pvt->_finalcodeposition=*codeposition;
	bool	retval=(parseNonTerminal(starttoken,output,
					&codepos,NULL) && !pvt->_error);
	if (codeposition) {
		*codeposition=pvt->_finalcodeposition;
	}
	return retval;
}

bool codetree::parseNonTerminal(const char *name,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {

	// find the definition
	xmldomnode	*defnode=
			pvt->_grammartag->
				getFirstTagChild("definition","name",name);
	if (defnode->isNullNode()) {
		return false;
	}

	// some variables...
	xmldomnode	*codenode=NULL;
	stringbuffer	*localtokenbuffer=NULL;
	uint8_t		debuglevel=2;

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
			debuglevel=1;
		}
		if (!charstring::compare(token,"literal")) {
			localtokenbuffer=new stringbuffer;
		}
	}

	debugPrintIndent(debuglevel);
	debugPrintf(debuglevel,"nonterminal \"%s\" at \"",name);
        debugSafePrintLength(debuglevel,*codeposition,20);
	debugPrintf(debuglevel,"\"...\n");

	// keep track of the current position in the code, if we don't find
	// this nonterminal then we'll need to reset the position in the code
	// to this
	const char	*startcodeposition=*codeposition;

	// there should be only one child
	if (parseChild(defnode->getFirstTagChild(),
			(codenode)?codenode:treeparent,
			codeposition,
			(tokenbuffer)?tokenbuffer:localtokenbuffer)) {

		debugPrintIndent(debuglevel);
		debugPrintf(debuglevel,"nonterminal \"%s\" found",name);

		// we found one of these, attach the node to the
		// code and value to the node, if necessary
		if (codenode) {
			treeparent->appendChild(codenode);
		}
		if (localtokenbuffer) {
			debugPrintf(debuglevel," - value: \"%s\"",
					localtokenbuffer->getString());
			codenode->setAttributeValue("value",
					localtokenbuffer->getString());
			delete localtokenbuffer;
		}

		debugPrintf(debuglevel,"\n");

		// advance the new code position
		return true;
	}

	debugPrintIndent(debuglevel);
	debugPrintf(debuglevel,"nonterminal \"%s\" not found at \"",name);
        debugSafePrintLength(debuglevel,startcodeposition,20);
	debugPrintf(debuglevel,"\"\n");

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

	pvt->_indent++;

	pvt->_finalcodeposition=*codeposition;

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
		retval=pvt->_previousparsechildretval;
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
			debugPrintIndent(3);
			debugPrintf(3,"resetting to \"");
			debugSafePrintLength(3,startcodeposition,20);
			debugPrintf(3,"\"\n");
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

	pvt->_indent--;
	pvt->_previousparsechildretval=retval;
	return retval;
}

bool codetree::parseConcatenation(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	debugPrintIndent(4);
	debugPrintf(4,"concatenation...\n");

	// all children must parse successfully
	for (xmldomnode *child=grammarnode->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		if (!parseChild(child,treeparent,codeposition,tokenbuffer)) {
			debugPrintIndent(4);
			debugPrintf(4,"concatenation failed\n");
			return false;
		}
	}

	debugPrintIndent(4);
	debugPrintf(4,"concatenation success\n");
	return true;
}

bool codetree::parseAlternation(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	debugPrintIndent(4);
	debugPrintf(4,"alternation...\n");

	// one of the children must parse successfully
	for (xmldomnode *child=grammarnode->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		if (parseChild(child,treeparent,codeposition,tokenbuffer)) {
			debugPrintIndent(4);
			debugPrintf(4,"alternation success\n");
			return true;
		}
	}

	debugPrintIndent(4);
	debugPrintf(4,"alternation failed\n");
	return false;
}

bool codetree::parseOption(xmldomnode *grammarnode, 
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	debugPrintIndent(4);
	debugPrintf(4,"option...\n");

	// there should be only one child and it doesn't
	// matter if it parses successfully or not
	if (!parseChild(grammarnode->getFirstTagChild(),treeparent,
						codeposition,tokenbuffer)) {
		debugPrintIndent(4);
		debugPrintf(4,"option failed\n");
	} else {
		debugPrintIndent(4);
		debugPrintf(4,"option success\n");
	}
	return true;
}

bool codetree::parseRepetition(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *tokenbuffer) {
	debugPrintIndent(4);
	debugPrintf(4,"repetition...\n");

	// there should be only one child, and zero or more instances of it
	xmldomnode	*child=grammarnode->getFirstTagChild();
	bool		anyfound=false;
	for (;;) {
		if (!parseChild(child,treeparent,codeposition,tokenbuffer)) {
			if (anyfound) {
				debugPrintIndent(4);
				debugPrintf(4,"repetition success\n");
			} else {
				debugPrintIndent(4);
				debugPrintf(4,"repetition failed\n");
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
	debugPrintIndent(4);
	debugPrintf(4,"exception...\n");

	// there should be only one child
	if (parseChild(grammarnode->getFirstTagChild(),
				treeparent,codeposition,tokenbuffer)) {
		debugPrintIndent(4);
		debugPrintf(4,"exception success\n");
		return true;
	}

	debugPrintIndent(4);
	debugPrintf(4,"exception failed\n");
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
		debugPrintIndent(4);
		debugPrintf(4,"terminal \"");
		debugSafePrint(4,value);
		debugPrintf(4,"\" found\n");
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
	if (!pvt->_grammar.parseString(grammar)) {
		return false;
	}
	pvt->_grammartag=
		pvt->_grammar.getRootNode()->getFirstTagChild("grammar");
	if (pvt->_grammartag->isNullNode()) {
		return false;
	}

	// re-init indent
	pvt->_indent=0;

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
	xmldomnode	*def=pvt->_grammartag->getFirstTagChild("definition",
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
		for (uint32_t i=0; i<pvt->_indent; i++) {
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

		// increase pvt->_indent
		if (blocktoken) {
			pvt->_indent++;
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
			pvt->_indent--;
		}
	}

	// write the end
	const char	*end=def->getAttributeValue("end");
	if (blocktoken && charstring::length(end)) {
		output->append("\n");
		for (uint32_t i=0; i<pvt->_indent; i++) {
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
