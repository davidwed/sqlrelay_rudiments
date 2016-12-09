// Copyright (c) 2013  David Muse
// See the file COPYING for more information

#include <rudiments/thread.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/snooze.h>
#include "test.cpp"

struct args {
	thread		*th;
	int32_t		id;
	bool		detach;
};

stringbuffer	threadcount[4];
stringbuffer	testcount;

void count(void *args) {
	struct args	*a=(struct args *)args;
	if (a->detach) {
		stringbuffer	title;
		title.append("detach ")->append(a->id);
		test(title.getString(),a->th->detach());
	}
	for (uint16_t c=0; c<100; c++) {
		threadcount[a->id].append(c);
	}
	if (!a->detach) {
		a->th->exit(&(a->id));
	}
}

int main(int argc, const char **argv) {

	header("threads");

	if (!thread::supported()) {
		stdoutput.printf("	not supported\n\n");
	}

	// create control
	for (uint16_t c=0; c<100; c++) {
		testcount.append(c);
	}

	// create thread 0
	thread	t0;

	struct args	a0;
	a0.th=&t0;
	a0.id=0;
	a0.detach=false;

	// create thread 1
	thread	t1;

	struct args	a1;
	a1.th=&t1;
	a1.id=1;
	a1.detach=false;


	stdoutput.printf("attached...\n");

	// run threads in attached mode
	test("spawn 0",t0.spawn((void *(*)(void *))count,(void *)&a0,false));
	test("spawn 1",t1.spawn((void *(*)(void *))count,(void *)&a1,false));

	// wait for the threads to exit
	int32_t	t0status=-1;
	test("wait 0",t0.wait(&t0status));
	test("status 0",t0status==0);
	int32_t	t1status=-1;
	test("wait 1",t1.wait(&t1status));
	test("status 1",t1status==1);

	// test results
	test("thread count 0",
		!charstring::compare(threadcount[0].getString(),
						testcount.getString()));
	test("thread count 1",
		!charstring::compare(threadcount[1].getString(),
						testcount.getString()));
	stdoutput.printf("\n");


	stdoutput.printf("detached...\n");

	// reset id's and detach mode
	a0.id=2;
	a0.detach=true;
	a1.id=3;
	a1.detach=true;

	// run threads in detached mode
	test("spawn detached 2",
		t0.spawn((void *(*)(void *))count,(void *)&a0,false));
	test("spawn detached 3",
		t1.spawn((void *(*)(void *))count,(void *)&a1,false));

	// we'll have to wait a bit
	snooze::macrosnooze(1);

	// test results
	test("thread count 2",
		!charstring::compare(threadcount[2].getString(),
						testcount.getString()));
	test("thread count 3",
		!charstring::compare(threadcount[3].getString(),
						testcount.getString()));

	stdoutput.printf("\n");
}
