// Copyright (c) 2013  David Muse
// See the file COPYING for more information
#include <rudiments/thread.h>
#include <rudiments/threadmutex.h>
#include <rudiments/semaphoreset.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>

#define	USE_THREADMUTEX 1
//#define	USE_SEMAPHORESET 1

struct args {
	thread		*th;
	int32_t		id;
};

#if defined(USE_THREADMUTEX)
	threadmutex	tm;
#elif defined(USE_SEMAPHORESET)
	semaphoreset	*sem;
	key_t		key;
#endif

void count(void *args) {

	struct args	*a=(struct args *)args;

	// acquire lock
	#if defined(USE_THREADMUTEX)
		tm.lock();
	#elif defined(USE_SEMAPHORESET)
		sem->wait(0);
	#endif

	// count to 50
	stdoutput.printf("  %d: ",a->id);
	for (uint16_t c=0; c<50; c++) {
		stdoutput.printf("%d",c%10);
	}
	stdoutput.printf("\n");

	// release lock
	#if defined(USE_THREADMUTEX)
		tm.unlock();
	#elif defined(USE_SEMAPHORESET)
		sem->signal(0);
	#endif

	// exit
	stdoutput.printf("  %d: exiting\n",a->id);
	a->th->exit(&(a->id));
}

int main(int argc, const char **argv) {

	stdoutput.printf("threads are%ssupported\n",
			(thread::supportsThreads())?" ":" not ");

	// initialize mutex
	#if defined(USE_SEMAPHORESET)
		sem=new semaphoreset;
		const int32_t	vals[]={1};
		if (!sem->create(file::generateKey("threadmutex",0),
			permissions::evalPermString("rw-r--r--"),1,vals)) {
			stdoutput.printf("failed to create semaphore\n");
			delete sem;
			process::exit(1);
		}
	#endif

	// initialize threads
	thread		t[5];
	struct args	a[5];
	for (uint8_t i=0; i<5; i++) {
		a[i].th=&t[i];
		a[i].id=i;
		t[i].setFunction((void*(*)(void*))count,&a[i]);
	}

	// lock mutex
	#if defined(USE_THREADMUTEX)
		tm.lock();
	#elif defined(USE_SEMAPHORESET)
		sem->wait(1);
	#endif

	// create threads
	for (uint8_t j=0; j<5; j++) {
		if (!t[j].create()) {
			stdoutput.printf(" %d: create failed\n",j);
		}
	}

	// unlock mutex
	#if defined(USE_THREADMUTEX)
		tm.unlock();
	#elif defined(USE_SEMAPHORESET)
		sem->signal(1);
	#endif

	// join threads
	for (uint8_t k=0; k<5; k++) {
		int32_t	tstatus=-1;
		if (!t[k].join(&tstatus)) {
			stdoutput.printf(" %d: join failed\n",k);
		}
		stdoutput.printf("t%d status: %d\n",k,tstatus);
	}

	stdoutput.printf("done\n");

	// clean up
	#if defined(USE_SEMAPHORESET)
		delete sem;
	#endif
	process::exit(0);
}
