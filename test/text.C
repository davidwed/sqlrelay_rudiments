// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/charstring.h>
#include <stdio.h>
#include <string.h>

int main(int argc, const char **argv) {

	// initialize a text buffer and print it out
	char    buffer[100];
	charstring::copyInto(buffer,"     hello there buddy     ");
	printf("!%s!\n",buffer);


	// trim the spaces off of the right hand side
	charstring::rightTrim(buffer);
	printf("!%s!\n",buffer);


	// trim the spaces off of the left hand side
	charstring::leftTrim(buffer);
	printf("!%s!\n",buffer);


	// strip the spaces out
	charstring::copyInto(buffer,"   hello      there   buddy  ");
	charstring::strip(buffer,' ');
	printf("!%s!\n",buffer);


	// strip the "   " out
	charstring::copyInto(buffer,"   hello       there  buddy  ");
	charstring::strip(buffer,"   ");
	printf("!%s!\n",buffer);


	// convert buffer to uppercase
	charstring::upper(buffer);
	printf("!%s!\n",buffer);


	// convert buffer to lowercase
	charstring::lower(buffer);
	printf("!%s!\n",buffer);


	// http escape the buffer
	char    *escbuffer=charstring::httpEscape("!@#$%^&*()hello-+");
	printf("!@#$\\%^&*()hello-+  http escaped is  %s\n",escbuffer);
	delete escbuffer;
	

	// evaluate a string to see if it's a number
	if (charstring::isNumber("-100.5")) {
		printf("-100.5 is a number\n");
	}
	if (!charstring::isNumber("-100.5.10")) {
		printf("-100.5.10 is not a number\n");
	}
	

	// evaluate a string to see if it's an integer
	if (charstring::isInteger("-100")) {
		printf("-100 is an integer\n");
	}
	if (!charstring::isInteger("-100.5")) {
		printf("-100.5.10 is not an integer\n");
	}


	// create a new string containing text surrounded by spaces
	char	hello[16];
	charstring::copyInto(hello,"     hello     ");
	printf("|%s|\n",hello);

	// left justify the text
	charstring::leftJustify(hello,15);
	printf("|%s|\n",hello);

	// right justify the text
	charstring::rightJustify(hello,15);
	printf("|%s|\n",hello);


	// center the text over and over...
	charstring::copyInto(hello,"hello          ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copyInto(hello," hello         ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copyInto(hello,"  hello        ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copyInto(hello,"   hello       ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copyInto(hello,"    hello      ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copyInto(hello,"     hello     ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copyInto(hello,"      hello    ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copyInto(hello,"       hello   ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copyInto(hello,"        hello  ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copyInto(hello,"         hello ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copyInto(hello,"          hello");
	charstring::center(hello,15);
	printf("|%s|\n",hello);


	// print the number of bytes necessary to store each number as a string
	printf("size of 1 is: %d\n",charstring::integerLength(1));
	printf("size of 10 is: %d\n",charstring::integerLength(10));
	printf("size of 100 is: %d\n",charstring::integerLength(100));
	printf("size of 1000 is: %d\n",charstring::integerLength(1000));
	printf("size of -1 is: %d\n",charstring::integerLength(-1));
	printf("size of -10 is: %d\n",charstring::integerLength(-10));
	printf("size of -100 is: %d\n",charstring::integerLength(-100));
	printf("size of -1000 is: %d\n",charstring::integerLength(-1000));
}
