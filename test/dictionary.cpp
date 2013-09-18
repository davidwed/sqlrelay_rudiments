// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/dictionary.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a dictionary containing string keys and string values
	dictionary< const char *, const char * >	dict;
	stdoutput.printf("string keys, string values:\n");

	// add values to the dictionary
	dict.setData("1","one");
	dict.setData("2","two");
	dict.setData("3","three");
	dict.setData("4","four");

	// display the length and contents of the dictionary
	stdoutput.printf("length: %lld\n",dict.getList()->getLength());
	dict.print();
	stdoutput.printf("\n");

	// remove values, displaying the dictionary after each removal
	dict.removeData("3");
	dict.print();
	stdoutput.printf("\n");

	dict.removeData("2");
	dict.print();
	stdoutput.printf("\n");

	dict.removeData("4");
	dict.print();
	stdoutput.printf("\n");

	dict.removeData("1");
	dict.print();



	// create a dictionary containing long integer keys and string values
	dictionary< uint16_t, const char * >	numdict;
	stdoutput.printf("integer keys, string values:\n");

	// add values to the dictionary
	numdict.setData(1,"one");
	numdict.setData(2,"two");
	numdict.setData(3,"three");
	numdict.setData(4,"four");

	// display the length and contents of the dictionary
	stdoutput.printf("length: %lld\n",numdict.getList()->getLength());
	numdict.print();
	stdoutput.printf("\n");

	// remove values, displaying the dictionary after each removal
	numdict.removeData(3);
	numdict.print();
	stdoutput.printf("\n");

	numdict.removeData(2);
	numdict.print();
	stdoutput.printf("\n");

	numdict.removeData(4);
	numdict.print();
	stdoutput.printf("\n");

	numdict.removeData(1);
	numdict.print();
	stdoutput.printf("\n");



	// create a dictionary containing double precision
	// floating point keys and string values
	dictionary< double, const char * >	doubledict;
	stdoutput.printf("floating point keys, string values:\n");

	// add values to the dictionary
	doubledict.setData(1.1,"one point one");
	doubledict.setData(2.2,"two point two");
	doubledict.setData(3.3,"three point three");
	doubledict.setData(4.4,"four point four");

	// display the length and contents of the dictionary
	stdoutput.printf("length: %lld\n",doubledict.getList()->getLength());
	doubledict.print();
	stdoutput.printf("\n");

	// remove values, displaying the dictionary after each removal
	doubledict.removeData(3.3);
	doubledict.print();
	stdoutput.printf("\n");

	doubledict.removeData(2.2);
	doubledict.print();
	stdoutput.printf("\n");

	doubledict.removeData(4.4);
	doubledict.print();
	stdoutput.printf("\n");

	doubledict.removeData(1.1);
	doubledict.print();
	stdoutput.printf("\n");
}
