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
#if defined(RUDIMENTS_HAVE_SYS_POLL_H)
	#include <sys/poll.h>
#elif defined(RUDIMENTS_HAVE_POLL_H)
	#include <poll.h>
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
	pvt->_filedescriptorlist.removeByValue(fd);
}

void listener::removeAllFileDescriptors() {
	pvt->_filedescriptorlist.clear();
}

int32_t listener::waitForNonBlockingRead(int32_t sec, int32_t usec) {
	return safeWait(sec,usec,true,false);
}

int32_t listener::waitForNonBlockingWrite(int32_t sec, int32_t usec) {
	return safeWait(sec,usec,false,true);
}

listenerlist *listener::getReadyList() {
	return &pvt->_readylist;
}

int32_t listener::safeWait(int32_t sec, int32_t usec, bool read, bool write) {

	// initialize the return value
	int32_t	result=0;

	#ifdef RUDIMENTS_HAVE_POLL

		// if we're using ssl, some of the filedescriptors may have
		// SSL data pending, in that case, we need to bypass the
		// poll altogether and just return those filedescriptors
		// in the ready list immediately
		#ifdef RUDIMENTS_HAS_SSL
			pvt->_readylist.clear();
		#endif

		// set up the fd's to be monitored, and how to montior them
		uint64_t	fdcount=pvt->_filedescriptorlist.getLength();
		struct pollfd	*fds=new struct pollfd[fdcount];
		listenerlistnode	*cur=
				pvt->_filedescriptorlist.getFirstNode();
		fdcount=0;
		while (cur) {

			// if we support SSL, check here to see if the
			// filedescriptor has SSL data pending
			#ifdef RUDIMENTS_HAS_SSL
				SSL	*ssl=(SSL *)cur->getValue()->getSSL();
				if (ssl && SSL_pending(ssl)) {
					pvt->_readylist.append(cur->getValue());
					result++;
				}
			#endif

			fds[fdcount].fd=cur->getValue()->getFileDescriptor();
			fds[fdcount].events=0;
			if (read) {
				fds[fdcount].events|=POLLIN;
			}
			if (write) {
				fds[fdcount].events|=POLLOUT;
			}
			fds[fdcount].revents=0;
			fdcount++;

			cur=cur->getNext();
		}

		// if we support SSL and at least 1 of the
		// filedescriptors had SSL data pending, return here
		#ifdef RUDIMENTS_HAS_SSL
			if (result) {
				delete[] fds;
				return result;
			}
		#endif

		// calculate the timeout
		int32_t	timeout=(sec*1000)+(usec/1000);

	#else

		// set up the timeout
		timeval	tv;
		timeval	*tvptr=(sec>-1 && usec>-1)?&tv:NULL;
	#endif

	for (;;) {

		#ifdef RUDIMENTS_HAVE_POLL

			// wait for data to be available on the file descriptor
			result=poll(fds,fdcount,timeout);

		#else

			// some versions of select modify the timeout,
			// so reset it every time
			tv.tv_sec=sec;
			tv.tv_usec=usec;

			// if we're using ssl, some of the filedescriptors may
			// have SSL data pending, in that case, we need to
			// bypass the select altogether and just return those
			// filedescriptors in the ready list immediately
			#ifdef RUDIMENTS_HAS_SSL
				pvt->_readylist.clear();
			#endif
			result=0;

			// select() will modify the list every time it's called
			// so the list has to be rebuilt every time...
			fd_set	fdlist;
			int32_t	largest=-1;
			FD_ZERO(&fdlist);
			listenerlistnode	*cur=
					pvt->_filedescriptorlist.getFirstNode();
			while (cur) {

				if (cur->getValue()->
						getFileDescriptor()>largest) {
					largest=cur->getValue()->
							getFileDescriptor();
				}

				FD_SET(cur->getValue()->
						getFileDescriptor(),&fdlist);

				// if we support SSL, check here to see if the
				// filedescriptor has SSL data pending
				#ifdef RUDIMENTS_HAS_SSL
					SSL	*ssl=
						(SSL *)cur->
						getValue()->getSSL();
					if (ssl && SSL_pending(ssl)) {
						pvt->_readylist.append(
							cur->getValue());
						result++;
					}
				#endif

				cur=cur->getNext();
			}

			// if we support SSL and at least 1 of the
			// filedescriptors had SSL data pending, return here
			#ifdef RUDIMENTS_HAS_SSL
				if (result) {
					return result;
				}
			#endif

			// wait for data to be available on the file descriptor
			result=select(largest+1,
						(read)?&fdlist:NULL,
						(write)?&fdlist:NULL,
						NULL,tvptr);
		#endif

		if (result==-1) {

			// if a signal caused the wait to fall through, retry
			if (pvt->_retryinterruptedwaits &&
				error::getErrorNumber()==EINTR) {
				continue;
			}
			return RESULT_ERROR;

		} else if (!result) {

			// timeout
			return RESULT_TIMEOUT;
		}

		// build the list of file descriptors that
		// caused the wait to fall through
		#ifndef RUDIMENTS_HAS_SSL
			pvt->_readylist.clear();
		#endif
	
		#ifdef RUDIMENTS_HAVE_POLL
			for (uint64_t i=0; i<fdcount; i++) {
				if (fds[i].revents) {
					cur=pvt->_filedescriptorlist.
							getFirstNode();
					while (cur) {
						if (cur->getValue()->
							getFileDescriptor()==
							fds[i].fd) {
							pvt->_readylist.
							append(
							cur->getValue());
							break;
						}
						cur=cur->getNext();
					}
				}
			}

		#else
	
			cur=pvt->_filedescriptorlist.getFirstNode();
			while (cur) {
				if (FD_ISSET(cur->getValue()->
						getFileDescriptor(),
						&fdlist)) {
					pvt->_readylist.append(
						cur->getValue());
				}
				cur=cur->getNext();
			}
		#endif

		// return the number of file descriptors that
		// caused the select() to fall through
		return result;
	}
}
