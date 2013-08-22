// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/parameterstring.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, char **argv) {

	parameterstring	pstr;

	pstr.parse("name1=value1;name2='value2';name3='\\'value3\\'';name4=\\\\value4\\\\");

	stdoutput.printf("By Name:\nname1=%s\nname2=%s\n"
				"name3=%s\nname4=%s\nname5=%s\n",
				pstr.getValue("name1"),
				pstr.getValue("name2"),
				pstr.getValue("name3"),
				pstr.getValue("name4"),
				pstr.getValue("name5"));
}
