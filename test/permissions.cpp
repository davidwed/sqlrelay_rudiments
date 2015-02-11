// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <stdlib.h>

#ifdef _WIN32
	const char	*ls="cacls tempfile";
#else
	const char	*ls="ls -l tempfile";
#endif

int main(int argc, const char **argv) {


        // Create a file with rw-r--r-- permissions
        file	fd;
	fd.open("tempfile",O_RDWR|O_CREAT,
                                permissions::evalPermString("rw-r--r--"));
	system(ls);


        // change the permissions to rw-rw-r--
        permissions::setFilePermissions(fd.getFileDescriptor(),
                                permissions::evalPermString("rw-rw-r--"));
	system(ls);


        // close and delete the file
        fd.close();
        file::remove("tempfile");


        // do the same as above using different methods
        fd.open("tempfile",O_RDWR|O_CREAT,
                                permissions::ownerReadWrite()|
                                permissions::groupRead()|
                                permissions::othersRead());
	system(ls);

        permissions::setFilePermissions(fd.getFileDescriptor(),
                                permissions::ownerReadWrite()|
                                permissions::groupReadWrite()|
                                permissions::othersRead());
	system(ls);
        fd.close();
        file::remove("tempfile");
}
