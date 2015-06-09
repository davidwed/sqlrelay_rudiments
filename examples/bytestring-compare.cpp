#include <rudiments/bytestring.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	unsigned char	buffer1[8];
	unsigned char	buffer2[8];


	// zero the buffers
	bytestring::zero(buffer1,sizeof(buffer1));
	bytestring::zero(buffer2,sizeof(buffer2));

	stdoutput.write("both zeroed:\n");
	stdoutput.printf("  does buffer1=buffer2?  %s\n\n",
		(!bytestring::compare(buffer1,buffer2,sizeof(buffer1)))?
								"yes":"no");

	// set buffer2 to all ones
	bytestring::set(buffer2,1,sizeof(buffer2));

	stdoutput.write("buffer2 is all ones:\n");
	stdoutput.printf("  does buffer1=buffer2?  %s\n\n",
		(!bytestring::compare(buffer1,buffer2,sizeof(buffer1)))?
								"yes":"no");

	// set the first half of buffer2 to zeros
	// and only compare the first half
	bytestring::set(buffer2,0,sizeof(buffer2)/2);

	stdoutput.write("first half of buffer2 is all zeros:\n");
	stdoutput.printf("  does 1/2 buffer1 = 1/2 buffer2?  %s\n\n",
		(!bytestring::compare(buffer1,buffer2,sizeof(buffer1)/2))?
								"yes":"no");

	process::exit(0);
}
