// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/file.h>
#include <rudiments/sys.h>
#include <rudiments/memorymap.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>

int main(int argc, const char **argv) {

	// open the file
	file	f;
	if (!f.open("xmls.xml",O_RDONLY)) {
		stdoutput.printf("failed to open xmls.xml\n");
		process::exit(1);
	}

	// loop, mapping and printing blocks of the file
	memorymap	mm;
	off64_t		offset=0;
	size_t		len=sys::getPageSize();
	const char	*ptr=NULL;
	bool		done=false;
	do {
		if (offset) {
			mm.detach();
		}
		if (offset+len>f.getSize()) {
			len=f.getSize()-offset;
			done=true;
		}
		if (!mm.attach(f.getFileDescriptor(),offset,len,
						PROT_READ,MAP_SHARED)) {
			break;
		}
		stdoutput.printf("%.*s",len,(const char *)mm.getData());
		offset=offset+len;
	} while (!done);
}
