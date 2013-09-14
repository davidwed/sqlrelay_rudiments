// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/listener.h>
#include <rudiments/error.h>

// for FD_ZERO/FD_SET on windows
#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAS_SSL
	// Redhat 6.2 needs _GNU_SOURCE
	#ifndef _GNU_SOURCE
		#define _GNU_SOURCE
	#endif
	#include <openssl/ssl.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

// some systems need string.h to provide memset() for FD_ZERO/FD_SET
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_SELECT_H
	#include <sys/select.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

class listenerprivate {
	friend class listener;
	private:
		listenerlist	_filedescriptorlist;
		listenerlist	_readylist;
		bool		_retryinterruptedwaits;
};

listener::listener() {
	pvt=new listenerprivate;
	pvt->_retryinterruptedwaits=true;
}

listener::~listener() {
	removeAllFileDescriptors();
	delete pvt;
}

void listener::retryInterruptedWaits() {
	pvt->_retryinterruptedwaits=true;
}

void listener::dontRetryInterruptedWaits() {
	pvt->_retryinterruptedwaits=false;
}

void listener::addFileDescriptor(filedescriptor *fd) {
	pvt->_filedescriptorlist.append(fd);
}

void listener::removeFileDescriptor(filedescriptor *fd) {
	pvt->_filedescriptorlist.removeByData(fd);
}

void listener::removeAllFileDescriptors() {
	pvt->_filedescriptorlist.clear();
}

int32_t listener::waitForNonBlockingRead(long sec, long usec) {
	return safeSelect(sec,usec,true,false);
}

int32_t listener::waitForNonBlockingWrite(long sec, long usec) {
	return safeSelect(sec,usec,false,true);
}

listenerlist *listener::getReadyList() {
	return &pvt->_readylist;
}

int32_t listener::safeSelect(long sec, long usec, bool read, bool write) {

	// set up the timeout
	timeval	tv;
	timeval	*tvptr=(sec>-1 && usec>-1)?&tv:NULL;

	for (;;) {

		// if we're using ssl, some of the filedescriptors may have
		// SSL data pending, in that case, we need to bypass the
		// select altogether and just return those filedescriptors
		// in the ready list immediately
		#ifdef RUDIMENTS_HAS_SSL
			pvt->_readylist.clear();
		#endif
		int32_t	selectresult=0;

		// some versions of select modify the timeout, so reset it
		// every time
		tv.tv_sec=sec;
		tv.tv_usec=usec;

		// select() will modify the list every time it's called
		// so the list has to be rebuilt every time...
		fd_set	fdlist;
		int32_t	largest=-1;
		FD_ZERO(&fdlist);
		listenerlistnode	*current=
				pvt->_filedescriptorlist.getFirstNode();
		while (current) {

			if (current->getData()->getFileDescriptor()>largest) {
				largest=current->getData()->getFileDescriptor();
			}

			FD_SET(current->getData()->getFileDescriptor(),&fdlist);

			// if we support SSL, check here to see if the
			// filedescriptor has SSL data pending
			#ifdef RUDIMENTS_HAS_SSL
				SSL	*ssl=
					(SSL *)current->getData()->getSSL();
				if (ssl && SSL_pending(ssl)) {
					pvt->_readylist.append(
						current->getData());
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
			if (pvt->_retryinterruptedwaits &&
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
			pvt->_readylist.clear();
		#endif
		current=pvt->_filedescriptorlist.getFirstNode();
		while (current) {
			if (FD_ISSET(current->getData()->getFileDescriptor(),
					&fdlist)) {
				pvt->_readylist.append(current->getData());
			}
			current=current->getNext();
		}

		// return the number of file descriptors that
		// caused the select() to fall through
		return selectresult;
	}
}
