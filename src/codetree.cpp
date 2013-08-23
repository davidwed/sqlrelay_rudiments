#include <rudiments/codetree.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/stdio.h>

// for fflush, stdout
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#define debugPrintIndent(level) if (pvt->_debuglevel>=level) { for (uint32_t i=0; i<pvt->_indentlevel; i++) { stdoutput.printf(" "); } }
#ifdef _MSC_VER
	#define debugPrintf(level,ARGS,...) if (pvt->_debuglevel>=level) { stdoutput.printf(ARGS,__VA_ARGS__); fflush(stdout); }
#else
	#define debugPrintf(level,ARGS...) if (pvt->_debuglevel>=level) { stdoutput.printf(ARGS); fflush(stdout); }
#endif
#define debugSafePrint(level,string) if (pvt->_debuglevel>=level) { stdoutput.safePrint(string); }
#define debugSafePrintLength(level,string,length) if (pvt->_debuglevel>=level) { stdoutput.safePrint(string,length); }

class grammar : public xmldom {
	protected:
		bool	tagStart(const char *name);
		bool	attributeName(const char *name);
};

bool grammar::tagStart(const char *name) {
	if (!charstring::compare(name,"letter")) {
		return xmldom::tagStart("ltr");
	} else if (!charstring::compare(name,"lowercaseletter")) {
		return xmldom::tagStart("lcl");
	} else if (!charstring::compare(name,"uppercaseletter")) {
		return xmldom::tagStart("ucl");
	} else if (!charstring::compare(name,"digit")) {
		return xmldom::tagStart("dgt");
	} else {
        	char	newname[2];
		newname[0]=name[0];
		newname[1]='\0';
		return xmldom::tagStart(newname);
	}
}

bool grammar::attributeName(const char *name) {
        char	newname[2];
	newname[0]=name[0];
	newname[1]='\0';
	return xmldom::attributeName(newname);
}

class codetreeprivate {
	friend class codetree;
	private:
		grammar		_grammar;
		xmldomnode	*_grammartag;
		bool		_error;
		uint32_t	_indentlevel;
		const char	*_indentstring;
		bool		_previousparsechildretval;
		bool		_break;
		const char	*_finalcodeposition;
		uint8_t		_debuglevel;
};

codetree::codetree() {
	pvt=new codetreeprivate;
	pvt->_error=false;
	pvt->_indentlevel=0;
	pvt->_indentstring="\t";
	pvt->_grammartag=NULL;
	pvt->_previousparsechildretval=true;
	pvt->_break=false;
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
			const char *startsymbol,
			xmldomnode *output,
			const char **codeposition) {

	// load grammar
	if (!pvt->_grammar.parseString(grammar)) {
		return false;
	}
	pvt->_grammartag=pvt->_grammar.getRootNode()->getFirstTagChild("g");
	if (pvt->_grammartag->isNullNode()) {
		return false;
	}

	// re-init the error
	pvt->_error=false;

	// parse, starting with the specified start symbol
	const char	*codepos=input;
	pvt->_finalcodeposition=*codeposition;
	bool	retval=(parseNonTerminal(startsymbol,output,
					&codepos,NULL) && !pvt->_error);
	if (codeposition) {
		*codeposition=pvt->_finalcodeposition;
	}
	return retval;
}

const char *codetree::getSymbolType(rudiments::xmldomnode *nt) {
	const char	*symboltype=nt->getAttributeValue("t");
	if (!charstring::length(symboltype)) {
		symboltype="inline";
	}
	return symboltype;
}

