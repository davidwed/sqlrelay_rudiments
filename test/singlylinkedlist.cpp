// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/singlylinkedlist.h>
#include <rudiments/stdio.h>

int	main(int argc, char **argv) {

	// create a list of integers
	singlylinkedlist<int32_t>	intl;

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
	intl.insertAfter(intl.getFirst()->getNext(),2);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	// remove values from the list, displaying the list after each removal
	stdoutput.printf("remove(3)\n");
	intl.remove(3);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	// remove values from the list, displaying the list after each removal
	stdoutput.printf("remove(1)\n");
	intl.remove(1);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	// append a bunch of 4's
	stdoutput.printf("append(4)\n");
	intl.append(4);
	intl.append(4);
	intl.append(4);
	intl.append(4);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	// remove 3rd node
	stdoutput.printf("remove(3rd node)\n");
	intl.remove(intl.getFirst()->getNext()->getNext());
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	// remove all 4's
	stdoutput.printf("removeAll(4)\n");
	intl.removeAll(4);
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());

	// clear the list, display the list
	stdoutput.printf("clear()\n");
	intl.clear();
	intl.print();
	stdoutput.printf("length: %d\n",intl.getLength());


	stdoutput.printf("\n\n");


	// create a list of strings
	singlylinkedlist< const char * >	strl;

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
	strl.insertAfter(strl.getFirst()->getNext(),"two");
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());

	// remove values from the list, displaying the list after each removal
	stdoutput.printf("remove(three)\n");
	strl.remove("three");
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());

	// remove values from the list, displaying the list after each removal
	stdoutput.printf("remove(one)\n");
	strl.remove("one");
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());

	// clear the list, display the list
	stdoutput.printf("clear()\n");
	strl.clear();
	strl.print();
	stdoutput.printf("length: %d\n",strl.getLength());


	stdoutput.printf("\n\n");
	stdoutput.printf("append(5,4,3,2,1,0)\n");
	strl.append("5");
	strl.append("4");
	strl.append("3");
	strl.append("2");
	strl.append("1");
	strl.append("0");
	strl.print();
	stdoutput.printf("sort()\n");
	strl.sort();
	strl.print();
	stdoutput.printf("sort() (again)\n");
	strl.sort();
	strl.print();
}
