// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/stringbuffer.h>

#include <stdio.h>


int	main() {

	stringbuffer	*str=new stringbuffer();
	str->append("hello ")->append("there ")->append("world ");
	printf("length: %d\n\"%s\"\n",str->getStringLength(),str->getString());

	str->append((long)1)->append(" ");
	str->append((long)2)->append(" ");
	str->append((long)3)->append(" ");
	str->append((long)4)->append(" ");
	str->append((long)5)->append(" ");
	printf("length: %d\n\"%s\"\n",str->getStringLength(),str->getString());

	str->append(" ");

	str->append(1.1,1,0)->append(" ");
	str->append(2.02,2,0)->append(" ");
	str->append(3.003,3,0)->append(" ");
	str->append(4.0004,4,0)->append(" ");
	str->append(5.00005,5,0)->append(" ");
	printf("length: %d\n\"%s\"\n",str->getStringLength(),str->getString());

	str->clear();
	for (int i=0; i<1024; i++) {
		str->append('*');
	}
	printf("length: %d\n%s\n",str->getStringLength(),str->getString());

	delete str;
}
