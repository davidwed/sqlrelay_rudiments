// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/dictionary.h>

#include <stdio.h>

void print(stringdictionary<char *> *dict) {

	char	*key;
	char	*value;
	for (int i=0; i<dict->getLength(); i++) {
		dict->getKey(i,&key);
		dict->getValue(key,&value);
		printf("%d: %s=%s\n",i,key,value);
	}
}

void print(numericdictionary<char *> *dict) {

	long	key;
	char	*value;
	for (int i=0; i<dict->getLength(); i++) {
		dict->getKey(i,&key);
		dict->getValue(key,&value);
		printf("%d: %d=%s\n",i,key,value);
	}
}

int main(int argv, const char **argc) {

	stringdictionary<char *>	dict;

	dict.setValue("1","one");
	dict.setValue("2","two");
	dict.setValue("3","three");
	dict.setValue("4","four");

	printf("length: %d\n",dict.getLength());

	print(&dict);
	printf("\n");

	dict.removeValue("3");
	print(&dict);
	printf("\n");

	dict.removeValue("2");
	print(&dict);
	printf("\n");

	dict.removeValue("4");
	print(&dict);
	printf("\n");

	dict.removeValue("1");
	print(&dict);

	numericdictionary<char *>	numdict;

	numdict.setValue(1,"one");
	numdict.setValue(2,"two");
	numdict.setValue(3,"three");
	numdict.setValue(4,"four");

	printf("length: %d\n",numdict.getLength());

	print(&numdict);
	printf("\n");

	numdict.removeValue(3);
	print(&numdict);
	printf("\n");

	numdict.removeValue(2);
	print(&numdict);
	printf("\n");

	numdict.removeValue(4);
	print(&numdict);
	printf("\n");

	numdict.removeValue(1);
	print(&numdict);
}
