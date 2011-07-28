// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/linkedlist.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int	main(int argc, char **argv) {

	// create a list of integers
	linkedlist<int>	intl;

	// append values to the list, displaying the list after each append
	printf("append(0)\n");
	intl.append(0);
	intl.print();

	printf("append(1)\n");
	intl.append(1);
	intl.print();

	printf("append(3)\n");
	intl.append(3);
	intl.print();

	// insert a value into the middle of the list, display the list
	printf("insert(2,2)\n");
	intl.insert(2,2);
	intl.print();

	// remove values from the list, displaying the list after each removal
	printf("removeByIndex(0)\n");
	intl.removeByIndex(0);
	intl.print();

	printf("removeByData(3)\n");
	intl.removeByData(3);
	intl.print();

	// change a value in the list, display the list
	printf("setDataByIndex(1,50)\n");
	intl.setDataByIndex(1,50);
	intl.print();

	// clear the list, display the list
	printf("clear()\n");
	intl.clear();
	intl.print();


	printf("\n\n");


	// create a list of strings
	linkedlist< const char * >	strl;

	// append values to the list, displaying the list after each append
	printf("append(zero)\n");
	strl.append("zero");
	strl.print();

	printf("append(one)\n");
	strl.append("one");
	strl.print();

	printf("append(three)\n");
	strl.append("three");
	strl.print();

	// insert a value into the middle of the list, display the list
	printf("insert(2,two)\n");
	strl.insert(2,"two");
	strl.print();

	// remove values from the list, displaying the list after each removal
	printf("removeByIndex(0)\n");
	strl.removeByIndex(0);
	strl.print();

	printf("removeByData(three)\n");
	strl.removeByData("three");
	strl.print();

	// change a value in the list, display the list
	printf("setDataByIndex(1,fifty)\n");
	strl.setDataByIndex(1,"fifty");
	strl.print();

	// clear the list, display the list
	printf("clear()\n");
	strl.clear();
	strl.print();
}
