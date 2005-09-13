// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLSAX_H
#define RUDIMENTS_XMLSAX_H

#include <rudiments/private/xmlsaxincludes.h>

// The xmlsax class is a base class for an XML SAX parser.  To use this
// class, you should create a class that inherits from it and implements the
// virtual methods.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class xmlsaxprivate;

class xmlsax {
	public:

			xmlsax();
		virtual	~xmlsax();


		// When these methods are run, they parse the XML.  As these
		// methods encounter tags, attributes, text, etc., they call
		// the callback methods below.
		bool		parseFile(const char *filename);
				// parse file "filename"
		bool		parseString(const char *string);
				// parse string "string"
		const char	*getError();
				// if parseFile() or parseString() fails,
				// returns the error that caused the failure

	protected:
		// Callback methods.  These methods are called as the XML is
		// parsed.
		virtual	bool	tagStart(const char *name);
				// Gets called when a start tag is parsed.
		virtual	bool	attributeName(const char *name);
				// Gets called when an attribute name
				// is parsed.
		virtual	bool	attributeValue(const char *value);
				// Gets called when an attribute value
				// is parsed.
		virtual	bool	text(const char *string);
				// Gets called when a block of text is parsed.
		virtual	bool	tagEnd(const char *name);
				// Gets called when an end tag is parsed.
		virtual	bool	comment(const char *string);
				// Gets called when a comment is parsed.
		virtual	bool	cdata(const char *string);
				// Gets called when cdata is parsed.

	#include <rudiments/private/xmlsax.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
