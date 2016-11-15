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

	// create the threads
	thread	t1;
	thread	t2;

	// define arguments to pass to each thread
	struct args	a1;
	a1.th=&t1;
	a1.id=1;
	a1.max=100;

	struct args	a2;
	a2.th=&t2;
	a2.id=2;
	a2.max=200;

	// spawn the threads
	t1.spawn((void *(*)(void *))count,(void *)&a1,true);
	t2.spawn((void *(*)(void *))count,(void *)&a2,true);
}
