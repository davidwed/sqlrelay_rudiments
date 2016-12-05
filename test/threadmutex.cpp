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

threadmutex	tm0;
threadmutex	tm1;

void sync(void *args) {

	struct args	*a=(struct args *)args;

	for (uint16_t i=0; i<10; i++) {
		if (a->id==0) {
			test("lock(1) (1)",tm1.lock());
			output.append(1);
			test("unlock(0) (1)",tm0.unlock());

			test("lock(1) (3)",tm1.lock());
			output.append(3);
			test("unlock(0) (3)",tm0.unlock());
		} else {
			test("lock(0) (3)",tm0.lock());
			output.append(2);
			test("unlock(1) (3)",tm1.unlock());

			test("lock(0) (4)",tm0.lock());
			output.append(4);
			test("unlock(1) (4)",tm1.unlock());
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

		// lock mutex
		test("initial lock",tm0.lock());

		// run threads
		for (uint16_t j=0; j<2; j++) {
			stringbuffer	title;
			title.append("spawn ")->append(j);
			test(title.getString(),
				t[j].spawn((void*(*)(void*))sync,
						(void *)&a[j],false));
		}

		// unlock mutex
		//test("final unlock",tm.unlock());

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

stdoutput.printf("%s\n",output.getString());
		// check output
		test("output",!charstring::compare(output.getString(),"43210"));

		stdoutput.printf("\n");

	} else {

		stdoutput.printf("threads not supported\n\n");
	}
}
