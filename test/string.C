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

	printf("equals: %d=1\n",s.equals("hello"));
	printf("equals: %d=1\n",s.equals("hello",3));
	printf("equals: %d=1\n",s.equalsIgnoringCase("HELLO"));
	printf("equals: %d=1\n",s.equalsIgnoringCase("HELLO",3));

	printf("gt: %d=1\n",s.greaterThan("a"));
	printf("gt: %d=1\n",s.greaterThan("a",3));
	printf("gt: %d=1\n",s.greaterThanIgnoringCase("A"));
	printf("gt: %d=1\n",s.greaterThanIgnoringCase("A",3));

	printf("lt: %d=1\n",s.lessThan("z"));
	printf("lt: %d=1\n",s.lessThan("z",3));
	printf("lt: %d=1\n",s.lessThanIgnoringCase("Z"));
	printf("lt: %d=1\n",s.lessThanIgnoringCase("Z",3));

	s.zero();
	s.append("hello");
	printf("contains: \"%s\"=llo\n",s.contains("llo"));
	printf("contains: \"%s\"=llo\n",s.contains('l'));

	char	*hello=s.duplicate();
	printf("%s\n",hello);
	delete[] hello;
}
