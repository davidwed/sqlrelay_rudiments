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

	dictionary<double,char *>	doubledict;

	doubledict.setData(1.1,"one point one");
	doubledict.setData(2.2,"two point two");
	doubledict.setData(3.3,"three point three");
	doubledict.setData(4.4,"four point four");

	printf("length: %d\n",doubledict.getList()->getLength());

	doubledict.print();
	printf("\n");

	doubledict.removeData(3.3);
	doubledict.print();
	printf("\n");

	doubledict.removeData(2.2);
	doubledict.print();
	printf("\n");

	doubledict.removeData(4.4);
	doubledict.print();
	printf("\n");

	doubledict.removeData(1.1);
	doubledict.print();
	printf("\n");
}
