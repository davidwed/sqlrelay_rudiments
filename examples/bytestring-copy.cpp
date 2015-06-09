#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	unsigned char	buffer[9];


	// zero the buffer
	bytestring::zero(buffer,sizeof(buffer));

	stdoutput.write("all zeros:\n  ");
	stdoutput.safePrint(buffer,sizeof(buffer));
	stdoutput.write("\n\n");


	// set the buffer to all ones
	bytestring::set(buffer,1,sizeof(buffer));

	stdoutput.write("all ones:\n  ");
	stdoutput.safePrint(buffer,sizeof(buffer));
	stdoutput.write("\n\n");


	// copy some data into the buffer
	unsigned char	data[]={0,1,2,3,4,5,6,7,8};
	bytestring::copy(buffer,data,sizeof(data));

	stdoutput.write("after copying 0-8:\n  ");
	stdoutput.safePrint(buffer,sizeof(buffer));
	stdoutput.write("\n\n");


	// copy some overlapping data from one part of the buffer to another
	bytestring::copyWithOverlap(buffer,buffer+3,sizeof(buffer)-3);

	stdoutput.write("after copying with overlap:\n  ");
	stdoutput.safePrint(buffer,sizeof(buffer));
	stdoutput.write("\n\n");


	// copy a string into the buffer
	unsigned char	str[]="hello";
	bytestring::copyUntil(buffer,str,'\0',sizeof(str));

	stdoutput.write("after copying \"hello\":\n  ");
	stdoutput.safePrint(buffer,sizeof(buffer));
	stdoutput.write("\n\n");


	// copy some data into the buffer, swapping bytes as they are copied
	bytestring::copySwapBytes(buffer,data,sizeof(data));

	stdoutput.write("after copying 0-8, swapping bytes:\n  ");
	stdoutput.safePrint(buffer,sizeof(buffer));
	stdoutput.write("\n\n");
}
