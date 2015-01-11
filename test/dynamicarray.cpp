// Copyright (c) 2015  David Muse
// See the file COPYING for more information

#include <rudiments/dynamicarray.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

struct test {
	char		str[100];
	uint32_t	uintval;
};

class testclass {
	public:
			testclass();
		uint32_t	getValue();
		void		setValue(uint32_t value);
	private:
		uint32_t	value;
};

testclass::testclass() {
	value=1;
}

uint32_t testclass::getValue() {
	return value;
}

void testclass::setValue(uint32_t value) {
	this->value=value;
}


int main(int argc, const char **argv) {

	stdoutput.printf("dynamicarray:\n");

	// Create a dynamicarray
	dynamicarray<struct test>	da(25,10);
	stdoutput.printf("create <test>...\n");
	test("create, getLength()",da.getLength()==0);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("set values...\n");
	uint32_t	count=1024*1024;
	for (uint32_t i=0; i<count; i++) {
		char	*val=charstring::parseNumber(i);
		charstring::copy(da[i].str,val);
		delete[] val;
		da[i].uintval=i;
	}
	test("get values, getLength()",(uint32_t)da.getLength()==count);
	bool	success=true;
	for (uint32_t i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(da[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (uint32_t i=0; success && i<count; i++) {
		success=(da[i].uintval==i);
	}
	test("get values, int",success);
	for (uint32_t i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(da[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (uint32_t i=count; success && i>0; i--) {
		success=(da[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	da.clear();
	test("clear, getLength()",da.getLength()==0);
	stdoutput.printf("\n");



	// create
	dynamicarray<testclass>	da1(25,10);
	stdoutput.printf("create <testclass>...\n");
	test("create, getLength()",da1.getLength()==0);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=1024*1024;
	success=true;
	for (uint32_t i=0; i<count; i++) {
		success=(da1[i].getValue()==1);
	}
	test("get values, default",success);
	test("get values, getLength()",(uint32_t)da1.getLength()==count);
	for (uint32_t i=0; success && i<count; i++) {
		da1[i].setValue(2);
	}
	for (uint32_t i=0; success && i<count; i++) {
		success=(da1[i].getValue()==2);
	}
	test("get values, forwards",success);
	for (uint32_t i=count; success && i>0; i--) {
		success=(da1[i-1].getValue()==2);
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	da1.clear();
	test("clear, getLength()",da1.getLength()==0);
	stdoutput.printf("\n");
}
