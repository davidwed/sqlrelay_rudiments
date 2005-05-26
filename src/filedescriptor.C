// Copyright (c) 2002 David Muse
// See the COPYING file for more information

//#define DEBUG_PASSFD 1
//#define DEBUG_WRITE 1
//#define DEBUG_READ 1

#include <rudiments/filedescriptor.h>
#include <rudiments/listener.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/rawbuffer.h>
#if defined(DEBUG_PASSFD) || defined(DEBUG_WRITE) || defined(DEBUG_READ)
	#include <rudiments/process.h>
#endif
#include <rudiments/error.h>

#ifdef HAVE_WINDOWS_H
	#include <windows.h>
#endif
#ifdef HAVE_WINSOCK2_H
	#include <winsock2.h>
#endif
#ifdef HAVE_IO_H
	#include <io.h>
#endif
#include <stdio.h>
#include <sys/time.h>
#ifdef HAVE_SYS_SELECT_H
	#include <sys/select.h>
#endif
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/fcntl.h>
#ifdef HAVE_IOCTL
	#include <sys/ioctl.h>
#endif
#ifdef HAVE_NETINET_IN_H
	#include <netinet/in.h>
#endif
#ifdef HAVE_NETINET_TCP_H
	#include <netinet/tcp.h>
#endif
#ifdef HAVE_SYS_UIO_H
	#include <sys/uio.h>
#endif
#include <limits.h>
#ifdef HAVE_ARPA_INET_H
	#include <arpa/inet.h>
#endif
#ifdef HAVE_BYTESWAP_H
	#include <byteswap.h>
#endif

// for FD_SET (macro that uses memset) on solaris
#include <string.h>

#ifdef NEED_XNET_PROTOTYPES
extern ssize_t __xnet_recvmsg (int, struct msghdr *, int);
extern ssize_t __xnet_sendmsg (int, const struct msghdr *, int);
#endif

// if SSIZE_MAX is undefined, choose a good safe value
// that should even work on 16-bit systems
#ifndef SSIZE_MAX
	#define SSIZE_MAX 32767
#endif

#ifdef DEBUG_WRITE
	#define DEBUG_WRITE_INT(type,number) \
		printf("%d: %s write(%d,%d)\n", \
			process::getProcessId(),type,fd,number);
	#define DEBUG_WRITE_FLOAT(type,number) \
		printf("%d: %s write(%d,%f)\n", \
			process::getProcessId(),type,fd,number);
	#define DEBUG_WRITE_CHAR(type,character) \
		printf("%d: %s write(%d,%d)\n", \
			process::getProcessId(),type,fd,character);
	#define DEBUG_WRITE_STRING(type,string,size) \
		printf("%d: %s write(%d,",process::getProcessId(),type,fd); \
		for (size_t i=0; i<((size<=160)?size:160); i++) { \
			printf("%c",string[i]); \
		} \
		if (size>160) { \
			printf("..."); \
		} \
		printf(",%d)\n",size);
	#define DEBUG_WRITE_VOID(type,buffer,size) \
		const unsigned char *ptr=\
			static_cast<const unsigned char *>(buffer); \
		printf("%d: %s write(%d,",process::getProcessId(),type,fd); \
		for (size_t i=0; i<((size<=160)?size:160); i++) { \
			printf("0x%02x ",ptr[i]); \
		} \
		if (size>160) { \
			printf("..."); \
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
	filedescriptorInit();
}

filedescriptor::filedescriptor(const filedescriptor &f) {
	filedescriptorClone(f);
}

filedescriptor &filedescriptor::operator=(const filedescriptor &f) {
	if (this!=&f) {
		delete[] writebuffer;
		filedescriptorClone(f);
	}
	return *this;
}

void filedescriptor::filedescriptorInit() {
	fd=-1;
	translatebyteorder=false;
	retryinterruptedreads=false;
	retryinterruptedwrites=false;
	retryinterruptedwaits=true;
#ifdef HAVE_FCNTL
	retryinterruptedfcntl=true;
#endif
#ifdef HAVE_IOCTL
	retryinterruptedioctl=true;
#endif
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
	type="filedescriptor";
	writebuffer=NULL;
	writebufferend=NULL;
	writebufferptr=NULL;
	readbuffer=NULL;
	readbufferend=NULL;
	readbufferhead=NULL;
	readbuffertail=NULL;
}

