// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLSAX_H
#define RUDIMENTS_XMLSAX_H

#include <rudiments/private/xmlsaxincludes.h>

// The xmlsax class is a base class for an XML SAX parser.  To use this
// class, you should create a class that inherits from it and implements the
// virtual methods.

class xmlsax : public errorhandler {
	public:

			xmlsax();
		virtual	~xmlsax();


		// When these methods are run, they parse the XML.  As these
		// methods encounter tags, attributes, text, etc., they call
		// the callback methods below.
		bool	parseFile(const char *filename);
			// parse file "filename"
		bool	parseString(const char *string);
			// parse string "string"

	protected:
		// Callback methods.  These methods are called as the XML is
		// parsed.
		virtual	bool	tagStart(char *name);
				// Gets called when a start tag is parsed.
		virtual	bool	attributeName(char *name);
				// Gets called when an attribute name
				// is parsed.
		virtual	bool	attributeValue(char *value);
				// Gets called when an attribute value
				// is parsed.
		virtual	bool	text(char *string);
				// Gets called when a block of text is parsed.
		virtual	bool	tagEnd(char *name);
				// Gets called when an end tag is parsed.
		virtual	bool	comment(char *string);
				// Gets called when a comment is parsed.
		virtual	bool	cdata(char *string);
				// Gets called when cdata is parsed.

	#include <rudiments/private/xmlsax.h>
};

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/xmlsaxinlines.h>
#endif

#endif
