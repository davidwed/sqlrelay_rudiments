// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/filedescriptor.h>
#include <rudiments/listener.h>
#include <rudiments/charstring.h>

#include <stdio.h>
#include <errno.h>
// some systems need string.h to provide memset() for FD_ZERO/FD_SET
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <limits.h>

//#define DEBUG_PASSFD 1

#ifdef NEED_XNET_PROTOTYPES
extern ssize_t __xnet_recvmsg (int, struct msghdr *, int);
extern ssize_t __xnet_sendmsg (int, const struct msghdr *, int);
#endif

// if SSIZE_MAX is undefined, choose a good safe value
// that should even work on 16-bit systems
#ifndef SSIZE_MAX
	#define SSIZE_MAX 32767
#endif

//#define DEBUG_WRITE 1
//#define DEBUG_READ 1

#ifdef DEBUG_WRITE
	#define DEBUG_WRITE_INT(type,number) \
		printf("%d: %s write(%d,%d)\n",getpid(),type,fd,number);
	#define DEBUG_WRITE_FLOAT(type,number) \
		printf("%d: %s write(%d,%f)\n",getpid(),type,fd,number);
	#define DEBUG_WRITE_CHAR(type,character) \
		printf("%d: %s write(%d,%d)\n",getpid(),type,fd,character);
	#define DEBUG_WRITE_STRING(type,string,size) \
		printf("%d: %s write(%d,",getpid(),type,fd); \
		for (size_t i=0; i<size; i++) { \
			printf("%c",string[i]); \
		} \
		printf(",%d)\n",size);
	#define DEBUG_WRITE_VOID(type,buffer,size) \
		unsigned char *ptr=static_cast<unsigned char *>(buffer); \
		printf("%d: %s write(%d,",getpid(),type,fd); \
		for (size_t i=0; i<size; i++) { \
			printf("0x%02x ",ptr[i]); \
		} \
		printf(",%d)\n",size);
#else
	#define DEBUG_WRITE_INT(type,number)
	#define DEBUG_WRITE_FLOAT(type,number)
	#define DEBUG_WRITE_CHAR(type,character)
	#define DEBUG_WRITE_STRING(type,string,size)
	#define DEBUG_WRITE_VOID(type,buffer,size)
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

filedescriptor::filedescriptor() {
	init();
	fd=-1;
}

