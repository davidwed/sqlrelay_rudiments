// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/variablebuffer.h>
#include <rudiments/rawbuffer.h>
#include "test.C"

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	printf("variablebuffer:\n");

	// create a buffer
	variablebuffer	vb(10,10);

	// append some string sequences to the buffer and display the contents
	// of the buffer byte by byte
	vb.append((unsigned char *)"12345",5);
	vb.append((unsigned char *)"12345",5);
	vb.append((unsigned char *)"12345",5);
	vb.append((unsigned char *)"12345",5);
	vb.append((unsigned char *)"12345",5);
	test("append()",
		rawbuffer::compare(vb.getBuffer(),
					"1234512345123451234512345",25)==0);


	// write 66666 to the buffer at position 0 and display it's contents
	// byte by byte (the first 5 bytes should be overwritten)
	vb.setPosition(0);
	vb.write((unsigned char *)"66666",5);
	test("write()",
		rawbuffer::compare(vb.getBuffer(),
					"6666612345123451234512345",25)==0);


	// write 66666 to the buffer at position 30 and display it's contents
	// byte by byte, displaying nonprintable characters as .'s
	// (there should be a gap in the buffer now containing random data)
	vb.setPosition(30);
	vb.write((unsigned char *)"66666",5);
	test("setPosition(), write()",
		rawbuffer::compare(vb.getBuffer()+30,"66666",5)==0);


	// set the current position to 50
	vb.setPosition(50);

	// Append 12345 to the buffer and display it's contents byte by byte,
	// displaying nonprintable characters as .'s
	// Since we used append() instead of write(), the data should not be
	// written at position 50, but rather just at the current end of
	// the buffer.
	vb.append((unsigned char *)"12345",5);
	test("setPosition(), append()",
		rawbuffer::compare(vb.getBuffer()+30,"6666612345",10)==0);

	// Write 12345 to the buffer at the current position and display it's
	// contents byte by byte, displaying nonprintable characters as .'s
	// The current position should just be the end of the buffer, since
	// we just appended.  So calling write() here is equivalent to calling
	// append.
	vb.write((unsigned char *)"12345",5);
	test("write() after setPosition() and append()",
		rawbuffer::compare(vb.getBuffer()+30,"666661234512345",15)==0);


	// read 4 bytes from position 0 of the buffer and display them
	unsigned char	buffer[5];
	buffer[4]=(unsigned char)NULL;
	vb.setPosition(0);
	unsigned long	sizeread=vb.read(buffer,4);
	test("read into buffer",rawbuffer::compare(buffer,"6666",4)==0);

	// read 4 bytes from position 5 of the buffer and display them
	vb.setPosition(5);
	sizeread=vb.read(buffer,4);
	test("read into buffer",rawbuffer::compare(buffer,"1234",4)==0);

	// read 4 bytes from position 60 of the buffer and display them
	// (since this is off of the end of the buffer, nothing should be
	// displayed)
	vb.setPosition(60);
	sizeread=vb.read(buffer,4);
	test("read into buffer",sizeread==0);
}
