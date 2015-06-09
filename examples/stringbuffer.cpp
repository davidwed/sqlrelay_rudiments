#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	stringbuffer	strb;

	// append strings, numbers, and characters...
	strb.append("hello, ")->append("goodbye, ");
	strb.append((uint64_t)1)->append(", ");
	strb.append((double)1.234,4,3)->append('.');

	stdoutput.write("after initial append:\n");
	stdoutput.printf("  contents: \"%s\"\n",strb.getString());
	stdoutput.printf("    length: %d\n",strb.getStringLength());
	stdoutput.write('\n');


	// clear the buffer and append more data to it
	strb.clear();
	strb.append("1 2 3 4 5 6 7 8 9 0");

	// overwrite the first 5 numbers...
	strb.setPosition(0);
	strb.write("0 9 8 7 6");

	stdoutput.write("after overwrite:\n");
	stdoutput.printf("  contents: \"%s\"\n",strb.getString());
	stdoutput.printf("    length: %d\n",strb.getStringLength());
	stdoutput.write('\n');


	// truncate the string after the first 2 numbers
	strb.truncate(3);

	stdoutput.write("after truncate:\n");
	stdoutput.printf("  contents: \"%s\"\n",strb.getString());
	stdoutput.printf("    length: %d\n",strb.getStringLength());
	stdoutput.write('\n');


	// detach the string
	char	*str=strb.detachString();

	stdoutput.write("after detach:\n");
	stdoutput.printf("         contents: \"%s\"\n",strb.getString());
	stdoutput.printf("           length: %d\n",strb.getStringLength());
	stdoutput.printf("  detached string: \"%s\"\n",str);
	stdoutput.write('\n');

	delete[] str;
}
