#include <rudiments/thread.h>
#include <rudiments/process.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>

// struct for passing arguments into the thread
struct args {
	thread		*th;
	int32_t		id;
	uint16_t	max;
};

// function that will be spawned as a thread
void count(void *args) {

	struct args	*a=(struct args *)args;

	a->th->detach();

	for (uint16_t c=0; c<a->max; c++) {
		stdoutput.printf("  %d: %d\n",a->id,c);
	}
}

int main(int argc, const char **argv) {

	// do we even support threads?
	if (!thread::supportsThreads()) {
		stdoutput.printf("thread are not supported\n");
		process::exit(1);
	}

	// create the threads and assign each a function to run
	thread	t1;
	thread	t2;
	t1.setFunction((void *(*)(void *))count);
	t2.setFunction((void *(*)(void *))count);

	// define arguments to pass to each thread
	struct args	a1;
	a1.th=&t1;
	a1.id=1;
	a1.max=100;

	struct args	a2;
	a2.th=&t2;
	a2.id=2;
	a2.max=200;

	// run the threads
	t1.run(&a1);
	t2.run(&a2);
}
