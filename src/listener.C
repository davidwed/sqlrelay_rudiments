// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/listener.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/listenerinlines.h>
#endif

#include <sys/time.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <errno.h>

void listener::addFileDescriptor(int fd) {
	if (last) {
		last->next=new filedescriptornode;
		last->next->fd=fd;
		last->next->previous=last;
		last->next->next=NULL;
		last=last->next;
	} else {
		first=new filedescriptornode;
		first->fd=fd;
		first->previous=NULL;
		first->next=NULL;
		last=first;
	}
}

void listener::removeFileDescriptor(int fd) {
	current=first;
	while (current) {
		if (current->fd==fd) {
			if (current->next) {
				current->next->previous=current->previous;
			}
			if (current->previous) {
				current->previous->next=current->next;
			}
			if (current==first) {
				first=current->next;
			}
			if (current==last) {
				last=current->previous;
			}
			filedescriptornode	*temp=current;
			current=current->next;
			delete temp;
		} else {
			current=current->next;
		}
	}
}

void listener::removeAllFileDescriptors() {
	current=first;
	while (current) {
		last=current->next;
		delete current;
		current=last;
	}
}

int listener::waitForNonBlockingRead(long sec, long usec) {
	return safeSelect(sec,usec,1,0);
}

int listener::waitForNonBlockingWrite(long sec, long usec) {
	return safeSelect(sec,usec,0,1);
}

int listener::safeSelect(long sec, long usec, int read, int write) {

	for (;;) {

		// not sure why this has to be rebuilt every time, but
		// it appears to have to be
		fd_set	fdlist;
		int	largest;
		FD_ZERO(&fdlist);
		current=first;
		while (current) {
			if (current->fd>largest) {
				largest=current->fd;
			}
			FD_SET(current->fd,&fdlist);
			current=current->next;
		}

		// set up the timeout
		timeval	*tv;
		if (sec>-1 && usec>-1) {
			tv=new timeval;
			tv->tv_sec=sec;
			tv->tv_usec=usec;
		} else {
			tv=NULL;
		}

		// wait for data to be available on the file descriptor
		int	selectresult=select(largest+1,(read)?&fdlist:NULL,
						(write)?&fdlist:NULL,
						NULL,tv);
	
		// clean up
		delete tv;

		// if a signal caused the select to fall through, retry
		if (selectresult==-1 && retryinterruptedwaits && errno==EINTR) {
			continue;
		} else if (selectresult>-1) {
	
			// return the file descriptor that
			// caused the select to fall through
			current=first;
			while (current) {
				if (FD_ISSET(current->fd,&fdlist)) {
					return current->fd;
				}
				current=current->next;
			}
			break;
		}
	
		// return the result of the select
		return selectresult;
	}

	return -1;
}
