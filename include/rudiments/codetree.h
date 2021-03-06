// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CODETREE_H
#define RUDIMENTS_CODETREE_H

#include <rudiments/private/codetreeincludes.h>

class codetreegrammar;
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
 *  <definition> tags, each defining a nonterminal.
 *
 *  Each <definition> tag must contain sets of <terminal>, <letter>,
 *  <lowercaseletter>, <uppercaseletter>, <digit>, <nonprintablecharacter>,
 *  <set>, <nonterminal>, <alternation>, <repetition>, <concatenation>,
 *  <option>, <exception> and <break> tags.
 *
 *  The <alternation>, <repetition>, <concatenation>, <option> and <exception>
 *  tags must contain combinations of other tags.
 *
 *  The <grammar> tag may contain two attributes:
 *
 *    indent    - Optional.  The string to use when indenting code as it's being
 *                written back out.  The string will be repeated once for each
 *                level of indention.  If omitted, tabs will be used.  Setting
 *                the attribute to "&#9;" will also cause tabs to be used.
 *                Otherwise whatever string is supplied will be repeated for
 *                each level of indentation.  To use 4 spaces, for example, set
 *                it to "    ".  To kill all indentation, set it to an empty
 *                string: "".
 *
 *    namespace - Optional.  If specified, when the grammar is used to parse
 *                source code, each tag in the resulting tree will be assigned
 *                this namespace.  Also, if specified, when the grammar is used
 *                to write a tree back out as code, nodes with a different
 *                namespace will be ignored.
 *
 *  The <definition> tag defines a nonterminal and has the following attributes:
 *
 *    name -  Required.  The name of the nonterminal.  This will also be the tag
 *            name used to represent the nonterminal in the XML representation
 *            of the code.
 *
 *    alias - Options.  A string to use in place of the name in the XML
 *            representation of the code.  May be the same as the name of
 *            another definition.  Useful when you want one definition to
 *            masquerade as another.
 *
 *    start - Optional.  A string to be printed before the nonterminal when
 *            writing it back out.
 *
 *            If a start attribute begins with a &#8; (backspace) then one level
 *            of indentation will be removed.  If the previous character in the
 *            output was a line feed then it will also be removed.
 *
 *            If a start attribute contains a &#10; (newline) then a new line
 *            and appropriate indentation will be inserted at that point.
 *
 *    end   - Optional.  A string to be printed after the nonterminal when
 *            writing it back out.
 *
 *            If an end attribute begins with a &#8; (backspace) then one level
 *            of indentation will be removed.  If the previous character in the
 *            output was a line feed then it will also be removed.
 *
 *            If an end attribute contains a &#10; (newline) then a new line
 *            and appropriate indentation will be inserted at that point.
 *
 *    type  - Optional.  If supplied, it indicates that a tag representing
 *            the nonterminal should be included in the XML representation of
 *            the code and that special formatting should be used when writing
 *            that XML back out as code.  There are 5 possible values:
 *            inline, literal, line, block, and none.
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
 *    tag   - Optional.  If set to "yes" then the nonterminal will be written
 *            back out like an xml tag, with opening and closing braces, and a
 *            trailing slash.  The name of the nonterminal will be used as the
 *            tag name.  Also, any attributes other than "value" that were
 *            added to the nonterminal will also be written out.  The start,
 *            end, and type attributes are also observed.
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
 *             can be used.  To specify the end of a line, use XML entity
 *             &#10; (newline).  To specify the beginning of a line, use
 *             XML entity &#2; (start of text).  To specify the end of all
 *             code, use XML entity &#4; (end of transmission).
 *
 *     case - Whether the terminal is case-sensitive or not.  Acceptable values
 *            are true or false.  Defaults to false.
 *
 *     Examples:
 *
 *         <terminal value="1"/>
 *         <terminal value="A"/>
 *         <terminal value="&#224;"/>
 *         <terminal value="||"/>
 *         <terminal value="&&"/>
 *         <terminal value="class" case="false"/>
 *         <terminal value="&#2;class"/>
 *         <terminal value="class&#10;"/>
 *
 * The <letter> tag refers to an alphabetic character, either upper or lower
 * case.  It is more efficient to use the <letter> tag than to define a
 * nonterminal for an alphabetic character containing terminals for each
 * character.
 *
 * The <lowercaseletter> tag refers to a lower-case alphabetic character.  It
 * is more efficient to use the <lowercaseletter> tag than to define a
 * nonterminal for an alphabetic character containing terminals for each
 * character.
 *
 * The <uppercaseletter> tag refers to a upper-case alphabetic character.  It
 * is more efficient to use the <uppercaseletter> tag than to define a
 * nonterminal for an alphabetic character containing terminals for each
 * character.
 *
 * The <digit> tag refers to a numeric digit.  It is more efficient to use the
 * <digit> tag than to define a nonterminal for a digit containing terminals
 * for each digit.
 *
 * The <nonprintablecharacter> tag refers to any non-printable character.
 * 
 * The <set> tag defines a set of ascii characters or XML entities.  It is more
 * efficient to use the <set> tag than to define a nonterminal for the set
 * containing terminals for each ascii character or XML entity.
 *
 *     value - The set of ascii characters or XML entities to match.
 *
 *     Example:
 *
 *        <definition name="symbol" type="none">
 *            <set value="~`!@#$%^&amp;*()_-+=[{]}|\;:'&quot;,&lt;.&gt;/? "/>
 *        </definition>
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
 * The <break> tag defines a terminal that, if found, will cause the enclosing
 * repetition to stop iterating.  This is useful when looking for a particular
 * set of terminating characters which would otherwise be found by one of the
 * sibling nonterminals.
 *
 *     Example:
 *     An XML comment starts with <!-- and ends with --> but can consist of
 *     any characters, digits or symbols internally.  In the definition below,
 *     without the <break>, the nonterminal "symbol" would consume the --> and
 *     the rest of the nonterminals would likely consume the entire rest of
 *     the XML.
 *
 *         <definition name="xmlcomment" type="line"
 *                                 start="&lt;-- " end=" --&gt;">
 *             <concatenation>
 *                 <terminal value="&lt;-- "/>
 *                 <repetition>
 *                     <alternation>
 *                         <break value=" --&gt;"/>
 *                         <nonterminal name="alphabetic_character"/>
 *                         <nonterminal name="digit"/>
 *                         <nonterminal name="symbol"/>
 *                         <nonterminal name="whitespace"/>
 *                     </alternation>
 *                 </repetition>
 *                 <terminal value=" --&gt;"/>
 *             </concatenation>
 *         </definition>
 *
 * The break tag has the following attributes:
 *
 *     value - The text of the break.  Ascii characters or XML entities
 *             can be used.  To specify the end of a line, use XML entity
 *             &#10; (newline).  To specify the beginning of a line, use
 *             XML entity &#2; (start of text).
 *
 *     case - Whether the break text is case-sensitive or not.  Acceptable
 *            values are true or false.  Defaults to false.
 *
 *     recursive - Acceptable values are true or false.  Defaults to false.
 *                 When set false, the parser only looks for the break text
 *                 at the point that it is defined.  When set true, the parser
 *                 looks for the break text when parsing siblings as well.
 *
 *                 In the above example, the parser would only look for " -->"
 *                 at the beginning of each repetition.  This is fine if each
 *                 of the nonterminals only processes a single character.
 *
 *                 But, if the nonterminal "symbol" looked for a string of
 *                 symbols rather than a single symbol, the break would need to
 *                 be defined as recursive or the parser would miss it if it
 *                 ran into something like "! -->".
 *
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

