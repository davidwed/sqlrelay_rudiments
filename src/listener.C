// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/listener.h>
#include <rudiments/error.h>

#include <stdlib.h>

// some systems need string.h to provide memset() for FD_ZERO/FD_SET
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

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

void listener::addFileDescriptor(filedescriptor *fd) {
	filedescriptorlist.append(fd);
}

void listener::removeFileDescriptor(filedescriptor *fd) {
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

		// if we're using ssl, some of the filedescriptors may have
		// SSL data pending, in that case, we need to bypass the
		// select altogether and just return those filedescriptors
		// in the ready list immediately
		#ifdef RUDIMENTS_HAS_SSL
			readylist.clear();
		#endif
		int	selectresult=0;

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

			if (current->getData()->getFileDescriptor()>largest) {
				largest=current->getData()->getFileDescriptor();
			}

			FD_SET(current->getData()->getFileDescriptor(),&fdlist);

			// if we support SSL, check here to see if the
			// filedescriptor has SSL data pending
			#ifdef RUDIMENTS_HAS_SSL
				SSL	*ssl=current->getData()->getSSL();
				if (ssl && SSL_pending(ssl)) {
					readylist.append(current->getData());
					selectresult++;
				}
			#endif

			current=current->getNext();
		}

		// if we support SSL and at least 1 of the filedescriptors
		// had SSL data pending, return here
		#ifdef RUDIMENTS_HAS_SSL
			if (selectresult) {
				return selectresult;
			}
		#endif

		// wait for data to be available on the file descriptor
		selectresult=select(largest+1,(read)?&fdlist:NULL,
						(write)?&fdlist:NULL,
						NULL,tvptr);

		if (selectresult==-1) {

			// if a signal caused the select to fall through, retry
			if (retryinterruptedwaits &&
				error::getErrorNumber()==EINTR) {
				continue;
			}
			return RESULT_ERROR;

		} else if (!selectresult) {

			// timeout
			return RESULT_TIMEOUT;
		}
	
		// build the list of file descriptors that
		// caused the select() to fall through
		#ifndef RUDIMENTS_HAS_SSL
			readylist.clear();
		#endif
		current=filedescriptorlist.getNodeByIndex(0);
		while (current) {
			if (FD_ISSET(current->getData()->getFileDescriptor(),
					&fdlist)) {
				readylist.append(current->getData());
			}
			current=current->getNext();
		}

		// return the number of file descriptors that
		// caused the select() to fall through
		return selectresult;
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
