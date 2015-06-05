#include <rudiments/stdio.h>

int main(int argc, const char **argv) {


	// standard output...
	stdoutput.write("This line is written to standard output.\n");
	stdoutput.write("Fully functional printf is also available:");
	stdoutput.printf("%s, %c, %05d\n", "hello",'c',25);


	// standard error...
	stderror.write("This line is written to standard error.\n");
	stdoutput.write("Fully functional printf is also available:");
	stdoutput.printf("%s, %c, %05d\n", "hello",'c',25);


	// standard input...
	char	input[1024];
	ssize_t	count=stdinput.read(input,sizeof(input));
	input[count]='\0';
}