class RUDIMENTS_DLLSPEC codetree {
	public:

		/** Creates a new instance of the codetree class. */
		codetree();

		/** Deletes this instance of the codetree class. */
		~codetree();

		/** Parses "input" using "grammar", starting with "startsymbol",
		 *  creates an XML representation of the code and appends it as
		 *  children to "output".  Returns true if parsing succeeded
		 *  and false otherwise.  If parsing fails, "codeposition" is
		 *  set to the location in the code that parsing failed. */
		bool	parse(const char *input,
					const char *grammar,
					const char *startsymbol,
					xmldomnode *output,
					const char **codeposition);

		/** Parses "input" using "grammar", starting with "startsymbol",
		 *  creates an XML representation of the code and appends it as
		 *  children to "output".  Returns true if parsing succeeded
		 *  and false otherwise.  If parsing fails, "codeposition" is
		 *  set to the location in the code that parsing failed. */
		bool	parse(const char *input,
					codetreegrammar *grammar,
					const char *startsymbol,
					xmldomnode *output,
					const char **codeposition);

		/** Interprets the XML representation of the code "input" using
		 *  "grammar" and appends it as code to "output".  Returns true
		 *  if this succeeds and false otherwise. */
		bool	write(xmldomnode *input,
					const char *grammar,
					stringbuffer *output);

		/** Interprets the XML representation of the code "input" using
		 *  "grammar" and appends it as code to "output".  Returns true
		 *  if this succeeds and false otherwise. */
		bool	write(xmldomnode *input,
					codetreegrammar *grammar,
					stringbuffer *output);

		/** Sets the debug level.  Debug is written to standard out. */
		void	setDebugLevel(uint8_t debuglevel);

	#include <rudiments/private/codetree.h>
};

class codetreegrammarprivate;

/** A child class of xmldom with various codetree-specific optimizations. */
class RUDIMENTS_DLLSPEC codetreegrammar : public xmldom {
	friend class codetree;
	public:
		/** Creates a new instance of the codetreegrammar class. */
		codetreegrammar();

		/** Deletes this instance of the codetreegrammar class. */
		~codetreegrammar();

	#include <rudiments/private/codetreegrammar.h>
};

#endif
