// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/container.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

container::container() {
	copyreferences=false;
	cascadeondelete=false;
}

container::~container() {
}


void container::copyReferences() {
	copyreferences=true;
}

void container::dontCopyReferences() {
	copyreferences=false;
}

void container::cascadeOnDelete() {
	cascadeondelete=true;
}

void container::dontCascadeOnDelete() {
	cascadeondelete=false;
}


bool container::save(filedescriptor *fd) {
	return false;
}

bool container::save(unsigned char *buffer) {
	return false;
}

bool container::load(filedescriptor *fd) {
	return false;
}

bool container::load(unsigned char *buffer) {
	return false;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
