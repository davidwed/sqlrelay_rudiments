// Copyright (c) 2002 David Muse
// See the COPYING file for more information

//#include <rudiments/private/config.h>

#include <sys/stat.h>

inline int permissions::setFilePermissions(const char *filename,
							mode_t perms) {
	return chmod(filename,perms);
}

inline int permissions::setFilePermissions(int fd, mode_t perms) {
	return fchmod(fd,perms);
}

inline mode_t permissions::everyoneReadWrite() {
	return S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
}

inline mode_t permissions::everyoneReadWriteExecute() {
	return S_IRUSR | S_IWUSR | S_IXUSR | 
		S_IRGRP | S_IWGRP | S_IXGRP | 
		S_IROTH | S_IWOTH | S_IXOTH;
}

inline mode_t permissions::ownerRead() {
	return S_IRUSR;
}

inline mode_t permissions::ownerWrite() {
	return S_IWUSR;
}

inline mode_t permissions::ownerExecute() {
	return S_IXUSR;
}

inline mode_t permissions::ownerReadWrite() {
	return S_IRUSR | S_IWUSR;
}

inline mode_t permissions::ownerReadExecute() {
	return S_IRUSR | S_IXUSR;
}

inline mode_t permissions::ownerReadWriteExecute() {
	return S_IRUSR | S_IWUSR | S_IXUSR;
}

inline mode_t permissions::groupRead() {
	return S_IRGRP;
}

inline mode_t permissions::groupWrite() {
	return S_IWGRP;
}

inline mode_t permissions::groupExecute() {
	return S_IXGRP;
}

inline mode_t permissions::groupReadWrite() {
	return S_IRGRP | S_IWGRP;
}

inline mode_t permissions::groupReadExecute() {
	return S_IRGRP | S_IXGRP;
}

inline mode_t permissions::groupReadWriteExecute() {
	return S_IRGRP | S_IWGRP | S_IXGRP;
}

inline mode_t permissions::othersRead() {
	return S_IROTH;
}

inline mode_t permissions::othersWrite() {
	return S_IWOTH;
}

inline mode_t permissions::othersExecute() {
	return S_IXOTH;
}

inline mode_t permissions::othersReadWrite() {
	return S_IROTH | S_IWOTH;
}

inline mode_t permissions::othersReadExecute() {
	return S_IROTH | S_IXOTH;
}

inline mode_t permissions::othersReadWriteExecute() {
	return S_IROTH | S_IWOTH | S_IXOTH;
}

inline mode_t permissions::saveInSwapSpace() {
	return S_ISVTX;
}

inline mode_t permissions::setUserId() {
	return S_ISUID;
}

inline mode_t permissions::setGroupId() {
	return S_ISGID;
}
