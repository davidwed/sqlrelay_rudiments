// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/charstring.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif


int main(int argc, const char **argv) {

	// create a new string 50 bytes long
	char	s[50];


	// set every byte in the string to NULL
	charstring::zero(s,50);


	// Append "hello there dave" to the string and display it.
	// The 3rd call to append() only appends the first 4 bytes of "dave !!!"
	charstring::append(s,"hello ");
	charstring::append(s,"there ");
	charstring::append(s,"dave !!!",4);
	printf("\"%s\"\n",s);


	// Replace the contents of the string and display it over and over.
	// Note that the 2nd and 4th call to copy() only copy part of the
	// string passed into them.
	charstring::copy(s,"what's ");
	printf("\"%s\"\n",s);
	charstring::copy(s,"up !!!",2);
	printf("\"%s\"\n",s);
	charstring::copy(s,2," !!!");
	printf("\"%s\"\n",s);
	charstring::copy(s,6,"!!!!!!",1);
	printf("\"%s\"\n",s);


	// clear the string again.
	charstring::zero(s,50);


	// Append "hello" to the string.
	charstring::append(s,"hello");

	// perform several comparisons, all of which should return 0
	printf("compare: %d=0\n",charstring::compare(s,"hello"));
	printf("compare: %d=0\n",charstring::compare(s,"hello",3));
	printf("compare: %d=0\n",charstring::compareIgnoringCase(s,"HELLO"));
	printf("compare: %d=0\n",charstring::compareIgnoringCase(s,"HELLO",3));

	// perform several contains() comparisons
	printf("findFirst: \"%s\"=llo\n",charstring::findFirst(s,"llo"));
	printf("contains: %d\n",charstring::contains(s,"llo"));
	printf("findFirst: \"%s\"=llo\n",charstring::findFirst(s,'l'));
	printf("contains: %d\n",charstring::contains(s,"llo"));



	// duplicate the string and display the duplicated string
	char	*hello=charstring::duplicate(s);
	char	*ell=charstring::subString(hello,1,3);
	char	*lle=charstring::subString(hello,3,1);
	printf("hello: %s\n",hello);
	printf("ell: %s\n",ell);
	printf("ell: %s\n",lle);

	// make sure to clean up what duplicate() returns
	delete[] hello;
	delete[] ell;
	delete[] lle;


	// split
	char		**list;
	uint64_t	listlength;
	charstring::split("hello||hi||bye||goodbye","||",false,
						&list,&listlength);
	printf("split(\"hello||hi||bye||goodbye\",\"||\")\n");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("hello||hi||bye||goodbye||","||",false,
						&list,&listlength);
	printf("split(\"hello||hi||bye||goodbye||\",\"||\")\n");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||hello||hi||bye||goodbye||","||",false,
						&list,&listlength);
	printf("split(\"||hello||hi||bye||goodbye||\",\"||\")\n");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||||hello||||hi||||bye||||goodbye||||","||",false,
							&list,&listlength);
	printf("split(\"||||hello||||hi||||bye||||goodbye||||\",\"||\")\n");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||||||||||","||",false,&list,&listlength);
	printf("split(\"||||||||||\",\"||\")\n");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("http://www.firstworks.com/application/app.cgi/skin/module/template.html","/",false,&list,&listlength);
	printf("split(\"http://www.firstworks.com/application/app.cgi/skin/module/template.html\",\"/\"");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	char	str[]="hello'\"\\hello'\"\\";
	char	*escapedstr=charstring::escape(str,"\"'");
	char	*unescapedstr=charstring::unescape(escapedstr);
	printf("str		: %s\n",str);
	printf("escapedstr	: %s\n",escapedstr);
	printf("unescapedstr	: %s\n",unescapedstr);
	delete[] unescapedstr;
	delete[] escapedstr;

	
	const char	*alphabet="aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz";
	printf("lengthContainingSet(\"%s\",\"gfedcba\")=%d\n",
		alphabet,charstring::lengthContainingSet(alphabet,"gfedcba"));
	printf("lengthNotContainingSet(\"%s\",\"hijklmnopqrstuvwxyz\")=%d\n",
		alphabet,charstring::lengthNotContainingSet(alphabet,
						"hijklmnopqrstuvwxyz"));


	printf("findFirstOfSet(\"%s\",\"mlk\")=\"%s\"\n",
		alphabet,charstring::findFirstOfSet(alphabet,"klm"));


	// initialize a text buffer and print it out
	char    buffer[100];
	charstring::copy(buffer,"     hello there buddy     ");
	printf("!%s!\n",buffer);


	// trim the spaces off of the right hand side
	charstring::rightTrim(buffer);
	printf("!%s!\n",buffer);


	// trim the spaces off of the left hand side
	charstring::leftTrim(buffer);
	printf("!%s!\n",buffer);


	// strip the spaces out
	charstring::copy(buffer,"   hello      there   buddy  ");
	charstring::strip(buffer,' ');
	printf("!%s!\n",buffer);


	// strip the "   " out
	charstring::copy(buffer,"   hello       there  buddy  ");
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
	charstring::copy(hello,"     hello     ");
	printf("|%s|\n",hello);

	// left justify the text
	charstring::leftJustify(hello,15);
	printf("|%s|\n",hello);

	// right justify the text
	charstring::rightJustify(hello,15);
	printf("|%s|\n",hello);


	// center the text over and over...
	charstring::copy(hello,"hello          ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello," hello         ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"  hello        ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"   hello       ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"    hello      ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"     hello     ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"      hello    ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"       hello   ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"        hello  ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"         hello ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"          hello");
	charstring::center(hello,15);
	printf("|%s|\n",hello);


	// print the number of bytes necessary to store each number as a string
	printf("size of 1 is: %d\n",charstring::integerLength((long)1));
	printf("size of 10 is: %d\n",charstring::integerLength((long)10));
	printf("size of 100 is: %d\n",charstring::integerLength((long)100));
	printf("size of 1000 is: %d\n",charstring::integerLength((long)1000));
	printf("size of -1 is: %d\n",charstring::integerLength((long)-1));
	printf("size of -10 is: %d\n",charstring::integerLength((long)-10));
	printf("size of -100 is: %d\n",charstring::integerLength((long)-100));
	printf("size of -1000 is: %d\n",charstring::integerLength((long)-1000));
}
