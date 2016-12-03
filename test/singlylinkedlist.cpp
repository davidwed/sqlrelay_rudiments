// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/singlylinkedlist.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, char **argv) {

	header("singlylinkedlist");

	// integers
	stdoutput.printf("integers:\n");
	singlylinkedlist<int32_t>	intl;

	// append
	intl.append((int32_t)0);
	test("append(0)/getLength()",intl.getLength()==1);
	test("append(0)/walk forwards",intl.getFirst()->getValue()==0);
	test("append(0)/walk backwards",intl.getLast()->getValue()==0);
	intl.append((int32_t)1);
	test("append(1)/getLength()",intl.getLength()==2);
	test("append(1)/walk forwards",intl.getFirst()->getValue()==0);
	test("append(1)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	intl.append((int32_t)3);
	test("append(3)/getLength()",intl.getLength()==3);
	test("append(3)/walk forwards",intl.getFirst()->getValue()==0);
	test("append(3)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("append(3)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getValue()==3);
	test("append(3)/walk backwards",intl.getLast()->getValue()==3);

	// insert
	intl.insertAfter(intl.getFirst()->getNext(),2);
	test("insertAfter(2,2)/getLength()",intl.getLength()==4);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->getValue()==0);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getValue()==2);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getNext()->
						getValue()==3);
	test("insertAfter(2,2)/walk backwards",intl.getLast()->getValue()==3);

	// remove
	intl.remove(3);
	test("remove(3)/getLength()",intl.getLength()==3);
	test("remove(3)/walk forwards",intl.getFirst()->getValue()==0);
	test("remove(3)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("remove(3)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getValue()==2);
	test("remove(3)/walk backwards",intl.getLast()->getValue()==2);

	// clear
	intl.clear();
	test("clear()/getLength()",intl.getLength()==0);
	test("clear()/getFirst()",!intl.getFirst());
	test("clear()/getLast()",!intl.getLast());
	stdoutput.printf("\n\n");


	// strings
	stdoutput.printf("strings:\n");
	singlylinkedlist<const char *>	strl;

	// append
	strl.append("zero");
	test("append(zero)/getLength()",strl.getLength()==1);
	test("append(zero)/walk forwards",!charstring::compare(
						strl.getFirst()->getValue(),
						"zero"));
	test("append(zero)/walk backwards",!charstring::compare(
						strl.getLast()->getValue(),
						"zero"));
	strl.append("one");
	test("append(one)/getLength()",strl.getLength()==2);
	test("append(one)/walk forwards",!charstring::compare(
						strl.getFirst()->getValue(),
						"zero"));
	test("append(one)/walk forwards",!charstring::compare(
						strl.getFirst()->
						getNext()->getValue(),
						"one"));
	strl.append("three");
	test("append(three)/getLength()",strl.getLength()==3);
	test("append(three)/walk forwards",!charstring::compare(
						strl.getFirst()->getValue(),
						"zero"));
	test("append(three)/walk forwards",!charstring::compare(
						strl.getFirst()->
						getNext()->getValue(),
						"one"));
	test("append(three)/walk forwards",!charstring::compare(
						strl.getFirst()->getNext()->
						getNext()->getValue(),
						"three"));
	test("append(three)/walk backwards",!charstring::compare(
						strl.getLast()->getValue(),
						"three"));

	// insert
	strl.insertAfter(strl.getFirst()->getNext(),"two");
	test("insertAfter(two,two)/getLength()",strl.getLength()==4);
	test("insertAfter(two,two)/walk forwards",!charstring::compare(
							strl.getFirst()->
							getValue(),
							"zero"));
	test("insertAfter(two,two)/walk forwards",!charstring::compare(
							strl.getFirst()->
							getNext()->getValue(),
							"one"));
	test("insertAfter(two,two)/walk forwards",!charstring::compare(
							strl.getFirst()->
							getNext()->getNext()->
							getValue(),
							"two"));
	test("insertAfter(two,two)/walk forwards",!charstring::compare(
							strl.getFirst()->
							getNext()->getNext()->
							getNext()->getValue(),
							"three"));

	// remove
	strl.remove("three");
	test("remove(three)/getLength()",strl.getLength()==3);
	test("remove(three)/walk forwards",!charstring::compare(
						strl.getFirst()->getValue(),
						"zero"));
	test("remove(three)/walk forwards",!charstring::compare(
						strl.getFirst()->
						getNext()->getValue(),
						"one"));
	test("remove(three)/walk forwards",!charstring::compare(
						strl.getFirst()->getNext()->
						getNext()->getValue(),
						"two"));

	// clear
	strl.clear();
	test("clear()/getLength()",strl.getLength()==0);
	test("clear()/getFirst()",!strl.getFirst());
	test("clear()/getLast()",!strl.getLast());
	stdoutput.printf("\n");


	// heap sort
	stdoutput.printf("heapSort() (from exactly backwards)\n");
	strl.append("5");
	strl.append("4");
	strl.append("3");
	strl.append("2");
	strl.append("1");
	strl.append("0");
	strl.heapSort();
	test("0",!charstring::compare(
				strl.getFirst()->getValue(),
				"0"));
	test("1",!charstring::compare(
				strl.getFirst()->getNext()->getValue(),
				"1"));
	test("2",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getValue(),
				"2"));
	test("3",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getValue(),
				"3"));
	test("4",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getValue(),
				"4"));
	test("5",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getNext()->
					getValue(),
				"5"));

	stdoutput.printf("heapSort() (after sorted from backwards)\n");
	strl.heapSort();
	test("0",!charstring::compare(
				strl.getFirst()->getValue(),
				"0"));
	test("1",!charstring::compare(
				strl.getFirst()->getNext()->getValue(),
				"1"));
	test("2",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getValue(),
				"2"));
	test("3",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getValue(),
				"3"));
	test("4",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getValue(),
				"4"));
	test("5",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getNext()->
					getValue(),
				"5"));



	strl.clear();
	stdoutput.printf("heapSort() (from random order)\n");
	strl.append("4");
	strl.append("5");
	strl.append("2");
	strl.append("6");
	strl.append("1");
	strl.append("7");
	strl.append("3");
	strl.append("0");
	strl.heapSort();
	test("0",!charstring::compare(
				strl.getFirst()->getValue(),
				"0"));
	test("1",!charstring::compare(
				strl.getFirst()->getNext()->getValue(),
				"1"));
	test("2",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getValue(),
				"2"));
	test("3",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getValue(),
				"3"));
	test("4",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getValue(),
				"4"));
	test("5",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getNext()->
					getValue(),
				"5"));

	stdoutput.printf("heapSort() (after sorted from random order)\n");
	strl.heapSort();
	test("0",!charstring::compare(
				strl.getFirst()->getValue(),
				"0"));
	test("1",!charstring::compare(
				strl.getFirst()->getNext()->getValue(),
				"1"));
	test("2",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getValue(),
				"2"));
	test("3",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getValue(),
				"3"));
	test("4",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getValue(),
				"4"));
	test("5",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getNext()->
					getValue(),
				"5"));
}
