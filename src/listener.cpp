// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/listener.h>
#include <rudiments/error.h>
#include <rudiments/linkedlist.h>

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
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_EVENT_H
	#include <sys/event.h>
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
#if defined(RUDIMENTS_HAVE_SYS_EPOLL_H)
	#include <sys/epoll.h>
#endif
#if defined(RUDIMENTS_HAVE_PORT_H)
	#include <port.h>
#endif
#if defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H)
	#include <rudiments/device.h>
	#include <sys/devpoll.h>
#endif
#if defined(RUDIMENTS_HAVE_SYS_POLL_H)
	#include <sys/poll.h>
#elif defined(RUDIMENTS_HAVE_POLL_H)
	#include <poll.h>
#endif

// disable this for now
#undef RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT

struct fddata_t {
	filedescriptor	*fd;
	bool		read;
	bool		write;
};

class listenerprivate {
	friend class listener;
	private:
		linkedlist< fddata_t * >	_fdlist;
		listenerlist			_readreadylist;
		listenerlist			_writereadylist;
		bool				_retryinterruptedwaits;
		bool				_dirty;

		#if defined(RUDIMENTS_HAVE_KQUEUE)
			int32_t			_kq;
			struct kevent		*_kevs;
			struct kevent		*_rkevs;
		#elif defined(RUDIMENTS_HAVE_EPOLL)
			int32_t			_epfd;
			struct epoll_event	*_evs;
			struct epoll_event	*_revs;
		#elif defined(RUDIMENTS_HAVE_PORT_CREATE)
			int32_t			_port;
			port_event_t		_pev;
		#elif defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H)
			device			_dpfd;
			struct dvpoll		_dvp;
			struct pollfd		*_fds;
		#elif defined(RUDIMENTS_HAVE_POLL)
			struct pollfd		*_fds;
		#elif defined(RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT)
			HANDLE 			_cp;
		#endif
};

listener::listener() {
	pvt=new listenerprivate;
	pvt->_retryinterruptedwaits=true;
	pvt->_dirty=true;
	#if defined(RUDIMENTS_HAVE_KQUEUE)
		pvt->_kq=-1;
		pvt->_kevs=NULL;
		pvt->_rkevs=NULL;
	#elif defined(RUDIMENTS_HAVE_EPOLL)
		pvt->_epfd=-1;
		pvt->_evs=NULL;
		pvt->_revs=NULL;
	#elif defined(RUDIMENTS_HAVE_PORT_CREATE)
		pvt->_port=-1;
	#elif defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H)
		pvt->_fds=NULL;
	#elif defined(RUDIMENTS_HAVE_POLL)
		pvt->_fds=NULL;
	#elif defined(RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT)
		pvt->_cp=NULL;
	#endif
}

listener::~listener() {
	removeAllFileDescriptors();
	cleanUp();
	delete pvt;
}

void listener::cleanUp() {
	#if defined(RUDIMENTS_HAVE_KQUEUE)
		if (pvt->_kq!=-1) {
			::close(pvt->_kq);
		}
		delete[] pvt->_kevs;
		delete[] pvt->_rkevs;
	#elif defined(RUDIMENTS_HAVE_EPOLL)
		if (pvt->_epfd!=-1) {
			::close(pvt->_epfd);
		}
		delete[] pvt->_evs;
		delete[] pvt->_revs;
	#elif defined(RUDIMENTS_HAVE_PORT_CREATE)
		if (pvt->_port!=-1) {
			::close(pvt->_port);
		}
	#elif defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H)
		pvt->_dpfd.close();
		delete[] pvt->_fds;
	#elif defined(RUDIMENTS_HAVE_POLL)
		delete[] pvt->_fds;
	#elif defined(RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT)
		CloseHandle(pvt->_cp);
	#endif
}

void listener::retryInterruptedWaits() {
	pvt->_retryinterruptedwaits=true;
}

void listener::dontRetryInterruptedWaits() {
	pvt->_retryinterruptedwaits=false;
}

void listener::addFileDescriptor(filedescriptor *fd) {
	addFileDescriptor(fd,true,true);
}

void listener::addFileDescriptor(filedescriptor *fd, bool read, bool write) {
	fddata_t	*fdd=new fddata_t;
	fdd->fd=fd;
	fdd->read=read;
	fdd->write=write;
	pvt->_fdlist.append(fdd);
	pvt->_dirty=true;
}

