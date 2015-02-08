// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/bytebuffer.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	stdoutput.printf("bytebuffer:\n");

	// Create a bytebuffer.
	bytebuffer	bb(10,10);
	stdoutput.printf("create...\n");
	test("create, getSize()",bb.getSize()==0);
	test("create, getPosition()",bb.getPosition()==0);
	test("create, getEnd()",bb.getEnd()==0);
	test("create, getActualSize()",bb.getActualSize()==10);
	stdoutput.printf("\n");

	// Append some string sequences to the buffer.
	stdoutput.printf("append 12345...\n");
	bb.append((unsigned char *)"12345",5);
	test("append(), getSize()",bb.getSize()==5);
	test("append(), getPosition()",bb.getPosition()==5);
	test("append(), getEnd()",bb.getEnd()==5);
	test("append(), getActualSize()",bb.getActualSize()==10);
	stdoutput.printf("append 12345...\n");
	bb.append((unsigned char *)"12345",5);
	test("append(), getSize()",bb.getSize()==10);
	test("append(), getPosition()",bb.getPosition()==10);
	test("append(), getEnd()",bb.getEnd()==10);
	test("append(), getActualSize()",bb.getActualSize()==10);
	stdoutput.printf("append 12345...\n");
	bb.append((unsigned char *)"12345",5);
	test("append(), getSize()",bb.getSize()==15);
	test("append(), getPosition()",bb.getPosition()==15);
	test("append(), getEnd()",bb.getEnd()==15);
	test("append(), getActualSize()",bb.getActualSize()==20);
	stdoutput.printf("append 12345...\n");
	bb.append((unsigned char *)"12345",5);
	test("append(), getSize()",bb.getSize()==20);
	test("append(), getPosition()",bb.getPosition()==20);
	test("append(), getEnd()",bb.getEnd()==20);
	test("append(), getActualSize()",bb.getActualSize()==20);
	stdoutput.printf("append 12345...\n");
	bb.append((unsigned char *)"12345",5);
	test("append(), getSize()",bb.getSize()==25);
	test("append(), getPosition()",bb.getPosition()==25);
	test("append(), getEnd()",bb.getEnd()==25);
	test("append(), getActualSize()",bb.getActualSize()==30);
	test("getBuffer()",
		bytestring::compare(bb.getBuffer(),
					"1234512345123451234512345",25)==0);
	test("getBuffer(), getSize()",bb.getSize()==25);
	test("getBuffer(), getPosition()",bb.getPosition()==25);
	test("getBuffer(), getEnd()",bb.getEnd()==25);
	test("getBuffer(), getActualSize()",bb.getActualSize()==25);
	stdoutput.printf("\n");


	// Write 66666 to the buffer at position 0.
	// (the first 5 bytes should be overwritten)
	stdoutput.printf("write 66666 at position 0...\n");
	bb.setPosition(0);
	test("setPosition(), getPosition()",bb.getPosition()==0);
	bb.write((unsigned char *)"66666",5);
	test("write(), getSize()",bb.getSize()==25);
	test("write(), getPosition()",bb.getPosition()==5);
	test("write(), getEnd()",bb.getEnd()==25);
	test("write(), getActualSize()",bb.getActualSize()==25);
	test("getBuffer()",
		bytestring::compare(bb.getBuffer(),
					"6666612345123451234512345",25)==0);
	test("getBuffer(), getSize()",bb.getSize()==25);
	test("getBuffer(), getPosition()",bb.getPosition()==5);
	test("getBuffer(), getEnd()",bb.getEnd()==25);
	test("getBuffer(), getActualSize()",bb.getActualSize()==25);
	stdoutput.printf("\n");


	// Write 66666 to the buffer at position 30.
	// (there should be a gap in the buffer now containing random data)
	stdoutput.printf("write 66666 at position 30...\n");
	bb.setPosition(30);
	test("setPosition(), getPosition()",bb.getPosition()==30);
	bb.write((unsigned char *)"66666",5);
	test("write(), getSize()",bb.getSize()==35);
	test("write(), getPosition()",bb.getPosition()==35);
	test("write(), getEnd()",bb.getEnd()==35);
	test("write(), getActualSize()",bb.getActualSize()==35);
	test("getBuffer()",
		bytestring::compare(bb.getBuffer()+30,"66666",5)==0);
	test("getBuffer(), getSize()",bb.getSize()==35);
	test("getBuffer(), getPosition()",bb.getPosition()==35);
	test("getBuffer(), getEnd()",bb.getEnd()==35);
	test("getBuffer(), getActualSize()",bb.getActualSize()==35);
	stdoutput.printf("\n");

	// Set the current position to 50.
	// Append 12345 to the buffer.
	// Since we used append() instead of write(), the data should not be
	// written at position 50, but rather just at the current end of
	// the buffer.
	stdoutput.printf("append 66666 after setting position to 50...\n");
	bb.setPosition(50);
	test("setPosition(), getPosition()",bb.getPosition()==50);
	bb.append((unsigned char *)"12345",5);
	test("append(), getSize()",bb.getSize()==40);
	test("append(), getPosition()",bb.getPosition()==40);
	test("append(), getEnd()",bb.getEnd()==40);
	test("append(), getActualSize()",bb.getActualSize()==45);
	test("getBuffer()",
		bytestring::compare(bb.getBuffer()+30,"6666612345",10)==0);
	test("getBuffer(), getSize()",bb.getSize()==40);
	test("getBuffer(), getPosition()",bb.getPosition()==40);
	test("getBuffer(), getEnd()",bb.getEnd()==40);
	test("getBuffer(), getActualSize()",bb.getActualSize()==40);
	stdoutput.printf("\n");

	// Write 12345 to the buffer at the current position.
	// The current position should just be the end of the buffer, since
	// we just appended.  So calling write() here is equivalent to calling
	// append.
	stdoutput.printf("write 12345 at current position...\n");
	bb.write((unsigned char *)"12345",5);
	test("append(), getSize()",bb.getSize()==45);
	test("append(), getPosition()",bb.getPosition()==45);
	test("append(), getEnd()",bb.getEnd()==45);
	test("append(), getActualSize()",bb.getActualSize()==50);
	test("getBuffer()",
		bytestring::compare(bb.getBuffer()+30,"666661234512345",15)==0);
	test("getBuffer(), getSize()",bb.getSize()==45);
	test("getBuffer(), getPosition()",bb.getPosition()==45);
	test("getBuffer(), getEnd()",bb.getEnd()==45);
	test("getBuffer(), getActualSize()",bb.getActualSize()==45);
	stdoutput.printf("\n");


	// read 4 bytes from position 0 of the buffer
	stdoutput.printf("read from position 0...\n");
	unsigned char	buffer[5];
	buffer[4]=(unsigned char)NULL;
	bb.setPosition(0);
	test("setPosition(), getPosition()",bb.getPosition()==0);
	ssize_t	sizeread=bb.read(buffer,4);
	test("read(), size",sizeread==4);
	test("read(), data",bytestring::compare(buffer,"6666",4)==0);
	stdoutput.printf("\n");

	// read 4 bytes from position 5 of the buffer
	stdoutput.printf("read from position 5...\n");
	bb.setPosition(5);
	test("setPosition(), getPosition()",bb.getPosition()==5);
	sizeread=bb.read(buffer,4);
	test("read(), size",sizeread==4);
	test("read(), data",bytestring::compare(buffer,"1234",4)==0);
	stdoutput.printf("\n");

	// read 4 bytes from position 60 of the buffer
	// (since this is off of the end of the buffer, nothing should be read)
	stdoutput.printf("read from position 60 (off the end)...\n");
	bb.setPosition(60);
	test("setPosition(), getPosition()",bb.getPosition()==60);
	sizeread=bb.read(buffer,4);
	test("read(), size",sizeread==0);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear\n");
	bb.clear();
	test("clear(), getSize()",bb.getSize()==0);
	test("clear(), getPosition()",bb.getPosition()==0);
	test("clear(), getEnd()",bb.getEnd()==0);
	test("clear(), getActualSize()",bb.getActualSize()==45);
	stdoutput.printf("\n");

	// detach
	stdoutput.printf("detach\n");
	for (uint64_t i=0; i<10; i++) {
		bb.append("1234567890");
	}
	test("append(), getSize()",bb.getSize()==100);
	test("append(), getPosition()",bb.getPosition()==100);
	test("append(), getEnd()",bb.getEnd()==100);
	test("append(), getActualSize()",bb.getActualSize()==105);
	unsigned char	*detachedbuffer=bb.detachBuffer();
	test("detach(), getSize()",bb.getSize()==0);
	test("detach(), getPosition()",bb.getPosition()==0);
	test("detach(), getEnd()",bb.getEnd()==0);
	test("detach(), getActualSize()",bb.getActualSize()==45);
	bool	validdata=true;
	for (uint64_t j=0; j<100; j=j+10) {
		if (bytestring::compare(detachedbuffer+j,"1234567890",10)) {
			stdoutput.printf("	"
				"data invalid at position %lld\n",j);
			validdata=false;
			break;
		}
	}
	delete[] detachedbuffer;
	test("detach(), data",validdata);
	stdoutput.printf("\n");
	

	// formatted writes
	stdoutput.printf("formatted writes\n");
	bb.appendFormatted("%0.5f",12.12345);
	test("appendFormatted(), data",
		!bytestring::compare(bb.getBuffer(),"12.12345",8));
	bb.writeFormatted("%0.5f",12.12345);
	test("writeFormatted(), data",
		!bytestring::compare(bb.getBuffer(),"12.1234512.12345",16));
	stdoutput.printf("\n");


	// read past the end
	stdoutput.printf("read past the end\n");
	bb.setPosition(0);
	unsigned char	readbuf[100];
	test("read(), length",bb.read(readbuf,100)==16);
	test("read(), data",
		!bytestring::compare(readbuf,"12.1234512.12345",16));
	stdoutput.printf("\n");


	// copy constructor
	stdoutput.printf("copy constructor\n");
	bytebuffer	cc(bb);
	test("copy constructor, getSize()",cc.getSize()==16);
	test("copy constructor, getPosition()",cc.getPosition()==16);
	test("copy constructor, getEnd()",cc.getEnd()==16);
	test("copy constructor, getActualSize()",cc.getActualSize()==45);
	test("copy constructor, data",
		!bytestring::compare(cc.getBuffer(),"12.1234512.12345",16));
	stdoutput.printf("\n");

	// = operator
	stdoutput.printf("= operator\n");
	bytebuffer	dd(100,100);
	dd=bb;
	test("= operator, getSize()",dd.getSize()==16);
	test("= operator, getPosition()",dd.getPosition()==16);
	test("= operator, getEnd()",dd.getEnd()==16);
	test("= operator, getActualSize()",dd.getActualSize()==45);
	test("= operator, data",
		!bytestring::compare(dd.getBuffer(),"12.1234512.12345",16));
	stdoutput.printf("\n");

	// initial contents
	stdoutput.printf("initial contents\n");
	unsigned char	*initialcontents=new unsigned char[100];
	for (uint16_t k=0; k<10; k=k+10) {
		bytestring::copy(initialcontents+k,"1234567890",10);
	}
	unsigned char	*iccopy=
		(unsigned char *)bytestring::duplicate(initialcontents,100);
	bytebuffer	ee(initialcontents,100,20);
	test("initial contents, getSize()",ee.getSize()==100);
	test("initial contents, getPosition()",ee.getPosition()==100);
	test("initial contents, getEnd()",ee.getEnd()==100);
	test("initial contents, getActualSize()",ee.getActualSize()==100);
	test("initial contents, data",
		!bytestring::compare(ee.getBuffer(),iccopy,100));
	test("getBuffer(), getSize()",ee.getSize()==100);
	test("getBuffer(), getPosition()",ee.getPosition()==100);
	test("getBuffer(), getEnd()",ee.getEnd()==100);
	test("getBuffer(), getActualSize()",ee.getActualSize()==100);
	ee.append("1234567890");
	test("append(), getSize()",ee.getSize()==110);
	test("append(), getPosition()",ee.getPosition()==110);
	test("append(), getEnd()",ee.getEnd()==110);
	test("append(), getActualSize()",ee.getActualSize()==120);
	test("append(), data",
		!bytestring::compare(ee.getBuffer(),iccopy,100) &&
		!bytestring::compare(ee.getBuffer()+100,"1234567890",10));
	stdoutput.printf("\n");

	// large writes
	bb.clear();
	uint16_t	count=10;
	uint64_t	size=1024*1024*10;
	unsigned char	*data=(unsigned char *)"1234567890";
	stdoutput.printf("%hd, %lld byte writes\n",count,size);
	for (uint16_t l=0; l<count; l++) {

		for (uint64_t m=0; m<size/10; m++) {
			bb.write(data,10);
		}

		const unsigned char	*buffer=bb.getBuffer();
		bool	validdata=true;
		for (uint64_t n=0; n<size; n=n+10) {
			if (bytestring::compare(buffer+n,data,10)) {
				stdoutput.printf("	"
					"data invalid at position %lld\n",n);
				validdata=false;
				break;
			}
		}

		test("write(), get various stats",validdata &&
			(bb.getSize()==size) && (bb.getPosition()==size) &&
			(bb.getEnd()==size) && (bb.getActualSize()==size));

		unsigned char	buf[10];
		validdata=true;
		for (uint64_t o=0; o<size; o=o+10) {
			bb.setPosition(o);
			bb.read(buf,10);
			if (bytestring::compare(buf,data,10)) {
				stdoutput.printf("	"
					"data invalid at position %lld\n",o);
				validdata=false;
				break;
			}
		}

		test("read(), data",validdata);

		bb.clear();
	}
	stdoutput.printf("\n");
}
