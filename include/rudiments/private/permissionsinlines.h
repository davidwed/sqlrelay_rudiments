// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <sys/stat.h>

INLINE int permissions::setFilePermissions(const char *filename,
							mode_t perms) {
	return chmod(filename,perms);
}

INLINE int permissions::setFilePermissions(int fd, mode_t perms) {
	return fchmod(fd,perms);
}

INLINE mode_t permissions::everyoneReadWrite() {
	return S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
}

INLINE mode_t permissions::everyoneReadWriteExecute() {
	return S_IRUSR | S_IWUSR | S_IXUSR | 
		S_IRGRP | S_IWGRP | S_IXGRP | 
		S_IROTH | S_IWOTH | S_IXOTH;
}

INLINE mode_t permissions::ownerRead() {
	return S_IRUSR;
}

INLINE mode_t permissions::ownerWrite() {
	return S_IWUSR;
}

INLINE mode_t permissions::ownerExecute() {
	return S_IXUSR;
}

INLINE mode_t permissions::ownerReadWrite() {
	return S_IRUSR | S_IWUSR;
}

INLINE mode_t permissions::ownerReadExecute() {
	return S_IRUSR | S_IXUSR;
}

INLINE mode_t permissions::ownerReadWriteExecute() {
	return S_IRUSR | S_IWUSR | S_IXUSR;
}

INLINE mode_t permissions::groupRead() {
	return S_IRGRP;
}

INLINE mode_t permissions::groupWrite() {
	return S_IWGRP;
}

INLINE mode_t permissions::groupExecute() {
	return S_IXGRP;
}

INLINE mode_t permissions::groupReadWrite() {
	return S_IRGRP | S_IWGRP;
}

INLINE mode_t permissions::groupReadExecute() {
	return S_IRGRP | S_IXGRP;
}

INLINE mode_t permissions::groupReadWriteExecute() {
	return S_IRGRP | S_IWGRP | S_IXGRP;
}

INLINE mode_t permissions::othersRead() {
	return S_IROTH;
}

INLINE mode_t permissions::othersWrite() {
	return S_IWOTH;
}

INLINE mode_t permissions::othersExecute() {
	return S_IXOTH;
}

INLINE mode_t permissions::othersReadWrite() {
	return S_IROTH | S_IWOTH;
}

INLINE mode_t permissions::othersReadExecute() {
	return S_IROTH | S_IXOTH;
}

INLINE mode_t permissions::othersReadWriteExecute() {
	return S_IROTH | S_IWOTH | S_IXOTH;
}

INLINE mode_t permissions::saveInSwapSpace() {
	return S_ISVTX;
}

INLINE mode_t permissions::setUserId() {
	return S_ISUID;
}

INLINE mode_t permissions::setGroupId() {
	return S_ISGID;
}