void listener::addReadFileDescriptor(filedescriptor *fd) {
	addFileDescriptor(fd,true,false);
}

void listener::addWriteFileDescriptor(filedescriptor *fd) {
	addFileDescriptor(fd,false,true);
}

void listener::removeFileDescriptor(filedescriptor *fd) {
	linkedlistnode< fddata_t * > *node=pvt->_fdlist.getFirst();
	while (node) {
		linkedlistnode< fddata_t * >	*next=node->getNext();
		if (node->getValue()->fd==fd) {
			delete node->getValue();
			pvt->_fdlist.remove(node);
		}
		node=next;
	}
	pvt->_dirty=true;
	return;
}

void listener::removeAllFileDescriptors() {
	for (linkedlistnode< fddata_t * > *node=pvt->_fdlist.getFirst();
						node; node=node->getNext()) {
		delete node->getValue();
	}
	pvt->_fdlist.clear();
	pvt->_dirty=true;
}

listenerlist *listener::getReadReadyList() {
	return &pvt->_readreadylist;
}

listenerlist *listener::getWriteReadyList() {
	return &pvt->_writereadylist;
}

int32_t listener::listen() {
	return listen(-1,-1);
}

int32_t listener::listen(int32_t sec, int32_t usec) {

	// initialize the return value
	int32_t	result=0;

	// clear the read ready list
	pvt->_readreadylist.clear();

	// return immediately if any of the filedescriptors
	// have SSL or GSS data pending
	for (linkedlistnode< fddata_t * >	*node=
					pvt->_fdlist.getFirst();
					node; node=node->getNext()) {

		#ifdef RUDIMENTS_HAS_SSL
			SSL	*ssl=(SSL *)node->getValue()->fd->getSSL();
			if (ssl && SSL_pending(ssl)) {
				pvt->_readreadylist.append(
						node->getValue()->fd);
				result++;
			}
		#endif
		
		securitycontext	*sctx=
			node->getValue()->fd->getSecurityContext();
		if (sctx && sctx->pending()) {
			pvt->_readreadylist.append(node->getValue()->fd);
			result++;
		}
	}
	if (result) {
		return result;
	}

	// clear the write ready list
	pvt->_writereadylist.clear();

	// rebuild the list of fd's to be monitored, if necessary
	#if defined(RUDIMENTS_HAVE_PORT_CREATE)
		// When using port_create() we must rebuild the list
		// every time.  rebuildMonitorList() handles dirtiness
		// internally when using port_create() too.
		if (!rebuildMonitorList()) {
			return RESULT_ERROR;
		}
	#elif defined(RUDIMENTS_HAVE_KQUEUE) || \
			defined(RUDIMENTS_HAVE_EPOLL) || \
			defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H) || \
			defined(RUDIMENTS_HAVE_POLL) || \
			defined(RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT)
		// only rebuild if dirty when using other methods
		if (pvt->_dirty) {
			if (!rebuildMonitorList()) {
				return RESULT_ERROR;
			}
		}
	#endif

	// set up the timeout
	#if defined(RUDIMENTS_HAVE_KQUEUE)
		struct timespec	ts;
		ts.tv_sec=sec;
		ts.tv_nsec=usec*1000;
		struct timespec	*tsptr=(sec>-1 && usec>-1)?&ts:NULL;
	#elif defined(RUDIMENTS_HAVE_EPOLL)
		int32_t	timeout=(sec>-1 && usec>-1)?
					(sec*1000)+(usec/1000):-1;
	#elif defined(RUDIMENTS_HAVE_PORT_CREATE)
		struct timespec	ts;
		ts.tv_sec=sec;
		ts.tv_nsec=usec*1000;
		struct timespec	*tsptr=(sec>-1 && usec>-1)?&ts:NULL;
	#elif defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H)
		pvt->_dvp.dp_timeout=
			(sec>-1 && usec>-1)?(sec*1000)+(usec/1000):-1;
	#elif defined(RUDIMENTS_HAVE_POLL)
		// In theory, any negative value will cause poll to wait
		// forever, but certain implementations (such as glibc-2.0.7)
		// require it to be -1.
		int32_t	timeout=(sec>-1 && usec>-1)?
					(sec*1000)+(usec/1000):-1;
	#elif defined(RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT)
		int32_t	timeout=(sec>-1 && usec>-1)?
					(sec*1000)+(usec/1000):INFINITE;
	#else
		timeval	tv;
		timeval	*tvptr=(sec>-1 && usec>-1)?&tv:NULL;
	#endif

	#if (defined(RUDIMENTS_HAVE_KQUEUE) || \
			defined(RUDIMENTS_HAVE_EPOLL) || \
			defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H) || \
			defined(RUDIMENTS_HAVE_POLL) || \
			defined(RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT)) && \
			!defined(RUDIMENTS_HAVE_PORT_CREATE)
		uint64_t	fdcount=pvt->_fdlist.getLength();
	#endif
	error::clearError();
	for (;;) {

		#if defined(RUDIMENTS_HAVE_KQUEUE)

			// wait for non-blocking io
			result=kevent(pvt->_kq,pvt->_kevs,fdcount,
						pvt->_rkevs,fdcount,
						tsptr);

		#elif defined(RUDIMENTS_HAVE_EPOLL)

			// wait for non-blocking io
			result=epoll_wait(pvt->_epfd,
						pvt->_revs,fdcount,
						timeout);

		#elif defined(RUDIMENTS_HAVE_PORT_CREATE)

			// wait for an event
			result=port_get(pvt->_port,&pvt->_pev,tsptr);
			if (!result) {
				result=1;
			} else if (result==-1 &&
					error::getErrorNumber()==ETIME) {
				result=0;
			}

		#elif defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H)

			// wait for non-blocking io
			result=pvt->_dpfd.ioCtl(DP_POLL,(void *)&pvt->_dvp);

		#elif defined(RUDIMENTS_HAVE_POLL)

			// wait for non-blocking io
			result=poll(pvt->_fds,fdcount,timeout);

		#elif defined(RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT)

			// wait for an operation to complete
			DWORD		bytes;
			filedescriptor	*fd;
			OVERLAPPED	*overlapped;
			result=(GetQueuedCompletionStatus(pvt->_cp,&bytes,
						(ULONG_PTR *)&fd,
						&overlapped,timeout)==TRUE);

		#else

			// clear the ready lists
			pvt->_readreadylist.clear();
			pvt->_writereadylist.clear();

			// initialize the return value
			result=0;

			// some versions of select modify the timeout,
			// so reset it every time
			tv.tv_sec=sec;
			tv.tv_usec=usec;

			// select() modifies the lists so they
			// have to be rebuilt every time...
			fd_set	readlist;
			fd_set	writelist;
			int32_t	readlargest=-1;
			int32_t	writelargest=-1;
			int32_t	largest=-1;
			FD_ZERO(&readlist);
			FD_ZERO(&writelist);
			for (linkedlistnode< fddata_t * >	*node=
						pvt->_fdlist.getFirst();
						node; node=node->getNext()) {

				int32_t	fd=node->getValue()->
						fd->getFileDescriptor();

				if (node->getValue()->read) {
					if (fd>readlargest) {
						readlargest=fd;
					}

					FD_SET(fd,&readlist);

					// if we support SSL, check here to
					// see if the filedescriptor has SSL
					// data pending
					#ifdef RUDIMENTS_HAS_SSL
						SSL	*ssl=(SSL *)node->
							getValue()->fd->getSSL();
						if (ssl && SSL_pending(ssl)) {
							pvt->_readreadylist.
								append(
								node->
								getValue()->fd);
							result++;
						}
					#endif
				}

				if (node->getValue()->write) {
					if (fd>writelargest) {
						writelargest=fd;
					}

					FD_SET(fd,&writelist);
				}

				if (fd>largest) {
					largest=fd;
				}
			}

			// if we support SSL then return here if even 1 of the
			// filedescriptors had SSL data pending
			#ifdef RUDIMENTS_HAS_SSL
				if (result) {
					return result;
				}
			#endif

			// wait for data to be available on the file descriptor
			result=select(largest+1,
					(readlargest>-1)?&readlist:NULL,
					(writelargest>-1)?&writelist:NULL,
					NULL,tvptr);
		#endif

		if (result==-1) {

			// if a signal caused the wait to fall through, retry
			if (pvt->_retryinterruptedwaits &&
				error::getErrorNumber()==EINTR) {
				continue;
			}
			result=RESULT_ERROR;

		} else if (!result) {

			// timeout
			result=RESULT_TIMEOUT;

		} else {

			// build the list of file descriptors that
			// caused the wait to fall through
			pvt->_readreadylist.clear();
			pvt->_writereadylist.clear();
			#if defined(RUDIMENTS_HAVE_KQUEUE)
				for (int32_t i=0; i<result; i++) {
					if (pvt->_rkevs[i].filter==
								EVFILT_READ) {
						pvt->_readreadylist.append(
							(filedescriptor *)
							pvt->_rkevs[i].udata);
					} else if (pvt->_rkevs[i].filter==
								EVFILT_WRITE) {
						pvt->_readreadylist.append(
							(filedescriptor *)
							pvt->_rkevs[i].udata);
					}
				}
			#elif defined(RUDIMENTS_HAVE_EPOLL)
				for (int32_t i=0; i<result; i++) {
					if (pvt->_revs[i].events&EPOLLIN) {
						pvt->_readreadylist.append(
							(filedescriptor *)
							pvt->_revs[i].data.ptr);
					}
					if (pvt->_revs[i].events&EPOLLOUT) {
						pvt->_writereadylist.append(
							(filedescriptor *)
							pvt->_revs[i].data.ptr);
					}
				}
			#elif defined(RUDIMENTS_HAVE_PORT_CREATE)
				// port_get() only returns a single fd
				int32_t	events=0;
				if (pvt->_pev.portev_events&POLLIN) {
					pvt->_readreadylist.append(
						(filedescriptor *)
						pvt->_pev.portev_user);
					events|=POLLIN;
				}
				if (pvt->_pev.portev_events&POLLOUT) {
					pvt->_writereadylist.append(
						(filedescriptor *)
						pvt->_pev.portev_user);
					events|=POLLOUT;
				}
			#elif defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H) || \
						defined(RUDIMENTS_HAVE_POLL)
				for (uint64_t i=0; i<fdcount; i++) {
					if (pvt->_fds[i].revents) {
						for (linkedlistnode< fddata_t * > *node=pvt->_fdlist.getFirst(); node; node=node->getNext()) {
							if (node->getValue()->fd->getFileDescriptor()==pvt->_fds[i].fd) {
								if (pvt->_fds[i].revents&POLLIN) {
									pvt->_readreadylist.append(node->getValue()->fd);
								}
								if (pvt->_fds[i].revents&POLLOUT) {
									pvt->_writereadylist.append(node->getValue()->fd);
								}
								break;
							}
						}
					}
				}

			#elif defined(RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT)

				// FIXME: how to determine read vs. write?
				if (true) {
					// read...
					pvt->_readreadylist.append(fd);
				} else {
					// write...
					pvt->_writereadylist.append(fd);
				}

			#else
				for (linkedlistnode< fddata_t * > *node=
						pvt->_fdlist.getFirst();
						node; node=node->getNext()) {
					if (FD_ISSET(node->getValue()->fd->
							getFileDescriptor(),
							&readlist)) {
						pvt->_readreadylist.append(
							node->getValue()->fd);
					}
					if (FD_ISSET(node->getValue()->fd->
							getFileDescriptor(),
							&writelist)) {
						pvt->_writereadylist.append(
							node->getValue()->fd);
					}
				}
			#endif

		}

		// return the result
		return result;
	}
}

