// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <stdlib.h>

int main(int argc, const char **argv) {


        // Create a file with rw-r--r-- permissions
        file	fd;
	fd.open("/tmp/tempfile",O_RDWR|O_CREAT,
                                permissions::evalPermString("rw-r--r--"));
	system("ls -l /tmp/tempfile");


        // change the permissions to rw-rw-r--
        permissions::setFilePermissions(fd.getFileDescriptor(),
                                permissions::evalPermString("rw-rw-r--"));
	system("ls -l /tmp/tempfile");


        // close and delete the file
        fd.close();
        file::remove("/tmp/tempfile");


        // do the same as above using different methods
        fd.open("/tmp/tempfile",O_RDWR|O_CREAT,
                                permissions::ownerReadWrite()|
                                permissions::groupRead()|
                                permissions::othersRead());
	system("ls -l /tmp/tempfile");

        permissions::setFilePermissions(fd.getFileDescriptor(),
                                permissions::ownerReadWrite()|
                                permissions::groupReadWrite()|
                                permissions::othersRead());
	system("ls -l /tmp/tempfile");
        fd.close();
        file::remove("/tmp/tempfile");
}
