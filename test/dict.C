// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/dictionary.h>

#include <stdio.h>

void print(stringdictionary<char *> *dict) {

	for (int i=0; i<dict->getList()->getLength(); i++) {
		printf("%d: %s=%s\n",i,
			dict->getList()->getNodeByIndex(i)->getData()->getKey(),
			dict->getList()->getNodeByIndex(i)->getData()->getData());
	}
}

void print(numericdictionary<char *> *dict) {

	for (int i=0; i<dict->getList()->getLength(); i++) {
		printf("%d: %d=%s\n",i,
			dict->getList()->getNodeByIndex(i)->getData()->getKey(),
			dict->getList()->getNodeByIndex(i)->getData()->getData());
	}
}

int main(int argv, const char **argc) {

	stringdictionary<char *>	dict;

	dict.setData("1","one");
	dict.setData("2","two");
	dict.setData("3","three");
	dict.setData("4","four");

	printf("length: %d\n",dict.getList()->getLength());

	print(&dict);
	printf("\n");

	dict.removeData("3");
	print(&dict);
	printf("\n");

	dict.removeData("2");
	print(&dict);
	printf("\n");

	dict.removeData("4");
	print(&dict);
	printf("\n");

	dict.removeData("1");
	print(&dict);

	numericdictionary<char *>	numdict;

	numdict.setData(1,"one");
	numdict.setData(2,"two");
	numdict.setData(3,"three");
	numdict.setData(4,"four");

	printf("length: %d\n",numdict.getList()->getLength());

	print(&numdict);
	printf("\n");

	numdict.removeData(3);
	print(&numdict);
	printf("\n");

	numdict.removeData(2);
	print(&numdict);
	printf("\n");

	numdict.removeData(4);
	print(&numdict);
	printf("\n");

	numdict.removeData(1);
	print(&numdict);
}
