// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/string.h>

#include <stdio.h>


int main(int argv, const char **argc) {

	string	s(50);

	s.zero();

	s.append("hello ");
	s.append("there ");
	s.append("dave !!!",4);
	printf("\"%s\"\n",s.getString());

	s.copyInto("what's ");
	printf("\"%s\"\n",s.getString());
	s.copyInto("up !!!",2);
	printf("\"%s\"\n",s.getString());
	s.copyInto(2," !!!");
	printf("\"%s\"\n",s.getString());
	s.copyInto(6,"!!!!!!",1);
	printf("\"%s\"\n",s.getString());

	s.zero();
	s.append("hello");
	printf("compare: %d=0\n",s.compare("hello"));
	printf("compare: %d=0\n",s.compare("hello",3));
	printf("compare: %d=0\n",s.compareIgnoringCase("HELLO"));
	printf("compare: %d=0\n",s.compareIgnoringCase("HELLO",3));

	s.zero();
	s.append("hello");
	printf("contains: \"%s\"=llo\n",s.contains("llo"));
	printf("contains: \"%s\"=llo\n",s.contains('l'));

	char	*hello=s.duplicate();
	printf("%s\n",hello);
	delete[] hello;
}
