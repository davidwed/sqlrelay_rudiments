// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <rudiments/compiler.h>
#include <rudiments/character.h>
#include <rudiments/sys.h>


#if (defined(_MSC_VER) && (_MSC_VER <= 1300)) || \
	(defined(__USLC__) && !defined(__GNUC__)) || \
	defined(__VMS)

	// degenerate debug macros for really incapable compilers
	// (old microsoft compilers, unixware native compiler, openvms compiler)
	static void debugPrintf(uint8_t level, const char *format, ...) {}
	static void debugSafePrint(uint8_t level, const char *string) {}
	static void debugSafePrintLength(uint8_t level, const char *string,
							uint32_t length) {}
#else

	// working debug macros for more modern compilers
	#ifdef _MSC_VER
		#define debugPrintf(level,ARGS,...) if (pvt->_debuglevel>=level) { stdoutput.printf(ARGS,__VA_ARGS__); stdoutput.flush(); }
	#else
		#define debugPrintf(level,ARGS...) if (pvt->_debuglevel>=level) { stdoutput.printf(ARGS); stdoutput.flush(); }
	#endif
	#define debugSafePrint(level,string) if (pvt->_debuglevel>=level) { stdoutput.safePrint(string); }
	#define debugSafePrintLength(level,string,length) if (pvt->_debuglevel>=level) { stdoutput.safePrint(string,length); }

#endif


class compilerprivate {
	friend class compiler;
	private:
		linkedlist< char * >		_modulepaths;
		linkedlist< dynamiclib * >	_plugins;
		linkedlist< compilermodule * >	_preprocessors;
		linkedlist< compilermodule * >	_processors;
		linkedlist< compilermodule * >	_postprocessors;

		const char	*_inputgrammar;
		const char	*_startsymbol;
		const char	*_outputgrammar;

		xmldomnode	*_metadata;

		uint8_t		_debuglevel;

		codetree	_ct;

		stringbuffer	_errorstr;
};

compiler::compiler() {
	pvt=new compilerprivate;

	pvt->_inputgrammar=NULL;
	pvt->_startsymbol=NULL;
	pvt->_outputgrammar=NULL;

	pvt->_metadata=NULL;

	pvt->_debuglevel=0;
}

compiler::~compiler() {

	clearModules(&(pvt->_preprocessors));
	clearModules(&(pvt->_processors));
	clearModules(&(pvt->_postprocessors));

	for (linkedlistnode< dynamiclib * >
			*plnode=pvt->_plugins.getFirst();
			plnode; plnode=plnode->getNext()) {
		delete plnode->getValue();
	}

	clearModulePaths();

	delete pvt;
}

void compiler::appendModulePath(const char *path) {
	pvt->_modulepaths.append(charstring::duplicate(path));
}

void compiler::clearModulePaths() {
	for (linkedlistnode< char * >
			*mpnode=pvt->_modulepaths.getFirst();
			mpnode; mpnode=mpnode->getNext()) {
		delete[] mpnode->getValue();
	}
	pvt->_modulepaths.clear();
}

linkedlist< char * >	*compiler::getModulePaths() {
	return &(pvt->_modulepaths);
}

bool compiler::appendModule(const char *filename,
				const char *classname,
				const char *id,
				const char *textparams,
				xmldomnode *xmlparams,
				linkedlist< compilermodule *> *modulelist) {

	pvt->_errorstr.clear();

	// find the module
	stringbuffer	modulefilename;
	if (pvt->_modulepaths.getLength()) {
		for (linkedlistnode< char * >
				*mpnode=pvt->_modulepaths.getFirst();
				mpnode; mpnode=mpnode->getNext()) {

			modulefilename.append(mpnode->getValue());
			modulefilename.append(sys::getDirectorySeparator());
			modulefilename.append(filename);

			if (file::exists(modulefilename.getString())) {
				break;
			}

			modulefilename.clear();
		}
	}
	if (!modulefilename.getStringLength()) {
		modulefilename.append(filename);
	}

	// load the module
	dynamiclib	*dl=new dynamiclib();
	if (!dl->open(modulefilename.getString(),true,true)) {
		char	*error=dl->getError();
		pvt->_errorstr.append(error);
		delete[] error;
		delete dl;
		return false;
	}
	pvt->_plugins.append(dl);

	// create the name of the function that creates the class
	stringbuffer	functionname;
	functionname.append("new_")->append(classname);

	// get the function itself
	compilermodule	*(*newCompilerModule)()=
			(compilermodule *(*)())
				dl->getSymbol(functionname.getString());
	if (!newCompilerModule) {
		char	*error=dl->getError();
		pvt->_errorstr.append(error);
		delete[] error;
		return false;
	}

	// create an instance of the class
	compilermodule	*cm=(*newCompilerModule)();
	if (cm) {

		// set id
		cm->setId(id);

		// set parameters
		if (xmlparams) {
			cm->setParameters(xmlparams);
		} else if (textparams && textparams[0]) {
			if (!cm->setParameters(textparams)) {
				pvt->_errorstr.append("invalid parameters");
				delete cm;
				return false;
			}
		}
	}

	// append the module to the specified list
	modulelist->append(cm);

	return true;;
}

