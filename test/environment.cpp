// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/environment.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("environment");

	// remove just in case
	environment::remove("TEST");

	test("TEST is empty",!environment::getValue("TEST"));
	test("setValue() TEST=value",
		environment::setValue("TEST","value"));
	test("getValue() TEST=value",
		!charstring::compare(environment::getValue("TEST"),"value"));
	test("setValue() TEST=newvalue",
		environment::setValue("TEST","newvalue"));
	test("getValue() TEST=newvalue",
		!charstring::compare(environment::getValue("TEST"),"newvalue"));
	test("remove() TEST",
		!charstring::compare(environment::getValue("TEST"),""));
	environment::setValue("TEST","value");
	environment::clear();
	test("clear()",!environment::getValue("TEST"));
}
