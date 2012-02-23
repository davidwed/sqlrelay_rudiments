// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLSAX_H
#define RUDIMENTS_XMLSAX_H

#include <rudiments/private/xmlsaxincludes.h>

/** The xmlsax class is a base class for an XML SAX parser.  To use this
 *  class, you should create a class that inherits from it and implements the
 *  virtual methods. */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class xmlsaxprivate;

class RUDIMENTS_DLLSPEC xmlsax {
	public:

		/** Creates an instance of the xmlsax class. */
		xmlsax();

		/** Deletes this instance of the xmlsax class. */
		virtual	~xmlsax();


		/** Parses file "filename" and calls the appropriate callback
		 *  when tags, attributes, text, etc. are encountered.
		 *
		 *  Returns true on success or false otherwise. */
		bool		parseFile(const char *filename);

		/** Parses "string"  and calls the appropriate callback
		 *  when tags, attributes, text, etc. are encountered.
		 *
		 *  Returns true on success or false otherwise. */
		bool		parseString(const char *string);

		/** If parseFile() or parseString() fails, this method
		 *  returns the error that caused the failure. */
		const char	*getError();

	protected:

		/** Gets called when a start tag is parsed. */
		virtual	bool	tagStart(const char *name);

		/** Gets called when an attribute name is parsed. */
		virtual	bool	attributeName(const char *name);

		/** Gets called when an attribute value is parsed. */
		virtual	bool	attributeValue(const char *value);

		/** Gets called when a block of text is parsed. */
		virtual	bool	text(const char *string);

		/** Gets called when an end tag is parsed. */
		virtual	bool	tagEnd(const char *name);

		/** Gets called when a comment is parsed. */
		virtual	bool	comment(const char *string);

		/** Gets called when cdata is parsed. */
		virtual	bool	cdata(const char *string);

	#include <rudiments/private/xmlsax.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