void compiler::clearModules(linkedlist< compilermodule * > *modulelist) {
	for (linkedlistnode< compilermodule * >
			*node=modulelist->getFirst();
			node; node=node->getNext()) {
		delete node->getValue();
	}
	modulelist->clear();
}


void compiler::appendPreProcessor(compilerpreprocessor *module) {
	pvt->_preprocessors.append(module);
}

bool compiler::appendPreProcessor(const char *filename,
						const char *classname,
						const char *id,
						xmldomnode *parameters) {
	return appendModule(filename,classname,id,
				NULL,parameters,&(pvt->_preprocessors));
}

bool compiler::appendPreProcessor(const char *filename,
						const char *classname,
						const char *id,
						const char *parameters) {
	return appendModule(filename,classname,id,
				parameters,NULL,&(pvt->_preprocessors));
}

void compiler::clearPreProcessors() {
	clearModules(&(pvt->_preprocessors));
}

linkedlist< compilermodule * > *compiler::getPreProcessors() {
	return &(pvt->_preprocessors);
}

void compiler::appendProcessor(compilerprocessor *module) {
	pvt->_processors.append(module);
}

bool compiler::appendProcessor(const char *filename,
						const char *classname,
						const char *id,
						xmldomnode *parameters) {
	return appendModule(filename,classname,id,
				NULL,parameters,&(pvt->_processors));
}

bool compiler::appendProcessor(const char *filename,
						const char *classname,
						const char *id,
						const char *parameters) {
	return appendModule(filename,classname,id,
				parameters,NULL,&(pvt->_processors));
}

void compiler::clearProcessors() {
	clearModules(&(pvt->_processors));
}

linkedlist< compilermodule * > *compiler::getProcessors() {
	return &(pvt->_processors);
}

void compiler::appendPostProcessor(compilerpostprocessor *module) {
	pvt->_postprocessors.append(module);
}

bool compiler::appendPostProcessor(const char *filename,
						const char *classname,
						const char *id,
						xmldomnode *parameters) {
	return appendModule(filename,classname,id,
				NULL,parameters,&(pvt->_postprocessors));
}

bool compiler::appendPostProcessor(const char *filename,
						const char *classname,
						const char *id,
						const char *parameters) {
	return appendModule(filename,classname,id,
				parameters,NULL,&(pvt->_postprocessors));
}

void compiler::clearPostProcessors() {
	clearModules(&(pvt->_postprocessors));
}

linkedlist< compilermodule * > *compiler::getPostProcessors() {
	return &(pvt->_postprocessors);
}

bool compiler::setInputGrammar(const char *grammar, const char *startsymbol) {
	pvt->_inputgrammar=grammar;
	pvt->_startsymbol=startsymbol;
	return true;
}

bool compiler::setOutputGrammar(const char *grammar) {
	pvt->_outputgrammar=grammar;
	return true;
}

void compiler::setMetaData(xmldomnode *metadata) {
	pvt->_metadata=metadata;
}

