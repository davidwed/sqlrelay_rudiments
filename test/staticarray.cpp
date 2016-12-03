// Copyright (c) 2015  David Muse
// See the file COPYING for more information

#include <rudiments/staticarray.h>
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

	header("staticarray");

	// Create a staticarray
	staticarray<struct test,COUNT>	sa1;
	stdoutput.printf("create <test>...\n");
	test("create, getLength()",sa1.getLength()==COUNT);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("set values...\n");
	uint32_t	count=COUNT;
	uint32_t	i;
	for (i=0; i<count; i++) {
		char	*val=charstring::parseNumber(i);
		charstring::copy(sa1[i].str,val);
		delete[] val;
		sa1[i].uintval=i;
	}
	test("get values, getLength()",(uint32_t)sa1.getLength()==count);
	bool	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(sa1[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(sa1[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(sa1[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(sa1[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// copy using =
	stdoutput.printf("copy (using =)...\n");
	staticarray<struct test,COUNT>	sa1copy1;
	sa1copy1=sa1;
	test("get values, getLength()",(uint32_t)sa1copy1.getLength()==count);
	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(sa1copy1[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(sa1copy1[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(sa1copy1[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(sa1copy1[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// copy using constructor
	stdoutput.printf("copy (using constructor)...\n");
	staticarray<struct test,COUNT>	sa1copy2(sa1);
	test("get values, getLength()",(uint32_t)sa1copy2.getLength()==count);
	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(sa1copy2[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(sa1copy2[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(sa1copy2[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(sa1copy2[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	sa1.clear();
	test("clear, getLength()",sa1.getLength()==COUNT);
	stdoutput.printf("\n");



	// create
	staticarray<testclass,COUNT>	sa2;
	stdoutput.printf("create <testclass>...\n");
	test("create, getLength()",sa2.getLength()==COUNT);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<count; i++) {
		success=(sa2[i].getValue()==1);
	}
	test("get values, default",success);
	test("get values, getLength()",(uint32_t)sa2.getLength()==count);
	for (i=0; success && i<count; i++) {
		sa2[i].setValue(2);
	}
	for (i=0; success && i<count; i++) {
		success=(sa2[i].getValue()==2);
	}
	test("get values, forwards",success);
	for (i=count; success && i>0; i--) {
		success=(sa2[i-1].getValue()==2);
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	sa2.clear();
	test("clear, getLength()",sa2.getLength()==COUNT);
	stdoutput.printf("\n");



	// create
	staticarray<testclass *,COUNT>	sa3;
	stdoutput.printf("create <testclass *>...\n");
	test("create, getLength()",sa3.getLength()==COUNT);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<count; i++) {
		sa3[i]=new testclass();
	}
	for (i=0; success && i<count; i++) {
		success=(sa3[i]->getValue()==1);
	}
	test("get values, default",success);
	for (i=0; i<count; i++) {
		sa3[i]->setValue(2);
	}
	for (i=0; success && i<count; i++) {
		success=(sa3[i]->getValue()==2);
	}
	test("get values, forwards",success);
	for (i=count; success && i>0; i--) {
		success=(sa3[i-1]->getValue()==2);
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	for (i=0; i<count; i++) {
		delete sa3[i];
	}
	sa3.clear();
	test("clear, getLength()",sa3.getLength()==COUNT);
	stdoutput.printf("\n");



	// create
	staticarray< staticarray< uint32_t, COUNT > ,COUNT>	sa4;
	stdoutput.printf("create nested...\n");
	test("create, getLength()",sa4.getLength()==COUNT);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<count; i++) {
		for (uint32_t j=0; j<count; j++) {
			sa4[i][j]=i*count+j;
		}
	}
	for (i=0; success && i<count; i++) {
		for (uint32_t j=0; success && j<count; j++) {
			success=(sa4[i][j]==i*count+j);
		}
	}
	test("get values, forwards",success);
	for (i=count; success && i>0; i--) {
		for (uint32_t j=count; success && j>0; j--) {
			success=(sa4[i-1][j-1]==(i-1)*count+(j-1));
		}
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	sa4.clear();
	test("clear, getLength()",sa4.getLength()==COUNT);
	stdoutput.printf("\n");
}
