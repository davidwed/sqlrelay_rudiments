// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/sys.h>
#include <rudiments/memorymap.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("memorymap");

	size_t	allocgran=sys::getAllocationGranularity();

	// create a file to test with
	file::remove("memorymap.txt");
	file	fd;
	test("create file",fd.create("memorymap.txt",
				permissions::evalPermString("rw-r--r--")));
	for (uint16_t i=0; i<10; i++) {
		for (size_t j=0; j<allocgran; j++) {
			fd.write((char)('0'+i));
		}
	}
	
	// loop, mapping blocks of the file
	memorymap	mm;
	for (uint16_t i=0; i<10; i++) {

		// attach to the next block
		test("attach",mm.attach(fd.getFileDescriptor(),
						i*allocgran,allocgran,
						PROT_READ,MAP_SHARED));

		// get a pointer to the block
		const char	*ptr=(const char *)mm.getData();
		test("valid ptr",(ptr!=NULL));

		// see if it contains the correct data
		bool	success=true;
		for (size_t j=0; j<allocgran; j++) {
			if (*(ptr+j)!='0'+i) {
				success=false;
				break;
			}
		}
		stringbuffer	blockname;
		blockname.append("block ")->append(i);
		test(blockname.getString(),success);

		// detach from the block
		test("detach",mm.detach());
	}
	stdoutput.printf("\n");

	// close the file
	fd.close();

	// clean up
	file::remove("memorymap.txt");
}
