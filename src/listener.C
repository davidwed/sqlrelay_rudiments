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

int listener::safeSelect(long sec, long usec, int read, int write) {

	for (;;) {

		// not sure why this has to be rebuilt every time, but
		// it appears to have to be
		fd_set	fdlist;
		int	largest;
		FD_ZERO(&fdlist);
		//primitivelistnode<int>	*current=
		listenerlistnode	*current=
				filedescriptorlist.getNodeByIndex(0);
		while(current) {
			if (current->getData()>largest) {
				largest=current->getData();
			}
			FD_SET(current->getData(),&fdlist);
			current=(listenerlistnode *)current->getNext();
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
			current=filedescriptorlist.getNodeByIndex(0);
			while (current) {
				if (FD_ISSET(current->getData(),&fdlist)) {
					return current->getData();
				}
				current=(listenerlistnode *)current->getNext();
			}
			break;
		}
	
		// return the result of the select
		return selectresult;
	}

	return -1;
}
