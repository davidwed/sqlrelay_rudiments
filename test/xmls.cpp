// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/xmlsax.h>
#include <rudiments/stdio.h>

class myxmlsax : public xmlsax {
	public:
				myxmlsax();
		bool		parseFile(const char *filename);
		bool		parseString(const char *string);
		const char	*getString();
	private:
		bool	xmlVersionStart();
		bool	xmlVersionEnd();
		bool	doctypeStart(const char *name);
		bool	externalSubset(const char *filename);
		bool	doctypeEnd();
		bool	tagStart(const char *ns, const char *name);
		bool	attributeName(const char *name);
		bool	attributeValue(const char *value);
		bool	text(const char *string);
		bool	tagEnd(const char *ns, const char *name);
		bool	comment(const char *string);
		bool	cdata(const char *string);
		void	indent(int spaces);
		int	ind;

		stringbuffer	str;
};

myxmlsax::myxmlsax() : xmlsax() {
	ind=0;
}

bool myxmlsax::parseFile(const char *filename) {
	str.clear();
	return xmlsax::parseFile(filename);
}

bool myxmlsax::parseString(const char *string) {
	str.clear();
	return xmlsax::parseString(string);
}

bool myxmlsax::xmlVersionStart() {
	str.append("XML version start:\n");
	return true;
}

bool myxmlsax::xmlVersionEnd() {
	str.append("XML version end:\n");
	return true;
}

bool myxmlsax::doctypeStart(const char *name) {
	str.append("DOCTYPE start: ")->append(name)->append("\n");
	return true;
}

bool myxmlsax::externalSubset(const char *filename) {
	str.append("	external subset: ")->append(filename)->append("\n");
	return true;
}

bool myxmlsax::doctypeEnd() {
	str.append("DOCTYPE end:\n");
	return true;
}

void myxmlsax::indent(int spaces) {
	for (int i=0; i<spaces; i++) {
		str.append("  ");
	}
}

bool myxmlsax::tagStart(const char *ns, const char *name) {
	indent(ind);
	str.append("tagStart: ");
	if (ns) {
		str.append(ns)->append(":");
	}
	str.append(name)->append("\n");
	ind++;
	return true;
}

bool myxmlsax::attributeName(const char *name) {
	indent(ind+1);
	str.append("attribute name: ")->append(name)->append("\n");
	return true;
}

bool myxmlsax::attributeValue(const char *value) {
	indent(ind+1);
	str.append("attribute value: ")->append(value)->append("\n");
	return true;
}

bool myxmlsax::text(const char *string) {
	indent(ind+1);
	str.append("text: \n")->append(string)->append("\n");
	return true;
}

bool myxmlsax::tagEnd(const char *ns, const char *name) {
	ind--;
	indent(ind);
	str.append("tagEnd: ");
	if (ns) {
		str.append(ns)->append(":");
	}
	str.append(name)->append("\n");
	return true;
}

bool myxmlsax::comment(const char *string) {
	indent(ind);
	str.append("comment: \n")->append(string)->append("\n");
	return true;
}

bool myxmlsax::cdata(const char *string) {
	indent(ind);
	str.append("cdata: \n")->append(string)->append("\n");
	return true;
}

const char	*myxmlsax::getString() {
	return str.getString();
}

#include "xml.h"

int main(int argc, const char **argv) {

	myxmlsax	x;
	stdoutput.printf("=====================================================\n");

	// parse string...
	x.parseString(xmls);
	stdoutput.printf("%s\n",x.getString());
	stdoutput.printf("=====================================================\n");

	// parse local file...
	if (!x.parseFile("xmls.xml")) {
		stdoutput.printf("failed to parse local xmls.xml\n");
	}
	stdoutput.printf("%s\n",x.getString());
	stdoutput.printf("=====================================================\n");

	// parse remote file...
	if (!x.parseFile("http://localhost/~dmuse/src/rudiments/test/xmls.xml")) {
		stdoutput.printf("failed to parse remote xmls.xml\n");
	}
	stdoutput.printf("%s\n",x.getString());
	stdoutput.printf("=====================================================\n");

	// bad strings
	stdoutput.printf("bad xml: %d %d %d %d %d %d %d\n",
		x.parseString("<xml><tag></xml>\n")==false,
		x.parseString("<xml><tag attribute/></xml>\n")==false,
		x.parseString("<xml><tag attribute/></xml>\n")==false,
		x.parseString("<xml><tag attribute=/></xml>\n")==false,
		x.parseString("<xml><tag attribute=\"/></xml>\n")==false,
		x.parseString("<xml><tag attribute=\"></tag></xml>\n")==false,
		x.parseString("<xml><tag1 attribute=\"\"></tag2></xml>\n")==false);

}
