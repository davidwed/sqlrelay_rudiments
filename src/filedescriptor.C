// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/filedescriptor.h>
#include <errno.h>
#include <stdio.h>
#ifdef HAVE_SYS_TIMES_H
	#include <sys/times.h>
#endif
#include <sys/time.h>
#ifdef HAVE_SYS_SELECT_H
	#include <sys/select.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <sys/fcntl.h>

// if SSIZE_MAX is undefined, choose a good safe value
// that should even work on 16-bit systems
#ifndef SSIZE_MAX
	#define SSIZE_MAX 32767
#endif


filedescriptor::filedescriptor(int filedesc) {
	fd=filedesc;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
	allowshortreads=0;
	lstnr=NULL;
#ifdef RUDIMENTS_HAS_SSL
	ctx=NULL;
	bio=NULL;
	ssl=NULL;
#endif
}

filedescriptor::filedescriptor() {
	fd=-1;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
	allowshortreads=0;
	lstnr=NULL;
#ifdef RUDIMENTS_HAS_SSL
	ctx=NULL;
	bio=NULL;
	ssl=NULL;
#endif
}

filedescriptor::~filedescriptor() {
	close();
#ifdef RUDIMENTS_HAS_SSL
	setSSLContext(NULL);
#endif
}

int filedescriptor::getFileDescriptor() const {
	return fd;
}

void filedescriptor::setFileDescriptor(int filedesc) {
	fd=filedesc;
}

#ifdef RUDIMENTS_HAS_SSL
void filedescriptor::setSSLContext(SSL_CTX *ctx) {
	if (!ctx) {
		deInitializeSSL();
	}
	this->ctx=ctx;
}

bool filedescriptor::initializeSSL() {
	if (fd==-1) {
		return false;
	}
	deInitializeSSL();
	if (ctx) {
		bio=newSSLBIO();
		ssl=SSL_new(ctx);
		SSL_set_bio(ssl,bio,bio);
	}
	return true;
}

void filedescriptor::deInitializeSSL() {
	if (ssl) {
		SSL_free(ssl);
		ssl=NULL;
	}
	if (bio) {
		// BIO_free causes a segfault, and none of the example code
		// that I've seen calls it, but the function exists so
		// presumably it has a purpose.
		//BIO_free(bio);
		bio=NULL;
	}
}

SSL *filedescriptor::getSSL() const {
	return ssl;
}

BIO *filedescriptor::newSSLBIO() const {
	return BIO_new_fd(fd,BIO_NOCLOSE);
}
#endif

bool filedescriptor::useNonBlockingMode() {
	return (fcntl(fd,F_SETFL,fcntl(fd,F_GETFL,0)|O_NONBLOCK)!=-1);
}

bool filedescriptor::useBlockingMode() {
	return (fcntl(fd,F_SETFL,fcntl(fd,F_GETFL,0)&(~O_NONBLOCK))!=-1);
}

bool filedescriptor::isUsingNonBlockingMode() {
	return (fcntl(fd,F_GETFL,0)&O_NONBLOCK);
}

ssize_t filedescriptor::write(unsigned short number) {
	return safeWrite((void *)&number,sizeof(unsigned short),-1,-1);
}

ssize_t filedescriptor::write(unsigned long number) {
	return safeWrite((void *)&number,sizeof(unsigned long),-1,-1);
}

ssize_t filedescriptor::write(short number) {
	return safeWrite((void *)&number,sizeof(short),-1,-1);
}

ssize_t filedescriptor::write(long number) {
	return safeWrite((void *)&number,sizeof(long),-1,-1);
}

ssize_t filedescriptor::write(float number) {
	return safeWrite((void *)&number,sizeof(float),-1,-1);
}

ssize_t filedescriptor::write(double number) {
	return safeWrite((void *)&number,sizeof(double),-1,-1);
}

ssize_t filedescriptor::write(unsigned char character) {
	return safeWrite((void *)&character,sizeof(unsigned char),-1,-1);
}

ssize_t filedescriptor::write(char character) {
	return safeWrite((void *)&character,sizeof(char),-1,-1);
}

ssize_t filedescriptor::write(const unsigned char *string, size_t size) {
	return safeWrite((void *)string,size,-1,-1);
}

ssize_t filedescriptor::write(const char *string, size_t size) {
	return safeWrite((void *)string,size,-1,-1);
}

ssize_t filedescriptor::write(const void *buffer, size_t size) {
	return safeWrite((void *)buffer,size,-1,-1);
}

ssize_t filedescriptor::write(unsigned short number, long sec, long usec) {
	return safeWrite((void *)&number,sizeof(unsigned short),sec,usec);
}

