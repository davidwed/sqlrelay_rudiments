// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CODETREE_H
#define RUDIMENTS_CODETREE_H

#include <rudiments/private/codetreeincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class codetreeprivate;

/** The codetree class implements a generic parser that can parse source code
 *  (or similarly structured input) and create an XML tree representing the
 *  code.  The class can also take that XML tree and write it back out as code.
 *
 *  A grammar must be specified to parse the input and write it back out.  This
 *  grammar must be in a specific XML format, based on the EBNF format with a
 *  few extensions.
 *
 *  The top-level tag is <grammar>.  Inside the grammar tag are sets of
 *  <definition> tags, each defining a nonterminal.  Each <definition> tag must
 *  contain sets of <terminal>, <nonterminal>, <alternation>, <repetition>,
 *  <concatenation>, <option> and <exception> tags.  All tags except <terminal>
 *  and <nonterminal> must contain combinations of other tags.
 *
 *  The <definition> tag defines a nonterminal and has the following attributes:
 *
 *    name - Required.  The name of the nonterminal.  This will also be the tag
 *           name used to represent the nonterminal in the XML representation
 *           of the code.
 *
 *    start - Optional.  A string to be printed before the nonterminal when
 *            writing it back out.
 *
 *    end - Optional.  A string to be printed after the nonterminal when
 *          writing it back out.
 *
 *    token - Optional.  If supplied, it indicates that the nonterminal is a
 *            "token".  A tag will represent it in the XML representation of
 *            the code and special formatting will be used when writing the XML
 *            back out as code.  There are 4 possible values:
 *            block, line, inline and literal.
 *
 *            When written back out, code inside of a block token is indented.
 *            In the XML representing the code, the tag representing a block
 *            token may contain tags for other tokens.  A good example of a
 *            block token in C++ is a code block, surrounded by a pair of curly
 *            braces.
 *
 *            When written back out, a carriage return is appended to line
 *            tokens.  In the XML representing the code, the tag representing a
 *            block token may contain tags for other tokens.  A good example of
 *            a line token in C++ is a statement, terminated by a semicolon.
 *
 *            When written back out, an inline token is simply printed inline
 *            without any additional indentation or carriage returns.  In the
 *            XML representing the code, the tag representing an inline token
 *            may contain tags for other tokens.  A good example of an inline
 *            token in C++ (or any programming language) is an arithmetic
 *            expression which may contain many terms and operators.
 *
 *            Literal tokens are also printed inline, however in the XML
 *            representing the code, the tag representing a literal token does
 *            not contain any additional tokens but rather has a "value"
 *            attribute, the value of which is composed of the concatenation of
 *            all terminals that were ultimately encountered while parsing the
 *            section of the code described by the nonterminal.  Good examples
 *            of literal tokens in C++ (or any programming language) are
 *            integers, floating point numbers and strings.
 *
 *            If omitted, the nonterminal is not a token and no tag will
 *            appear for it in the XML representing the code, but the
 *            nonterminal may be used to build up other literal tokens.
 *            Literal tokens must (ultimately) be composed of terminals, other
 *            literal tokens or nonterminals with no token type.
 *
 *     Examples:
 *
 *         <definition name="code_block" token="block" start="{" end="}">
 *             ... other tags ...
 *         </definition>
 *
 *         <definition name="statement" token="line" end=";">
 *             ... other tags ...
 *         </definition>
 *
 *         <definition name="expression" token="inline">
 *             ... other tags ...
 *         </definition>
 *
 *         <definition name="integer" token="literal">
 *             ... other tags ...
 *         </definition>
 *
 * 
 * The <terminal> tag refers to a terminal and has the following attributes:
 *
 *     value - The text of the terminal.  Ascii characters or XML entities
 *             can be used.
 *
 *     case - Whether the terminal is case-sensitive or not.  Acceptable values
 *            are true or false.
 *
 *     Examples:
 *
 *         <terminal value="1"/>
 *         <terminal value="A"/>
 *         <terminal value="&#224;"/>
 *         <terminal value="||"/>
 *         <terminal value="&&"/>
 *         <terminal value="class" case="false"/>
 *
 *
 * The <nonterminal> tag refers to a nonterminal defined elsewhere in the
 * grammar and has the following attributes:
 *
 *     name - The name of the nonterminal.
 *
 *     Examples:
 *
 *         <nonterminal name="code_block"/>
 *         <nonterminal name="statement"/>
 *         <nonterminal name="expression"/>
 *         <nonterminal name="integer"/>
 *
 *
 * The <alternation> tag indicates to the parser that one of the enclosed tags
 * must evaluate successfully for parsing to continue.
 *
 * The <repetition> tag indicates to the parser that the enclosed tag may be
 * evaluated zero or more times and that parsing should loop until it parses
 * unsuccessfully.
 *
 * The <concatenation> tag indicates to the parser that all of the enclosed
 * tags must evaluate succussfully, in the specified order, when parsing the
 * code for parsing to continue.
 *
 * The <option> tag indicates to the parser that the enclosed tag may or may
 * not evaluate succussfully and parsing should continue in either case.
 *
 * The <exception> tag defines an exception to the previous tag.
 *
 * These tags may be used together to define nonterminals and ultimately a set
 * of nonterminals may be used to define a grammar.
 *
 *     <grammar>
 *
 *         <definition name="digit">
 *             <alternation>
 *                 <terminal value="0">
 *                 <terminal value="1">
 *                 <terminal value="2">
 *                 <terminal value="3">
 *                 <terminal value="4">
 *                 <terminal value="5">
 *                 <terminal value="6">
 *                 <terminal value="7">
 *                 <terminal value="8">
 *                 <terminal value="9">
 *             </alternation>
 *         </definition>
 *
 *         <definition name="digit_excluding_zero">
 *             <nonterminal name="digit"/>
 *             <exception>
 *                 <terminal value="0"/>
 *             </exception>
 *         </definition>
 *
 *         <definition name="integer" token="literal">
 *             <alternation>
 *                 <terminal value="0"/>
 *                 <concatenation>
 *                     <option>
 *                         <terminal value="-"/>
 *                     </option>
 *                     <concatenation>
 *                         <nonterminal name="digit_excluding_zero"/>
 *                         <repetition>
 *                             <nonterminal name="digit"/>
 *                         </repetition>
 *                     </concatenation>
 *                 </concatenation>
 *             </alternation>
 *         </definition>
 *
 *     </grammar>
 */

class codetree {
	public:

		/** Creates a new instance of the codetree class. */
		codetree();

		/** Deletes this instance of the codetree class. */
		~codetree();

		/** Parses "input" using "grammar", starting with "starttoken",
		 *  creates an XML represenataion of the code and appends it as
		 *  children to "output".  Returns true if parsing succeeded
		 *  and false otherwise.  If parsing fails, "codeposition" is
		 *  set to the location in the code that parsing failed. */
		bool	parse(const char *input,
					const char *grammar,
					const char *starttoken,
					rudiments::xmldomnode *output,
					const char **codeposition);

		/** Interprets the XML representation of the code "input" using
		 *  "grammar" and appends it as code to "output".  Returns true
		 *  if this succeeds and false otherwise. */
		bool	write(rudiments::xmldomnode *input,
					const char *grammar,
					rudiments::stringbuffer *output);

		/** Sets the debug level.  Debug is written to standard out. */
		void	setDebugLevel(uint8_t debuglevel);

	#include <rudiments/private/codetree.h>
};

// @endcond

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
