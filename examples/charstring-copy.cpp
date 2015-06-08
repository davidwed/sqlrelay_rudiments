#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	char		buffer[32];


	// zero the buffer
	charstring::zero(buffer,sizeof(buffer));


	// append strings, integers and floats
	charstring::append(buffer,"Hello ");
	charstring::append(buffer,"there!",6);
	charstring::append(buffer," ");
	charstring::append(buffer,(uint64_t)1);
	charstring::append(buffer,(uint64_t)2);
	charstring::append(buffer,(uint64_t)3);
	charstring::append(buffer," ");
	charstring::append(buffer,1.234,4,3);
	stdoutput.printf("buffer: %s\n",buffer);
	stdoutput.write('\n');


	// get length
	stdoutput.printf("length(buffer)=%d\n",charstring::length(buffer));
	stdoutput.write('\n');


	// zero the buffer again
	charstring::zero(buffer,sizeof(buffer));


	// copy to the beginning of the buffer
	charstring::copy(buffer,"Hello!");
	stdoutput.printf("buffer: %s\n",buffer);


	// copy the specified number of bytes to the beginning of the buffer
	charstring::copy(buffer,"Hello again!",6);
	stdoutput.printf("buffer: %s\n",buffer);


	// copy to the specified offset
	charstring::copy(buffer,6,"again!");
	stdoutput.printf("buffer: %s\n",buffer);


	// copy the specified number of bytes to the specified offset
	charstring::copy(buffer,12," Hi!  blah blah blah",4);
	stdoutput.printf("buffer: %s\n",buffer);
	stdoutput.write('\n');


	// get length
	stdoutput.printf("length(buffer)=%d\n",charstring::length(buffer));
	stdoutput.write('\n');


	// zero the buffer again
	charstring::zero(buffer,sizeof(buffer));


	// safely copy a long string to a smaller buffer
	charstring::safeCopy(buffer,sizeof(buffer),
				"This string is longer than the buffer");
	stdoutput.printf("buffer: %.*s\n",sizeof(buffer),buffer);


	// safely copy the specified number of bytes of a
	// long string to a smaller buffer
	charstring::safeCopy(buffer,sizeof(buffer),
				"This string is longer than the buffer",36);
	stdoutput.printf("buffer: %.*s\n",sizeof(buffer),buffer);
	stdoutput.write('\n');


	// get length
	stdoutput.printf("length(buffer)=%d\n",charstring::length(buffer));
	stdoutput.write('\n');


	// duplicate a string
	char	*completedup=charstring::duplicate("Hello there!");
	char	*partialdup=charstring::duplicate("Hello there!",5);
	stdoutput.printf("complete duplicate of \"Hello there!\": \"%s\"\n",
								completedup);
	stdoutput.printf("partial duplicate of \"Hello there!\":  \"%s\"\n",
								partialdup);
	delete[] completedup;
	delete[] partialdup;

	process::exit(0);
}