ssize_t filedescriptor::write(unsigned long number, long sec, long usec) {
	return safeWrite((void *)&number,sizeof(unsigned long),sec,usec);
}

ssize_t filedescriptor::write(short number, long sec, long usec) {
	return safeWrite((void *)&number,sizeof(short),sec,usec);
}

ssize_t filedescriptor::write(long number, long sec, long usec) {
	return safeWrite((void *)&number,sizeof(long),sec,usec);
}

ssize_t filedescriptor::write(float number, long sec, long usec) {
	return safeWrite((void *)&number,sizeof(float),sec,usec);
}

ssize_t filedescriptor::write(double number, long sec, long usec) {
	return safeWrite((void *)&number,sizeof(double),sec,usec);
}

ssize_t filedescriptor::write(unsigned char character, long sec, long usec) {
	return safeWrite((void *)&character,sizeof(unsigned char),sec,usec);
}

ssize_t filedescriptor::write(char character, long sec, long usec) {
	return safeWrite((void *)&character,sizeof(char),sec,usec);
}

ssize_t filedescriptor::write(const unsigned char *string, size_t size,
							long sec, long usec) {
	return safeWrite((void *)string,size,sec,usec);
}

ssize_t filedescriptor::write(const char *string, size_t size,
							long sec, long usec) {
	return safeWrite((void *)string,size,sec,usec);
}

ssize_t filedescriptor::write(const void *buffer, size_t size,
							long sec, long usec) {
	return safeWrite((void *)buffer,size,sec,usec);
}

ssize_t filedescriptor::read(unsigned short *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned short),-1,-1);
}

ssize_t filedescriptor::read(unsigned long *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned long),-1,-1);
}

ssize_t filedescriptor::read(short *buffer) {
	return safeRead((void *)buffer,sizeof(short),-1,-1);
}

ssize_t filedescriptor::read(long *buffer) {
	return safeRead((void *)buffer,sizeof(long),-1,-1);
}

ssize_t filedescriptor::read(float *buffer) {
	return safeRead((void *)buffer,sizeof(float),-1,-1);
}

ssize_t filedescriptor::read(double *buffer) {
	return safeRead((void *)buffer,sizeof(double),-1,-1);
}

ssize_t filedescriptor::read(unsigned char *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned char),-1,-1);
}

ssize_t filedescriptor::read(char *buffer) {
	return safeRead((void *)buffer,sizeof(char),-1,-1);
}

ssize_t filedescriptor::read(unsigned char *buffer, size_t size) {
	return safeRead((void *)buffer,size,-1,-1);
}

ssize_t filedescriptor::read(char *buffer, size_t size) {
	return safeRead((void *)buffer,size,-1,-1);
}

ssize_t filedescriptor::read(void *buffer, size_t size) {
	return safeRead(buffer,size,-1,-1);
}

ssize_t filedescriptor::read(char **buffer, char *terminator) {
	return read(buffer,terminator,-1,-1);
}

ssize_t filedescriptor::read(unsigned short *buffer, long sec, long usec) {
	return safeRead((void *)buffer,sizeof(unsigned short),sec,usec);
}

ssize_t filedescriptor::read(unsigned long *buffer, long sec, long usec) {
	return safeRead((void *)buffer,sizeof(unsigned long),sec,usec);
}

ssize_t filedescriptor::read(short *buffer, long sec, long usec) {
	return safeRead((void *)buffer,sizeof(short),sec,usec);
}

ssize_t filedescriptor::read(long *buffer, long sec, long usec) {
	return safeRead((void *)buffer,sizeof(long),sec,usec);
}

ssize_t filedescriptor::read(float *buffer, long sec, long usec) {
	return safeRead((void *)buffer,sizeof(float),sec,usec);
}

ssize_t filedescriptor::read(double *buffer, long sec, long usec) {
	return safeRead((void *)buffer,sizeof(double),sec,usec);
}

ssize_t filedescriptor::read(unsigned char *buffer, long sec, long usec) {
	return safeRead((void *)buffer,sizeof(unsigned char),sec,usec);
}

ssize_t filedescriptor::read(char *buffer, long sec, long usec) {
	return safeRead((void *)buffer,sizeof(char),sec,usec);
}

ssize_t filedescriptor::read(unsigned char *buffer, size_t size,
							long sec, long usec) {
	return safeRead((void *)buffer,size,sec,usec);
}

ssize_t filedescriptor::read(char *buffer, size_t size, long sec, long usec) {
	return safeRead((void *)buffer,size,sec,usec);
}

ssize_t filedescriptor::read(void *buffer, size_t size, long sec, long usec) {
	return safeRead(buffer,size,sec,usec);
}