void filedescriptor::filedescriptorClone(const filedescriptor &f) {
	fd=f.fd;
	translatebyteorder=f.translatebyteorder;
	retryinterruptedreads=f.retryinterruptedreads;
	retryinterruptedwrites=f.retryinterruptedwrites;
	retryinterruptedwaits=f.retryinterruptedwaits;
#ifdef HAVE_FCNTL
	retryinterruptedfcntl=f.retryinterruptedfcntl;
#endif
#ifdef HAVE_IOCTL
	retryinterruptedioctl=f.retryinterruptedioctl;
#endif
	allowshortreads=f.allowshortreads;
	lstnr=f.lstnr;
	uselistenerinsidereads=f.uselistenerinsidereads;
	uselistenerinsidewrites=f.uselistenerinsidewrites;
#ifdef RUDIMENTS_HAS_SSL
	ctx=f.ctx;
	bio=f.bio;
	ssl=f.ssl;
	sslresult=f.sslresult;
#endif
	if (f.writebuffer) {
		ssize_t	writebuffersize=f.writebufferend-f.writebuffer;
		writebuffer=new unsigned char[writebuffersize];
		rawbuffer::copy(writebuffer,f.writebuffer,writebuffersize);
		writebufferend=writebuffer+writebuffersize;
		writebufferptr=writebuffer+(f.writebufferptr-f.writebuffer);
	} else {
		writebuffer=NULL;
		writebufferend=NULL;
		writebufferptr=NULL;
	}
}

filedescriptor::~filedescriptor() {
	delete[] readbuffer;
	delete[] writebuffer;
	close();
#ifdef RUDIMENTS_HAS_SSL
	setSSLContext(NULL);
#endif
}

bool filedescriptor::setWriteBufferSize(ssize_t size) const {
	if (size<0) {
		return false;
	}
	delete[] writebuffer;
	writebuffer=(size)?new unsigned char[size]:NULL;
	writebufferend=writebuffer+size;
	writebufferptr=writebuffer;
	return true;
}

bool filedescriptor::setReadBufferSize(ssize_t size) const {
	if (size<0) {
		return false;
	}
	delete[] readbuffer;
	readbuffer=(size)?new unsigned char[size]:NULL;
	readbufferend=readbuffer+size;
	readbufferhead=readbuffer;
	readbuffertail=readbuffer;
	return true;
}

int filedescriptor::getFileDescriptor() const {
	return fd;
}

void filedescriptor::setFileDescriptor(int filedesc) {
	fd=filedesc;
}

