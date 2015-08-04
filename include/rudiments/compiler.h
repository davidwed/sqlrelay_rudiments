// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_COMPILER_H
#define RUDIMENTS_COMPILER_H

#include <rudiments/private/compilerincludes.h>

class compilerprivate;
class compilermodule;
class compilerpreprocessor;
class compilerprocessor;
class compilerpostprocessor;

/** The compiler class provides a generic compiler framework.
 *
 *  Though targeted at code-to-code translation (transcompiling), it could
 *  be used as the basis for any type of translation or compilation system.
 *
 *  Translation/compilation is achieved in three steps:
 *
 *  * Pre-processing
 *  * Processing
 *  * Post-processing
 *
 *  In the pre-processing step, a unit of code and optional metadata is passed
 *  through a chain of pre-processing modules.  The output of each module is
 *  fed to the next as input.
 *
 *  In the processing step, the output of the final preprocessing module is
 *  parsed by an instance of the codetree class, using the provided input
 *  grammar, producing an xmldom tree which represents the code.  The tree and
 *  optional metadata is then passed through a chain of processing modules which
 *  convert the tree from representing one type of code to a tree representing
 *  another type of code.  The output of the final processing module is
 *  converted back to code, using the provided output grammar.
 *
 *  In the post-processing step, the output of the processing step is passed
 *  through a chain of post-processing modules.  The output of each module is
 *  fed to the next as input.  Ultimately a unit of compiled code is produced.
 *
 *
 *  Pre-processing modules should inherit from the compilerpreprocessor class.
 *  Processing modules should inherit from the compilerprocessor class.
 *  Post-processing modules should inherit from the compilerpostprocessor class.
 *  All modules must implement the process() method.
 *
 *
 *  The pre and post-processing modules directly manipulate code and might
 *  make heavy use of the file, charstring/bytestring and
 *  stringbuffer/bytebuffer classes.  The processing modules manipulate an
 *  xmldom tree representing code and would likely make heavy use of the
 *  xmldomnode class.
 *  
 *
 *  The input and output code may be text or binary.  The framework is geared
 *  toward text processing, but could be used to process binary data as well.
 *
 *
 *  See the codetree class for information on the grammars used to define the
 *  input and output formats.
 */

class RUDIMENTS_DLLSPEC compiler {
	public:

		/** Creates a new instance of the compiler class. */
		compiler();

		/** Deletes this instance of the compiler class. */
		virtual ~compiler();


		/** Appends "path" to the list of paths that will be searched
		 *  for pre-processor, processor and post-processor modules. */
		void	appendModulePath(const char *path);

		/** Removes all currently loaded module paths. */
		void	clearModulePaths();

		/** Returns the list of loaded module paths. */
		linkedlist< char * >	*getModulePaths();


		/** Appends preprocessor "module" to the list of
		 *  preprocessors. */
		void	appendPreProcessor(compilerpreprocessor *module);

		/** Dynamically loads preprocessor module "filename".
		 *  Calls function new_"classname" to create an instance of
		 *  the preprocessor class.
		 *  Sets its "id" and "parameters".
		 *  Appends the instance to the list of preprocessors.
		 *  Returns true on success and false on failure. */
		bool	appendPreProcessor(const char *filename,
						const char *classname,
						const char *id,
						xmldomnode *parameters);

		/** Dynamically loads preprocessor module "filename".
		 *  Calls function new_"classname" to create an instance of
		 *  the preprocessor class.
		 *  Sets its "id" and "parameters".
		 *  Appends the instance to the list of preprocessors.
		 *  Returns true on success and false on failure. */
		bool	appendPreProcessor(const char *filename,
						const char *classname,
						const char *id,
						const char *parameters);

		/** Removes all currently loaded preprocessor modules. */
		void	clearPreProcessors();

		/** Returns the list of loaded preprocessor modules. */
		linkedlist< compilermodule * >	*getPreProcessors();



