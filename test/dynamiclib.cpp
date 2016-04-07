// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/directory.h>
#include <rudiments/charstring.h>
#include <rudiments/dynamiclib.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("dynamiclib");

#ifdef _WIN32
	char	*f=charstring::duplicate("C:\\Windows\\System32\\msvcrt.dll");
#else
	// decide on a file to use
	const char	*dirs[]={
		"/lib64","/usr/lib64","/lib","/usr/lib",NULL
	};
	char	*f=NULL;
	for (const char * const *dir=dirs; *dir; dir++) {
		directory	d;
		if (d.open(*dir)) {
			d.rewind();
			for (;;) {
				char	*file=d.read();
				if (!file) {
					break;
				}
				if (!charstring::compare(file,"libc-",5) &&
					!charstring::compare(
						charstring::findLast(
								file,".so"),
						".so")) {
					f=file;
					break;
				}
				delete[] file;
			}
		}
		if (f) {
			break;
		}
	}
#endif
	test("file exists",f);

	// open valid file and get a symbol
	dynamiclib	d;
	test("open: valid file",d.open(f,false,false));
	void	*strcmp=d.getSymbol("strcmp");
	test("getSymbol: valid symbol",strcmp);
	test("getError: valid symbol",!d.getError());
	strcmp=d.getSymbol("strcmpbad");
	test("getSymbol: invalid symbol",!strcmp);
	test("getError: invalid symbol",d.getError());
	test("close",d.close());

	// open invalid file
	test("open: invalid file",!d.open("badfile",false,false));
	test("getError: invalid file",d.getError());

	delete[] f;
}
