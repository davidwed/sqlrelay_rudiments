// Copyright (c) 2015 David Muse
// See the COPYING file for more information


#include <rudiments/url.h>

class urlprivate {
	friend class url;
	private:
};

url::url() : file() {
	pvt=new urlprivate;
	type("url");
}

url::url(const url &u) : file(u) {
	pvt=new urlprivate;
	urlClone(u);
	type("url");
}

url &url::operator=(const url &u) {
	if (this!=&u) {
		file::operator=(u);
		urlClone(u);
	}
	return *this;
}

void url::urlClone(const url &f) {
}

url::~url() {
	delete pvt;
}

void url::lowLevelOpen(const char *name, int32_t flags,
				mode_t perms, bool useperms) {
}