bool codetree::parseNonTerminal(const char *name,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {

	// find the definition
	xmldomnode	*def=pvt->_grammartag->getFirstTagChild("d","n",name);
	if (def->isNullNode()) {
		return false;
	}

	// some variables...
	xmldomnode	*codenode=NULL;
	stringbuffer	*localntbuffer=NULL;
	uint8_t		debuglevel=2;

	// If a nonterminal buffer was passed in then we're apparently already
	// building up another nonterminal that stores a literal value.  In
	// that case we don't need to do anything but pass the current code
	// parent and nonterminal buffer forward.
	const char	*symboltype=getSymbolType(def);
	if (!ntbuffer) {

		// If no nonterminal buffer was passed in...

		// If this nonterminal has a type then we need to create a new
		// node for it that will be appended to the tree if we do,
		// indeed, find an instance of this nonterminal.  If it does
		// not have a type then we just need to pass the current code
		// parent forward.  If it has a type of literal, then we
		// need to create a buffer to build up the value in and pass it
		// forward.
		if (charstring::compare(symboltype,"none")) {
			codenode=new xmldomnode(treeparent->getTree(),
						treeparent->getNullNode(),
						TAG_XMLDOMNODETYPE,
						name,NULL);
			debuglevel=1;
		}
		if (!charstring::compare(symboltype,"literal")) {
			localntbuffer=new stringbuffer;
		}
	}

	debugPrintIndent(debuglevel);
	debugPrintf(debuglevel,"nonterminal (%s) \"%s\" at \"",symboltype,name);
        debugSafePrintLength(debuglevel,*codeposition,
			(charstring::length(*codeposition)>20)?
				20:charstring::length(*codeposition));
	debugPrintf(debuglevel,"\"... {\n");

	// keep track of the current position in the code, if we don't find
	// this nonterminal then we'll need to reset the position in the code
	// to this
	const char	*startcodeposition=*codeposition;

	// there should be only one child
	if (parseChild(def->getFirstTagChild(),
			(codenode)?codenode:treeparent,
			codeposition,
			(ntbuffer)?ntbuffer:localntbuffer)) {

		debugPrintIndent(debuglevel);
		debugPrintf(debuglevel,"} nonterminal \"%s\" found",name);

		// we found one of these, attach the node to the
		// code and value to the node, if necessary
		if (codenode) {
			treeparent->appendChild(codenode);
		}
		if (localntbuffer) {
			debugPrintf(debuglevel," - value: \"%s\"",
					localntbuffer->getString());
			codenode->setAttributeValue("value",
					localntbuffer->getString());
			delete localntbuffer;
		}

		debugPrintf(debuglevel,"\n");

		// advance the new code position
		return true;
	}

	debugPrintIndent(debuglevel);
	debugPrintf(debuglevel,"} nonterminal \"%s\" not found at \"",name);
        debugSafePrintLength(debuglevel,startcodeposition,
			(charstring::length(startcodeposition)>20)?
				20:charstring::length(startcodeposition));
	debugPrintf(debuglevel,"\"\n");

	// apparently we didn't find one of these, delete the
	// node and buffer and reset the position in the code
	delete codenode;
	delete localntbuffer;
	*codeposition=startcodeposition;
	return false;
}

bool codetree::parseChild(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {

	pvt->_indentlevel++;

	pvt->_finalcodeposition=*codeposition;

	// save the initial state
	uint64_t	startchildcount=treeparent->getChildCount();
	const char	*startcodeposition=*codeposition;
	stringbuffer	*localntbuffer=NULL;
	if (ntbuffer) {
		localntbuffer=new stringbuffer();
	}

	// handle the standard children
	bool		retval=true;
	const char	*name=grammarnode->getName();
	if (!charstring::compare(name,"c")) {
		if (!parseConcatenation(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"a")) {
		if (!parseAlternation(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"o")) {
		if (!parseOption(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"r")) {
		if (!parseRepetition(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"t")) {
		if (!parseTerminal(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"ltr")) {
		if (!parseLetter(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"lcl")) {
		if (!parseLowerCaseLetter(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"ucl")) {
		if (!parseUpperCaseLetter(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"dgt")) {
		if (!parseDigit(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"s")) {
		if (!parseSet(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"b")) {
		if (!parseBreak(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"n")) {
		if (!parseNonTerminal(grammarnode,treeparent,
					codeposition,localntbuffer)) {
			retval=false;
		}
	} else if (!charstring::compare(name,"e")) {
		// ignore exceptions if we encounter them directly
		retval=pvt->_previousparsechildretval;
	} else {
		retval=false;
	}

	// if parsing the child succeeded, process
	// any exception that might follow
	if (retval) {

		// if the next node is an exception...
		xmldomnode	*sibling=grammarnode->getNextTagSibling("e");
		if (!sibling->isNullNode()) {

			// create some local buffers
			stringbuffer	excntbuffer;
			xmldomnode	excnode(treeparent->getTree(),
						treeparent->getNullNode(),
						TAG_XMLDOMNODETYPE,
						"temp",NULL);

			// reparse the same code we just parsed
			const char	*exccodeposition=startcodeposition;

			// parse the exception
			if (parseException(sibling,&excnode,
					&exccodeposition,&excntbuffer)) {

				// if we've been building up a literal,
				// then see if the exception that we found
				// matches this last chunk or not
				if (localntbuffer &&
					!charstring::compare(
						excntbuffer.getString(),
						localntbuffer->getString())) {
					retval=false;
				}

				// FIXME: if we weren't building up a literal
				// then we need to compare the tree generated
				// by parsing the exception to the tree built
				// by parsing the previous child
			}
		}
	}

	if (!retval) {
		// reset to the initial state
		if (startcodeposition!=*codeposition) {
			debugPrintIndent(3);
			debugPrintf(3,"resetting to \"");
        		debugSafePrintLength(3,startcodeposition,
				(charstring::length(startcodeposition)>20)?
					20:charstring::length(
						startcodeposition));
			debugPrintf(3,"\"\n");
		}
		*codeposition=startcodeposition;
		uint64_t	childcount=treeparent->getChildCount();
		if (childcount>startchildcount) {
			treeparent->deleteChild(childcount-1);
		}
	} else {
		if (ntbuffer) {
			ntbuffer->append(localntbuffer->getString());
		}
	}
	delete localntbuffer;

	pvt->_indentlevel--;
	pvt->_previousparsechildretval=retval;
	return retval;
}

bool codetree::parseConcatenation(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {
	debugPrintIndent(4);
	debugPrintf(4,"concatenation... {\n");

	// all children must parse successfully
	for (xmldomnode *child=grammarnode->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		if (!parseChild(child,treeparent,codeposition,ntbuffer)) {
			debugPrintIndent(4);
			debugPrintf(4,"} concatenation failed\n");
			return false;
		}
	}

	debugPrintIndent(4);
	debugPrintf(4,"} concatenation success\n");
	return true;
}

bool codetree::parseAlternation(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {
	debugPrintIndent(4);
	debugPrintf(4,"alternation... {\n");

	// one of the children must parse successfully
	for (xmldomnode *child=grammarnode->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		if (parseChild(child,treeparent,codeposition,ntbuffer)) {
			debugPrintIndent(4);
			debugPrintf(4,"} alternation success\n");
			return true;
		}
	}

	debugPrintIndent(4);
	debugPrintf(4,"} alternation failed\n");
	return false;
}

bool codetree::parseOption(xmldomnode *grammarnode, 
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {
	debugPrintIndent(4);
	debugPrintf(4,"option... {\n");

	// there should be only one child and it doesn't
	// matter if it parses successfully or not
	if (!parseChild(grammarnode->getFirstTagChild(),treeparent,
						codeposition,ntbuffer)) {
		debugPrintIndent(4);
		debugPrintf(4,"} option failed\n");
	} else {
		debugPrintIndent(4);
		debugPrintf(4,"} option success\n");
	}
	return true;
}

bool codetree::parseRepetition(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {
	debugPrintIndent(4);
	debugPrintf(4,"repetition... {\n");

	// there should be only one child, and zero or more instances of it
	xmldomnode	*child=grammarnode->getFirstTagChild();
	bool		anyfound=false;
	for (;;) {

		// save old break state and reset break flag
		bool	oldbreak=pvt->_break;
		pvt->_break=false;

		// parse the child
		bool	parseresult=parseChild(child,treeparent,
						codeposition,ntbuffer);

		// process a break and restore break state
		if (pvt->_break) {
			parseresult=false;
			anyfound=true;
		}
		pvt->_break=oldbreak;

		if (!parseresult) {
			if (anyfound) {
				debugPrintIndent(4);
				debugPrintf(4,"} repetition success\n");
			} else {
				debugPrintIndent(4);
				debugPrintf(4,"} repetition failed\n");
			}
			return true;
		}
		anyfound=true;
	}
}

bool codetree::parseException(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {
	debugPrintIndent(4);
	debugPrintf(4,"exception... {\n");

	// there should be only one child
	if (parseChild(grammarnode->getFirstTagChild(),
				treeparent,codeposition,ntbuffer)) {
		debugPrintIndent(4);
		debugPrintf(4,"} exception success\n");
		return true;
	}

	debugPrintIndent(4);
	debugPrintf(4,"} exception failed\n");
	return false;
}

bool codetree::parseTerminal(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {

	// get the attributes of this terminal
	const char	*value=grammarnode->getAttributeValue("v");
	size_t		valuelength=charstring::length(value);
	bool		caseinsensitive=!charstring::compare(
					grammarnode->getAttributeValue("c"),
					"false");

	// see if the code matches this terminal
	bool	match=false;
	if (caseinsensitive) {
		match=!charstring::compareIgnoringCase(
					value,*codeposition,valuelength);
	} else {
		match=!charstring::compare(value,*codeposition,valuelength);
	}

	// if it matches, append the terminal to the
	// nonterminal and advance the code position
	if (match) {
		if (ntbuffer) {
			ntbuffer->append(*codeposition,valuelength);
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

bool codetree::parseLetter(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {

	// if it matches, append the terminal to the
	// nonterminal and advance the code position
	if (character::isAlphabetical((int32_t)(**codeposition))) {
		if (ntbuffer) {
			ntbuffer->append(**codeposition);
		}
		debugPrintIndent(4);
		debugPrintf(4,"letter found: \"%c\"\n",**codeposition);
		*codeposition=*codeposition+1;
		return true;
	}

	// if it didn't match, don't advance the code position
	return false;
}

bool codetree::parseLowerCaseLetter(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer) {

	// if it matches, append the terminal to the
	// nonterminal and advance the code position
	if (character::isLowerCase((int32_t)(**codeposition))) {
		if (ntbuffer) {
			ntbuffer->append(**codeposition);
		}
		debugPrintIndent(4);
		debugPrintf(4,"lower case letter found: \"%c\"\n",
							**codeposition);
		*codeposition=*codeposition+1;
		return true;
	}

	// if it didn't match, don't advance the code position
	return false;
}

bool codetree::parseUpperCaseLetter(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer) {

	// if it matches, append the terminal to the
	// nonterminal and advance the code position
	if (character::isUpperCase((int32_t)(**codeposition))) {
		if (ntbuffer) {
			ntbuffer->append(**codeposition);
		}
		debugPrintIndent(4);
		debugPrintf(4,"upper case letter found: \"%c\"\n",
							**codeposition);
		*codeposition=*codeposition+1;
		return true;
	}

	// if it didn't match, don't advance the code position
	return false;
}

bool codetree::parseDigit(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {

	// if it matches, append the terminal to the
	// nonterminal and advance the code position
	if (character::isDigit((int32_t)(**codeposition))) {
		if (ntbuffer) {
			ntbuffer->append(**codeposition);
		}
		debugPrintIndent(4);
		debugPrintf(4,"digit found: \"%c\"\n",**codeposition);
		*codeposition=*codeposition+1;
		return true;
	}

	// if it didn't match, don't advance the code position
	return false;
}

bool codetree::parseSet(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {

	// get the set value
	const char	*value=grammarnode->getAttributeValue("v");

	// if it matches, append the terminal to the
	// nonterminal and advance the code position
	if (character::inSet((int32_t)(**codeposition),value)) {
		if (ntbuffer) {
			ntbuffer->append(**codeposition);
		}
		debugPrintIndent(4);
		debugPrintf(4,"set member found: \"%c\"\n",**codeposition);
		debugPrintf(4,"set was \"%s\"\n",value);
		*codeposition=*codeposition+1;
		return true;
	}

	// if it didn't match, don't advance the code position
	return false;
}

bool codetree::parseBreak(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {

	// get the attributes of this terminal
	const char	*value=grammarnode->getAttributeValue("v");
	size_t		valuelength=charstring::length(value);
	bool		caseinsensitive=!charstring::compare(
					grammarnode->getAttributeValue("c"),
					"false");

	// see if the code matches this terminal, return true or false but
	// don't advance the code position or append the terminal to the
	// nonterminal
	bool	match=false;
	if (caseinsensitive) {
		match=!charstring::compareIgnoringCase(
					value,*codeposition,valuelength);
	} else {
		match=!charstring::compare(value,*codeposition,valuelength);
	}

	if (match) {
		pvt->_break=true;
		debugPrintIndent(4);
		debugPrintf(4,"break \"");
		debugSafePrint(4,value);
		debugPrintf(4,"\" found\n");
		return true;
	}
	return false;
}

bool codetree::parseNonTerminal(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {
	return parseNonTerminal(grammarnode->getAttributeValue("n"),
						treeparent,codeposition,
						ntbuffer);
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
		pvt->_grammar.getRootNode()->getFirstTagChild("g");
	if (pvt->_grammartag->isNullNode()) {
		return false;
	}

	// re-init indent
	pvt->_indentlevel=0;
	pvt->_indentstring=pvt->_grammartag->getAttributeValue("i");
	if (!pvt->_indentstring) {
		pvt->_indentstring="\t";
	}

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
	xmldomnode	*def=pvt->_grammartag->getFirstTagChild("d",
							"n",node->getName());
	if (def->isNullNode()) {
		return false;
	}

	// see if this is a block or line
	const char	*symboltype=getSymbolType(def);
	bool		block=!charstring::compare(symboltype,"block");
	bool		line=!charstring::compare(symboltype,"line");

	// write the start
	const char	*start=def->getAttributeValue("s");
	if (start && start[0]=='\n') {
		output->append('\n');
		start++;
	}
	if ((block && charstring::length(start)) || line) {
		for (uint32_t i=0; i<pvt->_indentlevel; i++) {
			output->append(pvt->_indentstring);
		}
	}
	output->append(start);
	if (block && charstring::length(start)) {
		output->append('\n');
	}

	// if the node has a value, just write that,
	// otherwise write its children
	const char	*value=node->getAttributeValue("value");
	if (charstring::length(value)) {
		output->append(value);
	} else {

		// increase pvt->_indentlevel
		if (block) {
			pvt->_indentlevel++;
		}

		// write the children
		for (xmldomnode *child=node->getFirstTagChild();
				!child->isNullNode();
				child=child->getNextTagSibling()) {
			if (!writeNode(child,output)) {
				return false;
			}
		}

		// decrease indentlevel
		if (block) {
			pvt->_indentlevel--;
		}
	}

	// write the end
	const char	*end=def->getAttributeValue("e");
	bool		backspace=(end && end[0]=='\b' &&
					charstring::length(end)>1);
	if (backspace) {
		output->setPosition(output->getStringLength()-1);
		end++;
	}
	if (block && charstring::length(end)) {
		if (output->getString()[output->getStringLength()-1]!='\n') {
			output->append('\n');
		}
		for (uint32_t i=0; i<pvt->_indentlevel; i++) {
			output->append(pvt->_indentstring);
		}
	}
	if (backspace) {
		output->write(end);
	} else {
		output->append(end);
	}
	if ((block || line) &&
		output->getString()[output->getStringLength()-1]!='\n') {
		output->append('\n');
	}
	return true;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
