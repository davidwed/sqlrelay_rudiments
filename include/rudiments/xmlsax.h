// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef XMLSAX_H
#define XMLSAX_H

#include <rudiments/private/config.h>

#include <rudiments/errorhandler.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/file.h>

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
		int	parseFile(const char *filename);
			// parse file "filename"
		int	parseString(const char *string);
			// parse string "string"


	protected:
		// Callback methods.  These methods are called as the XML is
		// parsed.
		virtual	int	tagStart(char *name);
				// Gets called when a start tag is parsed.
		virtual	int	attributeName(char *name);
				// Gets called when an attribute name
				// is parsed.
		virtual	int	attributeValue(char *value);
				// Gets called when an attribute value
				// is parsed.
		virtual	int	text(char *string);
				// Gets called when a block of text is parsed.
		virtual	int	tagEnd(char *name);
				// Gets called when an end tag is parsed.
		virtual	int	comment(char *string);
				// Gets called when a comment is parsed.
		virtual	int	cdata(char *string);
				// Gets called when cdata is parsed.

	#include <rudiments/private/xmlsax.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/xmlsaxinlines.h>
#endif

#endif
