// Copyright (c) 2013  David Muse
// See the file COPYING for more information
#include <rudiments/thread.h>
#include <rudiments/threadmutex.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/randomnumber.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>
#include "test.cpp"

struct args {
	thread		*th;
	int32_t		id;
};

stringbuffer	output;
threadmutex	tm;
uint16_t	which=1;

void sync(void *args) {

	struct args	*a=(struct args *)args;

	// We want strict alternation of the threads, where thread 0 writes 1
	// and 3 and thread 1 writes 2 and 4.  This is actually difficult to
	// implement with mutexes.  The implementation below uses a "spin-lock"
	// strategy.  Each thread grabs the mutex and checks to see if it's ok
	// to go.  If not then it unlocks and waits a random amount of time
	// before checking again.  Hopefully by then, the other thread will
	// have progressed to the point of allowing the first thread to go.
	// Primitive, but it works.

	randomnumber	r;
	r.setSeed(randomnumber::getSeed());

	for (uint16_t i=0; i<10; i++) {
		if (a->id==0) {
			do {
				int32_t	ms;
				r.generateScaledNumber(0,100,&ms);
				snooze::microsnooze(0,ms);
				tm.lock();
				if (which==1) {
					output.append(1);
					which=2;
				}
				tm.unlock();
			} while (which!=2);

			do {
				int32_t	ms;
				r.generateScaledNumber(0,100,&ms);
				snooze::microsnooze(0,ms);
				tm.lock();
				if (which==3) {
					output.append(3);
					which=4;
				}
				tm.unlock();
			} while (which!=4);
		} else {
			do {
				int32_t	ms;
				r.generateScaledNumber(0,100,&ms);
				snooze::microsnooze(0,ms);
				tm.lock();
				if (which==2) {
					output.append(2);
					which=3;
				}
				tm.unlock();
			} while (which!=3);

			do {
				int32_t	ms;
				r.generateScaledNumber(0,100,&ms);
				snooze::microsnooze(0,ms);
				tm.lock();
				if (which==4) {
					output.append(4);
					which=1;
				}
				tm.unlock();
			} while (which!=1);
		}
	}
	a->th->exit(&(a->id));
}

int main(int argc, const char **argv) {

	header("threadmutex");

	if (thread::supportsThreads()) {

		// initialize threads
		thread		t[2];
		struct args	a[2];
		for (uint16_t i=0; i<2; i++) {
			a[i].th=&t[i];
			a[i].id=i;
		}

		// run threads
		for (uint16_t j=0; j<2; j++) {
			stringbuffer	title;
			title.append("spawn ")->append(j);
			test(title.getString(),
				t[j].spawn((void*(*)(void*))sync,
						(void *)&a[j],false));
		}

		// wait for the threads to exit
		for (uint16_t k=0; k<2; k++) {

			int32_t	tstatus=-1;

			stringbuffer	title;
			title.append("wait ")->append(k);
			test(title.getString(),t[k].wait(&tstatus));

			title.clear();
			title.append("status ")->append(k);
			test(title.getString(),tstatus==k);
		}

		// check output
		test("output",!charstring::compare(output.getString(),
				"1234123412341234123412341234123412341234"));

		stdoutput.printf("\n");

	} else {

		stdoutput.printf("threads not supported\n\n");
	}
}
