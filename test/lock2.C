// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/file.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, const char **argv) {

	file	fl;
	fl.open("testfile",O_RDWR);

	printf("locking...\n");
	printf("locked=%d\n",fl.lockFile(F_RDLCK|F_WRLCK));

	printf("sleeping\n");
	sleep(100);
}
