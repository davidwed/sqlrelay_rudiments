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

#define COUNT 4096


int main(int argc, const char **argv) {

	header("dynamicarray");

	// Create a dynamicarray
	dynamicarray<struct test>	da1(25,10);
	stdoutput.printf("create <test>...\n");
	test("create, getLength()",da1.getLength()==0);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("set values...\n");
	uint32_t	count=COUNT;
	uint32_t	i;
	for (i=0; i<count; i++) {
		char	*val=charstring::parseNumber(i);
		charstring::copy(da1[i].str,val);
		delete[] val;
		da1[i].uintval=i;
	}
	test("get values, getLength()",(uint32_t)da1.getLength()==count);
	bool	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(da1[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(da1[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(da1[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(da1[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// copy using =
	stdoutput.printf("copy (using =)...\n");
	dynamicarray<struct test>	da1copy1(25,10);
	da1copy1=da1;
	test("get values, getLength()",(uint32_t)da1copy1.getLength()==count);
	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(da1copy1[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(da1copy1[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(da1copy1[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(da1copy1[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// copy using constructor
	stdoutput.printf("copy (using constructor)...\n");
	dynamicarray<struct test>	da1copy2(da1);
	test("get values, getLength()",(uint32_t)da1copy2.getLength()==count);
	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(da1copy2[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(da1copy2[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(da1copy2[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(da1copy2[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	da1.clear();
	test("clear, getLength()",da1.getLength()==0);
	stdoutput.printf("\n");



	// create
	dynamicarray<testclass>	da2(25,10);
	stdoutput.printf("create <testclass>...\n");
	test("create, getLength()",da2.getLength()==0);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<count; i++) {
		success=(da2[i].getValue()==1);
	}
	test("get values, default",success);
	test("get values, getLength()",(uint32_t)da2.getLength()==count);
	for (i=0; success && i<count; i++) {
		da2[i].setValue(2);
	}
	for (i=0; success && i<count; i++) {
		success=(da2[i].getValue()==2);
	}
	test("get values, forwards",success);
	for (i=count; success && i>0; i--) {
		success=(da2[i-1].getValue()==2);
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	da2.clear();
	test("clear, getLength()",da2.getLength()==0);
	stdoutput.printf("\n");



	// create
	dynamicarray<testclass *>	da3(25,10);
	stdoutput.printf("create <testclass *>...\n");
	test("create, getLength()",da3.getLength()==0);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<count; i++) {
		da3[i]=new testclass();
	}
	for (i=0; success && i<count; i++) {
		success=(da3[i]->getValue()==1);
	}
	test("get values, default",success);
	for (i=0; i<count; i++) {
		da3[i]->setValue(2);
	}
	for (i=0; success && i<count; i++) {
		success=(da3[i]->getValue()==2);
	}
	test("get values, forwards",success);
	for (i=count; success && i>0; i--) {
		success=(da3[i-1]->getValue()==2);
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	for (i=0; i<count; i++) {
		delete da3[i];
	}
	da3.clear();
	test("clear, getLength()",da3.getLength()==0);
	stdoutput.printf("\n");



	// create
	dynamicarray< dynamicarray< uint32_t > >	da4(25,10);
	stdoutput.printf("create nested...\n");
	test("create, getLength()",da4.getLength()==0);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<count; i++) {
		for (uint32_t j=0; j<count; j++) {
			da4[i][j]=i*count+j;
		}
	}
	for (i=0; success && i<count; i++) {
		for (uint32_t j=0; success && j<count; j++) {
			success=(da4[i][j]==i*count+j);
		}
	}
	test("get values, forwards",success);
	for (i=count; success && i>0; i--) {
		for (uint32_t j=count; success && j>0; j--) {
			success=(da4[i-1][j-1]==(i-1)*count+(j-1));
		}
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	da4.clear();
	test("clear, getLength()",da4.getLength()==0);
	stdoutput.printf("\n");
}