int filedescriptor::duplicate() const {
	int	result;
	do {
		result=dup(fd);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

bool filedescriptor::duplicate(int newfd) const {
	int	result;
	do {
		result=dup2(fd,newfd);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return (result==newfd);
}

#ifdef RUDIMENTS_HAS_SSL
void filedescriptor::setSSLContext(SSL_CTX *ctx) {
	if (!ctx) {
		deInitializeSSL();
	}
	this->ctx=ctx;
}

SSL_CTX *filedescriptor::getSSLContext() {
	return ctx;
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
	#if defined(HAVE_FCNTL) && defined(F_SETFL) && defined (F_GETFL)
		return (fcntl(F_SETFL,fcntl(F_GETFL,0)|O_NONBLOCK)!=-1);
	#else
		return false;
	#endif
}

bool filedescriptor::useBlockingMode() const {
	#if defined(HAVE_FCNTL) && defined(F_SETFL) && defined (F_GETFL)
		return (fcntl(F_SETFL,fcntl(F_GETFL,0)&(~O_NONBLOCK))!=-1);
	#else
		return false;
	#endif
}

bool filedescriptor::isUsingNonBlockingMode() const {
	#if defined(HAVE_FCNTL) && defined(F_GETFL)
		return (fcntl(F_GETFL,0)&O_NONBLOCK);
	#else
		return false;
	#endif
}

ssize_t filedescriptor::write(unsigned short number) const {
	return write(number,-1,-1);
}

ssize_t filedescriptor::write(unsigned long number) const {
	return write(number,-1,-1);
}

ssize_t filedescriptor::write(unsigned long long number) const {
	return write(number,-1,-1);
}

ssize_t filedescriptor::write(short number) const {
	DEBUG_WRITE_INT("short",number);
	return bufferedWrite(&number,sizeof(short),-1,-1);
}

ssize_t filedescriptor::write(long number) const {
	DEBUG_WRITE_INT("long",number);
	return bufferedWrite(&number,sizeof(long),-1,-1);
}

ssize_t filedescriptor::write(long long number) const {
	DEBUG_WRITE_INT("long long",number);
	return bufferedWrite(&number,sizeof(long long),-1,-1);
}

ssize_t filedescriptor::write(float number) const {
	DEBUG_WRITE_FLOAT("float",number);
	return bufferedWrite(&number,sizeof(float),-1,-1);
}

ssize_t filedescriptor::write(double number) const {
	DEBUG_WRITE_FLOAT("double",number);
	return bufferedWrite(&number,sizeof(double),-1,-1);
}

ssize_t filedescriptor::write(unsigned char character) const {
	DEBUG_WRITE_CHAR("uchar",character);
	return bufferedWrite(&character,sizeof(unsigned char),-1,-1);
}

ssize_t filedescriptor::write(bool value) const {
	DEBUG_WRITE_INT("bool",value);
	return bufferedWrite(&value,sizeof(bool),-1,-1);
}

ssize_t filedescriptor::write(char character) const {
	DEBUG_WRITE_CHAR("char",character);
	return bufferedWrite(&character,sizeof(char),-1,-1);
}

ssize_t filedescriptor::write(const unsigned char *string, size_t size) const {
	DEBUG_WRITE_STRING("ustring",string,size);
	return bufferedWrite(string,size,-1,-1);
}

ssize_t filedescriptor::write(const char *string, size_t size) const {
	DEBUG_WRITE_STRING("string",string,size);
	return bufferedWrite(string,size,-1,-1);
}

ssize_t filedescriptor::write(const unsigned char *string) const {
	DEBUG_WRITE_STRING("ustring",string,charstring::length(string));
	return bufferedWrite(string,charstring::length(string),-1,-1);
}

ssize_t filedescriptor::write(const char *string) const {
	DEBUG_WRITE_STRING("string",string,charstring::length(string));
	return bufferedWrite(string,charstring::length(string),-1,-1);
}

ssize_t filedescriptor::write(const void *buffer, size_t size) const {
	DEBUG_WRITE_VOID("void",buffer,size);
	return bufferedWrite(buffer,size,-1,-1);
}

ssize_t filedescriptor::write(unsigned short number,
					long sec, long usec) const {
	DEBUG_WRITE_INT("ushort",number);
	number=hostToNet(number);
	return bufferedWrite(&number,sizeof(unsigned short),sec,usec);
}

ssize_t filedescriptor::write(unsigned long number,
					long sec, long usec) const {
	DEBUG_WRITE_INT("ulong",number);
	number=hostToNet(number);
	return bufferedWrite(&number,sizeof(unsigned long),sec,usec);
}

ssize_t filedescriptor::write(unsigned long long number,
					long sec, long usec) const {
	DEBUG_WRITE_INT("ulonglong",number);
	number=hostToNet(number);
	return bufferedWrite(&number,sizeof(unsigned long long),sec,usec);
}

ssize_t filedescriptor::write(short number, long sec, long usec) const {
	DEBUG_WRITE_INT("short",number);
	return bufferedWrite(&number,sizeof(short),sec,usec);
}

ssize_t filedescriptor::write(long number, long sec, long usec) const {
	DEBUG_WRITE_INT("long",number);
	return bufferedWrite(&number,sizeof(long),sec,usec);
}

ssize_t filedescriptor::write(float number, long sec, long usec) const {
	DEBUG_WRITE_FLOAT("float",number);
	return bufferedWrite(&number,sizeof(float),sec,usec);
}

ssize_t filedescriptor::write(double number, long sec, long usec) const {
	DEBUG_WRITE_FLOAT("double",number);
	return bufferedWrite(&number,sizeof(double),sec,usec);
}

ssize_t filedescriptor::write(unsigned char character,
						long sec, long usec) const {
	DEBUG_WRITE_CHAR("uchar",character);
	return bufferedWrite(&character,sizeof(unsigned char),sec,usec);
}

ssize_t filedescriptor::write(bool value, long sec, long usec) const {
	DEBUG_WRITE_INT("bool",value);
	return bufferedWrite(&value,sizeof(bool),sec,usec);
}

ssize_t filedescriptor::write(char character, long sec, long usec) const {
	DEBUG_WRITE_CHAR("char",character);
	return bufferedWrite(&character,sizeof(char),sec,usec);
}

ssize_t filedescriptor::write(const unsigned char *string, size_t size,
						long sec, long usec) const {
	DEBUG_WRITE_STRING("ustring",string,size);
	return bufferedWrite(string,size,sec,usec);
}

ssize_t filedescriptor::write(const char *string, size_t size,
						long sec, long usec) const {
	DEBUG_WRITE_STRING("string",string,size);
	return bufferedWrite(string,size,sec,usec);
}

ssize_t filedescriptor::write(const unsigned char *string,
						long sec, long usec) const {
	DEBUG_WRITE_STRING("ustring",string,charstring::length(string));
	return bufferedWrite(string,charstring::length(string),sec,usec);
}

ssize_t filedescriptor::write(const char *string, long sec, long usec) const {
	DEBUG_WRITE_STRING("string",string,charstring::length(string));
	return bufferedWrite(string,charstring::length(string),sec,usec);
}

ssize_t filedescriptor::write(const void *buffer, size_t size,
						long sec, long usec) const {
	DEBUG_WRITE_VOID("void",buffer,size);
	return bufferedWrite(buffer,size,sec,usec);
}

ssize_t filedescriptor::read(unsigned short *buffer) const {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(unsigned long *buffer) const {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(unsigned long long *buffer) const {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(short *buffer) const {
	return bufferedRead(buffer,sizeof(short),-1,-1);
}

ssize_t filedescriptor::read(long *buffer) const {
	return bufferedRead(buffer,sizeof(long),-1,-1);
}

ssize_t filedescriptor::read(long long *buffer) const {
	return bufferedRead(buffer,sizeof(long long),-1,-1);
}

ssize_t filedescriptor::read(float *buffer) const {
	return bufferedRead(buffer,sizeof(float),-1,-1);
}

ssize_t filedescriptor::read(double *buffer) const {
	return bufferedRead(buffer,sizeof(double),-1,-1);
}

ssize_t filedescriptor::read(unsigned char *buffer) const {
	return bufferedRead(buffer,sizeof(unsigned char),-1,-1);
}

ssize_t filedescriptor::read(bool *buffer) const {
	return bufferedRead(buffer,sizeof(bool),-1,-1);
}

ssize_t filedescriptor::read(char *buffer) const {
	return bufferedRead(buffer,sizeof(char),-1,-1);
}

ssize_t filedescriptor::read(unsigned char *buffer, size_t size) const {
	return bufferedRead(buffer,size,-1,-1);
}

ssize_t filedescriptor::read(char *buffer, size_t size) const {
	return bufferedRead(buffer,size,-1,-1);
}

ssize_t filedescriptor::read(void *buffer, size_t size) const {
	return bufferedRead(buffer,size,-1,-1);
}

ssize_t filedescriptor::read(char **buffer, char *terminator) const {
	return read(buffer,terminator,-1,-1);
}

ssize_t filedescriptor::read(unsigned short *buffer,
					long sec, long usec) const {
	ssize_t	retval=bufferedRead(buffer,sizeof(unsigned short),sec,usec);
	*buffer=netToHost(*buffer);
	return retval;
}

ssize_t filedescriptor::read(unsigned long *buffer, long sec, long usec) const {
	ssize_t	retval=bufferedRead(buffer,sizeof(unsigned long),sec,usec);
	*buffer=netToHost(*buffer);
	return retval;
}

ssize_t filedescriptor::read(unsigned long long *buffer,
				long sec, long usec) const {
	ssize_t	retval=bufferedRead(buffer,sizeof(unsigned long long),sec,usec);
	*buffer=netToHost(*buffer);
	return retval;
}

ssize_t filedescriptor::read(short *buffer, long sec, long usec) const {
	return bufferedRead(buffer,sizeof(short),sec,usec);
}

ssize_t filedescriptor::read(long *buffer, long sec, long usec) const {
	return bufferedRead(buffer,sizeof(long),sec,usec);
}

ssize_t filedescriptor::read(float *buffer, long sec, long usec) const {
	return bufferedRead(buffer,sizeof(float),sec,usec);
}

ssize_t filedescriptor::read(double *buffer, long sec, long usec) const {
	return bufferedRead(buffer,sizeof(double),sec,usec);
}

ssize_t filedescriptor::read(unsigned char *buffer, long sec, long usec) const {
	return bufferedRead(buffer,sizeof(unsigned char),sec,usec);
}

ssize_t filedescriptor::read(bool *buffer, long sec, long usec) const {
	return bufferedRead(buffer,sizeof(bool),sec,usec);
}

ssize_t filedescriptor::read(char *buffer, long sec, long usec) const {
	return bufferedRead(buffer,sizeof(char),sec,usec);
}

ssize_t filedescriptor::read(unsigned char *buffer, size_t size,
						long sec, long usec) const {
	return bufferedRead(buffer,size,sec,usec);
}

ssize_t filedescriptor::read(char *buffer, size_t size,
						long sec, long usec) const {
	return bufferedRead(buffer,size,sec,usec);
}

ssize_t filedescriptor::read(void *buffer, size_t size,
						long sec, long usec) const {
	return bufferedRead(buffer,size,sec,usec);
}

bool filedescriptor::close() {
#ifdef RUDIMENTS_HAS_SSL
	if (ssl) {
		sslresult=SSL_shutdown(ssl);
	}
#endif
	if (fd!=-1) {
		int	result;
		do {
			result=::close(fd);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (result==-1) {
			return false;
		}
		fd=-1;
	}
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
	#ifdef HAVE_FCNTL
	retryinterruptedfcntl=true;
	#endif
}

void filedescriptor::dontRetryInterruptedFcntl() {
	#ifdef HAVE_FCNTL
	retryinterruptedfcntl=true;
	#endif
}

void filedescriptor::retryInterruptedIoctl() {
	#ifdef HAVE_IOCTL
	retryinterruptedioctl=true;
	#endif
}

void filedescriptor::dontRetryInterruptedIoctl() {
	#ifdef HAVE_IOCTL
	retryinterruptedioctl=true;
	#endif
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
						long sec, long usec) const {

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
		if ((sizeread=bufferedRead(&charbuffer,
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

ssize_t filedescriptor::bufferedRead(void *buf, ssize_t count,
						long sec, long usec) const {

	#ifdef DEBUG_READ
	printf("bufferedRead of %d bytes\n",count);
	#endif

	if (!count) {
		return 0;
	}

	if (!readbuffer) {
		#ifdef DEBUG_READ
		printf("no read buffer...\n");
		#endif
		return safeRead(buf,count,sec,usec);
	}

	unsigned char	*data=reinterpret_cast<unsigned char *>(buf);
	ssize_t		bytesread=0;
	ssize_t		bytesunread=count;

	for (;;) {

		// copy what we can from the buffer
		ssize_t	bytesavailabletocopy=readbuffertail-readbufferhead;
		if (bytesavailabletocopy) {

			#ifdef DEBUG_READ
			printf("%d bytes in read buffer\n",
					bytesavailabletocopy);
			#endif

			ssize_t	bytestocopy=(bytesavailabletocopy<bytesunread)?
						bytesavailabletocopy:
						bytesunread;

			#ifdef DEBUG_READ
			printf("copying %d bytes out of read buffer\n",
								bytestocopy);
			#endif

			rawbuffer::copy(data,readbufferhead,bytestocopy);
			data=data+bytestocopy;
			bytesread=bytesread+bytestocopy;
			readbufferhead=readbufferhead+bytestocopy;
			bytesunread=bytesunread-bytestocopy;

			// if we've read enough, break out
			if (bytesread==count) {
				#ifdef DEBUG_READ
				printf("yay, we're done reading\n");
				#endif
				return bytesread;
			}

			#ifdef DEBUG_READ
			printf("need to read %d more bytes\n",bytesunread);
			#endif
		}

		// if we've copied out everything in the buffer, read some more
		if (readbufferhead==readbuffertail) {

			#ifdef DEBUG_READ
			printf("attempting to fill read buffer, ");
			printf("reading %d bytes...\n",
				readbufferend-readbuffer);
			#endif

			bool	saveasr=allowshortreads;
			allowshortreads=true;
			ssize_t	result=safeRead(readbuffer,
						readbufferend-readbuffer,
						sec,usec);
			allowshortreads=saveasr;

			if (!result) {

				if (allowshortreads) {
					#ifdef DEBUG_READ
					printf("EOF\n");
					#endif
					return bytesread;
				}

				#ifdef DEBUG_READ
				printf("still need %d bytes, reading...\n",
								bytesunread);
				#endif
				result=safeRead(readbuffer,bytesunread,
								sec,usec);

				if (result>-1 && result!=bytesunread) {
					#ifdef DEBUG_READ
					printf("EOF\n");
					#endif
					return bytesread;
				}
			}

			if (result<0) {
				#ifdef DEBUG_READ
				printf("error reading...\n");
				#endif
				return result;
			}

			readbufferhead=readbuffer;
			readbuffertail=readbuffer+result;

			#ifdef DEBUG_READ
			printf("read %d bytes\n",result);
			#endif
		}
	}
}

ssize_t filedescriptor::safeRead(void *buf, ssize_t count,
						long sec, long usec) const {

	// FIXME: is this what we want to do?
	// maybe we should set some kind of error condition too
	if (!buf) {
		return 0;
	}

	#ifdef DEBUG_READ
	printf("%d: safeRead(%d,",process::getProcessId(),fd);
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
		error::clearError();
		#ifdef RUDIMENTS_HAS_SSL
		if (ssl) {
			for (bool done=false; !done ;) {

				#ifdef SSL_VOID_PTR
				actualread=SSL_read(ssl,ptr,sizetoread);
				#else
				actualread=SSL_read(ssl,
						static_cast<char *>(ptr),
						sizetoread);
				#endif
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
			for (int i=0; i<actualread; i++) {
				character::safePrint(
					(static_cast<unsigned char *>(ptr))[i]);
			}
			printf("(%ld bytes) ",actualread);
			fflush(stdout);
			#endif
		#ifdef RUDIMENTS_HAS_SSL
		}
		#endif

		// if we didn't read the number of bytes we expected to,
		// handle that...
		if (actualread!=sizetoread) {
			if (error::getErrorNumber()==EINTR) {
				#ifdef DEBUG_READ
				printf(" EINTR ");
				#endif
				// if we got an EINTR, then we may need to
				// retry the read
				if (retryinterruptedreads) {
					continue;
				} else {
					totalread=totalread+actualread;
					break;
				}
			} else if (actualread==0 &&
					error::getErrorNumber()==0) {
				// eof condition
				#ifdef DEBUG_READ
				printf(" EOF ");
				#endif
				break;
			} else if (actualread==-1) {
				// error condition
				#ifdef DEBUG_READ
				printf(")\n");
				#endif
				return RESULT_ERROR;
			}
		}

		totalread=totalread+actualread;

		// if we want to allow short reads, then break out here
		if (allowshortreads) {
			#ifdef DEBUG_READ
			printf(" SHORTREAD ");
			#endif
			break;
		}
	}

	#ifdef DEBUG_READ
	printf(",%d)\n",totalread);
	#endif
	return totalread;
}

ssize_t filedescriptor::bufferedWrite(const void *buf, ssize_t count,
						long sec, long usec) const {
	#ifdef DEBUG_WRITE
	printf("bufferedWrite of %d bytes\n",count);
	#endif

	if (!count) {
		return 0;
	}

	if (!writebuffer) {
		#ifdef DEBUG_WRITE
		printf("no write buffer...\n");
		#endif
		return safeWrite(buf,count,sec,usec);
	}

	const unsigned char	*data=
		reinterpret_cast<const unsigned char *>(buf);

	ssize_t	initialwritebuffersize=writebufferptr-writebuffer;
	bool	first=true;

	ssize_t	byteswritten=0;
	ssize_t	bytesunwritten=count;
	while (byteswritten<count) {

		ssize_t	writebuffersize=writebufferptr-writebuffer;
		ssize_t	writebufferspace=writebufferend-writebufferptr;

		#ifdef DEBUG_WRITE
		printf("	writebuffersize=%d\n",writebuffersize);
		printf("	writebufferspace=%d\n",writebufferspace);
		printf("	byteswritten=%d\n",byteswritten);
		printf("	bytesunwritten=%d\n",bytesunwritten);
		#endif

		if (bytesunwritten<=writebufferspace) {

			#ifdef DEBUG_WRITE
			printf("buffering %d bytes\n",bytesunwritten);
			#endif

			rawbuffer::copy(writebufferptr,data,bytesunwritten);
			writebufferptr=writebufferptr+bytesunwritten;
			byteswritten=byteswritten+bytesunwritten;

		} else {

			#ifdef DEBUG_WRITE
			printf("just buffering %d bytes\n",writebufferspace);
			#endif

			rawbuffer::copy(writebufferptr,data,writebufferspace);

			ssize_t	result=safeWrite(writebuffer,
					writebuffersize+writebufferspace,
					sec,usec);
			if (result!=writebuffersize+writebufferspace) {
				// FIXME: handle this better...
				printf("aaaaah, short write!!!!!\n");
			}
			if (result<0) {
				return result;
			}

			writebufferptr=writebuffer;
			// The first time the buffer is written, the number of
			// bytes that were already in the buffer need to be
			// taken into account when calculating byteswritten,
			// bytesunwritten and data.
			ssize_t	adjustment=(first)?initialwritebuffersize:0;
			if (first) {
				first=false;
			}
			byteswritten=byteswritten+result-adjustment;
			bytesunwritten=bytesunwritten-result+adjustment;
			data=data+result-adjustment;
		}
	}

	return byteswritten;
}

bool filedescriptor::flushWriteBuffer(long sec, long usec) const {
	if (!writebuffer) {
		return true;
	}
	ssize_t	writebuffersize=writebufferptr-writebuffer;
	bool	retval=(safeWrite(writebuffer,writebuffersize,
					sec,usec)==writebuffersize);
	writebufferptr=writebuffer;
	return retval;
}

ssize_t filedescriptor::safeWrite(const void *buf, ssize_t count,
						long sec, long usec) const {

	#ifdef DEBUG_WRITE
	printf("safeWrite of %d bytes\n",count);
	#endif

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

		error::clearError();
		#ifdef RUDIMENTS_HAS_SSL
		if (ssl) {
			for (bool done=false; !done ;) {

				#ifdef SSL_VOID_PTR
				retval=::SSL_write(ssl,buf,count);
				#else
				retval=::SSL_write(ssl,
						static_cast<const char *>(buf),
						count);
				#endif
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
			if (retryinterruptedwrites &&
				error::getErrorNumber()==EINTR) {
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
			if (retryinterruptedwaits &&
				error::getErrorNumber()==EINTR) {
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

unsigned long long filedescriptor::hostToNet(unsigned long long value) const {
	#if __BYTE_ORDER == __BIG_ENDIAN
		return value;
	#else
		return __bswap_64(value);
	#endif
}

unsigned short filedescriptor::netToHost(unsigned short value) const {
	return (translatebyteorder)?ntohs(value):value;
}

unsigned long filedescriptor::netToHost(unsigned long value) const {
	return (translatebyteorder)?ntohl(value):value;
}

unsigned long long filedescriptor::netToHost(unsigned long long value) const {
	#if __BYTE_ORDER == __BIG_ENDIAN
		return value;
	#else
		return __bswap_64(value);
	#endif
}

#ifdef RUDIMENTS_HAS_SSL
int filedescriptor::getSSLResult() const {
	return sslresult;
}
#endif

int filedescriptor::fcntl(int cmd, long arg) const {
	#ifdef HAVE_FCNTL
		int	result;
		do {
			result=::fcntl(fd,cmd,arg);
		} while (retryinterruptedfcntl && result==-1 &&
				error::getErrorNumber()==EINTR);
		return result;
	#else
		return -1;
	#endif
}

int filedescriptor::ioctl(int cmd, void *arg) const {
	#ifdef HAVE_IOCTL
		int	result;
		do {
			result=::ioctl(fd,cmd,arg);
		} while (retryinterruptedioctl && result==-1 &&
				error::getErrorNumber()==EINTR);
		return result;
	#else
		return -1;
	#endif
}

#ifdef __MINGW32__
bool filedescriptor::passFileDescriptor(int filedesc) const {
	return false;
}

bool filedescriptor::receiveFileDescriptor(int *filedesc) const {
	return false;
}
#else
bool filedescriptor::passFileDescriptor(int filedesc) const {

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
		//#ifdef HAVE_CMSG_SPACE
		//union {
			//struct cmsghdr	cm;
			//char		control[CMSG_SPACE(sizeof(int))];
		//} control;
		//messageheader.msg_control=control.control;
		//#else
		unsigned char	control[sizeof(struct cmsghdr)+sizeof(int)];
		messageheader.msg_control=(caddr_t)control;
		//#endif
		messageheader.msg_controllen=sizeof(control);

		struct cmsghdr	*cmptr;
		cmptr=CMSG_FIRSTHDR(&messageheader);
		cmptr->cmsg_level=SOL_SOCKET;
		cmptr->cmsg_type=SCM_RIGHTS;
		//#ifdef HAVE_CMSG_LEN
		//cmptr->cmsg_len=CMSG_LEN(sizeof(int));
		//#else
		cmptr->cmsg_len=sizeof(control);
		//#endif
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
	int	result;
	do {
		result=sendmsg(fd,&messageheader,0);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return (result!=-1);
}

bool filedescriptor::receiveFileDescriptor(int *filedesc) const {

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
		//#ifdef HAVE_CMSG_SPACE
		//union {
			//struct cmsghdr	cm;
			//char		control[CMSG_SPACE(sizeof(int))];
		//} control;
		//messageheader.msg_control=control.control;
		//#else
		unsigned char	control[sizeof(struct cmsghdr)+sizeof(int)];
		messageheader.msg_control=(caddr_t)control;
		//#endif
		messageheader.msg_controllen=sizeof(control);
	#else
		// old-style
		// The descriptor is received in the accrights
		int	newfiledesc;
		messageheader.msg_accrights=(caddr_t)&newfiledesc;
		messageheader.msg_accrightslen=sizeof(int);
	#endif

	// receive the msghdr
	int	result;
	do {
		result=recvmsg(fd,&messageheader,0);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result==-1) {
		return false;
	}


	// if we got valid data, set the passed-in descriptor to the 
	// descriptor we received and return success
	#ifdef HAVE_MSGHDR_MSG_CONTROLLEN

		struct cmsghdr  *cmptr=CMSG_FIRSTHDR(&messageheader);
		if (cmptr && cmptr->cmsg_len==
			//#ifdef HAVE_CMSG_LEN
			//CMSG_LEN(sizeof(int)) &&
			//#else
			//(sizeof(struct cmsghdr)+sizeof(int)) &&
			sizeof(control) &&
			//#endif
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
				printf("%d: ",process::getProcessId());
				printf("null cmptr\n");
			} else {
				//#ifdef HAVE_CMSG_LEN
				/*if (cmptr->cmsg_len!=CMSG_LEN(sizeof(int))) {
					printf("%d: ",process::getProcessId());
					printf("got cmsg_len=");
			       		printf("%d",static_cast<long>(
							cmptr->cmsg_len));
			       		printf(" instead of ");
					printf("%d",static_cast<long>(
							CMSG_LEN(sizeof(int))));
					printf("\n");
				}*/
				//#endif
				if (cmptr->cmsg_level!=SOL_SOCKET) {
					printf("%d: ",process::getProcessId());
					printf("got cmsg_level=");
					printf("%d",static_cast<long>(
							cmptr->cmsg_level));
					printf(" instead of ");
					printf("%d",static_cast<long>(
								SOL_SOCKET));
					printf("\n");
				}
				if (cmptr->cmsg_type!=SCM_RIGHTS) {
					printf("%d: ",process::getProcessId());
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
#endif

bool filedescriptor::useNaglesAlgorithm() {
	return setNoDelay(0);
}

bool filedescriptor::dontUseNaglesAlgorithm() {
	return setNoDelay(1);
}

bool filedescriptor::setNoDelay(int onoff) {
	int	value=onoff;
	return !setSockOpt(IPPROTO_TCP,TCP_NODELAY,
				(SETSOCKOPT_OPTVAL_TYPE)&value,
				(socklen_t)sizeof(int));
}

bool filedescriptor::getTcpWriteBufferSize(int *size) {
	socklen_t	intsize=sizeof(int);
	return getSockOpt(SOL_SOCKET,SO_SNDBUF,
					(GETSOCKOPT_OPTVAL_TYPE)size,
					&intsize)!=-1;
}

bool filedescriptor::setTcpWriteBufferSize(int size) {
	return !setSockOpt(SOL_SOCKET,SO_SNDBUF,
					(SETSOCKOPT_OPTVAL_TYPE)&size,
					static_cast<socklen_t>(sizeof(int)));
}

bool filedescriptor::getTcpReadBufferSize(int *size) {
	socklen_t	intsize=sizeof(int);
	return getSockOpt(SOL_SOCKET,SO_RCVBUF,
					(GETSOCKOPT_OPTVAL_TYPE)size,
					&intsize)!=-1;
}

bool filedescriptor::setTcpReadBufferSize(int size) {
	return setSockOpt(SOL_SOCKET,SO_RCVBUF,
				(SETSOCKOPT_OPTVAL_TYPE)&size,
				static_cast<socklen_t>(sizeof(int)))!=-1;
}

const char *filedescriptor::getType() const {
	return type;
}

char *filedescriptor::getPeerAddress() const {

	// initialize a socket address structure
	struct sockaddr_in	clientsin;
	socklen_t		size=sizeof(clientsin);
	rawbuffer::zero(&clientsin,sizeof(clientsin));

	// get the peer address
	int	result;
	do {
		result=getpeername(fd,
				reinterpret_cast<struct sockaddr *>(&clientsin),
				&size);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result==-1) {
		return NULL;
	}

	// convert the address to a string and return a copy of it
	return charstring::duplicate(inet_ntoa(clientsin.sin_addr));
}

int filedescriptor::getSockOpt(int level, int optname,
				void *optval, socklen_t *optlen) {
	int	result;
	do {
		result=getsockopt(fd,level,optname,optval,optlen);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

int filedescriptor::setSockOpt(int level, int optname,
				const void *optval, socklen_t optlen) {
	int	result;
	do {
		result=setsockopt(fd,level,optname,optval,optlen);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