		/** Appends processor "module" to the list of
		 *  processors. */
		void	appendProcessor(compilerprocessor *module);

		/** Dynamically loads processor module "filename".
		 *  Calls function new_"classname" to create an instance of
		 *  the processor class.
		 *  Sets its "id" and "parameters".
		 *  Appends the instance to the list of processors.
		 *  Returns true on success and false on failure. */
		bool	appendProcessor(const char *filename,
						const char *classname,
						const char *id,
						xmldomnode *parameters);

		/** Dynamically loads processor module "filename".
		 *  Calls function new_"classname" to create an instance of
		 *  the processor class.
		 *  Sets its "id" and "parameters".
		 *  Appends the instance to the list of processors.
		 *  Returns true on success and false on failure. */
		bool	appendProcessor(const char *filename,
						const char *classname,
						const char *id,
						const char *parameters);

		/** Removes all currently loaded processor modules. */
		void	clearProcessors();

		/** Returns the list of loaded processor modules. */
		linkedlist< compilermodule * >	*getProcessors();



		/** Appends postprocessor "module" to the list of
		 *  postprocessors. */
		void	appendPostProcessor(compilerpostprocessor *module);

		/** Dynamically loads postprocessor module "filename".
		 *  Calls function new_"classname" to create an instance of
		 *  the postprocessor class.
		 *  Sets its "id" and "parameters".
		 *  Appends the instance to the list of postprocessors.
		 *  Returns true on success and false on failure. */
		bool	appendPostProcessor(const char *filename,
						const char *classname,
						const char *id,
						xmldomnode *parameters);

		/** Dynamically loads postprocessor module "filename".
		 *  Calls function new_"classname" to create an instance of
		 *  the postprocessor class.
		 *  Sets its "id" and "parameters".
		 *  Appends the instance to the list of postprocessors.
		 *  Returns true on success and false on failure. */
		bool	appendPostProcessor(const char *filename,
						const char *classname,
						const char *id,
						const char *parameters);

		/** Removes all currently loaded postprocessor modules. */
		void	clearPostProcessors();

		/** Returns the list of loaded postprocessor modules. */
		linkedlist< compilermodule * >	*getPostProcessors();



		/** Sets the input grammar and start symbol.
		 *  Returns true on success and false if "grammar" is not a
		 *  valid grammar. */
		bool	setInputGrammar(const char *grammar,
					const char *startsymbol);

		/** Sets the output grammar.  Returns true on success and
		 *  false if "grammar" is not a valid grammar. */
		bool	setOutputGrammar(const char *grammar);



		/** Sets the compilation metadata.
		 *
		 *  The compilation metadata will be passed into every module
		 *  and may be examined and/or altered by any of them.
		 *
		 *  The metadata may contain any information the user wishes to
		 *  provide to or collect from the modules.
		 *
		 *  For example, during input, it may contain the names and
		 *  locations of source code files, headers or other support
		 *  files, search paths, database access information, etc.
		 *
		 *  When the compilation is complete, it may contain the
		 *  locations of files that were created by the process. */
		void	setMetaData(xmldomnode *metadata);


		/** Passes "input" through the preprocessors, input grammar,
		 *  processors, output grammar, and postprocessors.
		 *
		 *  If "output" is specified then the output is written to
		 *  that buffer.
		 *
		 *  Neither "input" nor "output" need necessarily be supplied.
		 *
		 *  Alternatively, preprocessing modules could load a file or
		 *  files, the locations of which are known or supplied in the
		 *  metadata, and the postprocessing modules could write output
		 *  to a file or files, the locations of which may be known by
		 *  the calling process or returned in the metadata.
		 *
		 *  Returns true on success and false on failure.  */
		bool	compile(const char *input, stringbuffer *output);


		/** Passes already-parsed tree "root" through the set of loaded
		 *  processor modules.
		 *
		 *  No pre-processing, parsing, post-processing, or writing
		 *  back out is done.
		 *
		 *  This is useful for nesting sub-compilers inside of
		 *  processor modules but could be used for other purposes
		 *  as well.
		 *
		 *  Returns true on success and false on failure.  */
		bool	process(xmldomnode *root);


