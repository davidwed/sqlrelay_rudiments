#include <rudiments/codetree.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/stdio.h>

// for fflush, stdout
#include <stdio.h>

// some debug macros
#define debugPrintIndent(level) if (pvt->_debuglevel>=level) { for (uint32_t i=0; i<pvt->_indentlevel; i++) { stdoutput.printf(" "); } }
#ifdef _MSC_VER
	#define debugPrintf(level,ARGS,...) if (pvt->_debuglevel>=level) { stdoutput.printf(ARGS,__VA_ARGS__); fflush(stdout); }
#else
	#define debugPrintf(level,ARGS...) if (pvt->_debuglevel>=level) { stdoutput.printf(ARGS); fflush(stdout); }
#endif
#define debugSafePrint(level,string) if (pvt->_debuglevel>=level) { stdoutput.safePrint(string); }
#define debugSafePrintLength(level,string,length) if (pvt->_debuglevel>=level) { stdoutput.safePrint(string,length); }


// define some constants to make the code more readable later
static const char	CONCATENATION='c';
static const char	ALTERNATION='a';
static const char	OPTION='o';
static const char	REPETITION='r';
static const char	TERMINAL='t';
static const char	LETTER='l';
static const char	LOWERCASELETTER='w';
static const char	UPPERCASELETTER='u';
static const char	DIGIT='i';
static const char	SET='s';
static const char	NONTERMINAL='n';
static const char	BREAK='b';
static const char	EXCEPTION='e';

static const char	*GRAMMAR="g";
static const char	*INDENT="i";

static const char	*DEFINITION="d";
static const char	*NAME="n";
static const char	*TYPE="t";
static const char	*START="s";
static const char	*END="e";

static const char	*VALUE="v";
static const char	*CASE="c";

static const char	INLINE='i';
static const char	LITERAL='l';
static const char	LINE='e';
static const char	BLOCK='b';
static const char	NONE='n';

static const char	STX=0x2;


// create a child class of xmldom that disables the string cache and replaces
// tag and attribute names with single-letter representations of them to
// improve performance
class grammar : public xmldom {
	public:
			grammar();
	protected:
		bool	tagStart(const char *name);
		bool	attributeName(const char *name);
		bool	attributeValue(const char *value);
	private:
		char	previousattribute;
};

grammar::grammar() : xmldom(false) {
	previousattribute='\0';
}

bool grammar::tagStart(const char *name) {

	// get the first letter of the name
	char	ch=name[0];

	// the first letters of these collide with other tags and require
	// special handling...
	// lowercaseletter collides with letter
	// digit collides with definition
	if (!charstring::compare(name,"lowercaseletter")) {
		ch=LOWERCASELETTER;
	} else if (!charstring::compare(name,"digit")) {
		ch=DIGIT;
	}

	// replace the name
        char	newname[2];
	newname[0]=ch;
	newname[1]='\0';
	return xmldom::tagStart(newname);
}

bool grammar::attributeName(const char *name) {
	previousattribute=name[0];
        char	newname[2];
	newname[0]=name[0];
	newname[1]='\0';
	return xmldom::attributeName(newname);
}

bool grammar::attributeValue(const char *value) {

	// only modify values for the type attribute
	if (previousattribute!=TYPE[0]) {
		return xmldom::attributeValue(value);
	}

	// the first letters of this one collides with other values and requires
	// special handling...
	// line collides with literal
	char	ch=value[0];
	if (!charstring::compare(value,"line")) {
		ch=LINE;
	}

	// replace the value
        char	newvalue[2];
	newvalue[0]=ch;
	newvalue[1]='\0';
	return xmldom::attributeValue(newvalue);
}


// private members of the codetree class
class codetreeprivate {
	friend class codetree;
	private:
		grammar		_grammar;
		xmldomnode	*_grammartag;
		bool		_error;
		uint32_t	_indentlevel;
		const char	*_indentstring;
		size_t		_indentlength;
		bool		_previousparsechildretval;
		bool		_break;
		const char	*_beginningofinput;
		const char	*_finalcodeposition;
		uint8_t		_debuglevel;
		stringbuffer	_excbuffer;
		xmldomnode	*_excnode;
		bool		_endofstring;
};


