// Copyright (c) 2016  David Muse
// See the file COPYING for more information

#include <rudiments/md5.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include <rudiments/file.h>

int main(int argc, const char **argv) {

	if (argc<2) {
		stdoutput.printf("usage: md5sum filename\n");
		process::exit(0);
	}

	file	f;
	if (!f.open(argv[1],O_RDONLY)) {
		process::exit(1);
	}

	unsigned char	*contents=(unsigned char *)f.getContents();

	md5	m;
	m.append(contents,f.getSize());
	stdoutput.printf("%s\n",m.getHash());

	delete[] contents;
}
