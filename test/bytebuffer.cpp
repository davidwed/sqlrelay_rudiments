// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/bytebuffer.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	stdoutput.printf("bytebuffer:\n");

	// create a buffer
	bytebuffer	bb(10,10);

	// append some string sequences to the buffer and display the contents
	// of the buffer byte by byte
	bb.append((unsigned char *)"12345",5);
	bb.append((unsigned char *)"12345",5);
	bb.append((unsigned char *)"12345",5);
	bb.append((unsigned char *)"12345",5);
	bb.append((unsigned char *)"12345",5);
	test("append()",
		rawbuffer::compare(bb.getBuffer(),
					"1234512345123451234512345",25)==0);


	// write 66666 to the buffer at position 0 and display it's contents
	// byte by byte (the first 5 bytes should be overwritten)
	bb.setPosition(0);
	bb.write((unsigned char *)"66666",5);
	test("write()",
		rawbuffer::compare(bb.getBuffer(),
					"6666612345123451234512345",25)==0);


	// write 66666 to the buffer at position 30 and display it's contents
	// byte by byte, displaying nonprintable characters as .'s
	// (there should be a gap in the buffer now containing random data)
	bb.setPosition(30);
	bb.write((unsigned char *)"66666",5);
	test("setPosition(), write()",
		rawbuffer::compare(bb.getBuffer()+30,"66666",5)==0);


	// set the current position to 50
	bb.setPosition(50);

	// Append 12345 to the buffer and display it's contents byte by byte,
	// displaying nonprintable characters as .'s
	// Since we used append() instead of write(), the data should not be
	// written at position 50, but rather just at the current end of
	// the buffer.
	bb.append((unsigned char *)"12345",5);
	test("setPosition(), append()",
		rawbuffer::compare(bb.getBuffer()+30,"6666612345",10)==0);

	// Write 12345 to the buffer at the current position and display it's
	// contents byte by byte, displaying nonprintable characters as .'s
	// The current position should just be the end of the buffer, since
	// we just appended.  So calling write() here is equivalent to calling
	// append.
	bb.write((unsigned char *)"12345",5);
	test("write() after setPosition() and append()",
		rawbuffer::compare(bb.getBuffer()+30,"666661234512345",15)==0);


	// read 4 bytes from position 0 of the buffer and display them
	unsigned char	buffer[5];
	buffer[4]=(unsigned char)NULL;
	bb.setPosition(0);
	ssize_t	sizeread=bb.read(buffer,4);
	test("read into buffer",rawbuffer::compare(buffer,"6666",4)==0);

	// read 4 bytes from position 5 of the buffer and display them
	bb.setPosition(5);
	sizeread=bb.read(buffer,4);
	test("read into buffer",rawbuffer::compare(buffer,"1234",4)==0);

	// read 4 bytes from position 60 of the buffer and display them
	// (since this is off of the end of the buffer, nothing should be
	// displayed)
	bb.setPosition(60);
	sizeread=bb.read(buffer,4);
	test("read into buffer",sizeread==0);
}
