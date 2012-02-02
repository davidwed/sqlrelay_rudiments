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

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_IO_H
	#include <io.h>
#endif
#include <stdio.h>
#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_SELECT_H
	#include <sys/select.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_FCNTL_H
	#include <fcntl.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_FCNTL_H
	#include <sys/fcntl.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_IOCTL_H
	#include <sys/ioctl.h>
#endif
#ifdef RUDIMENTS_HAVE_NETINET_IN_H
	#include <netinet/in.h>
#endif
#ifdef RUDIMENTS_HAVE_NETINET_TCP_H
	#include <netinet/tcp.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_UIO_H
	#include <sys/uio.h>
#endif
#include <limits.h>
#ifdef RUDIMENTS_HAVE_ARPA_INET_H
	#include <arpa/inet.h>
#endif
#ifdef RUDIMENTS_HAVE_BYTESWAP_H
	#include <byteswap.h>
#endif
#ifdef RUDIMENTS_HAVE_MACHINE_ENDIAN_H
	#include <machine/endian.h>
#endif
#ifdef RUDIMENTS_HAVE_OSSWAPHOSTTOLITTLEINT64
	#include <libkern/OSByteOrder.h>
#endif
#ifdef HAVE_SYS_POLL_H
	#include <sys/poll.h>
#elif  HAVE_POLL_H
	#include <poll.h>
#endif
#ifdef RUDIMENTS_HAVE_OS_SUPPORT_BYTEORDER_H
	#include <os/support/ByteOrder.h>
#endif

#ifndef __BYTE_ORDER
	#define __BYTE_ORDER BYTE_ORDER
#endif
#ifndef __BIG_ENDIAN
	#define __BIG_ENDIAN BIG_ENDIAN
#endif

// for FD_SET (macro that uses memset) on solaris
#include <string.h>

#ifdef RUDIMENTS_NEED_XNET_PROTOTYPES
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
			process::getProcessId(),type,pvt->_fd,number);
	#define DEBUG_WRITE_FLOAT(type,number) \
		printf("%d: %s write(%d,%f)\n", \
			process::getProcessId(),type,pvt->_fd,number);
	#define DEBUG_WRITE_CHAR(type,character) \
		printf("%d: %s write(%d,%d)\n", \
			process::getProcessId(),type,pvt->_fd,character);
	#define DEBUG_WRITE_STRING(type,string,size) \
		printf("%d: %s write(%d,",process::getProcessId(), \
							type,pvt->_fd); \
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
		printf("%d: %s write(%d,",process::getProcessId(), \
							type,pvt->_fd); \
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

class filedescriptorprivate {
	friend class filedescriptor;
	private:
		int32_t	_fd;
		bool	_retryinterruptedreads;
		bool	_retryinterruptedwrites;
		bool	_retryinterruptedwaits;
		#ifdef RUDIMENTS_HAVE_FCNTL
		bool	_retryinterruptedfcntl;
		#endif
		#ifdef RUDIMENTS_HAVE_IOCTL
		bool	_retryinterruptedioctl;
		#endif
		bool	_allowshortreads;
		bool	_allowshortwrites;
		bool	_translatebyteorder;

		listener	*_lstnr;
		bool		_uselistenerinsidereads;
		bool		_uselistenerinsidewrites;

		#ifdef RUDIMENTS_HAS_SSL
		SSL_CTX	*_ctx;
		SSL	*_ssl;
		BIO	*_bio;
		int32_t	_sslresult;
		#endif

		const char	*_type;

		unsigned char	*_writebuffer;
		unsigned char	*_writebufferend;
		unsigned char	*_writebufferptr;

		unsigned char	*_readbuffer;
		unsigned char	*_readbufferend;
		unsigned char	*_readbufferhead;
		unsigned char	*_readbuffertail;
};

filedescriptor::filedescriptor() {
	pvt=new filedescriptorprivate;
	filedescriptorInit();
}

filedescriptor::filedescriptor(const filedescriptor &f) {
	pvt=new filedescriptorprivate;
	filedescriptorClone(f);
}

filedescriptor &filedescriptor::operator=(const filedescriptor &f) {
	if (this!=&f) {
		delete[] pvt->_writebuffer;
		filedescriptorClone(f);
	}
	return *this;
}

void filedescriptor::filedescriptorInit() {
	pvt->_fd=-1;
	pvt->_retryinterruptedreads=false;
	pvt->_retryinterruptedwrites=false;
	pvt->_retryinterruptedwaits=true;
#ifdef RUDIMENTS_HAVE_FCNTL
	pvt->_retryinterruptedfcntl=true;
#endif
#ifdef RUDIMENTS_HAVE_IOCTL
	pvt->_retryinterruptedioctl=true;
#endif
	pvt->_allowshortreads=false;
	pvt->_allowshortwrites=false;
	pvt->_translatebyteorder=false;
	pvt->_lstnr=NULL;
	pvt->_uselistenerinsidereads=false;
	pvt->_uselistenerinsidewrites=false;
#ifdef RUDIMENTS_HAS_SSL
	pvt->_ctx=NULL;
	pvt->_bio=NULL;
	pvt->_ssl=NULL;
	pvt->_sslresult=1;
#endif
	pvt->_type="filedescriptor";
	pvt->_writebuffer=NULL;
	pvt->_writebufferend=NULL;
	pvt->_writebufferptr=NULL;
	pvt->_readbuffer=NULL;
	pvt->_readbufferend=NULL;
	pvt->_readbufferhead=NULL;
	pvt->_readbuffertail=NULL;
}

