// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/listener.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/listenerinlines.h>
#endif

#include <sys/time.h>
#include <sys/types.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <errno.h>

void	listener::addFileDescriptor(int fd) {
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

void	listener::removeFileDescriptor(int fd) {
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

void	listener::removeAllFileDescriptors() {
	current=first;
	while (current) {
		last=current->next;
		delete current;
		current=last;
	}
}

int	listener::waitForData(long timeoutsec, long timeoutusec) {

	for (;;) {

		// not sure why this has to rebuild every time, but
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
		if (timeoutsec>-1 && timeoutusec>-1) {
			tv=new timeval;
			tv->tv_sec=timeoutsec;
			tv->tv_usec=timeoutusec;
		} else {
			tv=NULL;
		}

		// wait for one of the file descriptors to be ready
		int	selectresult=select(largest+1,&fdlist,NULL,NULL,tv);
	
		// clean up
		delete tv;

		// if a signal caused the select to fall through, retry
		if (selectresult==-1 && errno==EINTR) {
			continue;
		}
	
		// handle non-error
		if (selectresult>-1) {
	
			// return the file descriptor that
			// caused the select to fall through
			current=first;
			while (current) {
				if (FD_ISSET(current->fd,&fdlist)) {
					return current->fd;
					break;
				}
				current=current->next;
			}
		}
	}

	return -1;
}
