// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/filedescriptor.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/filedescriptorinlines.h>
#endif
#include <errno.h>

// if SSIZE_MAX is undefined, choose a good safe value
// that should even work on 16-bit systems
#ifndef SSIZE_MAX
	#define SSIZE_MAX 32767
#endif


ssize_t	filedescriptor::read(char **buffer, char *terminator) {

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

ssize_t	filedescriptor::safeRead(void *buf, ssize_t count) {

	// handle cases where count>SSIZE_MAX by only 
	// reading SSIZE_MAX at a time
	ssize_t	realcount=count;
	if (count>SSIZE_MAX) {
		realcount=SSIZE_MAX;
	}

	ssize_t	retval;
	for (;;) {
		errno=0;
		retval=::read(fd,buf,realcount);
		if (retval!=realcount) {
			if (retryinterruptedreads && errno==EINTR) {
				// if we got an EINTR, then we need to
				// retry the read
				continue;
			} else if (retval==0 && errno==0) {
				// eof condition
				return 0;
			} else if (retval>-1 && errno==0) {
				// If we got no error, just didn't get
				// all of the data, we need to get the
				// rest.
				return retval+safeRead(
						(void *)((ssize_t)buf+retval),
							realcount-retval);
			}
		}
		break;
	}

	// handle cases where count>SSIZE_MAX by continuing to read
	if (realcount<count) {
		return retval+safeRead((void *)((ssize_t)buf+retval),
						count-retval);
	}

	return retval;
}

ssize_t	filedescriptor::safeWrite(const void *buf, ssize_t count) {

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