void filedescriptor::filedescriptorClone(const filedescriptor &f) {
	pvt->_fd=f.pvt->_fd;
	pvt->_translatebyteorder=f.pvt->_translatebyteorder;
	pvt->_retryinterruptedreads=f.pvt->_retryinterruptedreads;
	pvt->_retryinterruptedwrites=f.pvt->_retryinterruptedwrites;
	pvt->_retryinterruptedwaits=f.pvt->_retryinterruptedwaits;
#ifdef RUDIMENTS_HAVE_FCNTL
	pvt->_retryinterruptedfcntl=f.pvt->_retryinterruptedfcntl;
#endif
#ifdef RUDIMENTS_HAVE_IOCTL
	pvt->_retryinterruptedioctl=f.pvt->_retryinterruptedioctl;
#endif
	pvt->_allowshortreads=f.pvt->_allowshortreads;
	pvt->_allowshortwrites=f.pvt->_allowshortwrites;
	pvt->_lstnr=f.pvt->_lstnr;
	pvt->_uselistenerinsidereads=f.pvt->_uselistenerinsidereads;
	pvt->_uselistenerinsidewrites=f.pvt->_uselistenerinsidewrites;
#ifdef RUDIMENTS_HAS_SSL
	pvt->_ctx=f.pvt->_ctx;
	pvt->_bio=f.pvt->_bio;
	pvt->_ssl=f.pvt->_ssl;
	pvt->_sslresult=f.pvt->_sslresult;
#endif
	if (f.pvt->_writebuffer) {
		ssize_t	writebuffersize=f.pvt->_writebufferend-
						f.pvt->_writebuffer;
		pvt->_writebuffer=new unsigned char[writebuffersize];
		rawbuffer::copy(pvt->_writebuffer,
				f.pvt->_writebuffer,
				writebuffersize);
		pvt->_writebufferend=pvt->_writebuffer+writebuffersize;
		pvt->_writebufferptr=pvt->_writebuffer+
				(f.pvt->_writebufferptr-f.pvt->_writebuffer);
	} else {
		pvt->_writebuffer=NULL;
		pvt->_writebufferend=NULL;
		pvt->_writebufferptr=NULL;
	}
}

filedescriptor::~filedescriptor() {
	delete[] pvt->_readbuffer;
	delete[] pvt->_writebuffer;
	close();
#ifdef RUDIMENTS_HAS_SSL
	setSSLContext(NULL);
#endif
	delete pvt;
}

bool filedescriptor::setWriteBufferSize(ssize_t size) const {
	if (size<0) {
		return false;
	}
	delete[] pvt->_writebuffer;
	pvt->_writebuffer=(size)?new unsigned char[size]:NULL;
	pvt->_writebufferend=pvt->_writebuffer+size;
	pvt->_writebufferptr=pvt->_writebuffer;
	return true;
}

bool filedescriptor::setReadBufferSize(ssize_t size) const {
	if (size<0) {
		return false;
	}
	delete[] pvt->_readbuffer;
	pvt->_readbuffer=(size)?new unsigned char[size]:NULL;
	pvt->_readbufferend=pvt->_readbuffer+size;
	pvt->_readbufferhead=pvt->_readbuffer;
	pvt->_readbuffertail=pvt->_readbuffer;
	return true;
}

int32_t filedescriptor::getFileDescriptor() const {
	return pvt->_fd;
}

void filedescriptor::setFileDescriptor(int32_t filedesc) {
	pvt->_fd=filedesc;
}

