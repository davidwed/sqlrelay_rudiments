// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/charstring.h>

#include <stdio.h>


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
	printf("%s\n",hello);

	// make sure to clean up what duplicate() returns
	delete[] hello;


	// split
	char		**list;
	unsigned long	listlength;
	charstring::split("hello||hi||bye||goodbye","||",&list,&listlength);
	printf("split(\"hello||hi||bye||goodbye\",\"||\")\n");
	printf("%d items\n",listlength);
	for (unsigned long i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("hello||hi||bye||goodbye||","||",&list,&listlength);
	printf("split(\"hello||hi||bye||goodbye||\",\"||\")\n");
	printf("%d items\n",listlength);
	for (unsigned long i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||hello||hi||bye||goodbye||","||",&list,&listlength);
	printf("split(\"||hello||hi||bye||goodbye||\",\"||\")\n");
	printf("%d items\n",listlength);
	for (unsigned long i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||||hello||||hi||||bye||||goodbye||||","||",
							&list,&listlength);
	printf("split(\"||||hello||||hi||||bye||||goodbye||||\",\"||\")\n");
	printf("%d items\n",listlength);
	for (unsigned long i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||||||||||","||",&list,&listlength);
	printf("split(\"||||||||||\",\"||\")\n");
	printf("%d items\n",listlength);
	for (unsigned long i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("http://www.firstworks.com/application/app.cgi/skin/module/template.html","/",&list,&listlength);
	printf("split(\"http://www.firstworks.com/application/app.cgi/skin/module/template.html\",\"/\"");
	printf("%d items\n",listlength);
	for (unsigned long i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;
}