// ok, now finally the class itself...
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
	pvt->_excnode=NULL;
	pvt->_endofstring=false;
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

	// load the input grammar
	if (!pvt->_grammar.parseString(grammar)) {
		return false;
	}
	pvt->_grammartag=pvt->_grammar.getRootNode()->getFirstTagChild(GRAMMAR);
	if (pvt->_grammartag->isNullNode()) {
		return false;
	}

	// build non-terminal node associations
	// (see method for explanation)
	pvt->_grammartag->setData((void *)
		pvt->_grammartag->getFirstTagChild(DEFINITION,NAME,
							startsymbol));
	buildNonTerminalNodeAssociations(pvt->_grammartag);

	// initialize a node for processing exceptions
	pvt->_excnode=new xmldomnode(output->getTree(),
					output->getNullNode(),
					TAG_XMLDOMNODETYPE,
					"excnode",NULL);

	// re-init the error
	pvt->_error=false;

	// re-init endofstring flag
	pvt->_endofstring=false;

	// parse, starting with the specified start symbol
	const char	*codepos=input;
	pvt->_beginningofinput=input;
	pvt->_finalcodeposition=*codeposition;

	bool	retval=(parseNonTerminal(pvt->_grammartag,
					output,&codepos,NULL) && !pvt->_error);
	if (codeposition) {
		*codeposition=pvt->_finalcodeposition;
	}

	// clean up
	delete pvt->_excnode;
	pvt->_excnode=NULL;

	return retval;
}

void codetree::buildNonTerminalNodeAssociations(xmldomnode *node) {

	// For each <nonterminal> tag, this method attaches a pointer to the
	// corresponding <definition> tag.  That way, when the parser encounters
	// a nonterminal and needs to look up the definition it can do it
	// directly rather than having to search the tree.  This yields a
	// significant performance improvement at the cost of only a single
	// pass through the tree to assign the pointers.

	// if it's a nonterminal node
	if (node->getName()[0]=='n') {

		// find the associated definition and attach it to this node
		node->setData((void *)
			pvt->_grammartag->getFirstTagChild(DEFINITION,NAME,
						node->getAttributeValue(NAME)));
	}

	// process children
	for (xmldomnode *child=node->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		buildNonTerminalNodeAssociations(child);
	}
}

const char codetree::getSymbolType(xmldomnode *nt) {
	const char	*symboltype=nt->getAttributeValue(TYPE);
	if (!charstring::length(symboltype)) {
		return INLINE;
	}
	return symboltype[0];
}

