// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/dictionary.h>

#include <stdio.h>

void print(stringdictionary<char *> *dict) {

	char	*key;
	char	*value;
	for (int i=0; i<dict->getLength(); i++) {
		dict->getKey(i,&key);
		dict->getData(key,&value);
		printf("%d: %s=%s\n",i,key,value);
	}
}

void print(numericdictionary<char *> *dict) {

	long	key;
	char	*value;
	for (int i=0; i<dict->getLength(); i++) {
		dict->getKey(i,&key);
		dict->getData(key,&value);
		printf("%d: %d=%s\n",i,key,value);
	}
}

int main(int argv, const char **argc) {

	stringdictionary<char *>	dict;

	dict.setData("1","one");
	dict.setData("2","two");
	dict.setData("3","three");
	dict.setData("4","four");

	printf("length: %d\n",dict.getLength());

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

	printf("length: %d\n",numdict.getLength());

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