		/** Returns the error that occurred if compile() fails. */
		const char	*getError();


		/** Sets the debug level.  Debug is written to standard out. */
		void	setDebugLevel(uint8_t debuglevel);

	#include <rudiments/private/compiler.h>
};



/** The compilermodule class is a the base class for compilerpreprocessor,
 *  compilreprocessor and compilerpostprocessor modules.  It just stores the
 *  module id and parameters. */
class RUDIMENTS_DLLSPEC compilermodule {
	public:
		/** Creates a new instance of the compilermodule class. */
		compilermodule();

		/** Deletes this instance of the compilermodule class. */
		virtual	~compilermodule();

		/** Sets the id of this instance. */
		void	setId(const char *id);

		/** Sets parameters for this instance. */
		bool	setParameters(const char *parameters);

		/** Sets parameters for this instance. */
		void	setParameters(xmldomnode *parameters);

		/** Returns the id of this instance. */
		const char	*getId();

		/** Returns the parameters of this instance. */
		xmldomnode	*getParameters();

	protected:
		const char	*id;
		xmldomnode	*parameters;

	private:
		xmldom		*xmld;
};


/** The compilerpreprocessor class is the base class for compiler modules
 *  that process a unit of code in its native format, before it is parsed into
 *  a tree using a grammar describing that format.
 *
 *  Preprocessor modules might normalize whitespace, integrate header files,
 *  expand macros, or do simple translations that would be more costly to
 *  do as operations on a tree. */
class RUDIMENTS_DLLSPEC compilerpreprocessor : public compilermodule {
	public:
		/** Creates a new instance of the compilerpreprocessor class. */
		compilerpreprocessor();

		/** Deletes this instance of the compilerpreprocessor class. */
		virtual	~compilerpreprocessor();

		/** Processes "input", writing the result to "output",
		 *  optionally referring to and/or updating "metadata"
		 *  during the process.
		 *  Returns true on success and false on failure. */
		virtual bool	process(const char *input,
					stringbuffer *output,
					xmldomnode *metadata)=0;
};


/** The compilerprocessor class is the base class for compiler modules
 *  that process the tree that was built by parsing a unit of code based
 *  on a grammar describing its native format.
 *
 *  Processor modules take a tree that represents code in one language or
 *  format and alter the tree until it represents code in a different language
 *  or format. */
class RUDIMENTS_DLLSPEC compilerprocessor : public compilermodule {
	public:
		/** Creates a new instance of the compilerprocessor class. */
		compilerprocessor();

		/** Deletes this instance of the compilerprocessor class. */
		virtual	~compilerprocessor();

		/** Processes "tree", writing the result to "output",
		 *  optionally referring to and/or updating "metadata"
		 *  during the process.
		 *  Returns true on success and false on failure. */
		virtual bool	process(xmldomnode *tree,
					xmldomnode *metadata)=0;
};


/** The compilerpostprocessor class is the base class for compiler modules
 *  that process a unit of code in its native format, after the tree
 *  representing it has been written back using a grammar that describes that
 *  format.
 *
 *  Postprocessor modules might apply formatting rules for readability, split
 *  code up into multiple files, or do simple translations that would be more
 *  costly to do as operations on a tree. */
class RUDIMENTS_DLLSPEC compilerpostprocessor : public compilermodule {
	public:
		/** Creates a new instance of the compilerpostprocessor
		 *  class. */
		compilerpostprocessor();

		/** Deletes this instance of the compilerpostprocessor class. */
		virtual	~compilerpostprocessor();

		/** Processes "input", writing the result to "output",
		 *  optionally referring to and/or updating "metadata"
		 *  during the process.
		 *  Returns true on success and false on failure. */
		virtual bool	process(const char *input,
					stringbuffer *output,
					xmldomnode *metadata)=0;
};

#endif
