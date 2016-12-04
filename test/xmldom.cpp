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

	stdoutput.printf("normal...\n");
	test("parse string",x.parseString(normal));
	file::remove("normal.xml");
	test("write file",x.writeFile("normal.xml",
				permissions::evalPermString("rw-r--r--")));
	char	*normaldotxml=file::getContents("normal.xml");
	test("file contents",!charstring::compare(normal,normaldotxml));
	test("parse file",x.parseFile("normal.xml"));


	stdoutput.printf("ns...\n");
	test("parse string",x.parseString(ns));
	file::remove("ns.xml");
	test("write file",x.writeFile("ns.xml",
				permissions::evalPermString("rw-r--r--")));
	char	*nsdotxml=file::getContents("ns.xml");
	test("file contents",!charstring::compare(ns,nsdotxml));
	delete[] nsdotxml;
	test("parse file",x.parseFile("ns.xml"));
	file::remove("ns.xml");


	stdoutput.printf("escapes...\n");
	test("parse string",x.parseString(escapes));
	file::remove("escapes.xml");
	test("write file",x.writeFile("escapes.xml",
				permissions::evalPermString("rw-r--r--")));
	char	*escapesdotxml=file::getContents("escapes.xml");
	//test("file contents",!charstring::compare(escapes,escapesdotxml));
	delete[] escapesdotxml;
	test("parse file",x.parseFile("escapes.xml"));
	file::remove("escapes.xml");


	stdoutput.printf("cdata...\n");
	test("parse string",x.parseString(cdata));
	file::remove("cdata.xml");
	test("write file",x.writeFile("cdata.xml",
				permissions::evalPermString("rw-r--r--")));
	char	*cdatadotxml=file::getContents("cdata.xml");
	//test("file contents",!charstring::compare(cdata,cdatadotxml));
	delete[] cdatadotxml;
	test("parse file",x.parseFile("cdata.xml"));
	file::remove("cdata.xml");


	stdoutput.printf("target...\n");
	test("parse string",x.parseString(target));
	file::remove("target.xml");
	test("write file",x.writeFile("target.xml",
				permissions::evalPermString("rw-r--r--")));
	char	*targetdotxml=file::getContents("target.xml");
	test("file contents",!charstring::compare(target,targetdotxml));
	delete[] targetdotxml;
	test("parse file",x.parseFile("target.xml"));
	file::remove("target.xml");


	stdoutput.printf("single quotes...\n");
	test("parse string",x.parseString(singlequotes));
	file::remove("singlequotes.xml");
	test("write file",x.writeFile("singlequotes.xml",
				permissions::evalPermString("rw-r--r--")));
	char	*singlequotesdotxml=file::getContents("singlequotes.xml");
	test("file contents",!charstring::compare(normal,singlequotesdotxml));
	delete[] singlequotesdotxml;
	test("parse file",x.parseFile("singlequotes.xml"));
	file::remove("singlequotes.xml");


	stdoutput.printf("odd spacing...\n");
	test("parse string",x.parseString(oddspacing));
	file::remove("oddspacing.xml");
	test("write file",x.writeFile("oddspacing.xml",
				permissions::evalPermString("rw-r--r--")));
	char	*oddspacingdotxml=file::getContents("oddspacing.xml");
	test("file contents",!charstring::compare(normal,oddspacingdotxml));
	delete[] oddspacingdotxml;
	test("parse file",x.parseFile("oddspacing.xml"));
	file::remove("oddspacing.xml");


	file::remove("normal.xml");
	delete[] normaldotxml;
}
