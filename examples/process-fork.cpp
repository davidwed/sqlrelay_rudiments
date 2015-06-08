#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// see if fork() is supported and exit if it's not...
	if (!process::supportsFork()) {
		stdoutput.write("sorry, fork() not supported\n");
		process::exit(1);
	}

	// fork the process
	pid_t	pid=process::fork();

	if (pid) {
		// the parent process will run this code...
		stdoutput.write("I am the parent.  ");
		stdoutput.printf("The child's process id is %d\n",pid);
	} else {
		// the child process will run this code...
		stdoutput.write("I am the child.  ");
		stdoutput.printf("My process id is %d\n",
						process::getProcessId());
	}

	// both processes will run this code to exit
	process::exit(0);
}