bool listener::rebuildMonitorList() {

	// clean up
	#if defined(RUDIMENTS_HAVE_PORT_CREATE)
		// don't clean up unless the dirty
		// flag is set with port_create()
		if (pvt->_dirty) {
			cleanUp();
		}
	#else
		// clean up every time when not using port_create()
		cleanUp();
	#endif

	// reinit list resources
	uint64_t	fdcount=pvt->_fdlist.getLength();
	#if defined(RUDIMENTS_HAVE_KQUEUE)
		pvt->_kq=kqueue();
		if (pvt->_kq==-1) {
			return false;
		}
		pvt->_kevs=new struct kevent[fdcount];
		pvt->_rkevs=new struct kevent[fdcount];
	#elif defined(RUDIMENTS_HAVE_EPOLL)
		pvt->_epfd=epoll_create1(0);
		if (pvt->_epfd==-1) {
			return false;
		}
		pvt->_evs=new struct epoll_event[fdcount];
		pvt->_revs=new struct epoll_event[fdcount];
	#elif defined(RUDIMENTS_HAVE_PORT_CREATE)
		// don't create the port unless the
		// dirty flag is set with port_create()
		if (pvt->_dirty) {
			pvt->_port=port_create();
			if (pvt->_port==-1) {
				return false;
			}
		}
	#elif defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H)
		if (!pvt->_dpfd.open("/dev/poll",O_RDWR)) {
			return false;
		}
		pvt->_fds=new struct pollfd[fdcount];
	#elif defined(RUDIMENTS_HAVE_POLL)
		pvt->_fds=new struct pollfd[fdcount];
	#elif defined(RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT)
		pvt->_cp=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	#endif

	// set up the fd's to be monitored and how to monitor them
	fdcount=0;
	for (linkedlistnode< fddata_t * > *node=pvt->_fdlist.getFirst();
						node; node=node->getNext()) {

		#if defined(RUDIMENTS_HAVE_KQUEUE)

			short	filter=0;
			if (node->getValue()->read) {
				filter|=EVFILT_READ;
			}
			if (node->getValue()->write) {
				filter|=EVFILT_WRITE;
			}

			// do this here rather than inside of the EV_SET, older
			// compilers don't like the ifdef inside of a macro
			#ifdef RUDIMENTS_HAVE_KQUEUE_VOID_UDATA
			void		*fdptr=(void *)node->getValue()->fd;
			#else
			intptr_t	fdptr=(intptr_t)node->getValue()->fd;
			#endif

			EV_SET(&pvt->_kevs[fdcount],
				node->getValue()->fd->getFileDescriptor(),
				filter,EV_ADD|EV_ENABLE,0,0,fdptr);
			EV_SET(&pvt->_rkevs[fdcount],0,0,0,0,0,0);

		#elif defined(RUDIMENTS_HAVE_EPOLL)

			pvt->_evs[fdcount].data.ptr=
					(void *)node->getValue()->fd;
			pvt->_evs[fdcount].events=0;
			if (node->getValue()->read) {
				pvt->_evs[fdcount].events|=EPOLLIN;
			}
			if (node->getValue()->write) {
				pvt->_evs[fdcount].events|=EPOLLOUT;
			}
			if (epoll_ctl(pvt->_epfd,EPOLL_CTL_ADD,
				node->getValue()->fd->getFileDescriptor(),
				&pvt->_evs[fdcount])) {
				return false;
			}

		#elif defined(RUDIMENTS_HAVE_PORT_CREATE)

			int32_t	events=0;
			if (node->getValue()->read) {
				events|=POLLIN;
			}
			if (node->getValue()->write) {
				events|=POLLOUT;
			}
			if (port_associate(pvt->_port,PORT_SOURCE_FD,
				node->getValue()->fd->getFileDescriptor(),
				events,(void *)node->getValue()->fd)) {
				return false;
			}

		#elif defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H) || \
					defined(RUDIMENTS_HAVE_POLL)

			pvt->_fds[fdcount].fd=
				node->getValue()->fd->getFileDescriptor();
			pvt->_fds[fdcount].events=0;
			if (node->getValue()->read) {
				pvt->_fds[fdcount].events|=POLLIN;
			}
			if (node->getValue()->write) {
				pvt->_fds[fdcount].events|=POLLOUT;
			}
			pvt->_fds[fdcount].revents=0;

		#elif defined(RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT)

			// FIXME: read/write flag?
			if (node->getValue()->read) {
				// zero-byte non-blocking read?
			}
			if (node->getValue()->write) {
				// zero-byte non-blocking write?
			}
			filedescriptor	*fd=node->getValue()->fd;
			pvt->_cp=CreateIoCompletionPort(
					(HANDLE)
					fd->getHandleFromFileDescriptor(
						fd->getFileDescriptor()),
					pvt->_cp,
					(ULONG_PTR)node->getValue()->fd,
					0);

		#endif

		fdcount++;
	}

	#if defined(RUDIMENTS_HAVE_SYS_DEVPOLL_H) && \
		!defined(RUDIMENTS_HAVE_PORT_CREATE)
		if (pvt->_dpfd.write((const void *)pvt->_fds,
				sizeof(struct pollfd)*fdcount)!=
				(ssize_t)(sizeof(struct pollfd)*fdcount)) {
			return false;
		}
		pvt->_dvp.dp_nfds=fdcount;
		pvt->_dvp.dp_fds=pvt->_fds;
	#endif

	// not dirty any more
	pvt->_dirty=false;

	return true;
}
