// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/linkedlist.h>
//#include <rudiments/newlinkedlist.h>
#include <rudiments/stdio.h>

int	main(int argc, char **argv) {

	// create a list of integers
	linkedlist<int32_t>	intl;

	// append values to the list, displaying the list after each append
	stdoutput.printf("append(0)\n");
	intl.append(0);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	stdoutput.printf("append(1)\n");
	intl.append(1);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	stdoutput.printf("append(3)\n");
	intl.append(3);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	// insert a value into the middle of the list, display the list
	stdoutput.printf("insert(2,2)\n");
	intl.insert(2,2);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	// remove values from the list, displaying the list after each removal
	stdoutput.printf("removeByIndex(0)\n");
	intl.removeByIndex(0);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	stdoutput.printf("removeByValue(3)\n");
	intl.removeByValue(3);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	// change a value in the list, display the list
	stdoutput.printf("setValueByIndex(1,50)\n");
	intl.setValueByIndex(1,50);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	// clear the list, display the list
	stdoutput.printf("clear()\n");
	intl.clear();
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());


	stdoutput.printf("\n\n");


	// create a list of strings
	linkedlist< const char * >	strl;

	// append values to the list, displaying the list after each append
	stdoutput.printf("append(zero)\n");
	strl.append("zero");
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());

	stdoutput.printf("append(one)\n");
	strl.append("one");
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());

	stdoutput.printf("append(three)\n");
	strl.append("three");
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());

	// insert a value into the middle of the list, display the list
	stdoutput.printf("insert(2,two)\n");
	strl.insert(2,"two");
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());

	// remove values from the list, displaying the list after each removal
	stdoutput.printf("removeByIndex(0)\n");
	strl.removeByIndex(0);
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());

	stdoutput.printf("removeByValue(three)\n");
	strl.removeByValue("three");
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());

	// change a value in the list, display the list
	stdoutput.printf("setValueByIndex(1,fifty)\n");
	strl.setValueByIndex(1,"fifty");
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());

	// clear the list, display the list
	stdoutput.printf("clear()\n");
	strl.clear();
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());
}
