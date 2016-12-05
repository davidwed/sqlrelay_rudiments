// Copyright (c) 2013  David Muse
// See the file COPYING for more information
#include <rudiments/thread.h>
#include <rudiments/threadmutex.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

struct args {
	thread		*th;
	int32_t		id;
};

stringbuffer	output;

threadmutex	tm;

void count(void *args) {
	struct args	*a=(struct args *)args;
	tm.lock();
	output.append(a->id);
	tm.unlock();
	a->th->exit(&(a->id));
}

int main(int argc, const char **argv) {

	header("threadmutex");

	if (thread::supportsThreads()) {

		// initialize threads
		thread		t[5];
		struct args	a[5];
		for (uint8_t i=0; i<5; i++) {
			a[i].th=&t[i];
			a[i].id=i;
		}

		// lock mutex
		test("initial lock",tm.lock());

		// run threads
		for (uint8_t j=0; j<5; j++) {
			stringbuffer	title;
			title.append("spawn ")->append(j);
			test(title.getString(),
				t[j].spawn((void*(*)(void*))count,
						(void *)&a[j],false));
		}

		// unlock mutex
		test("final unlock",tm.unlock());

		// wait for the threads to exit
		for (uint8_t k=0; k<5; k++) {

			int32_t	tstatus=-1;

			stringbuffer	title;
			title.append("wait ")->append(k);
			test(title.getString(),t[k].wait(&tstatus));

			title.clear();
			title.append("status ")->append(k);
			test(title.getString(),tstatus==k);
		}

		// check output
		test("output",!charstring::compare(output.getString(),"43210"));

		stdoutput.printf("\n");

	} else {

		stdoutput.printf("threads not supported\n\n");
	}
}
