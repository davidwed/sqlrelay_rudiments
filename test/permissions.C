// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/permissions.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char **argv) {


        // Create a file with rw-r--r-- permissions
        int     fd=open("/tmp/tempfile",O_RDWR|O_CREAT,
                                permissions::evalPermString("rw-r--r--"));


        // change the permissions to rw-rw-r--
        permissions::setFilePermissions(fd,
                                permissions::evalPermString("rw-rw-r--"));


        // close and delete the file
        close(fd);
        unlink("/tmp/tempfile");


        // do the same as above using different methods
        fd=open("/tmp/tempfile",O_RDWR|O_CREAT,
                                permissions::ownerReadWrite()|
                                permissions::groupRead()|
                                permissions::othersRead());
        permissions::setFilePermissions(fd,
                                permissions::ownerReadWrite()|
                                permissions::groupReadWrite()|
                                permissions::othersRead());
        close(fd);
        unlink("/tmp/tempfile");
}
