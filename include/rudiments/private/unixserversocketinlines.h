// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline unixserversocket::unixserversocket() : unixsocket(), serversocket() {
	mask=0;
}

inline int	unixserversocket::listenOnSocket(const char *filename,
						mode_t mask, int backlog) {
	initialize(filename,mask);
	return bind() && listen(backlog);
}

inline int	unixserversocket::lingerOnClose(int timeout) {
	return serversocket::lingerOnClose(timeout);
}

inline int	unixserversocket::dontLingerOnClose() {
	return serversocket::dontLingerOnClose();
}

inline int	unixserversocket::reuseAddresses() {
	return serversocket::reuseAddresses();
}

inline int	unixserversocket::dontReuseAddresses() {
	return serversocket::dontReuseAddresses();
}

inline int	unixserversocket::listen(int backlog) {
	return serversocket::listen(backlog);
}

inline int	unixserversocket::passFileDescriptor(int descriptor) {
	return unixsocket::passFileDescriptor(descriptor);
}

inline int	unixserversocket::receiveFileDescriptor(int *descriptor) {
	return unixsocket::receiveFileDescriptor(descriptor);
}
