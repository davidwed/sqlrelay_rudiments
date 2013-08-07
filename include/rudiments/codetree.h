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
 *  The <grammar> tag may contain a single attribute:
 *
 *    indent - Optional.  The string to use when indenting code as it's being
 *             written back out.  The string will be repeated once for each
 *             level of indention.  If omitted, tabs will be used.  Setting the
 *             attribute to "&#9;" will also cause tabs to be used.  Otherwise
 *             whatever string is supplied will be repeated for each level
 *             of indentation.  To use 4 spaces, for example, set it to "    ".
 *             To kill all indentation, set it to an empty string: "".
 *
 *  The <definition> tag defines a nonterminal and has the following attributes:
 *
 *    name -  Required.  The name of the nonterminal.  This will also be the tag
 *            name used to represent the nonterminal in the XML representation
 *            of the code.
 *
 *    start - Optional.  A string to be printed before the nonterminal when
 *            writing it back out.
 *
 *            If a start attribute begins with &#10; (newline) then a new line
 *            will be inserted before the string is printed out, before
 *            any indentation.
 *
 *    end   - Optional.  A string to be printed after the nonterminal when
 *            writing it back out.
 *
 *    type  - Optional.  If supplied, it indicates that a tag representing
 *            the nonterminal should be included in the XML representation of
 *            the code and that special formatting should be used when writing
 *            that XML back out as code.  There are 5 possible values:
 *            inline, literal, line, block, line, and none.
 *
 *            If the type attribute is omitted, then "inline" is assumed.
 *
 *            When written back out, an "inline" nonterminal is simply printed
 *            inline without any additional indentation or new lines.  In the
 *            XML representing the code, the tag representing an inline
 *            may contain tags for other nonterminals.  A good example of an
 *            inline in C++ (or any programming language) is an arithmetic
 *            expression which may contain many terms and operators.
 *
 *            When written back out, "literal" nonterminals are also printed
 *            inline, however in the XML representing the code, the tag
 *            representing a literal does not contain any additional tags but
 *            rather has a "value" attribute composed of the concatenation of
 *            all terminals that were ultimately encountered while parsing the
 *            section of the code described by the nonterminal.  Good examples
 *            of literals in C++ (or any programming language) are integers,
 *            floating point numbers and strings.
 *
 *            When written back out, a new line is appended to "line"
 *            nonterminals.  In the XML representing the code, the tag
 *            representing a line may contain tags for other nonterminals.  A
 *            good example of a line in C++ is a statement, terminated by a
 *            semicolon.
 *
 *            When written back out, code inside of a "block" nonterminal is
 *            indented.  In the XML representing the code, the tag representing
 *            a block may contain tags for other nonterminals.  A good example
 *            of a block in C++ is a code block, surrounded by a pair of curly
 *            braces.
 *
 *            For nonterminals of type none, no tag will appear for the
 *            nonterminal in the XML representing the code, however the
 *            nonterminal may still be used to build up other nonterminals.
 *            A good example of a nonterminal of type none would be a digit
 *            containing terminals 0-9 that could be used to build up integers
 *            or floating point numbers.
 *
 *     Examples:
 *
 *         <definition name="code_block" type="block" start="{" end="}">
 *             ... other tags ...
 *         </definition>
 *
 *         <definition name="statement" type="line" end=";">
 *             ... other tags ...
 *         </definition>
 *
 *         <definition name="expression" type="inline">
 *             ... other tags ...
 *         </definition>
 *
 *         <definition name="integer" type="literal">
 *             ... other tags ...
 *         </definition>
 *
 *         <definition name="digit" type="none">
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
 *         <definition name="digit" type="none">
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
 *         <definition name="digit_excluding_zero" type="none">
 *             <nonterminal name="digit"/>
 *             <exception>
 *                 <terminal value="0"/>
 *             </exception>
 *         </definition>
 *
 *         <definition name="integer" type="literal">
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

		/** Parses "input" using "grammar", starting with "startsymbol",
		 *  creates an XML represenataion of the code and appends it as
		 *  children to "output".  Returns true if parsing succeeded
		 *  and false otherwise.  If parsing fails, "codeposition" is
		 *  set to the location in the code that parsing failed. */
		bool	parse(const char *input,
					const char *grammar,
					const char *startsymbol,
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
