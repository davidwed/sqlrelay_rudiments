// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/connectstring.h>
#include <stdio.h>

main() {

	connectstring	cstr;

	cstr.parse("name1=value1;name2='value2';name3='\\'value3\\'';name4=\\\\value4\\\\");

	printf("By Name:\nname1=%s\nname2=%s\nname3=%s\nname4=%s\nname5=%s\n",
				cstr.getValue("name1"),
				cstr.getValue("name2"),
				cstr.getValue("name3"),
				cstr.getValue("name4"),
				cstr.getValue("name5"));
}