bool compiler::compile(const char *input, stringbuffer *output) {

	pvt->_errorstr.clear();

	// initialize the input pointer to the input
	const char	*inptr=input;

	// we'll bounce between 2 buffers, alternately
	// using one for input and the other for output
	stringbuffer	buffer1;
	stringbuffer	buffer2;
	stringbuffer	*inbufptr=&buffer2;
	stringbuffer	*outbufptr=&buffer1;

	// pre-process...
	for (linkedlistnode< compilermodule *>
				*prenode=pvt->_preprocessors.getFirst();
				prenode; prenode=prenode->getNext()) {

		// swap buffers
		if (inbufptr==&buffer1) {
			inbufptr=&buffer2;
			outbufptr=&buffer1;
		} else {
			inbufptr=&buffer1;
			outbufptr=&buffer2;
		}

		// reset input
		if (inptr!=input) {
			inptr=inbufptr->getString();
		}

		// set final output if necessary
		if (!prenode->getNext() &&
			!pvt->_processors.getFirst() &&
			!pvt->_postprocessors.getFirst()) {
			outbufptr=output;
		}

		// clear the output buffer
		outbufptr->clear();

		// get the preprocessor module
		compilermodule	*mod=prenode->getValue();

		// debug
		debugPrintf(1,"\ninput to pre-processor \"%s\":\n\n"
			"%s\n\n========================================\n\n",
			mod->getId(),inptr);

		// pre-process
		if (!((compilerpreprocessor *)mod)->
				process(inptr,outbufptr,pvt->_metadata)) {
			pvt->_errorstr.append("pre-processor ");
			pvt->_errorstr.append(prenode->getValue()->getId());
			pvt->_errorstr.append(" failed");
			return false;
		}

		// debug
		debugPrintf(1,"\noutput from pre-processor \"%s\":\n\n"
			"%s\n\n========================================\n\n",
			mod->getId(),outbufptr->getString());
	}

	// process...
	if (pvt->_processors.getFirst()) {

		// reset input if necessary
		if (pvt->_preprocessors.getFirst()) {
			inptr=outbufptr->getString();
		}

		// parse into a tree
		xmldom		tree;
		tree.createRootNode();
		xmldomnode	*root=tree.getRootNode();
		const char	*codeposition=NULL;
		if (!pvt->_ct.parse(inptr,
					pvt->_inputgrammar,
					pvt->_startsymbol,
					root,&codeposition)) {
			pvt->_errorstr.append("failed to parse input at:\n");
			pvt->_errorstr.append(codeposition,10);
			return false;
		}

		// process...
		if (!process(root)) {
			return false;
		}

		// set final output if necessary
		if (!pvt->_postprocessors.getFirst()) {
			outbufptr=output;
		}

		// clear the output buffer
		outbufptr->clear();

		// write the tree back out
		if (!pvt->_ct.write(root,pvt->_outputgrammar,outbufptr)) {
			pvt->_errorstr.append("failed to write output");
			return false;
		}
	}
	
	// post-process...
	for (linkedlistnode< compilermodule *>
			*postnode=pvt->_postprocessors.getFirst();
			postnode; postnode=postnode->getNext()) {

		// swap buffers
		if (inbufptr==&buffer1) {
			inbufptr=&buffer2;
			outbufptr=&buffer1;
		} else {
			inbufptr=&buffer1;
			outbufptr=&buffer2;
		}

		// reset input
		inptr=inbufptr->getString();

		// set final output if necessary
		if (!postnode->getNext()) {
			outbufptr=output;
		}

		// clear the output buffer
		outbufptr->clear();

		// get the preprocessor module
		compilermodule	*mod=postnode->getValue();

		// debug
		debugPrintf(1,"\ninput to post-processor \"%s\":\n\n"
			"%s\n\n========================================\n\n",
			mod->getId(),inptr);

		// post-process
		if (!((compilerpostprocessor *)mod)->
				process(inptr,outbufptr,pvt->_metadata)) {
			pvt->_errorstr.append("post-processor ");
			pvt->_errorstr.append(postnode->getValue()->getId());
			pvt->_errorstr.append(" failed");
			return false;
		}

		// debug
		debugPrintf(1,"\noutput from post-processor \"%s\":\n\n"
			"%s\n\n========================================\n\n",
			mod->getId(),outbufptr->getString());
	}

	return true;
}

bool compiler::process(xmldomnode *root) {

	// process...
	for (linkedlistnode< compilermodule *>
				*node=pvt->_processors.getFirst();
				node; node=node->getNext()) {

		// get the processor module
		compilermodule	*mod=node->getValue();

		// debug
		debugPrintf(1,"\ninput to processor \"%s\":\n\n",
							mod->getId());
		if (pvt->_debuglevel>=1) {
			root->print(&stdoutput);
		}
		debugPrintf(1,"\n\n===================="
				"====================\n\n");

		// process
		if (!((compilerprocessor *)mod)->
				process(root,pvt->_metadata)) {
			pvt->_errorstr.append("processor ");
			pvt->_errorstr.append(
					node->getValue()->getId());
			pvt->_errorstr.append(" failed");
			return false;
		}

		// debug
		debugPrintf(1,"\noutput from processor \"%s\":\n\n",
							mod->getId());
		if (pvt->_debuglevel>=1) {
			root->print(&stdoutput);
		}
		debugPrintf(1,"\n\n===================="
				"====================\n\n");
	}

	return true;
}

const char *compiler::getError() {
	return pvt->_errorstr.getString();
}

void compiler::setDebugLevel(uint8_t debuglevel) {
	pvt->_debuglevel=debuglevel;
	pvt->_ct.setDebugLevel(debuglevel);
}



compilermodule::compilermodule() {
	id=NULL;
	parameters=NULL;
	xmld=NULL;
}

compilermodule::~compilermodule() {
	delete xmld;
}

void compilermodule::setId(const char *id) {
	this->id=id;
}

bool compilermodule::setParameters(const char *parameters) {
	if (this->parameters) {
		this->parameters=NULL;
	}
	xmld=new xmldom();
	bool	result=false;
	if (parameters) {
		while (character::isWhitespace(*parameters)) {
			parameters++;
		}
		if (*parameters=='<') {
			result=xmld->parseString(parameters);
		} else {
			result=xmld->parseFile(parameters);
		}
	}
	if (!result) {
		delete xmld;
		xmld=NULL;
		return false;
	}
	this->parameters=xmld->getRootNode()->getFirstTagChild();
	return true;
}

void compilermodule::setParameters(xmldomnode *parameters) {
	if (xmld) {
		delete xmld;
		xmld=NULL;
	}
	this->parameters=parameters;
}

const char *compilermodule::getId() {
	return id;
}

xmldomnode *compilermodule::getParameters() {
	return parameters;
}


compilerpreprocessor::compilerpreprocessor() : compilermodule() {
}

compilerpreprocessor::~compilerpreprocessor() {
}


compilerprocessor::compilerprocessor() : compilermodule() {
}

compilerprocessor::~compilerprocessor() {
}


compilerpostprocessor::compilerpostprocessor() : compilermodule() {
}

compilerpostprocessor::~compilerpostprocessor() {
}
