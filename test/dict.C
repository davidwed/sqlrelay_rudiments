// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/dictionary.h>

#include <stdio.h>

int main(int argv, const char **argc) {

	namevaluepairs	dict;

	dict.setData("1","one");
	dict.setData("2","two");
	dict.setData("3","three");
	dict.setData("4","four");

	printf("length: %d\n",dict.getList()->getLength());

	dict.print();
	printf("\n");

	dict.removeData("3");
	dict.print();
	printf("\n");

	dict.removeData("2");
	dict.print();
	printf("\n");

	dict.removeData("4");
	dict.print();
	printf("\n");

	dict.removeData("1");
	dict.print();

	numericdictionary<char *>	numdict;

	numdict.setData(1,"one");
	numdict.setData(2,"two");
	numdict.setData(3,"three");
	numdict.setData(4,"four");

	printf("length: %d\n",numdict.getList()->getLength());

	numdict.print();
	printf("\n");

	numdict.removeData(3);
	numdict.print();
	printf("\n");

	numdict.removeData(2);
	numdict.print();
	printf("\n");

	numdict.removeData(4);
	numdict.print();
	printf("\n");

	numdict.removeData(1);
	numdict.print();
	printf("\n");
}
