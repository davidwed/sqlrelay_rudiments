// Copyright (c) 2001-2016  David Muse
// See the file COPYING for more information

#include <rudiments/sharedmemory.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/directory.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

        sharedmemory    shm;

	const char	*str="This string is in shared memory.";

	if (argc==1) {

		header("sharedmemory");

		stdoutput.write("setup...\n");

		// create the key file
		file::remove("shmkey");
		file	fd;
		test("key file",fd.create("shmkey",
				permissions::evalPermString("rw-------")));
		fd.close();

		// create a 128 byte shared memory segment
        	sharedmemory    shm;
        	test("create shm",
			shm.create(file::generateKey("shmkey",1),128,
                                permissions::evalPermString("rw-------")));

		// write a string into the shared memory
        	test("write",charstring::copy(
        			(char *)shm.getPointer(),
				"This string is in shared memory.")==
        			(char *)shm.getPointer());

		// spawn the second process (to write 2, 4)
		stringbuffer	cmd;
		char	*pwd=directory::getCurrentWorkingDirectory();
		cmd.append(pwd)->append("/sharedmemory");
		#ifdef _WIN32
			cmd.append(".exe");
		#endif
		delete[] pwd;
		const char	*args1[]={"sharedmemory","child",NULL};
		process::spawn(cmd.getString(),args1,true);

		snooze::macrosnooze(1);

		// clean up key file
		file::remove("shmkey");

	} else {

		// attach to the shared memory segment
        	test("attach to shm",
			shm.attach(file::generateKey("shmkey",1),128));

		// display the data contained in the shared memory segment
        	test("read",!charstring::compare((char *)shm.getPointer(),str));
        	stdoutput.printf("\n");
	}
}
