// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a new string buffer
	stringbuffer	*str=new stringbuffer();

	// append a NULL to the buffer
	str->append((char *)NULL);

	// append "hello there world" to the buffer in 3 parts
	str->append("hello ")->append("there ")->append("world ");

	// display the length and contents of the buffer
	stdoutput.printf("length: %d\n\"%s\"\n",
			str->getStringLength(),str->getString());


	// append some long integers to the buffer
	str->append((int32_t)1)->append(" ");
	str->append((int32_t)2)->append(" ");
	str->append((int32_t)3)->append(" ");
	str->append((int32_t)4)->append(" ");
	str->append((int32_t)5)->append(" ");

	// display the length and contents of the buffer
	stdoutput.printf("length: %d\n\"%s\"\n",
			str->getStringLength(),str->getString());

	str->append(" ");


	// append some floating point numbers to the buffer
	str->append(1.1,1,0)->append(" ");
	str->append(2.02,2,0)->append(" ");
	str->append(3.003,3,0)->append(" ");
	str->append(4.0004,4,0)->append(" ");
	str->append(5.00005,5,0)->append(" ");

	// display the length and contents of the buffer
	stdoutput.printf("length: %d\n\"%s\"\n",
			str->getStringLength(),str->getString());


	// clear the buffer
	str->clear();


	// append 1024 *'s to the buffer and display it's length and contents
	for (int i=0; i<1024; i++) {
		str->append('*');
	}
	stdoutput.printf("length: %d\n%s\n",
			str->getStringLength(),str->getString());

	// delete the buffer
	delete str;




	// create another buffer
	stringbuffer	*sb=new stringbuffer();

	// append some string sequences to the buffer and display the contents
	// of the buffer byte by byte
	sb->append("12345");
	sb->append("12345");
	sb->append("12345");
	sb->append("12345");
	sb->append("12345");
	for (unsigned int i=0; i<sb->getStringLength(); i++) {
		stdoutput.printf("%c",sb->getString()[i]);
	}
	stdoutput.printf("\n");


	// write 66666 to the buffer at position 0 and display it's contents
	// byte by byte (the first 5 bytes should be overwritten)
	sb->setPosition(0);
	sb->write("66666");
	for (unsigned int i=0; i<sb->getStringLength(); i++) {
		stdoutput.printf("%c",sb->getString()[i]);
	}
	stdoutput.printf("\n");


	// write 66666 to the buffer at position 30 and display it's contents
	// byte by byte, displaying nonprintable characters as .'s
	// (there should be a gap in the buffer now containing random data)
	sb->setPosition(30);
	sb->write("66666");
	for (int i=0; i<35; i++) {
		if (sb->getString()[i]>=' ' && sb->getString()[i]<='~') {
			stdoutput.printf("%c",sb->getString()[i]);
		} else {
			stdoutput.printf(".");
		}
	}
	stdoutput.printf("\n");


	// set the current position to 50
	sb->setPosition(50);

	// Append 12345 to the buffer and display it's contents byte by byte,
	// displaying nonprintable characters as .'s
	// Since we used append() instead of write(), the data should not be
	// written at position 50, but rather just at the current end of
	// the buffer.
	sb->append("12345");
	for (int i=0; i<55; i++) {
		if (sb->getString()[i]>=' ' && sb->getString()[i]<='~') {
			stdoutput.printf("%c",sb->getString()[i]);
		} else {
			stdoutput.printf(".");
		}
	}
	stdoutput.printf("\n");

	// Write 12345 to the buffer at the current position and display it's
	// contents byte by byte, displaying nonprintable characters as .'s
	// The current position should just be the end of the buffer, since
	// we just appended.  So calling write() here is equivalent to calling
	// append.
	sb->write("12345");
	for (int i=0; i<55; i++) {
		if (sb->getString()[i]>=' ' && sb->getString()[i]<='~') {
			stdoutput.printf("%c",sb->getString()[i]);
		} else {
			stdoutput.printf(".");
		}
	}
	stdoutput.printf("\n");


	// clear the buffer
	sb->clear();

	// append 1024 0's to the buffer and display it's length and contents
	for (int i=0; i<1024; i++) {
		sb->append("0");
	}
	stdoutput.printf("length: %d\n%s\n",str->getStringLength(),str->getString());

	delete sb;
}