bool filedescriptor::close() {
#ifdef RUDIMENTS_HAS_SSL
	if (ssl) {
		SSL_shutdown(ssl);
	}
#endif
	if (::close(fd)==-1) {
		return false;
	}
	fd=-1;
	return true;
}

void filedescriptor::retryInterruptedReads() {
	retryinterruptedreads=1;
}

void filedescriptor::dontRetryInterruptedReads() {
	retryinterruptedreads=0;
}

void filedescriptor::retryInterruptedWrites() {
	retryinterruptedwrites=1;
}

void filedescriptor::dontRetryInterruptedWrites() {
	retryinterruptedwrites=0;
}

void filedescriptor::retryInterruptedWaits() {
	retryinterruptedwaits=1;
}

void filedescriptor::dontRetryInterruptedWaits() {
	retryinterruptedwaits=0;
}

void filedescriptor::allowShortReads() {
	allowshortreads=1;
}

void filedescriptor::dontAllowShortReads() {
	allowshortreads=0;
}

void filedescriptor::useListener(listener *lstnr) {
	this->lstnr=lstnr;
}

void filedescriptor::dontUseListener() {
	lstnr=NULL;
}

listener *filedescriptor::getListener() {
	return lstnr;
}

ssize_t filedescriptor::read(char **buffer, char *terminator,
						long sec, long usec) {

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
		if ((sizeread=safeRead((void *)&charbuffer,
				sizeof(char),sec,usec))<=RESULT_ERROR) {
			totalread=sizeread;
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

ssize_t filedescriptor::safeRead(void *buf, ssize_t count,
						long sec, long usec) {

	// The result of SSL_read may be undefined if count=0
	#ifdef RUDIMENTS_HAS_SSL
	/*if (!count) {
		return 0;
	}*/
	#endif

	ssize_t	totalread=0;
	ssize_t	sizetoread;
	ssize_t	actualread;
	while (totalread<count) {

		// only read SSIZE_MAX at a time
		sizetoread=count-totalread;
		if (sizetoread>SSIZE_MAX) {
			sizetoread=SSIZE_MAX;
		}

		// if necessary, select
		if (sec>-1 && usec>-1) {

			int	selectresult=safeSelect(sec,usec,1,0);

			// return error or timeout
			if (selectresult<0) {
				return selectresult;
			}
		}

		// read...
		errno=0;
		#ifdef RUDIMENTS_HAS_SSL
		if (ssl) {
			actualread=SSL_read(ssl,(void *)((long)buf+totalread),
								sizetoread);
		} else {
		#endif
			actualread=::read(fd,(void *)((long)buf+totalread),
								sizetoread);
		#ifdef RUDIMENTS_HAS_SSL
		}
		#endif

		// if we didn't read the number of bytes we expected to,
		// handle that...
		if (actualread!=sizetoread) {
			if (retryinterruptedreads && errno==EINTR) {
				// if we got an EINTR, then we need to
				// retry the read
				continue;
			} else if (actualread==0 && errno==0) {
				// eof condition
				break;
			}
		}

		totalread=totalread+actualread;

		// if we want to allow short reads, then break out here
		if (allowshortreads) {
			break;
		}
	}

	return totalread;
}

ssize_t filedescriptor::safeWrite(const void *buf, ssize_t count,
						long sec, long usec) {

	// The result of SSL_write may be undefined if count=0
	#ifdef RUDIMENTS_HAS_SSL
	/*if (!count) {
		return 0;
	}*/
	#endif

	ssize_t	retval;
	for (;;) {

		// if necessary, select
		if (sec>-1 && usec>-1) {

			int	selectresult=safeSelect(sec,usec,0,1);

			// return error or timeout
			if (selectresult<0) {
				return selectresult;
			}
		}

		errno=0;
		#ifdef RUDIMENTS_HAS_SSL
		if (ssl) {
			retval=::SSL_write(ssl,buf,count);
		} else {
		#endif
			retval=::write(fd,buf,count);
		#ifdef RUDIMENTS_HAS_SSL
		}
		#endif

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
		FD_ZERO(&fdlist);
		FD_SET(fd,&fdlist);

		// wait for data to be available on the file descriptor
		int	selectresult=select(fd+1,(read)?&fdlist:NULL,
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

		return selectresult;
	}
}

ssize_t filedescriptor::write(const unsigned char *string) {
	return safeWrite((void *)string,strlen((char *)string),-1,-1);
}

ssize_t filedescriptor::write(const char *string) {
	return safeWrite((void *)string,strlen(string),-1,-1);
}

ssize_t filedescriptor::write(const unsigned char *string,
							long sec, long usec) {
	return safeWrite((void *)string,strlen((char *)string),sec,usec);
}

ssize_t filedescriptor::write(const char *string, long sec, long usec) {
	return safeWrite((void *)string,strlen(string),sec,usec);
}
