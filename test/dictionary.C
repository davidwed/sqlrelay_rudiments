// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/dictionary.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	// create a dictionary containing string keys and string values
	namevaluepairs	dict;
	printf("string keys, string values:\n");

	// add values to the dictionary
	dict.setData("1","one");
	dict.setData("2","two");
	dict.setData("3","three");
	dict.setData("4","four");

	// display the length and contents of the dictionary
	printf("length: %ld\n",dict.getList()->getLength());
	dict.print();
	printf("\n");

	// remove values, displaying the dictionary after each removal
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



	// create a dictionary containing long integer keys and string values
	numericdictionary<char *>	numdict;
	printf("integer keys, string values:\n");

	// add values to the dictionary
	numdict.setData(1,"one");
	numdict.setData(2,"two");
	numdict.setData(3,"three");
	numdict.setData(4,"four");

	// display the length and contents of the dictionary
	printf("length: %ld\n",numdict.getList()->getLength());
	numdict.print();
	printf("\n");

	// remove values, displaying the dictionary after each removal
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



	// create a dictionary containing double precision
	// floating point keys and string values
	dictionary<double,char *>	doubledict;
	printf("floating point keys, string values:\n");

	// add values to the dictionary
	doubledict.setData(1.1,"one point one");
	doubledict.setData(2.2,"two point two");
	doubledict.setData(3.3,"three point three");
	doubledict.setData(4.4,"four point four");

	// display the length and contents of the dictionary
	printf("length: %ld\n",doubledict.getList()->getLength());
	doubledict.print();
	printf("\n");

	// remove values, displaying the dictionary after each removal
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
