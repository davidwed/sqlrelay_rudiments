#include <rudiments/thread.h>
#include <rudiments/threadmutex.h>
#include <rudiments/stdio.h>

// struct for passing arguments into the thread
struct args {
	thread		*th;
	threadmutex	*tm;
	int32_t		id;
};

// function that will be spawned as a thread
void count(void *args) {

	struct args	*a=(struct args *)args;

	// lock the mutex
	a->tm->lock();

	// count to 5, ten times
	stdoutput.printf("thread %d: ",a->id);
	for (uint16_t oc=0; oc<10; oc++) {
		for (uint16_t ic=0; ic<5; ic++) {
			stdoutput.printf("%d",ic);
		}
	}
	stdoutput.printf("\n");

	// unlock the mutex
	a->tm->unlock();

	// exit
	a->th->exit(&(a->id));
}

int main(int argc, const char **argv) {

	// create 5 threads
	thread		t[5];

	// create a mutex to synchronize them
	threadmutex	tm;

	// initialize threads
	struct args	a[5];
	for (uint16_t i=0; i<5; i++) {

		t[i].setFunction((void*(*)(void*))count);

		a[i].th=&t[i];
		a[i].tm=&tm;
		a[i].id=i;
	}

	// lock the mutex
	tm.lock();

	// run threads (each will block on the mutex)
	for (uint16_t j=0; j<5; j++) {
		if (!t[j].run(&a[j])) {
			stdoutput.printf(" %d: run failed\n",j);
		}
	}

	// unlock the mutex (allows the threads to proceed)
	tm.unlock();

	// join threads
	for (uint16_t k=0; k<5; k++) {
		int32_t	tstatus=-1;
		t[k].join(&tstatus);
		stdoutput.printf("thread %d status: %d\n",k,tstatus);
	}
}
