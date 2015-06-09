#include <rudiments/bytestring.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// initialize buffer
	unsigned char	buffer[]={0,2,0,2,0,2,0};

	stdoutput.write("buffer : ");
	stdoutput.safePrint(buffer,sizeof(buffer));
	stdoutput.write("\n\n");


	// find first and last 0
	unsigned char	*firstzero=(unsigned char *)
			bytestring::findFirst(buffer,0,sizeof(buffer));
	unsigned char	*lastzero=(unsigned char *)
			bytestring::findLast(buffer,0,sizeof(buffer));

	stdoutput.write("first 0 : ");
	stdoutput.safePrint(firstzero,sizeof(buffer)-(firstzero-buffer));
	stdoutput.write('\n');

	stdoutput.write(" last 0 : ");
	stdoutput.safePrint(lastzero,sizeof(buffer)-(lastzero-buffer));
	stdoutput.write("\n\n");


	// find first and last 0,2,0 patterns
	unsigned char	pattern[]={0,2,0};

	unsigned char	*firstzerotwozero=(unsigned char *)
			bytestring::findFirst(buffer,sizeof(buffer),
						pattern,sizeof(pattern));
	unsigned char	*lastzerotwozero=(unsigned char *)
			bytestring::findLast(buffer,sizeof(buffer),
						pattern,sizeof(pattern));

	stdoutput.write("first 0,2,0 : ");
	stdoutput.safePrint(firstzerotwozero,
				sizeof(buffer)-(firstzerotwozero-buffer));
	stdoutput.write('\n');

	stdoutput.write(" last 0,2,0 : ");
	stdoutput.safePrint(lastzerotwozero,
				sizeof(buffer)-(lastzerotwozero-buffer));
	stdoutput.write("\n\n");
	

	process::exit(0);
}
