// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/charstring.h>

#include <stdio.h>


int main(int argc, const char **argv) {

	// create a new string 50 bytes long
	charstring	s(50);


	// set every byte in the string to NULL
	s.zero();


	// Append "hello there dave" to the string and display it.
	// The 3rd call to append() only appends the first 4 bytes of "dave !!!"
	s.append("hello ");
	s.append("there ");
	s.append("dave !!!",4);
	printf("\"%s\"\n",s.getString());


	// Replace the contents of the string and display it over and over.
	// Note that the 2nd and 4th call to copyInto() only copy part of the
	// string passed into them.
	s.copyInto("what's ");
	printf("\"%s\"\n",s.getString());
	s.copyInto("up !!!",2);
	printf("\"%s\"\n",s.getString());
	s.copyInto(2," !!!");
	printf("\"%s\"\n",s.getString());
	s.copyInto(6,"!!!!!!",1);
	printf("\"%s\"\n",s.getString());


	// clear the string again.
	s.zero();


	// Append "hello" to the string.
	s.append("hello");

	// perform several comparisons, all of which should return 0
	printf("compare: %d=0\n",s.compare("hello"));
	printf("compare: %d=0\n",s.compare("hello",3));
	printf("compare: %d=0\n",s.compareIgnoringCase("HELLO"));
	printf("compare: %d=0\n",s.compareIgnoringCase("HELLO",3));

	// perform several contains() comparisons
	printf("contains: \"%s\"=llo\n",s.contains("llo"));
	printf("contains: \"%s\"=llo\n",s.contains('l'));



	// duplicate the string and display the duplicated string
	char	*hello=s.duplicate();
	printf("%s\n",hello);

	// make sure to clean up what duplicate() returns
	delete[] hello;
}
