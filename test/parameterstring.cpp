// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/parameterstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, char **argv) {

	header("parameterstring");

	parameterstring	pstr;

	test("parse",pstr.parse("name1=value1;name2='value2';name3='\\'value3\\'';name4=\\\\value4\\\\"));

	test("name1",!charstring::compare(pstr.getValue("name1"),"value1"));
	test("name2",!charstring::compare(pstr.getValue("name2"),"value2"));
	test("name3",!charstring::compare(pstr.getValue("name3"),"'value3'"));
	test("name4",!charstring::compare(pstr.getValue("name4"),"\\value4\\"));
	test("name5",!pstr.getValue("name5"));
	stdoutput.printf("\n");
}
