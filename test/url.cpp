// Copyright (c) 2015  David Muse
// See the file COPYING for more information

#include <rudiments/url.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("url");

	char	*testcontents=file::getContents("knowncontent.html");

	const char	*urlname=
			"http://rudiments.sourceforge.net/knowncontent.html";

	url	u;
	test("open",u.open(urlname,O_RDONLY));
	char	*contents=u.getContents();
	test("get contents",!charstring::compare(testcontents,contents));
	delete[] contents;
	test("close",u.close());

	test("open",u.open(urlname,O_RDONLY));
	const char	*ptr=testcontents;
	char	buffer[1024];
	for (;;) {
		ssize_t	s=u.read(buffer,sizeof(buffer));
		test("chunk",!charstring::compare(ptr,buffer,s));
		if (s<(ssize_t)sizeof(buffer)) {
			break;
		}
		ptr+=s;
	}
	test("close",u.close());

	stdoutput.printf("\n");
}
