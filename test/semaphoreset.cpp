// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/semaphoreset.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/directory.h>
#include <rudiments/process.h>
#include <rudiments/snooze.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#include "test.cpp"

void sync(semaphoreset *sem, file *semout, bool first) {

	for (uint16_t i=0; i<10; i++) {
		if (first) {
			test("wait(1) (1)",sem->wait(1));
			semout->setPositionRelativeToEnd(0);
			test("write 1",semout->write("1")==1);
			test("signal(0) (1)",sem->signal(0));

			test("wait(1) (2)",sem->wait(1));
			semout->setPositionRelativeToEnd(0);
			test("write 3",semout->write("3")==1);
			test("signal(0) (2)",sem->signal(0));
		} else {
			test("wait(0) (1)",sem->wait(0));
			semout->setPositionRelativeToEnd(0);
			test("write 2",semout->write("2")==1);
			test("signal(1) (1)",sem->signal(1));

			test("wait(0) (2)",sem->wait(0));
			semout->setPositionRelativeToEnd(0);
			test("write 4",semout->write("4")==1);
			test("signal(1) (2)",sem->signal(1));
		}
	}
}

int main(int argc, const char **argv) {

	semaphoreset	sem;

	if (argc==1) {

		header("semaphoreset");

		stdoutput.write("setup...\n");

		// create the key file
		file::remove("semkey");
		file	fd;
		test("key file",fd.create("semkey",
				permissions::evalPermString("rw-------")));
		fd.close();

		// create the semaphore
		int32_t	vals[2]={0,1};
		test("create sem",
			sem.create(file::generateKey("semkey",1),
				permissions::evalPermString("rw-------"),
				2,vals));


		// test synchronization

		// create the output file
		file::remove("semout");
		file	semout;
		test("output file",semout.create("semout",
				permissions::evalPermString("rw-r--r--")));
		stdoutput.write("\n");

		stdoutput.write("synchronization...\n");

		// spawn the second process (to write 2, 4)
		stringbuffer	cmd;
		char	*pwd=directory::getCurrentWorkingDirectory();
		cmd.append(pwd)->append("/semaphoreset");
		delete[] pwd;
		const char	*args1[]={"semaphoreset","child",NULL};
		process::spawn(cmd.getString(),args1,true);

		snooze::macrosnooze(1);

		// synchronization
		sync(&sem,&semout,true);

		snooze::macrosnooze(1);

		// verify synchronization
		char	*contents=semout.getContents();
		test("output",!charstring::compare(contents,
				"1234123412341234123412341234123412341234"));
		delete[] contents;
		stdoutput.write("\n");

		// clean up output file
		semout.close();
		file::remove("semout");

		
		// test timeout
		if (sem.supportsTimedSemaphoreOperations()) {

			stdoutput.write("timeout...\n");

			datetime	dt;
			dt.getSystemDateAndTime();
			time_t	epoch=dt.getEpoch();

			test("wait(0)",!sem.wait(0,2,0));

			dt.getSystemDateAndTime();
			test("time",dt.getEpoch()-epoch>=1);
			stdoutput.write("\n");
		}


		// clean up key file
		file::remove("semkey");

	} else {

		// attach to the semaphore
		test("attach to sem",
			sem.attach(file::generateKey("semkey",1),2));

		// open the output file
		file	semout;
		test("output file",semout.open("semout",O_WRONLY));
		stdoutput.write("\n");

		// synchronization
		sync(&sem,&semout,false);

		// clean up
		semout.close();
	}
}
