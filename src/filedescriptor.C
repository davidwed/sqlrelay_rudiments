// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/filedescriptor.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/filedescriptorinlines.h>
#endif
#include <errno.h>
#include <stdio.h>

// if SSIZE_MAX is undefined, choose a good safe value
// that should even work on 16-bit systems
#ifndef SSIZE_MAX
	#define SSIZE_MAX 32767
#endif


ssize_t filedescriptor::read(char **buffer, char *terminator) {

	// initialize a buffer
	int	buffersize=512;
	*buffer=new char[buffersize];

	// initialize termination detector
	int	termlen=strlen(terminator);
	char	*term=new char[termlen];
	for (int i=0; i<termlen; i++) {
		term[i]=(char)NULL;
	}

	// initialize some variables
	char	charbuffer;
	int	sizeread;
	int	totalread=0;
	int	escaped=0;
	int	copytobuffer;
	int	copytoterm;

	// loop, getting 1 character at a time
	while (1) {

		// read from the file descriptor
		if ((sizeread=safeRead((void *)&charbuffer,sizeof(char)))==-1) {
			totalread=-1;
			break;
		}
		totalread=totalread+sizeread;

		if (sizeread) {

			// handle escaping
			if (escaped) {

				copytobuffer=1;
				copytoterm=0;
				escaped=0;

			} else {

				if (charbuffer=='\\') {
					escaped=1;
				} else { 
					escaped=0;
				}

				if (escaped) {
					copytobuffer=0;
					copytoterm=0;
				} else {
					copytobuffer=1;
					copytoterm=1;
				}
			}

			// copy to return buffer
			if (copytobuffer) {

				// extend buffer if necessary
				if (totalread==buffersize) {
					char	*newbuffer=
						new char[buffersize+512];
					strncpy(newbuffer,*buffer,buffersize);
					delete *buffer;
					buffersize=buffersize+512;
					*buffer=newbuffer;
				}

				(*buffer)[totalread-1]=charbuffer;
				(*buffer)[totalread]=(char)NULL;
			}
			// handle escaping
			if (copytoterm) {

				// update terminator detector
				for (int i=0; i<termlen-1; i++) {
					term[i]=term[i+1];
				}
				term[termlen-1]=charbuffer;

				// check for termination
				if (!strncmp(term,terminator,termlen)) {
					break;
				}
			} else {

				// clear terminator
				for (int i=0; i<termlen; i++) {
					term[i]=(char)NULL;
				}
			}

		} else {
			break;
		}
	}

	delete[] term;
	return totalread;
}

ssize_t filedescriptor::safeRead(void *buf, ssize_t count) {

	ssize_t	totalread=0;
	ssize_t	sizetoread;
	ssize_t	actualread;
	while (totalread<count) {

		// only read SSIZE_MAX at a time
		sizetoread=count-totalread;
		if (sizetoread>SSIZE_MAX) {
			sizetoread=SSIZE_MAX;
		}

		// read...
		errno=0;
		actualread=::read(fd,(void *)((long)buf+totalread),sizetoread);

		// if we didn't read the number of bytes we expected to,
		// handle that...
		if (actualread!=sizetoread) {
			if (retryinterruptedreads && errno==EINTR) {
				// if we got an EINTR, then we need to
				// retry the read
				continue;
			} else if (actualread==0 && errno==0) {
				// eof condition
				return 0;
			}
		}

		totalread=totalread+actualread;
	}

	return totalread;
}

ssize_t filedescriptor::safeWrite(const void *buf, ssize_t count) {

	ssize_t	retval;
	for (;;) {
		errno=0;
		retval=::write(fd,buf,count);
		if (retval!=count) {
			if (retryinterruptedwrites && errno==EINTR) {
				continue;
			}
		}
		break;
	}
	return retval;
}

int filedescriptor::waitForNonBlockingRead(long sec, long usec) {
	return (lstnr)?lstnr->waitForNonBlockingRead(sec,usec):
			safeSelect(sec,usec,1,0);
}

int filedescriptor::waitForNonBlockingWrite(long sec, long usec) {
	return (lstnr)?lstnr->waitForNonBlockingWrite(sec,usec):
			safeSelect(sec,usec,0,1);
}

int filedescriptor::safeSelect(long sec, long usec, int read, int write) {

	for (;;) {

		// not sure why this has to be rebuilt every time, but
		// it appears to have to be
		fd_set	fdlist;
		FD_ZERO(&fdlist);
		FD_SET(fd,&fdlist);

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
		int	selectresult=select(fd+1,(read)?&fdlist:NULL,
						(write)?&fdlist:NULL,
						NULL,tv);
	
		// clean up
		delete tv;

		// if a signal caused the select to fall through, retry
		if (selectresult==-1) {
			if (retryinterruptedwaits && errno==EINTR) {
				continue;
			}
			break;
		}
	
		// return the result of the select
		return selectresult;
	}

	return -1;
}
