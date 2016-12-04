// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/xmldom.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>
#include "test.cpp"

#include "xml.h"

int main(int argc, const char **argv) {

	header("xmldom");

	xmldom	x;
	test("parse string",x.parseString(xmls));

	file::remove("xml.xml");
	test("write file",x.writeFile("xml.xml",
				permissions::evalPermString("rw-r--r--")));

	/*char	*xmldotxml=file::getContents("xml.xml");
	test("file contents",!charstring::compare(xmls,xmldotxml));
	delete[] xmldotxml;*/

	test("parse file",x.parseFile("xml.xml"));
	file::remove("xml.xml");
}
