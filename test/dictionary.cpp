// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/dictionary.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a dictionary containing string keys and string values
	dictionary< const char *, const char * >	dict;
	stdoutput.printf("string keys, string values:\n");

	// add values to the dictionary
	dict.setValue("1","one");
	dict.setValue("2","two");
	dict.setValue("3","three");
	dict.setValue("4","four");

	// display the length and contents of the dictionary
	stdoutput.printf("length: %lld\n",dict.getList()->getLength());
	dict.print();
	stdoutput.printf("\n");

	// remove values, displaying the dictionary after each removal
	dict.removeValue("3");
	dict.print();
	stdoutput.printf("\n");

	dict.removeValue("2");
	dict.print();
	stdoutput.printf("\n");

	dict.removeValue("4");
	dict.print();
	stdoutput.printf("\n");

	dict.removeValue("1");
	dict.print();



	// create a dictionary containing long integer keys and string values
	dictionary< uint16_t, const char * >	numdict;
	stdoutput.printf("integer keys, string values:\n");

	// add values to the dictionary
	numdict.setValue(1,"one");
	numdict.setValue(2,"two");
	numdict.setValue(3,"three");
	numdict.setValue(4,"four");

	// display the length and contents of the dictionary
	stdoutput.printf("length: %lld\n",numdict.getList()->getLength());
	numdict.print();
	stdoutput.printf("\n");

	// remove values, displaying the dictionary after each removal
	numdict.removeValue(3);
	numdict.print();
	stdoutput.printf("\n");

	numdict.removeValue(2);
	numdict.print();
	stdoutput.printf("\n");

	numdict.removeValue(4);
	numdict.print();
	stdoutput.printf("\n");

	numdict.removeValue(1);
	numdict.print();
	stdoutput.printf("\n");



	// create a dictionary containing double precision
	// floating point keys and string values
	dictionary< double, const char * >	doubledict;
	stdoutput.printf("floating point keys, string values:\n");

	// add values to the dictionary
	doubledict.setValue(1.1,"one point one");
	doubledict.setValue(2.2,"two point two");
	doubledict.setValue(3.3,"three point three");
	doubledict.setValue(4.4,"four point four");

	// display the length and contents of the dictionary
	stdoutput.printf("length: %lld\n",doubledict.getList()->getLength());
	doubledict.print();
	stdoutput.printf("\n");

	// remove values, displaying the dictionary after each removal
	doubledict.removeValue(3.3);
	doubledict.print();
	stdoutput.printf("\n");

	doubledict.removeValue(2.2);
	doubledict.print();
	stdoutput.printf("\n");

	doubledict.removeValue(4.4);
	doubledict.print();
	stdoutput.printf("\n");

	doubledict.removeValue(1.1);
	doubledict.print();
	stdoutput.printf("\n");
}
