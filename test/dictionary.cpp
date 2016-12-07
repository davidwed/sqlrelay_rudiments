// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/dictionary.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("dictionary");

	// string/string
	stdoutput.printf("string keys, string values:\n");
	dictionary< const char *, const char * >	strstrdict;

	// add values
	strstrdict.setValue("1","one");
	strstrdict.setValue("2","one");
	strstrdict.setValue("3","one");
	strstrdict.setValue("4","one");
	test("setValue()/getValue() 1",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("setValue()/getValue() 2",
		!charstring::compare(strstrdict.getValue("2"),"one"));
	test("setValue()/getValue() 3",
		!charstring::compare(strstrdict.getValue("3"),"one"));
	test("setValue()/getValue() 4",
		!charstring::compare(strstrdict.getValue("4"),"one"));
	test("getValue(): 5",!strstrdict.getValue("5"));

	// override values
	strstrdict.setValue("1","one");
	strstrdict.setValue("2","two");
	strstrdict.setValue("3","three");
	strstrdict.setValue("4","four");
	test("setValue()/getValue() 1,one (override)",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("setValue()/getValue() 2,two (override)",
		!charstring::compare(strstrdict.getValue("2"),"two"));
	test("setValue()/getValue() 3,three (override)",
		!charstring::compare(strstrdict.getValue("3"),"three"));
	test("setValue()/getValue() 4,four (override)",
		!charstring::compare(strstrdict.getValue("4"),"four"));
	test("getValue(): 5,NULL",!strstrdict.getValue("5"));

	// keys
	test("getKeys()->getLength",
		strstrdict.getKeys()->getLength()==4);
	test("getValue(): 1,one",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("getValue(): 2,two",
		!charstring::compare(strstrdict.getValue("2"),"two"));
	test("getValue(): 3,three",
		!charstring::compare(strstrdict.getValue("3"),"three"));
	test("getValue(): 4,four",
		!charstring::compare(strstrdict.getValue("4"),"four"));
	test("getValue(): 5,NULL",!strstrdict.getValue("5"));

	// get value (reference)
	const char	*val=NULL;
	strstrdict.getValue("1",&val);
	test("getValue() by reference: 1,one",
		!charstring::compare(val,"one"));
	strstrdict.getValue("2",&val);
	test("getValue() by reference: 2,two",
		!charstring::compare(val,"two"));
	strstrdict.getValue("3",&val);
	test("getValue() by reference: 3,three",
		!charstring::compare(val,"three"));
	strstrdict.getValue("4",&val);
	test("getValue() by reference: 4,four",
		!charstring::compare(val,"four"));
	test("getValue() by reference: 5,NULL",!strstrdict.getValue("5"));

	// get node
	test("getNode() 1,one",
		!charstring::compare(
			strstrdict.getNode("1")->getValue(),"one"));
	test("getNode() 2,two",
		!charstring::compare(
			strstrdict.getNode("2")->getValue(),"two"));
	test("getNode() 3,three",
		!charstring::compare(
			strstrdict.getNode("3")->getValue(),"three"));
	test("getNode() 4,four",
		!charstring::compare(
			strstrdict.getNode("4")->getValue(),"four"));
	test("getNode() 5,NULL",!strstrdict.getNode("5"));

	// keys
	linkedlistnode< const char * > *strkey=strstrdict.getKeys()->getFirst();
	test("before start key",!strkey->getPrevious());
	test("key 1",!charstring::compare(strkey->getValue(),"1"));
	strkey=strkey->getNext();
	test("key 2",!charstring::compare(strkey->getValue(),"2"));
	strkey=strkey->getNext();
	test("key 3",!charstring::compare(strkey->getValue(),"3"));
	strkey=strkey->getNext();
	test("key 4",!charstring::compare(strkey->getValue(),"4"));
	test("after end key",!strkey->getNext());

	// remove
	strstrdict.remove("3");
	test("remove 3: getKeys()->getLength",
		strstrdict.getKeys()->getLength()==3);
	test("remove 3: getValue(): 1,one",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("remove 3: getValue(): 2,two",
		!charstring::compare(strstrdict.getValue("2"),"two"));
	test("remove 3: getValue(): 3,NULL",
		!strstrdict.getValue("3"));
	test("remove 3: getValue(): 4,four",
		!charstring::compare(strstrdict.getValue("4"),"four"));

	strstrdict.remove("2");
	test("remove 2: getKeys()->getLength",
		strstrdict.getKeys()->getLength()==2);
	test("remove 2: getValue(): 1,one",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("remove 2: getValue(): 2,NULL",
		!strstrdict.getValue("2"));
	test("remove 2: getValue(): 3,NULL",
		!strstrdict.getValue("3"));
	test("remove 2: getValue(): 4,four",
		!charstring::compare(strstrdict.getValue("4"),"four"));

	strstrdict.remove("1");
	test("remove 1: getKeys()->getLength",
		strstrdict.getKeys()->getLength()==1);
	test("remove 1: getValue(): 1,NULL",
		!strstrdict.getValue("1"));
	test("remove 1: getValue(): 2,NULL",
		!strstrdict.getValue("2"));
	test("remove 1: getValue(): 3,NULL",
		!strstrdict.getValue("3"));
	test("remove 1: getValue(): 4,four",
		!charstring::compare(strstrdict.getValue("4"),"four"));

	strstrdict.remove("4");
	test("remove 4: getKeys()->getLength",
		strstrdict.getKeys()->getLength()==0);
	test("remove 4: getValue(): 1,NULL",
		!strstrdict.getValue("1"));
	test("remove 4: getValue(): 2,NULL",
		!strstrdict.getValue("2"));
	test("remove 4: getValue(): 3,NULL",
		!strstrdict.getValue("3"));
	test("remove 4: getValue(): 4,NULL",
		!strstrdict.getValue("4"));

	// keys
	test("empty keys",!strstrdict.getKeys()->getFirst());

	// clear
	strstrdict.setValue("1","one");
	strstrdict.setValue("2","two");
	strstrdict.setValue("3","three");
	strstrdict.setValue("4","four");
	strstrdict.clear();
	test("clear: getKeys()->getLength",
		strstrdict.getKeys()->getLength()==0);
	stdoutput.printf("\n");



	// int/string
	stdoutput.printf("int keys, string values:\n");
	dictionary< int64_t , const char * >	intstrdict;

	// add values
	intstrdict.setValue(1,"one");
	intstrdict.setValue(2,"one");
	intstrdict.setValue(3,"one");
	intstrdict.setValue(4,"one");
	test("setValue()/getValue() 1",
		!charstring::compare(intstrdict.getValue(1),"one"));
	test("setValue()/getValue() 2",
		!charstring::compare(intstrdict.getValue(2),"one"));
	test("setValue()/getValue() 3",
		!charstring::compare(intstrdict.getValue(3),"one"));
	test("setValue()/getValue() 4",
		!charstring::compare(intstrdict.getValue(4),"one"));
	test("getValue(): 5",!intstrdict.getValue(5));

	// override values
	intstrdict.setValue(1,"one");
	intstrdict.setValue(2,"two");
	intstrdict.setValue(3,"three");
	intstrdict.setValue(4,"four");
	test("setValue()/getValue() 1,one (override)",
		!charstring::compare(intstrdict.getValue(1),"one"));
	test("setValue()/getValue() 2,two (override)",
		!charstring::compare(intstrdict.getValue(2),"two"));
	test("setValue()/getValue() 3,three (override)",
		!charstring::compare(intstrdict.getValue(3),"three"));
	test("setValue()/getValue() 4,four (override)",
		!charstring::compare(intstrdict.getValue(4),"four"));
	test("getValue(): 5,NULL",!intstrdict.getValue(5));

	// keys
	test("getKeys()->getLength",
		intstrdict.getKeys()->getLength()==4);
	test("getValue(): 1,one",
		!charstring::compare(intstrdict.getValue(1),"one"));
	test("getValue(): 2,two",
		!charstring::compare(intstrdict.getValue(2),"two"));
	test("getValue(): 3,three",
		!charstring::compare(intstrdict.getValue(3),"three"));
	test("getValue(): 4,four",
		!charstring::compare(intstrdict.getValue(4),"four"));
	test("getValue(): 5,NULL",!intstrdict.getValue(5));

	// get value (reference)
	intstrdict.getValue(1,&val);
	test("getValue() by reference: 1,one",
		!charstring::compare(val,"one"));
	intstrdict.getValue(2,&val);
	test("getValue() by reference: 2,two",
		!charstring::compare(val,"two"));
	intstrdict.getValue(3,&val);
	test("getValue() by reference: 3,three",
		!charstring::compare(val,"three"));
	intstrdict.getValue(4,&val);
	test("getValue() by reference: 4,four",
		!charstring::compare(val,"four"));
	test("getValue() by reference: 5,NULL",!intstrdict.getValue(5));

	// get node
	test("getNode() 1,one",
		!charstring::compare(
			intstrdict.getNode(1)->getValue(),"one"));
	test("getNode() 2,two",
		!charstring::compare(
			intstrdict.getNode(2)->getValue(),"two"));
	test("getNode() 3,three",
		!charstring::compare(
			intstrdict.getNode(3)->getValue(),"three"));
	test("getNode() 4,four",
		!charstring::compare(
			intstrdict.getNode(4)->getValue(),"four"));
	test("getNode() 5,NULL",!intstrdict.getNode(5));

	// keys
	linkedlistnode< int64_t > *intkey=intstrdict.getKeys()->getFirst();
	test("before start key",!intkey->getPrevious());
	test("key 1",intkey->getValue()==1);
	intkey=intkey->getNext();
	test("key 2",intkey->getValue()==2);
	intkey=intkey->getNext();
	test("key 3",intkey->getValue()==3);
	intkey=intkey->getNext();
	test("key 4",intkey->getValue()==4);
	test("after end key",!intkey->getNext());

	// remove
	intstrdict.remove(3);
	test("remove 3: getKeys()->getLength",
		intstrdict.getKeys()->getLength()==3);
	test("remove 3: getValue(): 1,one",
		!charstring::compare(intstrdict.getValue(1),"one"));
	test("remove 3: getValue(): 2,two",
		!charstring::compare(intstrdict.getValue(2),"two"));
	test("remove 3: getValue(): 3,NULL",
		!intstrdict.getValue(3));
	test("remove 3: getValue(): 4,four",
		!charstring::compare(intstrdict.getValue(4),"four"));

	intstrdict.remove(2);
	test("remove 2: getKeys()->getLength",
		intstrdict.getKeys()->getLength()==2);
	test("remove 2: getValue(): 1,one",
		!charstring::compare(intstrdict.getValue(1),"one"));
	test("remove 2: getValue(): 2,NULL",
		!intstrdict.getValue(2));
	test("remove 2: getValue(): 3,NULL",
		!intstrdict.getValue(3));
	test("remove 2: getValue(): 4,four",
		!charstring::compare(intstrdict.getValue(4),"four"));

	intstrdict.remove(1);
	test("remove 1: getKeys()->getLength",
		intstrdict.getKeys()->getLength()==1);
	test("remove 1: getValue(): 1,NULL",
		!intstrdict.getValue(1));
	test("remove 1: getValue(): 2,NULL",
		!intstrdict.getValue(2));
	test("remove 1: getValue(): 3,NULL",
		!intstrdict.getValue(3));
	test("remove 1: getValue(): 4,four",
		!charstring::compare(intstrdict.getValue(4),"four"));

	intstrdict.remove(4);
	test("remove 4: getKeys()->getLength",
		intstrdict.getKeys()->getLength()==0);
	test("remove 4: getValue(): 1,NULL",
		!intstrdict.getValue(1));
	test("remove 4: getValue(): 2,NULL",
		!intstrdict.getValue(2));
	test("remove 4: getValue(): 3,NULL",
		!intstrdict.getValue(3));
	test("remove 4: getValue(): 4,NULL",
		!intstrdict.getValue(4));

	// keys
	test("empty keys",!intstrdict.getKeys()->getFirst());

	// clear
	intstrdict.setValue(1,"one");
	intstrdict.setValue(2,"two");
	intstrdict.setValue(3,"three");
	intstrdict.setValue(4,"four");
	intstrdict.clear();
	test("clear: getKeys()->getLength",
		intstrdict.getKeys()->getLength()==0);
	stdoutput.printf("\n");
}
