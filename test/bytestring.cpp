// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("bytestring");

	// duplicate
	stdoutput.printf("duplicate...\n");
	unsigned char	*dup=(unsigned char *)bytestring::duplicate(
						(const void *)"1234567890",10);
	test("duplicate(), compare()",
			!bytestring::compare(dup,"1234567890",10));
	delete[] dup;
	dup=(unsigned char *)bytestring::duplicate(
					(const void *)"2345678901",10);
	test("duplicate(), compare()",
			!bytestring::compare(dup,"2345678901",10));
	delete[] dup;
	dup=(unsigned char *)bytestring::duplicate(
					(const void *)"3456789012",10);
	test("duplicate(), compare()",
			!bytestring::compare(dup,"3456789012",10));
	delete[] dup;
	stdoutput.printf("\n");


	// copy
	stdoutput.printf("copy...\n");
	unsigned char	data[10];
	bytestring::copy(data,"1234567890",10);
	test("copy(), compare()",!bytestring::compare(data,"1234567890",10));
	bytestring::copy(data,"2345678901",10);
	test("copy(), compare()",!bytestring::compare(data,"2345678901",10));
	bytestring::copy(data,"3456789012",10);
	test("copy(), compare()",!bytestring::compare(data,"3456789012",10));
	stdoutput.printf("\n");
	

	// copy with overlap
	stdoutput.printf("copyWithOverlap...\n");
	bytestring::copy(data,"1234567890",10);
	bytestring::copyWithOverlap(data,data+2,5);
	test("copyWithOverlap(), compare()",
				!bytestring::compare(data,"3456767890",10));
	bytestring::copyWithOverlap(data,data+2,5);
	test("copyWithOverlap(), compare()",
				!bytestring::compare(data,"5676767890",10));
	bytestring::copyWithOverlap(data,data+2,5);
	test("copyWithOverlap(), compare()",
				!bytestring::compare(data,"7676767890",10));
	stdoutput.printf("\n");


	// copy until
	stdoutput.printf("copyUntil...\n");
	bytestring::copy(data,"0000000000",10);
	bytestring::copyUntil(data,"1234567890",'5',10);
	test("copyUntil(), compare()",
				!bytestring::compare(data,"1234500000",10));
	bytestring::copyUntil(data,"1234567890",'6',10);
	test("copyUntil(), compare()",
				!bytestring::compare(data,"1234560000",10));
	bytestring::copyUntil(data,"1234567890",'7',10);
	test("copyUntil(), compare()",
				!bytestring::compare(data,"1234567000",10));
	stdoutput.printf("\n");

	// copy swap bytes
	stdoutput.printf("copySwapBytes...\n");
	bytestring::copy(data,"0000000000",10);
	bytestring::copySwapBytes(data,"1234567890",10);
	test("copySwapBytes(), compare()",
				!bytestring::compare(data,"2143658709",10));
	stdoutput.printf("\n");

	// set
	stdoutput.printf("set...\n");
	bytestring::set(data,'0',10);
	test("set(), compare()",!bytestring::compare(data,"0000000000",10));
	bytestring::set(data,'1',5);
	test("set(), compare()",!bytestring::compare(data,"1111100000",10));
	bytestring::set(data,'2',2);
	test("set(), compare()",!bytestring::compare(data,"2211100000",10));
	stdoutput.printf("\n");

	// zero
	stdoutput.printf("zero...\n");
	bytestring::set(data,'1',10);
	bytestring::zero(data,5);
	unsigned char	comp[]={0,0,0,0,0,'1','1','1','1','1'};
	test("set(), compare()",!bytestring::compare(data,comp,10));
	stdoutput.printf("\n");

	// find first
	stdoutput.printf("findFirst...\n");
	bytestring::copy(data,"1234567890",10);
	test("findFirst(), compare()",
		!bytestring::compare(
			bytestring::findFirst(data,10,"123",3),
			"1234567890",10));
	test("findFirst(), compare()",
		!bytestring::compare(
			bytestring::findFirst(data,10,"234",3),
			"234567890",9));
	test("findFirst(), compare()",
		!bytestring::compare(
			bytestring::findFirst(data,10,"567",3),
			"567890",6));
	test("findFirst(), compare()",
		!bytestring::compare(
			bytestring::findFirst(data,10,"678",3),
			"67890",5));
	stdoutput.printf("\n");

	// find last
	stdoutput.printf("findLast...\n");
	bytestring::copy(data,"1212120000",10);
	test("findLast(), compare()",
		!bytestring::compare(
			bytestring::findLast(data,10,"12",2),
			"120000",6));
	test("findLast(), compare()",
		!bytestring::compare(
			bytestring::findLast(data,10,"121",3),
			"12120000",8));
	stdoutput.printf("\n");
}
