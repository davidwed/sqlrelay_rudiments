#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// standard output...
	stdoutput.write("This line is written to standard output.\n");
	stdoutput.write("Fully functional printf is also available: ");
	stdoutput.printf("%s, %c, %05d\n", "hello",'c',25);
	stdoutput.write('\n');


	// standard error...
	stderror.write("This line is written to standard error.\n");
	stderror.write("Fully functional printf is also available: ");
	stderror.printf("%s, %c, %05d\n", "hello",'c',25);
	stderror.write('\n');


	// standard input...
	stdoutput.write("Type something and hit return...\n");
	char	input[1024];
	ssize_t	count=stdinput.read(input,sizeof(input));
	input[count]='\0';
	stdoutput.printf("You typed: %s\n",input);
}