int32_t filedescriptor::duplicate() const {
	int32_t	result;
	do {
		result=dup(pvt->_fd);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

bool filedescriptor::duplicate(int32_t newfd) const {
	int32_t	result;
	do {
		result=dup2(pvt->_fd,newfd);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return (result==newfd);
}

#ifdef RUDIMENTS_HAS_SSL
void filedescriptor::setSSLContext(SSL_CTX *ctx) {
	if (!ctx) {
		deInitializeSSL();
	}
	pvt->_ctx=ctx;
}

SSL_CTX *filedescriptor::getSSLContext() {
	return pvt->_ctx;
}

bool filedescriptor::initializeSSL() {
	if (pvt->_fd==-1) {
		return false;
	}
	deInitializeSSL();
	if (pvt->_ctx) {
		pvt->_bio=newSSLBIO();
		pvt->_ssl=SSL_new(pvt->_ctx);
		SSL_set_bio(pvt->_ssl,pvt->_bio,pvt->_bio);
	}
	return true;
}

void filedescriptor::deInitializeSSL() {
	if (pvt->_ssl) {
		SSL_free(pvt->_ssl);
		pvt->_ssl=NULL;
	}
	if (pvt->_bio) {
		// BIO_free causes a segfault, and none of the example code
		// that I've seen calls it, but the function exists so
		// presumably it has a purpose.
		//BIO_free(pvt->_bio);
		pvt->_bio=NULL;
	}
}

SSL *filedescriptor::getSSL() const {
	return pvt->_ssl;
}

BIO *filedescriptor::newSSLBIO() const {
	return BIO_new_fd(pvt->_fd,BIO_NOCLOSE);
}
#endif

bool filedescriptor::useNonBlockingMode() const {
	#if defined(RUDIMENTS_HAVE_FCNTL) && \
		defined(F_SETFL) && defined (F_GETFL)
		return (fCntl(F_SETFL,fCntl(F_GETFL,0)|O_NONBLOCK)!=-1);
	#else
		return false;
	#endif
}

bool filedescriptor::useBlockingMode() const {
	#if defined(RUDIMENTS_HAVE_FCNTL) && \
		defined(F_SETFL) && defined (F_GETFL)
		return (fCntl(F_SETFL,fCntl(F_GETFL,0)&(~O_NONBLOCK))!=-1);
	#else
		return false;
	#endif
}

bool filedescriptor::isUsingNonBlockingMode() const {
	#if defined(RUDIMENTS_HAVE_FCNTL) && defined(F_GETFL)
		return (fCntl(F_GETFL,0)&O_NONBLOCK);
	#else
		return false;
	#endif
}

ssize_t filedescriptor::write(uint16_t number) const {
	return write(number,-1,-1);
}

ssize_t filedescriptor::write(uint32_t number) const {
	return write(number,-1,-1);
}

ssize_t filedescriptor::write(uint64_t number) const {
	return write(number,-1,-1);
}

ssize_t filedescriptor::write(int16_t number) const {
	DEBUG_WRITE_INT("int16_t",number);
	return bufferedWrite(&number,sizeof(int16_t),-1,-1);
}

ssize_t filedescriptor::write(int32_t number) const {
	DEBUG_WRITE_INT("int32_t",number);
	return bufferedWrite(&number,sizeof(int32_t),-1,-1);
}

ssize_t filedescriptor::write(int64_t number) const {
	DEBUG_WRITE_INT("int64_t",number);
	return bufferedWrite(&number,sizeof(int64_t),-1,-1);
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

ssize_t filedescriptor::write(uint16_t number,
				long sec, long usec) const {
	DEBUG_WRITE_INT("uint16_t",number);
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return bufferedWrite(&number,sizeof(uint16_t),sec,usec);
}

ssize_t filedescriptor::write(uint32_t number,
				long sec, long usec) const {
	DEBUG_WRITE_INT("uint32_t",number);
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return bufferedWrite(&number,sizeof(uint32_t),sec,usec);
}

ssize_t filedescriptor::write(uint64_t number,
				long sec, long usec) const {
	DEBUG_WRITE_INT("uint64_t",number);
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return bufferedWrite(&number,sizeof(uint64_t),sec,usec);
}

ssize_t filedescriptor::write(int16_t number,
				long sec, long usec) const {
	DEBUG_WRITE_INT("int16_t",number);
	return bufferedWrite(&number,sizeof(int16_t),sec,usec);
}

ssize_t filedescriptor::write(int32_t number,
				long sec, long usec) const {
	DEBUG_WRITE_INT("int32_t",number);
	return bufferedWrite(&number,sizeof(int32_t),sec,usec);
}

ssize_t filedescriptor::write(int64_t number,
				long sec, long usec) const {
	DEBUG_WRITE_INT("int64_t",number);
	return bufferedWrite(&number,sizeof(int64_t),sec,usec);
}

ssize_t filedescriptor::write(float number,
				long sec,long usec) const {
	DEBUG_WRITE_FLOAT("float",number);
	return bufferedWrite(&number,sizeof(float),sec,usec);
}

ssize_t filedescriptor::write(double number,
				long sec, long usec) const {
	DEBUG_WRITE_FLOAT("double",number);
	return bufferedWrite(&number,sizeof(double),sec,usec);
}

ssize_t filedescriptor::write(unsigned char character,
				long sec, long usec) const {
	DEBUG_WRITE_CHAR("uchar",character);
	return bufferedWrite(&character,sizeof(unsigned char),sec,usec);
}

ssize_t filedescriptor::write(bool value,
				long sec, long usec) const {
	DEBUG_WRITE_INT("bool",value);
	return bufferedWrite(&value,sizeof(bool),sec,usec);
}

ssize_t filedescriptor::write(char character,
				long sec, long usec) const {
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

ssize_t filedescriptor::write(const char *string,
					long sec, long usec) const {
	DEBUG_WRITE_STRING("string",string,charstring::length(string));
	return bufferedWrite(string,charstring::length(string),sec,usec);
}

ssize_t filedescriptor::write(const void *buffer, size_t size,
					long sec, long usec) const {
	DEBUG_WRITE_VOID("void",buffer,size);
	return bufferedWrite(buffer,size,sec,usec);
}

ssize_t filedescriptor::read(uint16_t *buffer) const {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(uint32_t *buffer) const {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(uint64_t *buffer) const {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(int16_t *buffer) const {
	return bufferedRead(buffer,sizeof(int16_t),-1,-1);
}

ssize_t filedescriptor::read(int32_t *buffer) const {
	return bufferedRead(buffer,sizeof(int32_t),-1,-1);
}

ssize_t filedescriptor::read(int64_t *buffer) const {
	return bufferedRead(buffer,sizeof(int64_t),-1,-1);
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

ssize_t filedescriptor::read(char **buffer, const char *terminator) const {
	return read(buffer,terminator,-1,-1);
}

ssize_t filedescriptor::read(uint16_t *buffer,
				long sec, long usec) const {
	ssize_t	retval=bufferedRead(buffer,sizeof(uint16_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(uint32_t *buffer,
				long sec, long usec) const {
	ssize_t	retval=bufferedRead(buffer,sizeof(uint32_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(uint64_t *buffer,
				long sec, long usec) const {
	ssize_t	retval=bufferedRead(buffer,sizeof(uint64_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(int16_t *buffer,
				long sec, long usec) const {
	return bufferedRead(buffer,sizeof(int16_t),sec,usec);
}

ssize_t filedescriptor::read(int32_t *buffer,
				long sec, long usec) const {
	return bufferedRead(buffer,sizeof(int32_t),sec,usec);
}

ssize_t filedescriptor::read(int64_t *buffer,
				long sec, long usec) const {
	return bufferedRead(buffer,sizeof(int64_t),sec,usec);
}

ssize_t filedescriptor::read(float *buffer,
				long sec, long usec) const {
	return bufferedRead(buffer,sizeof(float),sec,usec);
}

ssize_t filedescriptor::read(double *buffer,
				long sec, long usec) const {
	return bufferedRead(buffer,sizeof(double),sec,usec);
}

ssize_t filedescriptor::read(unsigned char *buffer,
				long sec, long usec) const {
	return bufferedRead(buffer,sizeof(unsigned char),sec,usec);
}

ssize_t filedescriptor::read(bool *buffer,
				long sec, long usec) const {
	return bufferedRead(buffer,sizeof(bool),sec,usec);
}

ssize_t filedescriptor::read(char *buffer,
				long sec, long usec) const {
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
	if (pvt->_ssl) {
		pvt->_sslresult=SSL_shutdown(pvt->_ssl);
	}
#endif
	if (pvt->_fd!=-1) {
		int32_t	result;
		do {
			result=lowLevelClose();
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (result==-1) {
			return false;
		}
		pvt->_fd=-1;
	}
	return true;
}

int32_t filedescriptor::lowLevelClose() {
	return ::close(pvt->_fd);
}

void filedescriptor::retryInterruptedReads() {
	pvt->_retryinterruptedreads=true;
}

void filedescriptor::dontRetryInterruptedReads() {
	pvt->_retryinterruptedreads=false;
}

void filedescriptor::retryInterruptedWrites() {
	pvt->_retryinterruptedwrites=true;
}

void filedescriptor::dontRetryInterruptedWrites() {
	pvt->_retryinterruptedwrites=false;
}

void filedescriptor::retryInterruptedWaits() {
	pvt->_retryinterruptedwaits=true;
}

void filedescriptor::dontRetryInterruptedWaits() {
	pvt->_retryinterruptedwaits=false;
}

void filedescriptor::retryInterruptedFcntl() {
	#ifdef RUDIMENTS_HAVE_FCNTL
	pvt->_retryinterruptedfcntl=true;
	#endif
}

void filedescriptor::dontRetryInterruptedFcntl() {
	#ifdef RUDIMENTS_HAVE_FCNTL
	pvt->_retryinterruptedfcntl=true;
	#endif
}

void filedescriptor::retryInterruptedIoctl() {
	#ifdef RUDIMENTS_HAVE_IOCTL
	pvt->_retryinterruptedioctl=true;
	#endif
}

void filedescriptor::dontRetryInterruptedIoctl() {
	#ifdef RUDIMENTS_HAVE_IOCTL
	pvt->_retryinterruptedioctl=true;
	#endif
}

void filedescriptor::allowShortReads() {
	pvt->_allowshortreads=true;
}

void filedescriptor::dontAllowShortReads() {
	pvt->_allowshortreads=false;
}

void filedescriptor::allowShortWrites() {
	pvt->_allowshortwrites=true;
}

void filedescriptor::dontAllowShortWrites() {
	pvt->_allowshortwrites=false;
}

void filedescriptor::useListener(listener *lstnr) {
	pvt->_lstnr=lstnr;
}

void filedescriptor::useListenerInsideReads() {
	pvt->_uselistenerinsidereads=true;
}

void filedescriptor::dontUseListenerInsideReads() {
	pvt->_uselistenerinsidereads=false;
}

void filedescriptor::useListenerInsideWrites() {
	pvt->_uselistenerinsidewrites=true;
}

void filedescriptor::dontUseListenerInsideWrites() {
	pvt->_uselistenerinsidewrites=false;
}

void filedescriptor::dontUseListener() {
	pvt->_lstnr=NULL;
}

listener *filedescriptor::getListener() {
	return pvt->_lstnr;
}

ssize_t filedescriptor::read(char **buffer, const char *terminator,
						long sec, long usec) const {

	// initialize a buffer
	int32_t	buffersize=512;
	if (buffer) {
		*buffer=new char[buffersize];
	}

	// initialize termination detector
	int32_t	termlen=charstring::length(terminator);
	char	*term=new char[termlen];
	for (int32_t i=0; i<termlen; i++) {
		term[i]='\0';
	}

	// initialize some variables
	char	charbuffer;
	int32_t	sizeread;
	int32_t	totalread=0;
	int32_t	escaped=0;
	int32_t	copytobuffer;
	int32_t	copytoterm;

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
				for (int32_t i=0; i<termlen-1; i++) {
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
				for (int32_t i=0; i<termlen; i++) {
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

	if (!pvt->_readbuffer) {
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
		ssize_t	bytesavailabletocopy=pvt->_readbuffertail-
						pvt->_readbufferhead;
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

			rawbuffer::copy(data,pvt->_readbufferhead,bytestocopy);
			data=data+bytestocopy;
			bytesread=bytesread+bytestocopy;
			pvt->_readbufferhead=pvt->_readbufferhead+bytestocopy;
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
		if (pvt->_readbufferhead==pvt->_readbuffertail) {

			#ifdef DEBUG_READ
			printf("attempting to fill read buffer, ");
			printf("reading %d bytes...\n",
				pvt->_readbufferend-pvt->_readbuffer);
			#endif

			bool	saveasr=pvt->_allowshortreads;
			pvt->_allowshortreads=true;
			ssize_t	result=safeRead(pvt->_readbuffer,
						pvt->_readbufferend-
						pvt->_readbuffer,
						sec,usec);
			pvt->_allowshortreads=saveasr;

			if (!result) {

				if (pvt->_allowshortreads) {
					#ifdef DEBUG_READ
					printf("EOF\n");
					#endif
					return bytesread;
				}

				#ifdef DEBUG_READ
				printf("still need %d bytes, reading...\n",
								bytesunread);
				#endif
				result=safeRead(pvt->_readbuffer,
						bytesunread,
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

			pvt->_readbufferhead=pvt->_readbuffer;
			pvt->_readbuffertail=pvt->_readbuffer+result;

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
	printf("%d: safeRead(%d,",process::getProcessId(),pvt->_fd);
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
		if ((sec>-1 && usec>-1) || pvt->_uselistenerinsidereads) {

			int32_t	selectresult=(pvt->_uselistenerinsidereads)?
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
		if (pvt->_ssl) {
			for (bool done=false; !done ;) {

				#ifdef RUDIMENTS_SSL_VOID_PTR
				actualread=SSL_read(pvt->_ssl,ptr,sizetoread);
				#else
				actualread=SSL_read(pvt->_ssl,
						static_cast<char *>(ptr),
						sizetoread);
				#endif
				pvt->_sslresult=actualread;

				switch (SSL_get_error(pvt->_ssl,actualread)) {
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
			actualread=lowLevelRead(ptr,sizetoread);
			#ifdef DEBUG_READ
			for (int32_t i=0; i<actualread; i++) {
				character::safePrint(
					(static_cast<unsigned char *>(ptr))[i]);
			}
			printf("(%ld bytes) ",actualread);
			if (actualread==-1) {
				printf("%s ",error::getErrorString());
			}
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
				if (pvt->_retryinterruptedreads) {
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
		if (pvt->_allowshortreads) {
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

ssize_t filedescriptor::lowLevelRead(void *buf, ssize_t count) const {
	return ::read(pvt->_fd,buf,count);
}

ssize_t filedescriptor::bufferedWrite(const void *buf, ssize_t count,
					long sec, long usec) const {
	#ifdef DEBUG_WRITE
	printf("bufferedWrite of %d bytes\n",count);
	#endif

	if (!count) {
		return 0;
	}

	if (!pvt->_writebuffer) {
		#ifdef DEBUG_WRITE
		printf("no write buffer...\n");
		#endif
		return safeWrite(buf,count,sec,usec);
	}

	const unsigned char	*data=
		reinterpret_cast<const unsigned char *>(buf);

	ssize_t	initialwritebuffersize=pvt->_writebufferptr-pvt->_writebuffer;
	bool	first=true;

	ssize_t	byteswritten=0;
	ssize_t	bytesunwritten=count;
	while (byteswritten<count) {

		ssize_t	writebuffersize=pvt->_writebufferptr-
						pvt->_writebuffer;
		ssize_t	writebufferspace=pvt->_writebufferend-
						pvt->_writebufferptr;

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

			rawbuffer::copy(pvt->_writebufferptr,
					data,bytesunwritten);
			pvt->_writebufferptr=pvt->_writebufferptr+
							bytesunwritten;
			byteswritten=byteswritten+bytesunwritten;

		} else {

			#ifdef DEBUG_WRITE
			printf("just buffering %d bytes\n",writebufferspace);
			#endif

			rawbuffer::copy(pvt->_writebufferptr,
					data,writebufferspace);

			bool	saveasw=pvt->_allowshortwrites;
			pvt->_allowshortwrites=true;
			ssize_t	result=safeWrite(pvt->_writebuffer,
					writebuffersize+writebufferspace,
					sec,usec);
			pvt->_allowshortwrites=saveasw;

			if (result!=writebuffersize+writebufferspace) {
				return result;
			}

			pvt->_writebufferptr=pvt->_writebuffer;
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
	if (!pvt->_writebuffer) {
		return true;
	}
	ssize_t	writebuffersize=pvt->_writebufferptr-pvt->_writebuffer;
	bool	retval=(safeWrite(pvt->_writebuffer,writebuffersize,
						sec,usec)==writebuffersize);
	pvt->_writebufferptr=pvt->_writebuffer;
	return retval;
}

ssize_t filedescriptor::safeWrite(const void *buf, ssize_t count,
					long sec, long usec) const {

	// FIXME: is this what we want to do?
	// maybe we should set some kind of error condition too
	if (!buf) {
		return 0;
	}

	#ifdef DEBUG_WRITE
	printf("%d: safeWrite(%d,",process::getProcessId(),pvt->_fd);
	#endif

	// The result of SSL_write may be undefined if count=0
	#ifdef RUDIMENTS_HAS_SSL
	if (!count) {
		return 0;
	}
	#endif

	ssize_t	totalwrite=0;
	ssize_t	sizetowrite;
	ssize_t	actualwrite;
	while (totalwrite<count) {

		// only write SSIZE_MAX at a time
		sizetowrite=count-totalwrite;
		if (sizetowrite>SSIZE_MAX) {
			sizetowrite=SSIZE_MAX;
		}

		// if necessary, select
		if ((sec>-1 && usec>-1) || pvt->_uselistenerinsidewrites) {

			int32_t	selectresult=(pvt->_uselistenerinsidewrites)?
					waitForNonBlockingWrite(sec,usec):
					safeSelect(sec,usec,false,true);

			// return error or timeout
			if (selectresult<0) {
				#ifdef DEBUG_WRITE
				printf(")\n");
				#endif
				return selectresult;
			}
		}

		// set a pointer to the position in the buffer that we need
		// to write data from
		const void	*ptr=static_cast<const void *>(
				static_cast<const unsigned char *>(buf)+
				totalwrite);

		error::clearError();
		#ifdef RUDIMENTS_HAS_SSL
		if (pvt->_ssl) {
			for (bool done=false; !done ;) {

				#ifdef RUDIMENTS_SSL_VOID_PTR
				actualwrite=::SSL_write(pvt->_ssl,ptr,count);
				#else
				actualwrite=::SSL_write(pvt->_ssl,
						static_cast<const char *>(ptr),
						sizetowrite);
				#endif
				pvt->_sslresult=actualwrite;

				switch (SSL_get_error(pvt->_ssl,actualwrite)) {
					case SSL_ERROR_WANT_READ:
					case SSL_ERROR_WANT_WRITE:
						continue;
					case SSL_ERROR_WANT_X509_LOOKUP:
					case SSL_ERROR_SSL:
						actualwrite=-1;
					case SSL_ERROR_ZERO_RETURN:
					case SSL_ERROR_SYSCALL:
					case SSL_ERROR_NONE:
					default:
						done=true;
				}
			}
		} else {
		#endif

			actualwrite=lowLevelWrite(ptr,sizetowrite);
			#ifdef DEBUG_WRITE
			for (int32_t i=0; i<actualwrite; i++) {
				character::safePrint(
				(static_cast<const unsigned char *>(ptr))[i]);
			}
			printf("(%ld bytes) ",actualwrite);
			if (actualwrite==-1) {
				printf("%s ",error::getErrorString());
			}
			fflush(stdout);
			#endif

		#ifdef RUDIMENTS_HAS_SSL
		}
		#endif

		// if we didn't read the number of bytes we expected to,
		// handle that...
		if (actualwrite!=sizetowrite) {
			if (error::getErrorNumber()==EINTR) {
				#ifdef DEBUG_WRITE
				printf(" EINTR ");
				#endif
				// if we got an EINTR, then we may need to
				// retry the write
				if (pvt->_retryinterruptedwrites) {
					continue;
				} else {
					totalwrite=totalwrite+actualwrite;
					break;
				}
			} else if (actualwrite==0 &&
					error::getErrorNumber()==0) {
				// eof condition
				#ifdef DEBUG_WRITE
				printf(" EOF ");
				#endif
				break;
			} else if (actualwrite==-1) {
				// error condition
				#ifdef DEBUG_WRITE
				printf(")\n");
				#endif
				return RESULT_ERROR;
			}
		}

		totalwrite=totalwrite+actualwrite;

		// if we want to allow short writes, then break out here
		if (pvt->_allowshortwrites) {
			#ifdef DEBUG_WRITE
			printf(" SHORTWRITE ");
			#endif
			break;
		}
	}

	#ifdef DEBUG_WRITE
	printf(",%d)\n",totalwrite);
	#endif
	return totalwrite;
}

ssize_t filedescriptor::lowLevelWrite(const void *buf, ssize_t count) const {
	return ::write(pvt->_fd,buf,count);
}

int32_t filedescriptor::waitForNonBlockingRead(long sec, long usec) const {
	return (pvt->_lstnr)?pvt->_lstnr->waitForNonBlockingRead(sec,usec):
				safeSelect(sec,usec,true,false);
}

int32_t filedescriptor::waitForNonBlockingWrite(long sec, long usec) const {
	return (pvt->_lstnr)?pvt->_lstnr->waitForNonBlockingWrite(sec,usec):
				safeSelect(sec,usec,false,true);
}

int32_t filedescriptor::safeSelect(long sec, long usec,
				bool read, bool write) const {

	// FD_SET will crash if you pass it -1 on some systems, just bail here
	if (pvt->_fd==-1) {
		return RESULT_ERROR;
	}

	#ifdef RUDIMENTS_HAS_SSL
		if (read && pvt->_ssl && SSL_pending(pvt->_ssl)) {
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
		FD_SET(pvt->_fd,&fdlist);

		// wait for data to be available on the file descriptor
		int32_t	selectresult=select(pvt->_fd+1,(read)?&fdlist:NULL,
						(write)?&fdlist:NULL,
						NULL,tvptr);

		if (selectresult==-1) {

			// if a signal caused the select to fall through, retry
			if (pvt->_retryinterruptedwaits &&
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

int32_t filedescriptor::safePoll(long sec, long usec,
				bool read, bool write) const {

	#ifdef RUDIMENTS_HAVE_POLL

		// bail immediately if the file descriptor is invalid
		if (pvt->_fd==-1) {
			return RESULT_ERROR;
		}

		#ifdef RUDIMENTS_HAS_SSL
			if (read && pvt->_ssl && SSL_pending(pvt->_ssl)) {
				return 1;
			}
		#endif

		// set up the fd's to be monitored, and how to monitor them
		struct pollfd	fds;
		fds.fd=pvt->_fd;
		fds.events=0;
		if (read) {
			fds.events|=POLLIN;
		}
		if (write) {
			fds.events|=POLLOUT;
		}

		// calculate the timeout
		int32_t	timeout=(sec*1000)+(usec/1000);

		for (;;) {

			// poll...
			int32_t	pollresult=poll(&fds,1,timeout);

			if (pollresult==-1) {

				// if a signal caused the poll
				// to fall through, retry
				if (pvt->_retryinterruptedwaits &&
					error::getErrorNumber()==EINTR) {
					continue;
				}
				return RESULT_ERROR;

			} else if (!pollresult) {

				// timeout
				return RESULT_TIMEOUT;
			}

			return pollresult;
		}
	#else
		return safeSelect(sec,usec,read,write);
	#endif
}

void filedescriptor::translateByteOrder() {
	pvt->_translatebyteorder=true;
}

void filedescriptor::dontTranslateByteOrder() {
	pvt->_translatebyteorder=false;
}

uint16_t filedescriptor::hostToNet(uint16_t value) {
	return htons(value);
}

uint32_t filedescriptor::hostToNet(uint32_t value) {
	return htonl(value);
}

uint64_t filedescriptor::hostToNet(uint64_t value) {
	#if __BYTE_ORDER == __BIG_ENDIAN
		return value;
	#else
		#if defined(RUDIMENTS_HAVE_BSWAP_64)
			return bswap_64(value);
		#elif defined(RUDIMENTS_HAVE___BSWAP64)
			return __bswap64(value);
		#elif defined(RUDIMENTS_HAVE_BSWAP64)
			return bswap64(value);
		#elif defined(RUDIMENTS_HAVE_SWAP64)
			return swap64(value);
		#elif defined(RUDIMENTS_HAVE_SWAP_INT64)
			return __swap_int64(value);
		#elif defined(RUDIMENTS_HAVE_OSSWAPHOSTTOLITTLEINT64)
			return OSSwapHostToLittleInt64(value);
		#else
			return
			(((uint64_t)hostToNet((uint32_t)(value&0x00000000FFFFFFFFLLU)))<<32)|
			((uint64_t)hostToNet((uint32_t)((value&0xFFFFFFFF00000000LLU)>>32)));
		#endif
	#endif
}

uint16_t filedescriptor::netToHost(uint16_t value) {
	return ntohs(value);
}

uint32_t filedescriptor::netToHost(uint32_t value) {
	return ntohl(value);
}

uint64_t filedescriptor::netToHost(uint64_t value) {
	#if __BYTE_ORDER == __BIG_ENDIAN
		return value;
	#else
		#if defined(RUDIMENTS_HAVE_BSWAP_64)
			return bswap_64(value);
		#elif defined(RUDIMENTS_HAVE___BSWAP64)
			return __bswap64(value);
		#elif defined(RUDIMENTS_HAVE_BSWAP64)
			return bswap64(value);
		#elif defined(RUDIMENTS_HAVE_SWAP64)
			return swap64(value);
		#elif defined(RUDIMENTS_HAVE_SWAP_INT64)
			return __swap_int64(value);
		#elif defined(RUDIMENTS_HAVE_OSSWAPLITTLETOHOSTINT64)
			return OSSwapLittleToHostInt64(value);
		#else
			return
			(((uint64_t)netToHost((uint32_t)(value&0x00000000FFFFFFFFLLU)))<<32)|
			((uint64_t)netToHost((uint32_t)((value&0xFFFFFFFF00000000LLU)>>32)));
		#endif
	#endif
}

#ifdef RUDIMENTS_HAS_SSL
int32_t filedescriptor::getSSLResult() const {
	return pvt->_sslresult;
}
#endif

int32_t filedescriptor::fCntl(int32_t cmd, long arg) const {
	#ifdef RUDIMENTS_HAVE_FCNTL
		int32_t	result;
		do {
			result=fcntl(pvt->_fd,cmd,arg);
		} while (pvt->_retryinterruptedfcntl && result==-1 &&
				error::getErrorNumber()==EINTR);
		return result;
	#else
		return -1;
	#endif
}

int32_t filedescriptor::ioCtl(int32_t cmd, void *arg) const {
	#ifdef RUDIMENTS_HAVE_IOCTL
		int32_t	result;
		do {
			result=ioctl(pvt->_fd,cmd,arg);
		} while (pvt->_retryinterruptedioctl && result==-1 &&
				error::getErrorNumber()==EINTR);
		return result;
	#else
		return -1;
	#endif
}

#ifdef MINGW32
bool filedescriptor::passFileDescriptor(int32_t filedesc) const {
	error::setErrorNumber(ENOSYS);
	return false;
}

bool filedescriptor::receiveFileDescriptor(int32_t *filedesc) const {
	error::setErrorNumber(ENOSYS);
	return false;
}
#else
bool filedescriptor::passFileDescriptor(int32_t filedesc) const {

	// have to use sendmsg to pass a file descriptor. 
	// sendmsg can only send a msghdr
	struct	msghdr	messageheader;

	// these must be null for stream sockets
	messageheader.msg_name=NULL;
	messageheader.msg_namelen=0;

	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_FLAGS
	// initialize flags to 0
	messageheader.msg_flags=0;
	#endif

	// must send at least 1 iovector with 1 byte of real data
	struct iovec	iovector[1];
	iovector[0].iov_base=(RUDIMENTS_IOV_BASE_TYPE)" ";
	iovector[0].iov_len=sizeof(char);
	messageheader.msg_iov=iovector;
	messageheader.msg_iovlen=1;

	// use other parts of the msghdr structure to send the descriptor
	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN

		// new-style:
		// The descriptor is passed in the msg_control
		//#ifdef RUDIMENTS_HAVE_CMSG_SPACE
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
		//#ifdef RUDIMENTS_HAVE_CMSG_LEN
		//cmptr->cmsg_len=CMSG_LEN(sizeof(int));
		//#else
		cmptr->cmsg_len=sizeof(control);
		//#endif
		*(reinterpret_cast<int32_t *>(CMSG_DATA(cmptr)))=filedesc;

		// FIXME: is this necessary???
		messageheader.msg_controllen=cmptr->cmsg_len;
	#else
		// old-style:
		// The descriptor is passed in the accrights
		messageheader.msg_accrights=(caddr_t)&filedesc;
		messageheader.msg_accrightslen=sizeof(int);
	#endif

	// finally, send the msghdr
	int32_t	result;
	do {
		result=sendmsg(pvt->_fd,&messageheader,0);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return (result!=-1);
}

bool filedescriptor::receiveFileDescriptor(int32_t *filedesc) const {

	// have to use recvmsg to receive a file descriptor. 
	// recvmsg can only send a msghdr
	struct msghdr	messageheader;

	// these must be null for stream sockets
	messageheader.msg_name=NULL;
	messageheader.msg_namelen=0;

	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_FLAGS
	// initialize flags to 0
	messageheader.msg_flags=0;
	#endif

	// the process that's going to handoff it's socket will also 
	// send a single iovector with a single byte of data in it, 
	// so we'll receive that too
	struct iovec	iovector[1];
	char		ptr;
	iovector[0].iov_base=(RUDIMENTS_IOV_BASE_TYPE)&ptr;
	iovector[0].iov_len=sizeof(char);
	messageheader.msg_iov=iovector;
	messageheader.msg_iovlen=1;

	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN
		// new-style:
		// The descriptor is received in the msg_control
		//#ifdef RUDIMENTS_HAVE_CMSG_SPACE
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
		int32_t	newfiledesc;
		messageheader.msg_accrights=(caddr_t)&newfiledesc;
		messageheader.msg_accrightslen=sizeof(int);
	#endif

	// receive the msghdr
	int32_t	result;
	do {
		// wait 120 seconds for data to come in
		// FIXME: this should be configurable
		if (safePoll(120,0,false,true)<1) {
			return false;
		}
		result=recvmsg(pvt->_fd,&messageheader,0);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result==-1) {
		return false;
	}


	// if we got valid data, set the passed-in descriptor to the 
	// descriptor we received and return success
	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN

		struct cmsghdr  *cmptr=CMSG_FIRSTHDR(&messageheader);
		if (cmptr && cmptr->cmsg_len==
			//#ifdef RUDIMENTS_HAVE_CMSG_LEN
			//CMSG_LEN(sizeof(int)) &&
			//#else
			//(sizeof(struct cmsghdr)+sizeof(int)) &&
			sizeof(control) &&
			//#endif
			cmptr->cmsg_level==SOL_SOCKET &&
			cmptr->cmsg_type==SCM_RIGHTS) {

			// if we got good data, set the descriptor and return
			*filedesc=
			*(reinterpret_cast<int32_t *>(CMSG_DATA(cmptr)));
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
				//#ifdef RUDIMENTS_HAVE_CMSG_LEN
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

bool filedescriptor::setNoDelay(int32_t onoff) {
	int32_t	value=onoff;
	return !setSockOpt(IPPROTO_TCP,TCP_NODELAY,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&value,
				(socklen_t)sizeof(int));
}

bool filedescriptor::getTcpWriteBufferSize(int32_t *size) {
	socklen_t	intsize=sizeof(int);
	return getSockOpt(SOL_SOCKET,SO_SNDBUF,
				(RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE)size,
				&intsize)!=-1;
}

bool filedescriptor::setTcpWriteBufferSize(int32_t size) {
	return !setSockOpt(SOL_SOCKET,SO_SNDBUF,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&size,
				static_cast<socklen_t>(sizeof(int)));
}

bool filedescriptor::getTcpReadBufferSize(int32_t *size) {
	socklen_t	intsize=sizeof(int);
	return getSockOpt(SOL_SOCKET,SO_RCVBUF,
				(RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE)size,
				&intsize)!=-1;
}

bool filedescriptor::setTcpReadBufferSize(int32_t size) {
	return setSockOpt(SOL_SOCKET,SO_RCVBUF,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&size,
				static_cast<socklen_t>(sizeof(int)))!=-1;
}

const char *filedescriptor::getType() const {
	return pvt->_type;
}

char *filedescriptor::getPeerAddress() const {

	// initialize a socket address structure
	struct sockaddr_in	clientsin;
	socklen_t		size=sizeof(clientsin);
	rawbuffer::zero(&clientsin,sizeof(clientsin));

	// get the peer address
	int32_t	result;
	do {
		result=getpeername(pvt->_fd,
				reinterpret_cast<struct sockaddr *>(&clientsin),
				&size);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result==-1) {
		return NULL;
	}

	// convert the address to a string and return a copy of it
	return charstring::duplicate(inet_ntoa(clientsin.sin_addr));
}

int32_t filedescriptor::getSockOpt(int32_t level, int32_t optname,
				void *optval, socklen_t *optlen) {
	int32_t	result;
	do {
		result=getsockopt(pvt->_fd,level,optname,
			(RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE)optval,optlen);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

int32_t filedescriptor::setSockOpt(int32_t level, int32_t optname,
				const void *optval, socklen_t optlen) {
	int32_t	result;
	do {
		result=setsockopt(pvt->_fd,level,optname,
			(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)optval,optlen);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

const char *filedescriptor::type() const {
	return pvt->_type;
}

void filedescriptor::type(const char *tp) {
	pvt->_type=tp;
}

int32_t filedescriptor::fd() const {
	return pvt->_fd;
}

void filedescriptor::fd(int32_t filedes) {
	pvt->_fd=filedes;
}

#ifdef RUDIMENTS_HAS_SSL
SSL_CTX *filedescriptor::ctx() {
	return pvt->_ctx;
}

SSL *filedescriptor::ssl() {
	return pvt->_ssl;
}

int32_t filedescriptor::sslresult() {
	return pvt->_sslresult;
}

void filedescriptor::sslresult(int32_t sslrslt) {
	pvt->_sslresult=sslrslt;
}
#endif

bool filedescriptor::closeOnExec() {
	#if defined(RUDIMENTS_HAVE_FD_CLOEXEC)
		return !fCntl(F_SETFD,fCntl(F_GETFD,FD_CLOEXEC)|FD_CLOEXEC);
	#elif defined(RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT)
		return SetHandleInformation((HANDLE)_get_osfhandle(pvt->_fd),
							HANDLE_FLAG_INHERIT,0);
	#else
		#error no FD_CLOEXEC or anything like it
	#endif
}

bool filedescriptor::dontCloseOnExec() {
	#if defined(RUDIMENTS_HAVE_FD_CLOEXEC)
		return !fCntl(F_SETFD,fCntl(F_GETFD,FD_CLOEXEC)&(~FD_CLOEXEC));
	#elif defined(RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT)
		return SetHandleInformation((HANDLE)_get_osfhandle(pvt->_fd),
							HANDLE_FLAG_INHERIT,
							HANDLE_FLAG_INHERIT);
	#else
		#error no FD_CLOEXEC or anything like it
	#endif
}

bool filedescriptor::getCloseOnExec() {
	#if defined(RUDIMENTS_HAVE_FD_CLOEXEC)
		return fCntl(F_GETFD,FD_CLOEXEC);
	#elif defined(RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT)
		DWORD	inherit;
		if (GetHandleInformation((HANDLE)_get_osfhandle(pvt->_fd),
								&inherit)) {
			return (bool)(inherit&HANDLE_FLAG_INHERIT);
		}
		return false;
	#else
		#error no FD_CLOEXEC or anything like it
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif