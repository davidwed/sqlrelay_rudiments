// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/listener.h>

#include <stdlib.h>
#include <sys/time.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <errno.h>

listener::listener() {
	retryinterruptedwaits=true;
}

listener::~listener() {
	removeAllFileDescriptors();
}

void listener::retryInterruptedWaits() {
	retryinterruptedwaits=true;
}

void listener::dontRetryInterruptedWaits() {
	retryinterruptedwaits=false;
}

void listener::addFileDescriptor(int fd) {
	filedescriptorlist.append(fd);
}

void listener::removeFileDescriptor(int fd) {
	filedescriptorlist.removeByData(fd);
}

void listener::removeAllFileDescriptors() {
	filedescriptorlist.clear();
}

int listener::waitForNonBlockingRead(long sec, long usec) {
	return safeSelect(sec,usec,true,false);
}

int listener::waitForNonBlockingWrite(long sec, long usec) {
	return safeSelect(sec,usec,false,true);
}

listenerlist *listener::getReadyList() {
	return &readylist;
}

int listener::safeSelect(long sec, long usec, bool read, bool write) {

	// set up the timeout
	timeval	tv;
	timeval	*tvptr=(sec>-1 && usec>-1)?&tv:NULL;

	for (;;) {

		// some versions of select modify the timeout, so reset it
		// every time
		tv.tv_sec=sec;
		tv.tv_usec=usec;

		// select() will modify the list every time it's called
		// so the list has to be rebuilt every time...
		fd_set	fdlist;
		int	largest=-1;
		FD_ZERO(&fdlist);
		listenerlistnode	*current=
				filedescriptorlist.getNodeByIndex(0);
		while(current) {
			if (current->getData()>largest) {
				largest=current->getData();
			}
			FD_SET(current->getData(),&fdlist);
			current=current->getNext();
		}

		// wait for data to be available on the file descriptor
		int	selectresult=select(largest+1,(read)?&fdlist:NULL,
						(write)?&fdlist:NULL,
						NULL,tvptr);

		if (selectresult==-1) {

			// if a signal caused the select to fall through, retry
			if (retryinterruptedwaits && errno==EINTR) {
				continue;
			}
			return RESULT_ERROR;

		} else if (!selectresult) {

			// timeout
			return RESULT_TIMEOUT;
		}
	
		// build the list of file descriptors that
		// caused the select() to fall through
		readylist.clear();
		current=filedescriptorlist.getNodeByIndex(0);
		while (current) {
			if (FD_ISSET(current->getData(),&fdlist)) {
				readylist.append(current->getData());
			}
			current=current->getNext();
		}

		// return the number of file descriptors that
		// caused the select() to fall through
		return selectresult;
	}
}