bool codetree::parseChild(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {

	// handle end-of-string
	if (**codeposition=='\0') {
		debugPrintIndent(2);
		debugPrintf(2,"end of string\n");
		pvt->_endofstring=true;
		return true;
	}

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
	if (name) {
		switch (name[0]) {
			case CONCATENATION:
				if (!parseConcatenation(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case ALTERNATION:
				if (!parseAlternation(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case OPTION:
				if (!parseOption(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case REPETITION:
				if (!parseRepetition(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case TERMINAL:
				if (!parseTerminal(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case LETTER:
				if (!parseLetter(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case LOWERCASELETTER:
				if (!parseLowerCaseLetter(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case UPPERCASELETTER:
				if (!parseUpperCaseLetter(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case DIGIT:
				if (!parseDigit(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case SET:
				if (!parseSet(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case BREAK:
				if (!parseBreak(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case NONTERMINAL:
				if (!parseNonTerminal(grammarnode,
							treeparent,
							codeposition,
							localntbuffer)) {
					retval=false;
				}
				break;
			case EXCEPTION:
				// ignore exceptions if we
				// encounter them directly
				retval=pvt->_previousparsechildretval;
				break;
			default:
				retval=false;
		}
	} else {
		retval=false;
	}

	// if parsing the child succeeded, process
	// any exception that might follow
	if (retval) {

		// if the next node is an exception...
		xmldomnode	*sibling=grammarnode->getNextTagSibling();
		name=sibling->getName();
		if (!sibling->isNullNode() && name && name[0]=='e') {

			// create some local buffers
			pvt->_excbuffer.clear();
			pvt->_excnode->deleteChildren();

			// reparse the same code we just parsed
			const char	*exccodeposition=startcodeposition;

			// parse the exception...
			//
			// If parseException succeeds and stops at the same
			// place we stopped parsing the last chunk, then the
			// last chunk must be equal to one of the exceptions in
			// the list.  There's no need to compare to be sure.
			retval=!(parseException(sibling,pvt->_excnode,
					&exccodeposition,&pvt->_excbuffer) &&
					exccodeposition==*codeposition);
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

	// keep track of the start position
	const char	*startcodeposition=*codeposition;

	// save old break state and reset break flag
	bool	oldbreak=pvt->_break;
	pvt->_break=false;

	// one of the children must parse successfully
	bool	retval=false;
	for (xmldomnode *child=grammarnode->getFirstTagChild();
		!child->isNullNode(); child=child->getNextTagSibling()) {
		if (parseChild(child,treeparent,codeposition,ntbuffer)) {
			retval=true;
			break;
		}
	}

	// process a break and restore break state...
	// if a break was encountered and it resulted in
	// nothing being read then consider that a failure
	if (pvt->_break && *codeposition==startcodeposition) {
		retval=false;
	}
	pvt->_break=oldbreak;

	if (retval) {
		debugPrintIndent(4);
		debugPrintf(4,"} alternation success\n");
	} else {
		debugPrintIndent(4);
		debugPrintf(4,"} alternation failed\n");
	}
	return retval;
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
		//bool	oldbreak=pvt->_break;
		//pvt->_break=false;

		// parse the child
		bool	parseresult=parseChild(child,treeparent,
						codeposition,ntbuffer);

		// process a break and restore break state
		//if (pvt->_break) {
			//parseresult=false;
			//anyfound=true;
		//}
		//pvt->_break=oldbreak;

		if (!parseresult || pvt->_endofstring) {
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
	const char	*value=grammarnode->getAttributeValue(VALUE);
	size_t		valuelength=charstring::length(value);
	const char	*casesensitive=grammarnode->getAttributeValue(CASE);

	// first, check for beginning-of-line characters,
	// which should only occur at the beginning of a value
	// (STX is used to represent the beginning of a line)
	if (value && value[0]==STX) {
		if (*codeposition==pvt->_beginningofinput ||
					*(*codeposition-1)=='\n') {
			value++;
			valuelength--;
		} else {
			return false;
		}
	}

	// see if the code matches this terminal
	bool	match=(casesensitive && casesensitive[0]=='f')?
			!charstring::compareIgnoringCase(
					value,*codeposition,valuelength):
			!charstring::compare(
					value,*codeposition,valuelength);

	// if it matches, append the terminal to the
	// nonterminal and advance the code position
	if (match) {
		debugPrintIndent(4);
		debugPrintf(4,"terminal \"");
		debugSafePrint(4,value);
		debugPrintf(4,"\" found\n");
		if (ntbuffer) {
			ntbuffer->append(*codeposition,valuelength);
		}
		*codeposition=*codeposition+valuelength;
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
		debugPrintIndent(4);
		debugPrintf(4,"letter found: \"%c\"\n",**codeposition);
		if (ntbuffer) {
			ntbuffer->append(**codeposition);
		}
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
		debugPrintIndent(4);
		debugPrintf(4,"lower case letter found: \"%c\"\n",
							**codeposition);
		if (ntbuffer) {
			ntbuffer->append(**codeposition);
		}
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
		debugPrintIndent(4);
		debugPrintf(4,"upper case letter found: \"%c\"\n",
							**codeposition);
		if (ntbuffer) {
			ntbuffer->append(**codeposition);
		}
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
		debugPrintIndent(4);
		debugPrintf(4,"digit found: \"%c\"\n",**codeposition);
		if (ntbuffer) {
			ntbuffer->append(**codeposition);
		}
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
	const char	*value=grammarnode->getAttributeValue(VALUE);

	// if it matches, append the terminal to the
	// nonterminal and advance the code position
	if (character::inSet((int32_t)(**codeposition),value)) {
		debugPrintIndent(4);
		debugPrintf(4,"set member found: \"%c\" from set \"%s\"\n",
							**codeposition,value);
		if (ntbuffer) {
			ntbuffer->append(**codeposition);
		}
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
	const char	*value=grammarnode->getAttributeValue(VALUE);
	size_t		valuelength=charstring::length(value);
	const char	*casesensitive=grammarnode->getAttributeValue(CASE);

	// first, check for beginning-of-line characters,
	// which should only occur at the beginning of a value
	// (STX is used to represent the beginning of a line)
	if (value && value[0]==STX) {
		if (*codeposition==pvt->_beginningofinput ||
					*(*codeposition-1)=='\n') {
			value++;
			valuelength--;
		} else {
			return false;
		}
	}

	// see if the code matches this terminal
	bool	match=(casesensitive && casesensitive[0]=='f')?
			!charstring::compareIgnoringCase(
					value,*codeposition,valuelength):
			!charstring::compare(
					value,*codeposition,valuelength);

	// return true or false but don't advance the code position or append
	// the value to the nonterminal
	if (match) {
		debugPrintIndent(2);
		debugPrintf(2,"break \"");
		debugSafePrint(2,value);
		debugPrintf(2,"\" found\n");
		pvt->_break=true;
		return true;
	}
	return false;
}

bool codetree::parseNonTerminal(xmldomnode *grammarnode,
				xmldomnode *treeparent,
				const char **codeposition,
				stringbuffer *ntbuffer) {

	// find the definition
	xmldomnode	*def=(xmldomnode *)grammarnode->getData();
	if (!def || def->isNullNode()) {
		debugPrintIndent(1);
		debugPrintf(1,"ERROR: nonterminal %s not found\n",
					grammarnode->getAttributeValue(NAME));
		return false;
	}

	// get the name
	const char	*name=def->getAttributeValue(NAME);

	// some variables...
	xmldomnode	*codenode=NULL;
	stringbuffer	*localntbuffer=NULL;

	// If a nonterminal buffer was passed in then we're apparently already
	// building up another nonterminal that stores a literal value.  In
	// that case we don't need to do anything but pass the current code
	// parent and nonterminal buffer forward.
	const char	symboltype=getSymbolType(def);
	if (!ntbuffer) {

		// If no nonterminal buffer was passed in...

		// If this nonterminal has a type then we need to create a new
		// node for it that will be appended to the tree if we do,
		// indeed, find an instance of this nonterminal.  If it does
		// not have a type then we just need to pass the current code
		// parent forward.  If it has a type of literal, then we
		// need to create a buffer to build up the value in and pass it
		// forward too...
		if (symboltype!=NONE) {
			codenode=new xmldomnode(treeparent->getTree(),
						treeparent->getNullNode(),
						TAG_XMLDOMNODETYPE,
						name,NULL);
		}
		if (symboltype==LITERAL) {
			localntbuffer=new stringbuffer;
		}
	}

	debugPrintIndent(2);
	debugPrintf(2,"nonterminal (%c) \"%s\" at \"",symboltype,name);
        debugSafePrintLength(2,*codeposition,
			(charstring::length(*codeposition)>20)?
				20:charstring::length(*codeposition));
	debugPrintf(2,"\"... {\n");

	// keep track of the current position in the code, if we don't find
	// this nonterminal then we'll need to reset the position in the code
	// to this
	const char	*startcodeposition=*codeposition;

	// there should be only one child
	if (parseChild(def->getFirstTagChild(),
			(codenode)?codenode:treeparent,
			codeposition,
			(ntbuffer)?ntbuffer:localntbuffer)) {


		debugPrintIndent(2);
		debugPrintf(2,"} nonterminal \"%s\" found",name);

		// we found one of these, attach the node to the
		// code and value to the node, if necessary
		if (codenode) {
			treeparent->appendChild(codenode);
		}
		if (localntbuffer) {
			debugPrintf(2," - value: \"%s\"",
					localntbuffer->getString());
			codenode->setAttributeValue("value",
					localntbuffer->getString());
			delete localntbuffer;
		}

		debugPrintf(2,"\n");

		// advance the new code position
		return true;
	}

	debugPrintIndent(2);
	debugPrintf(2,"} nonterminal \"%s\" not found at \"",name);
        debugSafePrintLength(2,startcodeposition,
			(charstring::length(startcodeposition)>20)?
				20:charstring::length(startcodeposition));
	debugPrintf(2,"\"\n");

	// apparently we didn't find one of these, delete the
	// node and buffer and reset the position in the code
	delete codenode;
	delete localntbuffer;
	*codeposition=startcodeposition;
	return false;
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
	pvt->_grammartag=pvt->_grammar.getRootNode()->getFirstTagChild(GRAMMAR);
	if (pvt->_grammartag->isNullNode()) {
		return false;
	}

	// re-init indent
	pvt->_indentlevel=0;
	pvt->_indentstring=pvt->_grammartag->getAttributeValue(INDENT);
	if (!pvt->_indentstring) {
		pvt->_indentstring="\t";
	}
	pvt->_indentlength=charstring::length(pvt->_indentstring);

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
	xmldomnode	*def=pvt->_grammartag->getFirstTagChild(
							DEFINITION,NAME,
							node->getName());
	if (def->isNullNode()) {
		debugPrintIndent(1);
		debugPrintf(1,"ERROR: definition %s not found\n",
						node->getName());
		return false;
	}

	// see if this is a block or line
	const char	symboltype=getSymbolType(def);
	bool		block=(symboltype==BLOCK);
	bool		line=(symboltype==LINE);

	// write the start
	const char	*start=def->getAttributeValue(START);
	if (line || (block && charstring::length(start))) {
		indent(output);
	}
	writeStartEnd(output,start);
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
	const char	*end=def->getAttributeValue(END);
	if (block && charstring::length(end)) {
		if (output->getString()[output->getStringLength()-1]!='\n') {
			output->append('\n');
		}
		indent(output);
	}
	writeStartEnd(output,end);
	if (line || (block && charstring::length(end))) {
		output->append('\n');
	}
	return true;
}

void codetree::indent(stringbuffer *output) {
	for (uint32_t i=0; i<pvt->_indentlevel; i++) {
		output->append(pvt->_indentstring);
	}
}

void codetree::writeStartEnd(stringbuffer *output, const char *string) {

	if (!string) {
		return;
	}

	// save indent level
	uint32_t	startindentlevel=pvt->_indentlevel;

	// if it started with a backspace then either remove the preceeding
	// carriage-return/line-feed or back up one level of indention
	if (string[0]=='\b') {

		// make sure not to attempt to indent less than 0
		if (pvt->_indentlevel) {
			pvt->_indentlevel--;
		}

		const char	*outstr=output->getString();
		size_t		outpos=output->getPosition()-1;

		if (outstr[outpos]=='\n' || outstr[outpos]=='\r') { 
			output->truncate(outpos);
		} else {
			if (!charstring::compare(
				outstr+output->getPosition()-pvt->_indentlength,
				pvt->_indentstring)) {
				output->truncate(outpos-pvt->_indentlength+1);
			}
		}

		string++;
	}

	// Print the start/end string. When \n's are encountered,
	// bump down and indent
	for (const char *c=string; *c; c++) {
		output->append(*c);
		if (*c=='\n' && *(c+1)!='\0') {
			indent(output);
		}
	}

	// restore indent level
	pvt->_indentlevel=startindentlevel;
}
