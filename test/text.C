// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/text.h>
#include <stdio.h>
#include <string.h>

int	main() {

	// initialize a text buffer and print it out
	char    buffer[100];
	strcpy(buffer,"     hello there buddy     ");
	printf("!%s!\n",buffer);

	// trim the spaces off of the right hand side
	text::rightTrim(buffer);
	printf("!%s!\n",buffer);

	// trim the spaces off of the left hand side
	text::leftTrim(buffer);
	printf("!%s!\n",buffer);

	// strip the spaces out
	strcpy(buffer,"   hello      there   buddy  ");
	text::strip(buffer,' ');
	printf("!%s!\n",buffer);

	// strip the "   " out
	strcpy(buffer,"   hello       there  buddy  ");
	text::strip(buffer,"   ");
	printf("!%s!\n",buffer);

	// convert buffer to uppercase
	text::upper(buffer);
	printf("!%s!\n",buffer);

	// convert buffer to lowercase
	text::lower(buffer);
	printf("!%s!\n",buffer);

	// http escape the buffer
	char    *escbuffer=text::httpEscape("!@#$%^&*()hello-+");
	printf("!@#$\%^&*()hello-+  http escaped is  %s\n",escbuffer);
	delete escbuffer;
	
	// evaluate a string to see if it's a number
	if (text::isNumber("-100.5")) {
		printf("-100.5 is a number\n");
	}
	if (!text::isNumber("-100.5.10")) {
		printf("-100.5.10 is not a number\n");
	}
	
	// evaluate a string to see if it's an integer
	if (text::isInteger("-100")) {
		printf("-100 is an integer\n");
	}
	if (!text::isInteger("-100.5")) {
		printf("-100.5.10 is not an integer\n");
	}

	char	string[16];
	strcpy(string,"     hello     ");
	printf("|%s|\n",string);
	text::leftJustify(string,15);
	printf("|%s|\n",string);
	text::rightJustify(string,15);
	printf("|%s|\n",string);

	strcpy(string,"     hello     ");
	printf("|%s|\n",string);
	text::rightJustify(string,15);
	printf("|%s|\n",string);
	text::leftJustify(string,15);
	printf("|%s|\n",string);


	strcpy(string,"hello          ");
	text::center(string,15);
	printf("|%s|\n",string);
	strcpy(string," hello         ");
	text::center(string,15);
	printf("|%s|\n",string);
	strcpy(string,"  hello        ");
	text::center(string,15);
	printf("|%s|\n",string);
	strcpy(string,"   hello       ");
	text::center(string,15);
	printf("|%s|\n",string);
	strcpy(string,"    hello      ");
	text::center(string,15);
	printf("|%s|\n",string);
	strcpy(string,"     hello     ");
	text::center(string,15);
	printf("|%s|\n",string);
	strcpy(string,"      hello    ");
	text::center(string,15);
	printf("|%s|\n",string);
	strcpy(string,"       hello   ");
	text::center(string,15);
	printf("|%s|\n",string);
	strcpy(string,"        hello  ");
	text::center(string,15);
	printf("|%s|\n",string);
	strcpy(string,"         hello ");
	text::center(string,15);
	printf("|%s|\n",string);
	strcpy(string,"          hello");
	text::center(string,15);
	printf("|%s|\n",string);

	printf("size of 1 is: %d\n",text::integerLength(1));
	printf("size of 10 is: %d\n",text::integerLength(10));
	printf("size of 100 is: %d\n",text::integerLength(100));
	printf("size of 1000 is: %d\n",text::integerLength(1000));
	printf("size of -1 is: %d\n",text::integerLength(-1));
	printf("size of -10 is: %d\n",text::integerLength(-10));
	printf("size of -100 is: %d\n",text::integerLength(-100));
	printf("size of -1000 is: %d\n",text::integerLength(-1000));
}
