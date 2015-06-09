#include <rudiments/bytebuffer.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	bytebuffer	bb;

	// append strings, numbers, and characters...
	bb.append("hello, ")->append("goodbye, ");
	bb.append((uint64_t)1)->append(", ");
	bb.append((double)1.234)->append('.');

	stdoutput.write("after initial append:\n");
	stdoutput.write("  contents: ");
	stdoutput.safePrint(bb.getBuffer(),bb.getSize());
	stdoutput.write('\n');
	stdoutput.printf("      size: %d\n",bb.getSize());
	stdoutput.write('\n');


	// clear the buffer and append more data to it
	const unsigned char	data[]={1,2,3,4,5,6,7,8};
	bb.clear();
	bb.append(data,sizeof(data));

	// overwrite the first 5 numbers...
	bb.setPosition(0);
	const unsigned char	newdata[]={8,7,6,5};
	bb.write(newdata,sizeof(newdata));

	stdoutput.write("after overwrite:\n");
	stdoutput.write("  contents: ");
	stdoutput.safePrint(bb.getBuffer(),bb.getSize());
	stdoutput.write('\n');
	stdoutput.printf("      size: %d\n",bb.getSize());
	stdoutput.write('\n');


	// truncate the string after the first 2 numbers
	bb.truncate(3);

	stdoutput.write("after truncate:\n");
	stdoutput.write("  contents: ");
	stdoutput.safePrint(bb.getBuffer(),bb.getSize());
	stdoutput.write('\n');
	stdoutput.printf("      size: %d\n",bb.getSize());
	stdoutput.write('\n');


	// detach the string
	size_t		ddatasize=bb.getSize();
	unsigned char	*ddata=bb.detachBuffer();

	stdoutput.write("after detach:\n");
	stdoutput.write("       contents: ");
	stdoutput.safePrint(bb.getBuffer(),bb.getSize());
	stdoutput.write('\n');
	stdoutput.printf("           size: %d\n",bb.getSize());
	stdoutput.write("  detached data: ");
	stdoutput.safePrint(ddata,ddatasize);
	stdoutput.write('\n');
	stdoutput.printf("           size: %d\n",ddatasize);
	stdoutput.write('\n');

	delete[] ddata;

	process::exit(0);
}