void filedescriptor::init() {
	translatebyteorder=false;
	retryinterruptedreads=false;
	retryinterruptedwrites=false;
	retryinterruptedwaits=true;
	retryinterruptedfcntl=true;
	retryinterruptedioctl=true;
	allowshortreads=false;
	lstnr=NULL;
	uselistenerinsidereads=false;
	uselistenerinsidewrites=false;
#ifdef RUDIMENTS_HAS_SSL
	ctx=NULL;
	bio=NULL;
	ssl=NULL;
	sslresult=1;
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

int filedescriptor::duplicate() const {
	return dup(fd);
}

bool filedescriptor::duplicate(int newfd) const {
	return (dup2(fd,newfd)==newfd);
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

bool filedescriptor::useNonBlockingMode() const {
	return (fcntl(F_SETFL,fcntl(F_GETFL,0)|O_NONBLOCK)!=-1);
}

bool filedescriptor::useBlockingMode() const {
	return (fcntl(F_SETFL,fcntl(F_GETFL,0)&(~O_NONBLOCK))!=-1);
}

bool filedescriptor::isUsingNonBlockingMode() const {
	return (fcntl(F_GETFL,0)&O_NONBLOCK);
}

ssize_t filedescriptor::write(unsigned short number) {
	return write(number,-1,-1);
}

ssize_t filedescriptor::write(unsigned long number) {
	return write(number,-1,-1);
}

ssize_t filedescriptor::write(short number) {
	DEBUG_WRITE_INT("short",number);
	return safeWrite(static_cast<const void *>(&number),
						sizeof(short),-1,-1);
}

ssize_t filedescriptor::write(long number) {
	DEBUG_WRITE_INT("long",number);
	return safeWrite(static_cast<const void *>(&number),
						sizeof(long),-1,-1);
}

ssize_t filedescriptor::write(float number) {
	DEBUG_WRITE_FLOAT("float",number);
	return safeWrite(static_cast<const void *>(&number),
						sizeof(float),-1,-1);
}

ssize_t filedescriptor::write(double number) {
	DEBUG_WRITE_FLOAT("double",number);
	return safeWrite(static_cast<const void *>(&number),
						sizeof(double),-1,-1);
}

ssize_t filedescriptor::write(unsigned char character) {
	DEBUG_WRITE_CHAR("uchar",character);
	return safeWrite(static_cast<const void *>(&character),
						sizeof(unsigned char),-1,-1);
}

ssize_t filedescriptor::write(bool value) {
	DEBUG_WRITE_INT("bool",value);
	return safeWrite(static_cast<const void *>(&value),
						sizeof(bool),-1,-1);
}

ssize_t filedescriptor::write(char character) {
	DEBUG_WRITE_CHAR("char",character);
	return safeWrite(static_cast<const void *>(&character),
						sizeof(char),-1,-1);
}

ssize_t filedescriptor::write(const unsigned char *string, size_t size) {
	DEBUG_WRITE_STRING("ustring",string,size);
	return safeWrite(static_cast<const void *>(string),size,-1,-1);
}

ssize_t filedescriptor::write(const char *string, size_t size) {
	DEBUG_WRITE_STRING("string",string,size);
	return safeWrite(static_cast<const void *>(string),size,-1,-1);
}

ssize_t filedescriptor::write(const unsigned char *string) {
	DEBUG_WRITE_STRING("ustring",string,charstring::length(string));
	return safeWrite(static_cast<const void *>(string),
				charstring::length(string),-1,-1);
}

ssize_t filedescriptor::write(const char *string) {
	DEBUG_WRITE_STRING("string",string,charstring::length(string));
	return safeWrite(static_cast<const void *>(string),
				charstring::length(string),-1,-1);
}

ssize_t filedescriptor::write(const void *buffer, size_t size) {
	DEBUG_WRITE_VOID("void",buffer,size);
	return safeWrite(static_cast<const void *>(buffer),size,-1,-1);
}

ssize_t filedescriptor::write(unsigned short number, long sec, long usec) {
	DEBUG_WRITE_INT("ushort",number);
	number=hostToNet(number);
	return safeWrite(static_cast<const void *>(&number),
				sizeof(unsigned short),sec,usec);
}

ssize_t filedescriptor::write(unsigned long number, long sec, long usec) {
	DEBUG_WRITE_INT("ulong",number);
	number=hostToNet(number);
	return safeWrite(static_cast<const void *>(&number),
				sizeof(unsigned long),sec,usec);
}

ssize_t filedescriptor::write(short number, long sec, long usec) {
	DEBUG_WRITE_INT("short",number);
	return safeWrite(static_cast<const void *>(&number),
					sizeof(short),sec,usec);
}

ssize_t filedescriptor::write(long number, long sec, long usec) {
	DEBUG_WRITE_INT("long",number);
	return safeWrite(static_cast<const void *>(&number),
					sizeof(long),sec,usec);
}

ssize_t filedescriptor::write(float number, long sec, long usec) {
	DEBUG_WRITE_FLOAT("float",number);
	return safeWrite(static_cast<const void *>(&number),
					sizeof(float),sec,usec);
}

ssize_t filedescriptor::write(double number, long sec, long usec) {
	DEBUG_WRITE_FLOAT("double",number);
	return safeWrite(static_cast<const void *>(&number),
					sizeof(double),sec,usec);
}

ssize_t filedescriptor::write(unsigned char character, long sec, long usec) {
	DEBUG_WRITE_CHAR("uchar",character);
	return safeWrite(static_cast<const void *>(&character),
					sizeof(unsigned char),sec,usec);
}

ssize_t filedescriptor::write(bool value, long sec, long usec) {
	DEBUG_WRITE_INT("bool",value);
	return safeWrite(static_cast<const void *>(&value),
					sizeof(bool),sec,usec);
}

ssize_t filedescriptor::write(char character, long sec, long usec) {
	DEBUG_WRITE_CHAR("char",character);
	return safeWrite(static_cast<const void *>(&character),
						sizeof(char),sec,usec);
}

ssize_t filedescriptor::write(const unsigned char *string, size_t size,
							long sec, long usec) {
	DEBUG_WRITE_STRING("ustring",string,size);
	return safeWrite(static_cast<const void *>(string),size,sec,usec);
}

ssize_t filedescriptor::write(const char *string, size_t size,
							long sec, long usec) {
	DEBUG_WRITE_STRING("string",string,size);
	return safeWrite(static_cast<const void *>(string),size,sec,usec);
}

ssize_t filedescriptor::write(const unsigned char *string,
						long sec, long usec) {
	DEBUG_WRITE_STRING("ustring",string,charstring::length(string));
	return safeWrite(static_cast<const void *>(string),
				charstring::length(string),sec,usec);
}

ssize_t filedescriptor::write(const char *string, long sec, long usec) {
	DEBUG_WRITE_STRING("string",string,charstring::length(string));
	return safeWrite(static_cast<const void *>(string),
				charstring::length(string),sec,usec);
}

ssize_t filedescriptor::write(const void *buffer, size_t size,
							long sec, long usec) {
	DEBUG_WRITE_VOID("void",buffer,size);
	return safeWrite(buffer,size,sec,usec);
}

ssize_t filedescriptor::read(unsigned short *buffer) {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(unsigned long *buffer) {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(short *buffer) {
	return safeRead(static_cast<void *>(buffer),sizeof(short),-1,-1);
}

ssize_t filedescriptor::read(long *buffer) {
	return safeRead(static_cast<void *>(buffer),sizeof(long),-1,-1);
}

ssize_t filedescriptor::read(float *buffer) {
	return safeRead(static_cast<void *>(buffer),sizeof(float),-1,-1);
}

ssize_t filedescriptor::read(double *buffer) {
	return safeRead(static_cast<void *>(buffer),sizeof(double),-1,-1);
}

ssize_t filedescriptor::read(unsigned char *buffer) {
	return safeRead(static_cast<void *>(buffer),
				sizeof(unsigned char),-1,-1);
}

ssize_t filedescriptor::read(bool *buffer) {
	return safeRead(static_cast<void *>(buffer),sizeof(bool),-1,-1);
}

ssize_t filedescriptor::read(char *buffer) {
	return safeRead(static_cast<void *>(buffer),sizeof(char),-1,-1);
}

ssize_t filedescriptor::read(unsigned char *buffer, size_t size) {
	return safeRead(static_cast<void *>(buffer),size,-1,-1);
}

ssize_t filedescriptor::read(char *buffer, size_t size) {
	return safeRead(static_cast<void *>(buffer),size,-1,-1);
}

ssize_t filedescriptor::read(void *buffer, size_t size) {
	return safeRead(buffer,size,-1,-1);
}

ssize_t filedescriptor::read(char **buffer, char *terminator) {
	return read(buffer,terminator,-1,-1);
}

ssize_t filedescriptor::read(unsigned short *buffer, long sec, long usec) {
	ssize_t	retval=safeRead(static_cast<void *>(buffer),
					sizeof(unsigned short),sec,usec);
	*buffer=netToHost(*buffer);
	return retval;
}

ssize_t filedescriptor::read(unsigned long *buffer, long sec, long usec) {
	ssize_t	retval=safeRead(static_cast<void *>(buffer),
					sizeof(unsigned long),sec,usec);
	*buffer=netToHost(*buffer);
	return retval;
}

ssize_t filedescriptor::read(short *buffer, long sec, long usec) {
	return safeRead(static_cast<void *>(buffer),
					sizeof(short),sec,usec);
}

ssize_t filedescriptor::read(long *buffer, long sec, long usec) {
	return safeRead(static_cast<void *>(buffer),sizeof(long),sec,usec);
}

ssize_t filedescriptor::read(float *buffer, long sec, long usec) {
	return safeRead(static_cast<void *>(buffer),sizeof(float),sec,usec);
}

ssize_t filedescriptor::read(double *buffer, long sec, long usec) {
	return safeRead(static_cast<void *>(buffer),sizeof(double),sec,usec);
}

ssize_t filedescriptor::read(unsigned char *buffer, long sec, long usec) {
	return safeRead(static_cast<void *>(buffer),
					sizeof(unsigned char),sec,usec);
}

ssize_t filedescriptor::read(bool *buffer, long sec, long usec) {
	return safeRead(static_cast<void *>(buffer),sizeof(bool),sec,usec);
}

ssize_t filedescriptor::read(char *buffer, long sec, long usec) {
	return safeRead(static_cast<void *>(buffer),sizeof(char),sec,usec);
}

ssize_t filedescriptor::read(unsigned char *buffer, size_t size,
							long sec, long usec) {
	return safeRead(static_cast<void *>(buffer),size,sec,usec);
}

ssize_t filedescriptor::read(char *buffer, size_t size, long sec, long usec) {
	return safeRead(static_cast<void *>(buffer),size,sec,usec);
}

ssize_t filedescriptor::read(void *buffer, size_t size, long sec, long usec) {
	return safeRead(buffer,size,sec,usec);
}

bool filedescriptor::close() {
#ifdef RUDIMENTS_HAS_SSL
	if (ssl) {
		sslresult=SSL_shutdown(ssl);
	}
#endif
	if (::close(fd)==-1) {
		return false;
	}
	fd=-1;
	return true;
}

void filedescriptor::retryInterruptedReads() {
	retryinterruptedreads=true;
}

void filedescriptor::dontRetryInterruptedReads() {
	retryinterruptedreads=false;
}

void filedescriptor::retryInterruptedWrites() {
	retryinterruptedwrites=true;
}

void filedescriptor::dontRetryInterruptedWrites() {
	retryinterruptedwrites=false;
}

void filedescriptor::retryInterruptedWaits() {
	retryinterruptedwaits=true;
}

void filedescriptor::dontRetryInterruptedWaits() {
	retryinterruptedwaits=false;
}

void filedescriptor::retryInterruptedFcntl() {
	retryinterruptedfcntl=true;
}

void filedescriptor::dontRetryInterruptedFcntl() {
	retryinterruptedfcntl=true;
}

void filedescriptor::retryInterruptedIoctl() {
	retryinterruptedioctl=true;
}

void filedescriptor::dontRetryInterruptedIoctl() {
	retryinterruptedioctl=true;
}

void filedescriptor::allowShortReads() {
	allowshortreads=true;
}

void filedescriptor::dontAllowShortReads() {
	allowshortreads=false;
}

void filedescriptor::useListener(listener *lstnr) {
	this->lstnr=lstnr;
}

void filedescriptor::useListenerInsideReads() {
	uselistenerinsidereads=true;
}

void filedescriptor::dontUseListenerInsideReads() {
	uselistenerinsidereads=false;
}

void filedescriptor::useListenerInsideWrites() {
	uselistenerinsidewrites=true;
}

void filedescriptor::dontUseListenerInsideWrites() {
	uselistenerinsidewrites=false;
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
	if (buffer) {
		*buffer=new char[buffersize];
	}

	// initialize termination detector
	int	termlen=charstring::length(terminator);
	char	*term=new char[termlen];
	for (int i=0; i<termlen; i++) {
		term[i]='\0';
	}

	// initialize some variables
	char	charbuffer;
	int	sizeread;
	int	totalread=0;
	int	escaped=0;
	int	copytobuffer;
	int	copytoterm;

	// loop, getting 1 character at a time
	for (;;) {

		// read from the file descriptor
		if ((sizeread=safeRead(static_cast<void *>(&charbuffer),
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
			if (copytobuffer && buffer) {

				// extend buffer if necessary
				if (totalread==buffersize) {
					char	*newbuffer=
						new char[buffersize+512];
					charstring::copy(newbuffer,
							*buffer,buffersize);
					delete *buffer;
					buffersize=buffersize+512;
					*buffer=newbuffer;
				}

				(*buffer)[totalread-1]=charbuffer;
				(*buffer)[totalread]='\0';
			}

			if (copytoterm) {

				// update terminator detector
				for (int i=0; i<termlen-1; i++) {
					term[i]=term[i+1];
				}
				term[termlen-1]=charbuffer;

				// check for termination
				if (!charstring::compare(term,terminator,
								termlen)) {
					break;
				}

			} else {

				// clear terminator
				for (int i=0; i<termlen; i++) {
					term[i]='\0';
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

	// FIXME: is this what we want to do?
	// maybe we should set some kind of error condition too
	if (!buf) {
		return 0;
	}

	#ifdef DEBUG_READ
	printf("%d: safeRead(%d,",getpid(),fd);
	#endif

	// The result of SSL_read may be undefined if count=0
	#ifdef RUDIMENTS_HAS_SSL
	if (!count) {
		return 0;
	}
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
		if (sec>-1 && usec>-1 || uselistenerinsidereads) {

			int	selectresult=(uselistenerinsidereads)?
					waitForNonBlockingRead(sec,usec):
					safeSelect(sec,usec,true,false);

			// return error or timeout
			if (selectresult<0) {
				#ifdef DEBUG_READ
				printf(")\n");
				#endif
				return selectresult;
			}

			// FIXME: if uselistenerinsidereads is set, and data
			// is available on a different file descriptor than
			// this one (and none is available on this one),
			// return the current size (a short read).  Apps should
			// respond to this short read condition by checking the
			// listener's ready list... maybe?
		}

		// set a pointer to the position in the buffer that we need
		// to read data into
		void	*ptr=static_cast<void *>(
				static_cast<unsigned char *>(buf)+
				totalread);

		// read...
		errno=0;
		#ifdef RUDIMENTS_HAS_SSL
		if (ssl) {
			for (bool done=false; !done ;) {

				actualread=SSL_read(ssl,ptr,sizetoread);
				sslresult=actualread;

				switch (SSL_get_error(ssl,actualread)) {
					case SSL_ERROR_WANT_READ:
					case SSL_ERROR_WANT_WRITE:
						continue;
					case SSL_ERROR_WANT_X509_LOOKUP:
					case SSL_ERROR_SSL:
						actualread=-1;
					case SSL_ERROR_ZERO_RETURN:
					case SSL_ERROR_SYSCALL:
					case SSL_ERROR_NONE:
					default:
						done=true;
				}
			}
		} else {
		#endif
			actualread=::read(fd,ptr,sizetoread);
			#ifdef DEBUG_READ
			for (int i=0; i<sizetoread; i++) {
				printf("0x%02x ",
					(static_cast<unsigned char *>(ptr))[i]);
			}
			#endif
		#ifdef RUDIMENTS_HAS_SSL
		}
		#endif

		// if we didn't read the number of bytes we expected to,
		// handle that...
		if (actualread!=sizetoread) {
			if (errno==EINTR) {
				// if we got an EINTR, then we may need to
				// retry the read
				if (retryinterruptedreads) {
					continue;
				} else {
					totalread=totalread+actualread;
					break;
				}
			} else if (actualread==0 && errno==0) {
				// eof condition
				break;
			} else if (actualread==-1) {
				// error condition
				#ifdef DEBUG_READ
				printf(")\n");
				#endif
				return -1;
			}
		}

		totalread=totalread+actualread;

		// if we want to allow short reads, then break out here
		if (allowshortreads) {
			break;
		}
	}

	#ifdef DEBUG_READ
	printf(",%d)\n",totalread);
	#endif
	return totalread;
}

ssize_t filedescriptor::safeWrite(const void *buf, ssize_t count,
						long sec, long usec) {

	// The result of SSL_write may be undefined if count=0
	#ifdef RUDIMENTS_HAS_SSL
	if (!count) {
		return 0;
	}
	#endif

	ssize_t	retval;
	for (;;) {

		// if necessary, select
		if (sec>-1 && usec>-1 || uselistenerinsidewrites) {

			int	selectresult=(uselistenerinsidewrites)?
					waitForNonBlockingWrite(sec,usec):
					safeSelect(sec,usec,false,true);

			// return error or timeout
			if (selectresult<0) {
				return selectresult;
			}
		}

		errno=0;
		#ifdef RUDIMENTS_HAS_SSL
		if (ssl) {
			for (bool done=false; !done ;) {

				retval=::SSL_write(ssl,buf,count);
				sslresult=retval;

				switch (SSL_get_error(ssl,retval)) {
					case SSL_ERROR_WANT_READ:
					case SSL_ERROR_WANT_WRITE:
						continue;
					case SSL_ERROR_WANT_X509_LOOKUP:
					case SSL_ERROR_SSL:
					case SSL_ERROR_ZERO_RETURN:
					case SSL_ERROR_SYSCALL:
					case SSL_ERROR_NONE:
					default:
						done=true;
				}
			}
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

int filedescriptor::waitForNonBlockingRead(long sec, long usec) const {
	return (lstnr)?lstnr->waitForNonBlockingRead(sec,usec):
			safeSelect(sec,usec,true,false);
}

int filedescriptor::waitForNonBlockingWrite(long sec, long usec) const {
	return (lstnr)?lstnr->waitForNonBlockingWrite(sec,usec):
			safeSelect(sec,usec,false,true);
}

int filedescriptor::safeSelect(long sec, long usec,
				bool read, bool write) const {

	#ifdef RUDIMENTS_HAS_SSL
		if (read && ssl && SSL_pending(ssl)) {
			return 1;
		}
	#endif

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

void filedescriptor::translateByteOrder() {
	translatebyteorder=true;
}

void filedescriptor::dontTranslateByteOrder() {
	translatebyteorder=false;
}

unsigned short filedescriptor::hostToNet(unsigned short value) const {
	return (translatebyteorder)?htons(value):value;
}

unsigned long filedescriptor::hostToNet(unsigned long value) const {
	return (translatebyteorder)?htonl(value):value;
}

unsigned short filedescriptor::netToHost(unsigned short value) const {
	return (translatebyteorder)?ntohs(value):value;
}

unsigned long filedescriptor::netToHost(unsigned long value) const {
	return (translatebyteorder)?ntohl(value):value;
}

#ifdef RUDIMENTS_HAS_SSL
int filedescriptor::getSSLResult() const {
	return sslresult;
}
#endif

int filedescriptor::fcntl(int cmd, long arg) const {
	int	result;
	do {
		result=::fcntl(fd,cmd,arg);
	} while (retryinterruptedfcntl && result==-1 && errno==EINTR);
	return result;
}

int filedescriptor::ioctl(int cmd, void *arg) const {
	int	result;
	do {
		result=::ioctl(fd,cmd,arg);
	} while (retryinterruptedioctl && result==-1 && errno==EINTR);
	return result;
}

bool filedescriptor::passFileDescriptor(int filedesc) {

	// have to use sendmsg to pass a file descriptor. 
	// sendmsg can only send a msghdr
	struct	msghdr	messageheader;

	// these must be null for stream sockets
	messageheader.msg_name=NULL;
	messageheader.msg_namelen=0;

	#ifdef HAVE_MSGHDR_MSG_FLAGS
	// initialize flags to 0
	messageheader.msg_flags=0;
	#endif

	// must send at least 1 iovector with 1 byte of real data
	struct iovec	iovector[1];
	iovector[0].iov_base=(IOV_BASE_TYPE)" ";
	iovector[0].iov_len=sizeof(char);
	messageheader.msg_iov=iovector;
	messageheader.msg_iovlen=1;

	// use other parts of the msghdr structure to send the descriptor
	#ifdef HAVE_MSGHDR_MSG_CONTROLLEN

		// new-style:
		// The descriptor is passed in the msg_control
		#ifdef HAVE_CMSG_SPACE
		union {
			struct cmsghdr	cm;
			char		control[CMSG_SPACE(sizeof(int))];
		} control;
		messageheader.msg_control=control.control;
		#else
		unsigned char	control[sizeof(struct cmsghdr)+sizeof(int)];
		messageheader.msg_control=(caddr_t)control;
		#endif
		messageheader.msg_controllen=sizeof(control);

		struct cmsghdr	*cmptr;
		cmptr=CMSG_FIRSTHDR(&messageheader);
		cmptr->cmsg_level=SOL_SOCKET;
		cmptr->cmsg_type=SCM_RIGHTS;
		#ifdef HAVE_CMSG_LEN
		cmptr->cmsg_len=CMSG_LEN(sizeof(int));
		#else
		cmptr->cmsg_len=sizeof(control);
		#endif
		*(reinterpret_cast<int *>(CMSG_DATA(cmptr)))=filedesc;

		// FIXME: is this necessary???
		messageheader.msg_controllen=cmptr->cmsg_len;
	#else
		// old-style:
		// The descriptor is passed in the accrights
		messageheader.msg_accrights=(caddr_t)&filedesc;
		messageheader.msg_accrightslen=sizeof(int);
	#endif

	// finally, send the msghdr
	return sendmsg(fd,&messageheader,0)!=-1;
}

bool filedescriptor::receiveFileDescriptor(int *filedesc) {

	// have to use recvmsg to receive a file descriptor. 
	// recvmsg can only send a msghdr
	struct msghdr	messageheader;

	// these must be null for stream sockets
	messageheader.msg_name=NULL;
	messageheader.msg_namelen=0;

	#ifdef HAVE_MSGHDR_MSG_FLAGS
	// initialize flags to 0
	messageheader.msg_flags=0;
	#endif

	// the process that's going to handoff it's socket will also 
	// send a single iovector with a single byte of data in it, 
	// so we'll receive that too
	struct iovec	iovector[1];
	char		ptr;
	iovector[0].iov_base=(IOV_BASE_TYPE)&ptr;
	iovector[0].iov_len=sizeof(char);
	messageheader.msg_iov=iovector;
	messageheader.msg_iovlen=1;

	#ifdef HAVE_MSGHDR_MSG_CONTROLLEN
		// new-style:
		// The descriptor is received in the msg_control
		#ifdef HAVE_CMSG_SPACE
		union {
			struct cmsghdr	cm;
			char		control[CMSG_SPACE(sizeof(int))];
		} control;
		messageheader.msg_control=control.control;
		#else
		unsigned char	control[sizeof(struct cmsghdr)+sizeof(int)];
		messageheader.msg_control=(caddr_t)control;
		#endif
		messageheader.msg_controllen=sizeof(control);
	#else
		// old-style
		// The descriptor is received in the accrights
		int	newfiledesc;
		messageheader.msg_accrights=(caddr_t)&newfiledesc;
		messageheader.msg_accrightslen=sizeof(int);
	#endif

	// receive the msghdr
	if (recvmsg(fd,&messageheader,0)==-1) {
		return false;
	}


	// if we got valid data, set the passed-in descriptor to the 
	// descriptor we received and return success
	#ifdef HAVE_MSGHDR_MSG_CONTROLLEN

		struct cmsghdr  *cmptr=CMSG_FIRSTHDR(&messageheader);
		if (cmptr && cmptr->cmsg_len==
			#ifdef HAVE_CMSG_LEN
			CMSG_LEN(sizeof(int)) &&
			#else
			(sizeof(struct cmsghdr)+sizeof(int)) &&
			#endif
			cmptr->cmsg_level==SOL_SOCKET &&
			cmptr->cmsg_type==SCM_RIGHTS) {

			// if we got good data, set the descriptor and return
			*filedesc=*(reinterpret_cast<int *>(CMSG_DATA(cmptr)));
			return true;
		}
#ifdef DEBUG_PASSFD
		else {

			// if we got bad data, be specific about what was
			// wrong, this will help debug problems with different
			// platforms
			if (!cmptr) {
				printf("%d: ",getpid());
				printf("null cmptr\n");
			} else {
				#ifdef HAVE_CMSG_LEN
				if (cmptr->cmsg_len!=CMSG_LEN(sizeof(int))) {
					printf("%d: ",getpid());
					printf("got cmsg_len=");
			       		printf("%d",static_cast<long>(
							cmptr->cmsg_len));
			       		printf(" instead of ");
					printf("%d",static_cast<long>(
							CMSG_LEN(sizeof(int))));
					printf("\n");
				}
				#endif
				if (cmptr->cmsg_level!=SOL_SOCKET) {
					printf("%d: ",getpid());
					printf("got cmsg_level=");
					printf("%d",static_cast<long>(
							cmptr->cmsg_level));
					printf(" instead of ");
					printf("%d",static_cast<long>(
								SOL_SOCKET));
					printf("\n");
				}
				if (cmptr->cmsg_type!=SCM_RIGHTS) {
					printf("%d: ",getpid());
					printf("got cmsg_type=");
					printf("%d",static_cast<long>(
							cmptr->cmsg_type));
					printf(" instead of ");
					printf("%d",static_cast<long>(
								SCM_RIGHTS));
					printf("\n");
				}
			}
		}
#endif
	#else
		if (messageheader.msg_accrightslen==sizeof(int)) {
			*filedesc=newfiledesc;
			return true;
		}
	#endif

	// if we're here then we must have received some bad data
	return false;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
