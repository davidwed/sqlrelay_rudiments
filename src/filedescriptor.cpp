// Copyright (c) 2002 David Muse
// See the COPYING file for more information

//#define DEBUG_PASSFD 1
//#define DEBUG_WRITE 1
//#define DEBUG_READ 1
//#define DEBUG_BUFFERING 1

#if defined(DEBUG_PASSFD) || defined(DEBUG_WRITE) || \
	defined(DEBUG_READ) || defined(DEBUG_BUFFERING)
#ifdef _MSC_VER
	#define debugPrintf(ARGS,...) if (this!=&stdoutput) { stdoutput.printf(ARGS,__VA_ARGS__); }
#else
	#define debugPrintf(ARGS...) if (this!=&stdoutput) { stdoutput.printf(ARGS); }
#endif
#define debugSafePrint(string) if (this!=&stdoutput) { stdoutput.safePrint(string); }
#endif

#include <rudiments/filedescriptor.h>
#include <rudiments/listener.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/bytestring.h>
#if defined(DEBUG_PASSFD) || defined(DEBUG_WRITE) || \
		defined(DEBUG_READ) || defined(RUDIMENTS_HAVE_DUPLICATEHANDLE)
	#include <rudiments/process.h>
#endif
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAS_SSL
	// Redhat 6.2 needs _GNU_SOURCE
	#ifndef _GNU_SOURCE
		#define _GNU_SOURCE
	#endif
	#include <openssl/ssl.h>
#endif

#ifdef RUDIMENTS_HAVE_IO_H
	#include <io.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	#include <sys/time.h>
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

#include <stdio.h>

// NOTE: These next two headers must be included in this order or LITTLE_ENDIAN
// will be multiply-defined on linux libc4 systems.  Other systems are
// unaffected.
#ifdef RUDIMENTS_HAVE_NETINET_TCP_H
	// some libc5 systems need this extern "C" wrapper
	extern "C" {
	#include <netinet/tcp.h>
	}
#endif
#ifdef RUDIMENTS_HAVE_NETINET_IN_H
	#include <netinet/in.h>
#endif


#ifdef RUDIMENTS_HAVE_SYS_UIO_H
	#include <sys/uio.h>
#endif
#ifdef RUDIMENTS_HAVE_LIMITS_H
	#include <limits.h>
#endif
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
#ifdef RUDIMENTS_HAVE_SYS_BYTEORDER_H
	#include <sys/byteorder.h>
#endif
#ifdef RUDIMENTS_HAVE_OS_SUPPORT_BYTEORDER_H
	#include <os/support/ByteOrder.h>
#endif

// apparently on windows, there are no byte-order macros of any kind
#ifdef _WIN32
	#define __LITTLE_ENDIAN	1234
	#define __BIG_ENDIAN	4321
	#define __BYTE_ORDER	__LITTLE_ENDIAN
#endif

// On solaris (and probably others), BYTE_ORDER is undefined (even with
// underscore prefixes.  Either _BIG_ENDIAN or _LITTLE_ENDIAN is
// defined, but it's just "defined", not set to any value.
#if !defined(__BYTE_ORDER) && \
	!defined(_BYTE_ORDER) && \
	!defined(BYTE_ORDER) && \
	(defined(_BIG_ENDIAN) || defined(_LITTLE_ENDIAN))

	#define __LITTLE_ENDIAN 1234
	#define __BIG_ENDIAN	4321

	#ifdef _LITTLE_ENDIAN
		#define __BYTE_ORDER	__LITTLE_ENDIAN
	#else
		#define __BYTE_ORDER	__BIG_ENDIAN
	#endif
#endif

#ifndef __BYTE_ORDER
	#if defined(BYTE_ORDER)
		#define __BYTE_ORDER BYTE_ORDER
	#elif defined (_BYTE_ORDER)
		#define __BYTE_ORDER _BYTE_ORDER
	#endif
#endif

#ifndef __BIG_ENDIAN
	#if defined(BIG_ENDIAN)
		#define __BIG_ENDIAN BIG_ENDIAN
	#elif defined(_BIG_ENDIAN)
		#define __BIG_ENDIAN _BIG_ENDIAN
	#endif
#endif

// for FD_SET (macro that uses memset) on solaris
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif

#ifdef RUDIMENTS_NEED_XNET_PROTOTYPES
extern ssize_t __xnet_recvmsg (int, struct msghdr *, int);
extern ssize_t __xnet_sendmsg (int, const struct msghdr *, int);
#endif

#ifndef CMSG_ALIGN
	#define CMSG_ALIGN(len)	(((len) + sizeof(size_t)-1) & \
					(size_t)~(sizeof(size_t)-1))
#endif

#ifndef CMSG_LEN
	#define CMSG_LEN(len)	(CMSG_ALIGN(sizeof(struct cmsghdr))+(len))
#endif

// SCO OpenServer < 5.0.7 has an error in the sys/socket.h header.
// Internally, libc supports accrights/accrightslen but the header defines
// the struct as having control/controllen components.  This hack works
// around the problem.
// http://www.linuxmisc.com/9-unix-programmer/af8e2f1e03a2b913.htm
#ifdef RUDIMENTS_HAVE_BAD_SCO_MSGHDR
	#undef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN
	#define msg_accrights msg_control
	#define msg_accrightslen msg_controllen
#endif

// if SSIZE_MAX is undefined, choose a good safe value
// that should even work on 16-bit systems
#ifndef SSIZE_MAX
	#define SSIZE_MAX 32767
#endif

// most platforms FILE struct have a member for the file descriptor,
// try to find it...
// some platforms, like solaris 11.2 hide it altogether
#undef FD
#if defined(RUDIMENTS_HAVE_FILE_FILENO)
	#define FD f->_fileno
#elif defined(RUDIMENTS_HAVE_FILE_FILE)
	#ifdef __VMS
		#define FD ((struct _iobuf *)f)->_file
	#else
		#define FD f->_file
	#endif
#elif defined(RUDIMENTS_HAVE_FILE__FILE)
	#define FD f->__file
#elif defined(RUDIMENTS_HAVE_FILE_FILEDES)
	#define FD f->__filedes
#elif defined(RUDIMENTS_HAVE_FILE__FD)
	#define FD f->_fd
#endif

class filedescriptorprivate {
	friend class filedescriptor;
	private:
		bool	_retryinterruptedreads:1;
		bool	_retryinterruptedwrites:1;
		bool	_retryinterruptedwaits:1;
		bool	_retryinterruptedfcntl:1;
		bool	_retryinterruptedioctl:1;
		bool	_allowshortreads:1;
		bool	_allowshortwrites:1;
		bool	_translatebyteorder:1;

		int32_t	_fd;

		#ifdef RUDIMENTS_HAS_SSL
		SSL_CTX	*_sslctx;
		SSL	*_ssl;
		BIO	*_bio;
		int32_t	_sslresult;
		#endif

		gssapicontext	*_gssapictx;

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

filedescriptor::filedescriptor(int32_t fd) {
	pvt=new filedescriptorprivate;
	filedescriptorInit();
	setFileDescriptor(fd);
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
	setFileDescriptor(-1);
	pvt->_retryinterruptedreads=false;
	pvt->_retryinterruptedwrites=false;
	pvt->_retryinterruptedwaits=true;
	pvt->_retryinterruptedfcntl=true;
	pvt->_retryinterruptedioctl=true;
	pvt->_allowshortreads=false;
	pvt->_allowshortwrites=false;
	pvt->_translatebyteorder=false;
#ifdef RUDIMENTS_HAS_SSL
	pvt->_sslctx=NULL;
	pvt->_bio=NULL;
	pvt->_ssl=NULL;
	pvt->_sslresult=1;
#endif
	pvt->_gssapictx=NULL;
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
	setFileDescriptor(f.pvt->_fd);
	pvt->_translatebyteorder=f.pvt->_translatebyteorder;
	pvt->_retryinterruptedreads=f.pvt->_retryinterruptedreads;
	pvt->_retryinterruptedwrites=f.pvt->_retryinterruptedwrites;
	pvt->_retryinterruptedwaits=f.pvt->_retryinterruptedwaits;
	pvt->_retryinterruptedfcntl=f.pvt->_retryinterruptedfcntl;
	pvt->_retryinterruptedioctl=f.pvt->_retryinterruptedioctl;
	pvt->_allowshortreads=f.pvt->_allowshortreads;
	pvt->_allowshortwrites=f.pvt->_allowshortwrites;
#ifdef RUDIMENTS_HAS_SSL
	pvt->_sslctx=f.pvt->_sslctx;
	pvt->_bio=f.pvt->_bio;
	pvt->_ssl=f.pvt->_ssl;
	pvt->_sslresult=f.pvt->_sslresult;
#endif
	pvt->_gssapictx=f.pvt->_gssapictx;
	if (f.pvt->_writebuffer) {
		ssize_t	writebuffersize=f.pvt->_writebufferend-
						f.pvt->_writebuffer;
		pvt->_writebuffer=new unsigned char[writebuffersize];
		bytestring::copy(pvt->_writebuffer,
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

	// FIXME: this should be passed down into the gssapicontext
	// so it knows how to chunk writes

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
		debugPrintf("setting write buffer size to %d\n",size);
	#endif

	if (size<0) {
		#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
			debugPrintf("invalid size: %d\n",size);
		#endif
		return false;
	}

	delete[] pvt->_writebuffer;
	pvt->_writebuffer=(size)?new unsigned char[size]:NULL;
	pvt->_writebufferend=pvt->_writebuffer+size;
	pvt->_writebufferptr=pvt->_writebuffer;

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
		debugPrintf("done setting write buffer size\n");
	#endif
	return true;
}

bool filedescriptor::setReadBufferSize(ssize_t size) const {

	// gssapicontext does it's own read buffering, which interferes
	// with buffering at this level, so ignore it if we're using
	// gssapi and request buffering
	// FIXME: this should actually be passed down into the gssapicontext
	// so it knows when to return a short-read
	if (size && pvt->_gssapictx) {
		return true;
	}


	#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
		debugPrintf("setting read buffer size to %d\n",size);
	#endif

	if (size<0) {
		#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("invalid size: %d\n",size);
		#endif
		return false;
	}
	delete[] pvt->_readbuffer;
	pvt->_readbuffer=(size)?new unsigned char[size]:NULL;
	pvt->_readbufferend=pvt->_readbuffer+size;
	pvt->_readbufferhead=pvt->_readbuffer;
	pvt->_readbuffertail=pvt->_readbuffer;

	#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
		debugPrintf("done setting read buffer size\n");
	#endif
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
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE__DUP)
			result=_dup(pvt->_fd);
		#elif defined(RUDIMENTS_HAVE_DUP)
			result=dup(pvt->_fd);
		#else
			#error no dup or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

bool filedescriptor::duplicate(int32_t newfd) const {
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE__DUP2)
			result=_dup2(pvt->_fd,newfd);
		#elif defined(RUDIMENTS_HAVE_DUP2)
			result=dup2(pvt->_fd,newfd);
		#else
			#error no dup2 or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return (result==newfd);
}

bool filedescriptor::supportsSSL() {
#ifdef RUDIMENTS_HAS_SSL
	return true;
#else
	return false;
#endif
}

void filedescriptor::setSSLContext(void *ctx) {
#ifdef RUDIMENTS_HAS_SSL
	if (!ctx) {
		deInitializeSSL();
	}
	pvt->_sslctx=(SSL_CTX *)ctx;
#endif
}

void *filedescriptor::getSSLContext() {
#ifdef RUDIMENTS_HAS_SSL
	return (void *)pvt->_sslctx;
#else
	return NULL;
#endif
}

bool filedescriptor::initializeSSL() {
#ifdef RUDIMENTS_HAS_SSL
	if (pvt->_fd==-1) {
		return false;
	}
	deInitializeSSL();
	if (pvt->_sslctx) {
		pvt->_bio=(BIO *)newSSLBIO();
		pvt->_ssl=SSL_new(pvt->_sslctx);
		SSL_set_bio(pvt->_ssl,pvt->_bio,pvt->_bio);
	}
	return true;
#else
	return false;
#endif
}

void filedescriptor::deInitializeSSL() {
#ifdef RUDIMENTS_HAS_SSL
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
#endif
}

void *filedescriptor::getSSL() const {
#ifdef RUDIMENTS_HAS_SSL
	return (void *)pvt->_ssl;
#else
	return NULL;
#endif
}

void *filedescriptor::newSSLBIO() const {
#ifdef RUDIMENTS_HAS_SSL
	return (void *)BIO_new_fd(pvt->_fd,BIO_NOCLOSE);
#else
	return NULL;
#endif
}

bool filedescriptor::supportsGSSAPI() {
#ifdef RUDIMENTS_HAS_KRB5
	return true;
#else
	return false;
#endif
}

void filedescriptor::setGSSAPIContext(gssapicontext *ctx) {
	pvt->_gssapictx=ctx;

	// gssapicontext does it's own read buffering, which interferes
	// with buffering at this level, so disable read buffering if
	// we're using gssapi.
	//
	// Note: this is a little kludgy, as the app won't necessarily know
	// to re-enable read buffering if it disables gssapi.
	if (ctx) {
		setReadBufferSize(0);
	}
}

gssapicontext *filedescriptor::getGSSAPIContext() {
	return pvt->_gssapictx;
}

bool filedescriptor::supportsBlockingNonBlockingModes() {
	#if defined(RUDIMENTS_HAVE_FCNTL) && \
		defined(F_SETFL) && defined (F_GETFL)
		return true;
	#else
		return false;
	#endif
}

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
	return bufferedWrite(&number,sizeof(int16_t),-1,-1);
}

ssize_t filedescriptor::write(int32_t number) const {
	return bufferedWrite(&number,sizeof(int32_t),-1,-1);
}

ssize_t filedescriptor::write(int64_t number) const {
	return bufferedWrite(&number,sizeof(int64_t),-1,-1);
}

ssize_t filedescriptor::write(float number) const {
	return bufferedWrite(&number,sizeof(float),-1,-1);
}

ssize_t filedescriptor::write(double number) const {
	return bufferedWrite(&number,sizeof(double),-1,-1);
}

ssize_t filedescriptor::write(unsigned char character) const {
	return bufferedWrite(&character,sizeof(unsigned char),-1,-1);
}

ssize_t filedescriptor::write(bool value) const {
	return bufferedWrite(&value,sizeof(bool),-1,-1);
}

ssize_t filedescriptor::write(char character) const {
	return bufferedWrite(&character,sizeof(char),-1,-1);
}

ssize_t filedescriptor::write(const unsigned char *string, size_t size) const {
	return bufferedWrite(string,size,-1,-1);
}

ssize_t filedescriptor::write(const char *string, size_t size) const {
	return bufferedWrite(string,size,-1,-1);
}

ssize_t filedescriptor::write(const unsigned char *string) const {
	return bufferedWrite(string,charstring::length(string),-1,-1);
}

ssize_t filedescriptor::write(const char *string) const {
	return bufferedWrite(string,charstring::length(string),-1,-1);
}

ssize_t filedescriptor::write(const void *buffer, size_t size) const {
	return bufferedWrite(buffer,size,-1,-1);
}

ssize_t filedescriptor::write(uint16_t number,
				int32_t sec, int32_t usec) const {
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return bufferedWrite(&number,sizeof(uint16_t),sec,usec);
}

ssize_t filedescriptor::write(uint32_t number,
				int32_t sec, int32_t usec) const {
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return bufferedWrite(&number,sizeof(uint32_t),sec,usec);
}

ssize_t filedescriptor::write(uint64_t number,
				int32_t sec, int32_t usec) const {
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return bufferedWrite(&number,sizeof(uint64_t),sec,usec);
}

ssize_t filedescriptor::write(int16_t number,
				int32_t sec, int32_t usec) const {
	return bufferedWrite(&number,sizeof(int16_t),sec,usec);
}

ssize_t filedescriptor::write(int32_t number,
				int32_t sec, int32_t usec) const {
	return bufferedWrite(&number,sizeof(int32_t),sec,usec);
}

ssize_t filedescriptor::write(int64_t number,
				int32_t sec, int32_t usec) const {
	return bufferedWrite(&number,sizeof(int64_t),sec,usec);
}

ssize_t filedescriptor::write(float number,
				int32_t sec,int32_t usec) const {
	return bufferedWrite(&number,sizeof(float),sec,usec);
}

ssize_t filedescriptor::write(double number,
				int32_t sec, int32_t usec) const {
	return bufferedWrite(&number,sizeof(double),sec,usec);
}

ssize_t filedescriptor::write(unsigned char character,
				int32_t sec, int32_t usec) const {
	return bufferedWrite(&character,sizeof(unsigned char),sec,usec);
}

ssize_t filedescriptor::write(bool value,
				int32_t sec, int32_t usec) const {
	return bufferedWrite(&value,sizeof(bool),sec,usec);
}

ssize_t filedescriptor::write(char character,
				int32_t sec, int32_t usec) const {
	return bufferedWrite(&character,sizeof(char),sec,usec);
}

ssize_t filedescriptor::write(const unsigned char *string, size_t size,
					int32_t sec, int32_t usec) const {
	return bufferedWrite(string,size,sec,usec);
}

ssize_t filedescriptor::write(const char *string, size_t size,
					int32_t sec, int32_t usec) const {
	return bufferedWrite(string,size,sec,usec);
}

ssize_t filedescriptor::write(const unsigned char *string,
					int32_t sec, int32_t usec) const {
	return bufferedWrite(string,charstring::length(string),sec,usec);
}

ssize_t filedescriptor::write(const char *string,
					int32_t sec, int32_t usec) const {
	return bufferedWrite(string,charstring::length(string),sec,usec);
}

ssize_t filedescriptor::write(const void *buffer, size_t size,
					int32_t sec, int32_t usec) const {
	return bufferedWrite(buffer,size,sec,usec);
}

ssize_t filedescriptor::read(uint16_t *buffer) {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(uint32_t *buffer) {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(uint64_t *buffer) {
	return read(buffer,-1,-1);
}

ssize_t filedescriptor::read(int16_t *buffer) {
	return bufferedRead(buffer,sizeof(int16_t),-1,-1);
}

ssize_t filedescriptor::read(int32_t *buffer) {
	return bufferedRead(buffer,sizeof(int32_t),-1,-1);
}

ssize_t filedescriptor::read(int64_t *buffer) {
	return bufferedRead(buffer,sizeof(int64_t),-1,-1);
}

ssize_t filedescriptor::read(float *buffer) {
	return bufferedRead(buffer,sizeof(float),-1,-1);
}

ssize_t filedescriptor::read(double *buffer) {
	return bufferedRead(buffer,sizeof(double),-1,-1);
}

ssize_t filedescriptor::read(unsigned char *buffer) {
	return bufferedRead(buffer,sizeof(unsigned char),-1,-1);
}

ssize_t filedescriptor::read(bool *buffer) {
	return bufferedRead(buffer,sizeof(bool),-1,-1);
}

ssize_t filedescriptor::read(char *buffer) {
	return bufferedRead(buffer,sizeof(char),-1,-1);
}

ssize_t filedescriptor::read(unsigned char *buffer, size_t size) {
	return bufferedRead(buffer,size,-1,-1);
}

ssize_t filedescriptor::read(char *buffer, size_t size) {
	return bufferedRead(buffer,size,-1,-1);
}

ssize_t filedescriptor::read(void *buffer, size_t size) {
	return bufferedRead(buffer,size,-1,-1);
}

ssize_t filedescriptor::read(char **buffer, const char *terminator) {
	return read(buffer,terminator,0,'\0',-1,-1);
}

ssize_t filedescriptor::read(char **buffer,
				const char *terminator, size_t maxbytes) {
	return read(buffer,terminator,maxbytes,'\0',-1,-1);
}

ssize_t filedescriptor::read(char **buffer, const char *terminator,
						int32_t sec, int32_t usec) {
	return read(buffer,terminator,0,'\0',sec,usec);
}

ssize_t filedescriptor::read(uint16_t *buffer,
				int32_t sec, int32_t usec) {
	ssize_t	retval=bufferedRead(buffer,sizeof(uint16_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(uint32_t *buffer,
				int32_t sec, int32_t usec) {
	ssize_t	retval=bufferedRead(buffer,sizeof(uint32_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(uint64_t *buffer,
				int32_t sec, int32_t usec) {
	ssize_t	retval=bufferedRead(buffer,sizeof(uint64_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(int16_t *buffer,
				int32_t sec, int32_t usec) {
	return bufferedRead(buffer,sizeof(int16_t),sec,usec);
}

ssize_t filedescriptor::read(int32_t *buffer,
				int32_t sec, int32_t usec) {
	return bufferedRead(buffer,sizeof(int32_t),sec,usec);
}

ssize_t filedescriptor::read(int64_t *buffer,
				int32_t sec, int32_t usec) {
	return bufferedRead(buffer,sizeof(int64_t),sec,usec);
}

ssize_t filedescriptor::read(float *buffer,
				int32_t sec, int32_t usec) {
	return bufferedRead(buffer,sizeof(float),sec,usec);
}

ssize_t filedescriptor::read(double *buffer,
				int32_t sec, int32_t usec) {
	return bufferedRead(buffer,sizeof(double),sec,usec);
}

ssize_t filedescriptor::read(unsigned char *buffer,
				int32_t sec, int32_t usec) {
	return bufferedRead(buffer,sizeof(unsigned char),sec,usec);
}

ssize_t filedescriptor::read(bool *buffer,
				int32_t sec, int32_t usec) {
	return bufferedRead(buffer,sizeof(bool),sec,usec);
}

ssize_t filedescriptor::read(char *buffer,
				int32_t sec, int32_t usec) {
	return bufferedRead(buffer,sizeof(char),sec,usec);
}

ssize_t filedescriptor::read(unsigned char *buffer, size_t size,
					int32_t sec, int32_t usec) {
	return bufferedRead(buffer,size,sec,usec);
}

ssize_t filedescriptor::read(char *buffer, size_t size,
					int32_t sec, int32_t usec) {
	return bufferedRead(buffer,size,sec,usec);
}

ssize_t filedescriptor::read(void *buffer, size_t size,
					int32_t sec, int32_t usec) {
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
		error::clearError();
		do {
			result=lowLevelClose();
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (result==-1) {
			return false;
		}
		setFileDescriptor(-1);
	}
	return true;
}

int32_t filedescriptor::lowLevelClose() {
	#if defined(RUDIMENTS_HAVE__CLOSE)
		return _close(pvt->_fd);
	#elif defined(RUDIMENTS_HAVE_CLOSE)
		return ::close(pvt->_fd);
	#else
		#error no close or anything like it
	#endif
}

void filedescriptor::retryInterruptedReads() {
	pvt->_retryinterruptedreads=true;
}

void filedescriptor::dontRetryInterruptedReads() {
	pvt->_retryinterruptedreads=false;
}

bool filedescriptor::getRetryInterruptedReads() const {
	return pvt->_retryinterruptedreads;
}

void filedescriptor::retryInterruptedWrites() {
	pvt->_retryinterruptedwrites=true;
}

void filedescriptor::dontRetryInterruptedWrites() {
	pvt->_retryinterruptedwrites=false;
}

bool filedescriptor::getRetryInterruptedWrites() const {
	return pvt->_retryinterruptedwrites;
}

void filedescriptor::retryInterruptedWaits() {
	pvt->_retryinterruptedwaits=true;
}

void filedescriptor::dontRetryInterruptedWaits() {
	pvt->_retryinterruptedwaits=false;
}

bool filedescriptor::getRetryInterruptedWaits() const {
	return pvt->_retryinterruptedwaits;
}

void filedescriptor::retryInterruptedFcntl() {
	pvt->_retryinterruptedfcntl=true;
}

void filedescriptor::dontRetryInterruptedFcntl() {
	pvt->_retryinterruptedfcntl=true;
}

bool filedescriptor::getRetryInterruptedFcntl() const {
	return pvt->_retryinterruptedfcntl;
}

void filedescriptor::retryInterruptedIoctl() {
	pvt->_retryinterruptedioctl=true;
}

void filedescriptor::dontRetryInterruptedIoctl() {
	pvt->_retryinterruptedioctl=true;
}

bool filedescriptor::getRetryInterruptedIoctl() const {
	return pvt->_retryinterruptedioctl;
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

ssize_t filedescriptor::read(char **buffer, const char *terminator,
				size_t maxbytes, int32_t sec, int32_t usec) {
	return read(buffer,terminator,maxbytes,'\0',sec,usec);
}

ssize_t filedescriptor::read(char **buffer, const char *terminator,
				size_t maxbytes, char escapechar,
				int32_t sec, int32_t usec) {

	// initialize the return buffer
	if (buffer) {
		*buffer=NULL;
	}

	// initialize a temp buffer
	stringbuffer	temp;

	// initialize termination detector
	int32_t	termlen=charstring::length(terminator);
	char	*term=new char[termlen];
	bytestring::zero(term,termlen);

	// initialize some variables
	int32_t	sizeread;
	char	charbuffer;
	bool	escaped=false;
	bool	copytobuffer;
	bool	copytoterm;
	ssize_t	retval=RESULT_SUCCESS;

	// loop, getting 1 character at a time
	for (;;) {

		// read from the file descriptor
		sizeread=read(&charbuffer,sec,usec);
		if (sizeread<=0) {
			retval=sizeread;
			break;
		}

		// handle escaping
		if (escaped) {
			copytobuffer=true;
			copytoterm=false;
			escaped=false;
		} else {
			escaped=((escapechar!='\0')?
					(charbuffer==escapechar):false);
			copytobuffer=!escaped;
			copytoterm=!escaped;
		}

		// copy to return buffer
		if (copytobuffer && buffer) {
			temp.append(charbuffer);
		}

		if (copytoterm) {

			// update terminator detector
			for (int32_t i=0; i<termlen-1; i++) {
				term[i]=term[i+1];
			}
			term[termlen-1]=charbuffer;

			// check for termination
			if (!charstring::compare(term,terminator,termlen)) {
				break;
			}

		} else {

			// clear terminator
			bytestring::zero(term,termlen);
		}

		// max-bytes-read condition
		if (maxbytes && temp.getStringLength()>maxbytes) {
			retval=RESULT_MAX;
			break;
		}
	}

	if (retval>=RESULT_SUCCESS) {

		// get the size to return
		retval=temp.getStringLength();

		// set the return buffer
		if (buffer) {
			*buffer=temp.detachString();
		}
	}

	// clean up
	delete[] term;

	return retval;
}

ssize_t filedescriptor::bufferedRead(void *buf, ssize_t count,
					int32_t sec, int32_t usec) {

	#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
	debugPrintf("bufferedRead of %d bytes\n",(int)count);
	#endif

	if (!count) {
		return 0;
	}

	if (!pvt->_readbuffer) {
		#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
		debugPrintf("no read buffer...\n");
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

			#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("%d bytes in read buffer\n",
						(int)bytesavailabletocopy);
			#endif

			ssize_t	bytestocopy=(bytesavailabletocopy<bytesunread)?
						bytesavailabletocopy:
						bytesunread;

			#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("copying %d bytes "
						"out of read buffer\n",
						(int)bytestocopy);
			#endif

			bytestring::copy(data,pvt->_readbufferhead,bytestocopy);
			data=data+bytestocopy;
			bytesread=bytesread+bytestocopy;
			pvt->_readbufferhead=pvt->_readbufferhead+bytestocopy;
			bytesunread=bytesunread-bytestocopy;

			// if we've read enough, break out
			if (bytesread==count) {
				#if defined(DEBUG_READ) && \
					 defined(DEBUG_BUFFERING)
				debugPrintf("yay, we're done reading\n");
				#endif
				return bytesread;
			}

			#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("need to read %d more bytes\n",
							(int)bytesunread);
			#endif
		}

		// if we've copied out everything in the buffer, read some more
		if (pvt->_readbufferhead==pvt->_readbuffertail) {

			#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("attempting to fill read buffer, ");
			debugPrintf("reading %d bytes...\n",
						(int)(pvt->_readbufferend-
							pvt->_readbuffer));
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
					#if defined(DEBUG_READ) && \
						defined(DEBUG_BUFFERING)
					debugPrintf("EOF\n");
					#endif
					return bytesread;
				}

				#if defined(DEBUG_READ) && \
					defined(DEBUG_BUFFERING)
				debugPrintf("still need %d bytes, "
							"reading...\n",
							(int)bytesunread);
				#endif
				result=safeRead(pvt->_readbuffer,
						bytesunread,
						sec,usec);

				if (result>-1 && result!=bytesunread) {
					#if defined(DEBUG_READ) && \
						defined(DEBUG_BUFFERING)
					debugPrintf("EOF\n");
					#endif
					return bytesread;
				}
			}

			if (result<0) {
				#if defined(DEBUG_READ) && \
					defined(DEBUG_BUFFERING)
				debugPrintf("error reading...\n");
				#endif
				return result;
			}

			pvt->_readbufferhead=pvt->_readbuffer;
			pvt->_readbuffertail=pvt->_readbuffer+result;

			#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("read %d bytes\n",(int)result);
			#endif
		}
	}
}

ssize_t filedescriptor::safeRead(void *buf, ssize_t count,
					int32_t sec, int32_t usec) {

	// FIXME: is this what we want to do?
	// maybe we should set some kind of error condition too
	if (!buf) {
		return 0;
	}

	#ifdef DEBUG_READ
	debugPrintf("%d: safeRead(%d,(attempting %d bytes)",
			(int)process::getProcessId(),(int)pvt->_fd,(int)count);
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
	bool	isusingnonblockingmode=isUsingNonBlockingMode();
	while (totalread<count) {

		// only read SSIZE_MAX at a time
		sizetoread=count-totalread;
		if (sizetoread>SSIZE_MAX) {
			sizetoread=SSIZE_MAX;
		}

		// wait if necessary
		if (sec>-1 && usec>-1) {

			int32_t	waitresult=waitForNonBlockingRead(sec,usec);

			// return error or timeout
			if (waitresult<0) {
				#ifdef DEBUG_READ
				debugPrintf(")\n");
				#endif
				return waitresult;
			}
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

			#ifdef DEBUG_READ
		        debugPrintf(" (SSL) ");
			#endif

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
		} else
		#endif
		if (pvt->_gssapictx) {

			#ifdef DEBUG_READ
		        debugPrintf(" (GSSAPI) ");
			#endif

			actualread=pvt->_gssapictx->read(ptr,sizetoread);
		} else {
			actualread=lowLevelRead(ptr,sizetoread);
		}

		#ifdef DEBUG_READ
		for (int32_t i=0; i<actualread; i++) {
			debugSafePrint(
				(static_cast<unsigned char *>(ptr))[i]);
		}
		debugPrintf("(%ld bytes) ",(long)actualread);
		if (actualread==-1) {
			debugPrintf("%s ",error::getErrorString());
		}
		stdoutput.flush();
		#endif

		// if we didn't read the number of bytes we expected to,
		// handle that...
		if (actualread!=sizetoread) {
			if (isusingnonblockingmode &&
				error::getErrorNumber()==EAGAIN) {
				#ifdef DEBUG_READ
				debugPrintf(" EAGAIN ");
				#endif
				// if we got an EAGAIN, then presumably we're
				// in non-blocking mode, there was nothing
				// to read and we're done
				break;
			} else if (error::getErrorNumber()==EINTR) {
				#ifdef DEBUG_READ
				debugPrintf(" EINTR ");
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
				debugPrintf(" EOF ");
				#endif
				break;
			} else if (actualread==-1) {
				// error condition
				#ifdef DEBUG_READ
				debugPrintf(")\n");
				#endif
				return RESULT_ERROR;
			}
		}

		totalread=totalread+actualread;

		// if we want to allow short reads, then break out here
		if (pvt->_allowshortreads) {
			#ifdef DEBUG_READ
			debugPrintf(" SHORTREAD ");
			#endif
			break;
		}
	}

	#ifdef DEBUG_READ
	debugPrintf(",%d)\n",(int)totalread);
	#endif
	return totalread;
}

ssize_t filedescriptor::lowLevelRead(void *buf, ssize_t count) {
	#if defined(RUDIMENTS_HAVE__READ)
		return _read(pvt->_fd,buf,count);
	#elif defined(RUDIMENTS_HAVE_READ)
		return ::read(pvt->_fd,buf,count);
	#else
		#error no read or anything like it
	#endif
}

ssize_t filedescriptor::bufferedWrite(const void *buf, ssize_t count,
					int32_t sec, int32_t usec) const {
	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf("bufferedWrite of %d bytes\n",(int)count);
	#endif

	if (!count) {
		return 0;
	}

	if (!pvt->_writebuffer) {
		#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
		debugPrintf("no write buffer...\n");
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

		#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
		debugPrintf("	writebuffersize=%d\n",
						(int)writebuffersize);
		debugPrintf("	writebufferspace=%d\n",
						(int)writebufferspace);
		debugPrintf("	byteswritten=%d\n",
						(int)byteswritten);
		debugPrintf("	bytesunwritten=%d\n",
						(int)bytesunwritten);
		#endif

		if (bytesunwritten<=writebufferspace) {

			#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
			debugPrintf("buffering %d bytes\n",
						(int)bytesunwritten);
			#endif

			bytestring::copy(pvt->_writebufferptr,
					data,bytesunwritten);
			pvt->_writebufferptr=pvt->_writebufferptr+
							bytesunwritten;
			byteswritten=byteswritten+bytesunwritten;

		} else {

			#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
			debugPrintf("just buffering %d bytes\n",
						(int)writebufferspace);
			#endif
			#if defined(DEBUG_BUFFERING)
			debugPrintf("auto-flush write buffer: %d bytes\n",
				(int)(writebuffersize+writebufferspace));
			#endif

			bytestring::copy(pvt->_writebufferptr,
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

bool filedescriptor::flushWriteBuffer(int32_t sec, int32_t usec) const {
	if (!pvt->_writebuffer) {
		return true;
	}
	ssize_t	writebuffersize=pvt->_writebufferptr-pvt->_writebuffer;
	#if defined(DEBUG_BUFFERING)
		debugPrintf("flush write buffer: %d bytes\n",
						(int)writebuffersize);
	#endif
	bool	retval=(safeWrite(pvt->_writebuffer,writebuffersize,
						sec,usec)==writebuffersize);
	pvt->_writebufferptr=pvt->_writebuffer;
	return retval;
}

ssize_t filedescriptor::safeWrite(const void *buf, ssize_t count,
					int32_t sec, int32_t usec) const {

	// FIXME: is this what we want to do?
	// maybe we should set some kind of error condition too
	if (!buf) {
		return 0;
	}

	#ifdef DEBUG_WRITE
	debugPrintf("%d: safeWrite(%d,",
			(int)process::getProcessId(),(int)pvt->_fd);
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

		// wait if necessary
		if (sec>-1 && usec>-1) {

			int32_t	waitresult=waitForNonBlockingWrite(sec,usec);

			// return error or timeout
			if (waitresult<0) {
				#ifdef DEBUG_WRITE
				debugPrintf(")\n");
				#endif
				return waitresult;
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

			#ifdef DEBUG_WRITE
		        debugPrintf(" (SSL) ");
			#endif
			
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
		} else
		#endif
		if (pvt->_gssapictx) {

			#ifdef DEBUG_WRITE
		        debugPrintf(" (GSSAPI) ");
			#endif

			actualwrite=pvt->_gssapictx->write(ptr,sizetowrite);
		} else {
			actualwrite=lowLevelWrite(ptr,sizetowrite);
		}

		#ifdef DEBUG_WRITE
		for (int32_t i=0; i<actualwrite; i++) {
			debugSafePrint(
			(static_cast<const unsigned char *>(ptr))[i]);
		}
		debugPrintf("(%ld bytes) ",(long)actualwrite);
		if (actualwrite==-1) {
			debugPrintf("%s ",error::getErrorString());
		}
		stdoutput.flush();
		#endif

		// if we didn't read the number of bytes we expected to,
		// handle that...
		if (actualwrite!=sizetowrite) {
			if (error::getErrorNumber()==EINTR) {
				#ifdef DEBUG_WRITE
				debugPrintf(" EINTR ");
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
				debugPrintf(" EOF ");
				#endif
				break;
			} else if (actualwrite==-1) {
				// error condition
				#ifdef DEBUG_WRITE
				debugPrintf(")\n");
				#endif
				return RESULT_ERROR;
			}
		}

		totalwrite=totalwrite+actualwrite;

		// if we want to allow short writes, then break out here
		if (pvt->_allowshortwrites) {
			#ifdef DEBUG_WRITE
			debugPrintf(" SHORTWRITE ");
			#endif
			break;
		}
	}

	#ifdef DEBUG_WRITE
	debugPrintf(",%d)\n",(int)totalwrite);
	#endif
	return totalwrite;
}

ssize_t filedescriptor::lowLevelWrite(const void *buf, ssize_t count) const {
	#if defined(RUDIMENTS_HAVE__WRITE)
		return _write(pvt->_fd,buf,count);
	#elif defined(RUDIMENTS_HAVE_WRITE)
		return ::write(pvt->_fd,buf,count);
	#else
		#error no write or anything like it
	#endif
}

int32_t filedescriptor::waitForNonBlockingRead(
				int32_t sec, int32_t usec) const {
	listener	lstnr;
	lstnr.addReadFileDescriptor((filedescriptor *)this);
	return lstnr.listen(sec,usec);
}

int32_t filedescriptor::waitForNonBlockingWrite(
				int32_t sec, int32_t usec) const {
	listener	lstnr;
	lstnr.addWriteFileDescriptor((filedescriptor *)this);
	return lstnr.listen(sec,usec);
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
	#if defined(RUDIMENTS_HAVE_HTONLL)
		return htonll(value);
	#elif __BYTE_ORDER == __BIG_ENDIAN
		return value;
	#elif defined(RUDIMENTS_HAVE_BSWAP_64)
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
		#ifdef RUDIMENTS_HAVE_LONG_LONG
			return
			(((uint64_t)hostToNet(
				(uint32_t)(value&0x00000000FFFFFFFFLL)))<<32)|
			((uint64_t)hostToNet(
				(uint32_t)((value&0xFFFFFFFF00000000LL)>>32)));
		#else
			return htonl(value);
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
	#if defined(RUDIMENTS_HAVE_NTOHLL)
		return ntohll(value);
	#elif __BYTE_ORDER == __BIG_ENDIAN
		return value;
	#elif defined(RUDIMENTS_HAVE_BSWAP_64)
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
		#ifdef RUDIMENTS_HAVE_LONG_LONG
			return
			(((uint64_t)netToHost(
				(uint32_t)(value&0x00000000FFFFFFFFLL)))<<32)|
			((uint64_t)netToHost(
				(uint32_t)((value&0xFFFFFFFF00000000LL)>>32)));
		#else
			return ntohl(value);
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
		error::clearError();
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
		error::clearError();
		do {
			result=ioctl(pvt->_fd,cmd,arg);
		} while (pvt->_retryinterruptedioctl && result==-1 &&
					error::getErrorNumber()==EINTR);
		return result;
	#else
		return -1;
	#endif
}

bool filedescriptor::passFileDescriptor(int32_t fd) {

#if (defined(RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN) && \
		defined(RUDIMENTS_HAVE_CMSGHDR)) || \
		defined(RUDIMENTS_HAVE_MSGHDR_MSG_ACCRIGHTS)

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

		// new-style: the descriptor is passed in the msg_control...

		// On OS X 10.7, CMSG_LEN ultimately makes a functon call, so
		// this array must be dynamically allocated.
		unsigned char	*control=new unsigned char[
						CMSG_LEN(sizeof(int32_t))];
		messageheader.msg_control=(caddr_t)control;
		messageheader.msg_controllen=CMSG_LEN(sizeof(int32_t));

		struct cmsghdr	*cmptr=CMSG_FIRSTHDR(&messageheader);
		cmptr->cmsg_level=SOL_SOCKET;
		cmptr->cmsg_type=SCM_RIGHTS;
		cmptr->cmsg_len=CMSG_LEN(sizeof(int32_t));

		// Why this weirdness?
		//
		// You'd think that this would be simpler:
		// 	*((int32_t *)CMSG_DATA(cmptr))=fd;
		// and should work as CMSG_DATA(cmptr) just points to a generic
		// buffer.
		//
		// The data type of that buffer varies from system to system
		// though and on some systems, some versions of g++ will throw
		// a warning like: "dereferencing type-punned pointer will break
		// strict-aliasing rules" if you cast and then assign directly
		// in one statement, presumably because it can't tell that it's
		// a generic buffer, or how big it is, and worries that you
		// might overrun it.
		//
		// What we want to do is safe though, and for some reason the
		// compiler doesn't mind if you split it up like this, so we
		// will.
		int32_t	*data=(int32_t *)CMSG_DATA(cmptr);
		*data=fd;
	#else
		// old-style: the descriptor is passed in the accrights...
		messageheader.msg_accrights=(caddr_t)&fd;
		messageheader.msg_accrightslen=sizeof(int32_t);
	#endif

	// finally, send the msghdr
	int32_t	result;
	error::clearError();
	do {
		result=sendmsg(pvt->_fd,&messageheader,0);
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					pvt->_retryinterruptedwrites);

	// clean up
	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN
		delete[] control;
	#endif

	return (result!=-1);

#elif defined(RUDIMENTS_HAVE_DUPLICATEHANDLE)

	// get a handle to this process
	// Apparently we can't just use GetCurrentProcess() or
	// we'll just get the pseudo-handle for our process.
	HANDLE	localprocesshandle=OpenProcess(PROCESS_DUP_HANDLE,FALSE,
						(DWORD)GetCurrentProcessId());
	if (!localprocesshandle) {
		return false;
	}

	// tell the other process to go
	if (write(true)!=sizeof(bool)) {
		return false;
	}

	// read the process id from the other side
	uint32_t	otherpid;
	if (read(&otherpid)!=sizeof(uint32_t)) {
		return false;
	}

	// get a handle to that process
	bool	success=true;
	HANDLE	otherprocesshandle=OpenProcess(PROCESS_DUP_HANDLE,
						FALSE,(DWORD)otherpid);
	if (!otherprocesshandle) {
		success=false;
	}

	// get the handle from the fd
	HANDLE	localhandle=INVALID_HANDLE_VALUE;
	if (success) {
		localhandle=(HANDLE)getHandleFromFileDescriptor(fd);
		success=(localhandle!=INVALID_HANDLE_VALUE);
	}

	// duplicate the handle
	HANDLE	otherhandle=INVALID_HANDLE_VALUE;
	if (success) {
		success=(DuplicateHandle(localprocesshandle,
						localhandle,
						otherprocesshandle,
						&otherhandle,
						0,TRUE,
						DUPLICATE_SAME_ACCESS)==TRUE);
	}

	// send otherhandle to other process
	bool	retval=(write((uint64_t)otherhandle)==sizeof(uint64_t));

	// close the other process handle
	CloseHandle(otherprocesshandle);

	// send done flag
	return retval;
#else
	RUDIMENTS_SET_ENOSYS
	return false;
#endif
}

bool filedescriptor::receiveFileDescriptor(int32_t *fd) {

#if (defined(RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN) && \
		defined(RUDIMENTS_HAVE_CMSGHDR)) || \
		defined(RUDIMENTS_HAVE_MSGHDR_MSG_ACCRIGHTS)

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
		// new-style: the descriptor is passed in the msg_control...

		// On OS X 10.7, CMSG_LEN ultimately makes a functon call, so
		// this array must be dynamically allocated.
		unsigned char	*control=new unsigned char[
						CMSG_LEN(sizeof(int32_t))];
		messageheader.msg_control=(caddr_t)control;
		messageheader.msg_controllen=CMSG_LEN(sizeof(int32_t));
	#else
		// old-style: the descriptor is received in the accrights...
		int32_t	newfd;
		messageheader.msg_accrights=(caddr_t)&newfd;
		messageheader.msg_accrightslen=sizeof(int32_t);
	#endif

	// receive the msghdr
	int32_t	result;
	error::clearError();
	do {
		// wait 120 seconds for data to come in
		// FIXME: this should be configurable
		bool	oldwaits=pvt->_retryinterruptedwaits;
		pvt->_retryinterruptedwaits=pvt->_retryinterruptedreads;
		result=waitForNonBlockingRead(120,0);
		pvt->_retryinterruptedwaits=oldwaits;
		if (result==RESULT_TIMEOUT) {
			#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN
				delete[] control;
			#endif
			return false;
		}
		if (result>-1) {
			result=recvmsg(pvt->_fd,&messageheader,0);
		}
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					pvt->_retryinterruptedreads);
	if (result==-1) {
		#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN
			delete[] control;
		#endif
		return false;
	}


	// if we got valid data, set the passed-in descriptor to the 
	// descriptor we received and return success
	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN

		struct cmsghdr  *cmptr=CMSG_FIRSTHDR(&messageheader);
		if (cmptr && cmptr->cmsg_len==CMSG_LEN(sizeof(int32_t)) &&
				cmptr->cmsg_level==SOL_SOCKET &&
				cmptr->cmsg_type==SCM_RIGHTS) {

			// Why this weirdness?
			//
			// You'd think that this would be simpler:
			// 	*fd=*((int32_t *)CMSG_DATA(cmptr));
			// and should work as CMSG_DATA(cmptr) just points to a
			// generic buffer.
			//
			// The data type of that buffer varies from system to
			// system though and on some systems, some versions of
			// g++ will throw a warning like: "dereferencing
			// type-punned pointer will break strict-aliasing rules"
			// if you cast and then dereference directly in one
			// statement, presumably because it can't tell that
			// it's a generic buffer, or how big it is, and worries
			// that you might address past the end of it.
			//
			// What we want to do is safe though, and for some
			// reason the compiler doesn't mind if you split it up
			// like this, so we will.
			int32_t	*data=(int32_t *)CMSG_DATA(cmptr);
			*fd=*data;

			delete[] control;
			return true;
		}
		#ifdef DEBUG_PASSFD
		else {

			// if we got bad data, be specific about what was
			// wrong, this will help debug problems with different
			// platforms
			if (!cmptr) {
				debugPrintf("%d: ",
						(int)process::getProcessId());
				debugPrintf("null cmptr\n");
			} else {
				if (cmptr->cmsg_level!=SOL_SOCKET) {
					debugPrintf("%d: ",
						(int)process::getProcessId());
					debugPrintf("got cmsg_level=%ld",
						static_cast<long>(
							cmptr->cmsg_level));
					debugPrintf(" instead of %ld",
						static_cast<long>(SOL_SOCKET));
					debugPrintf("\n");
				}
				if (cmptr->cmsg_type!=SCM_RIGHTS) {
					debugPrintf("%d: ",
						(int)process::getProcessId());
					debugPrintf("got cmsg_type=%ld",
						static_cast<long>(
							cmptr->cmsg_type));
					debugPrintf(" instead of %ld",
						static_cast<long>(SCM_RIGHTS));
					debugPrintf("\n");
				}
			}
		}
		#endif

		delete[] control;
	#else
		if (messageheader.msg_accrightslen==sizeof(int32_t)) {
			*fd=newfd;
			return true;
		}
	#endif

	// if we're here then we must have received some bad data
	return false;

#elif defined(RUDIMENTS_HAVE_DUPLICATEHANDLE)

	// wait for the other process to tell us to go
	bool	go;
	if (read(&go)!=sizeof(bool)) {
		return false;
	}

	// send our process id
	uint32_t	pid=process::getProcessId();
	if (write(pid)!=sizeof(uint32_t)) {
		return false;
	}

	// get the handle from the other process
	uint64_t	handle;
	if (read(&handle)!=sizeof(uint64_t)) {
		return false;
	}

	// get file descriptor from handle
	if ((HANDLE)handle!=INVALID_HANDLE_VALUE) {
		*fd=_open_osfhandle((long)handle,0);
		return true;
	}
	return false;
#else
	RUDIMENTS_SET_ENOSYS
	return false;
#endif
}

bool filedescriptor::passSocket(int32_t sock) {

#if defined(RUDIMENTS_HAVE_WSADUPLICATESOCKET)

	// tell the other process to go
	if (write(true)!=sizeof(bool)) {
		return false;
	}

	// read the process id from the other side
	uint32_t	otherpid;
	if (read(&otherpid)!=sizeof(uint32_t)) {
		return false;
	}

	// duplicate the socket
	WSAPROTOCOL_INFO	wpinfo;
	if (WSADuplicateSocket((SOCKET)sock,otherpid,&wpinfo)) {
		return false;
	}

	// write the wsaprotocol_info to the other side
	if (write((void *)&wpinfo,sizeof(wpinfo))!=sizeof(wpinfo)) {
		return false;
	}

	// get result from the other process
	bool	result;
	if (read(&result)!=sizeof(bool)) {
		return false;
	}
	return result;
#else
	return passFileDescriptor(sock);
#endif
}

bool filedescriptor::receiveSocket(int32_t *sock) {

#if defined(RUDIMENTS_HAVE_WSADUPLICATESOCKET)

	// wait for the other process to tell us to go
	bool	go;
	if (read(&go)!=sizeof(bool)) {
		return false;
	}

	// send our process id
	uint32_t	pid=process::getProcessId();
	if (write(pid)!=sizeof(uint32_t)) {
		return false;
	}

	// read a wsaprotocol_info from the other side
	WSAPROTOCOL_INFO	wpinfo;
	if (read((void *)&wpinfo,sizeof(wpinfo))!=sizeof(wpinfo)) {
		return false;
	}

	// create the socket
	*sock=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,
					&wpinfo,0,WSA_FLAG_OVERLAPPED);
	bool	result=(*sock!=INVALID_SOCKET);

	// tell the other process how it went
	if (write(result)!=sizeof(bool)) {
		return false;
	}
	return result;
#else
	return receiveFileDescriptor(sock);
#endif
}

bool filedescriptor::useNaglesAlgorithm() {
	return setNoDelay(0);
}

bool filedescriptor::dontUseNaglesAlgorithm() {
	return setNoDelay(1);
}

bool filedescriptor::setNoDelay(int32_t onoff) {
#ifdef TCP_NODELAY
	int32_t	value=onoff;
	return !setSockOpt(IPPROTO_TCP,TCP_NODELAY,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&value,
				(socklen_t)sizeof(int));
#else
	RUDIMENTS_SET_ENOSYS
	return false;
#endif
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

bool filedescriptor::disableIPv4() {
#ifdef IPV6_V6ONLY
	int32_t	no=0;
	return setSockOpt(IPPROTO_IPV6,IPV6_V6ONLY,
		(void *)&no,static_cast<socklen_t>(sizeof(int32_t)))!=-1;
#else
	RUDIMENTS_SET_ENOSYS
	return false;
#endif
}

bool filedescriptor::enableIPv4() {
#ifdef IPV6_V6ONLY
	int32_t	yes=1;
	return setSockOpt(IPPROTO_IPV6,IPV6_V6ONLY,
		(void *)&yes,static_cast<socklen_t>(sizeof(int32_t)))!=-1;
#else
	RUDIMENTS_SET_ENOSYS
	return false;
#endif
}

const char *filedescriptor::getType() const {
	return pvt->_type;
}

char *filedescriptor::getPeerAddress() const {

	// initialize a socket address structure
	struct sockaddr_in		clientsin;
	RUDIMENTS_SOCKLEN_OR_SIZE_T	size=sizeof(clientsin);
	bytestring::zero(&clientsin,sizeof(clientsin));

	// get the peer address
	int32_t	result;
	error::clearError();
	do {
		result=getpeername(pvt->_fd,
				reinterpret_cast<struct sockaddr *>(&clientsin),
				&size);
	} while (result==-1 && error::getErrorNumber()==EINTR);

	// if getpeername was successful and the peer was an inet socket,
	// convert the address to a string and return a copy of it,
	// otherwise return NULL
	if (result!=-1 && ((struct sockaddr *)&clientsin)->sa_family==AF_INET) {
		return charstring::duplicate(inet_ntoa(clientsin.sin_addr));
	}
	return NULL;
}

int32_t filedescriptor::getSockOpt(int32_t level, int32_t optname,
				void *optval, socklen_t *optlen) {
	int32_t	result;
	RUDIMENTS_SOCKLEN_OR_SIZE_T	tempoptlen;
	if (optlen) {
		tempoptlen=*optlen;
	}
	error::clearError();
	do {
		result=getsockopt(pvt->_fd,level,optname,
			(RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE)optval,
			&tempoptlen);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (optlen) {
		*optlen=tempoptlen;
	}
	return result;
}

int32_t filedescriptor::setSockOpt(int32_t level, int32_t optname,
				const void *optval, socklen_t optlen) {
	int32_t	result;
	error::clearError();
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
	setFileDescriptor(filedes);
}

void *filedescriptor::sslctx() {
#ifdef RUDIMENTS_HAS_SSL
	return (void *)pvt->_sslctx;
#else
	return NULL;
#endif
}

void *filedescriptor::ssl() {
#ifdef RUDIMENTS_HAS_SSL
	return (void *)pvt->_ssl;
#else
	return NULL;
#endif
}

int32_t filedescriptor::sslresult() {
#ifdef RUDIMENTS_HAS_SSL
	return pvt->_sslresult;
#else
	return 0;
#endif
}

void filedescriptor::sslresult(int32_t sslrslt) {
#ifdef RUDIMENTS_HAS_SSL
	pvt->_sslresult=sslrslt;
#endif
}

gssapicontext *filedescriptor::gssapictx() {
	return pvt->_gssapictx;
}

bool filedescriptor::closeOnExec() {
	#if defined(RUDIMENTS_HAVE_FD_CLOEXEC)
		return !fCntl(F_SETFD,fCntl(F_GETFD,FD_CLOEXEC)|FD_CLOEXEC);
	#elif defined(RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT)
		return SetHandleInformation(
				(HANDLE)getHandleFromFileDescriptor(pvt->_fd),
				HANDLE_FLAG_INHERIT,0)!=0;
	#else
		#error no FD_CLOEXEC or anything like it
	#endif
}

bool filedescriptor::dontCloseOnExec() {
	#if defined(RUDIMENTS_HAVE_FD_CLOEXEC)
		return !fCntl(F_SETFD,fCntl(F_GETFD,FD_CLOEXEC)&(~FD_CLOEXEC));
	#elif defined(RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT)
		return SetHandleInformation(
				(HANDLE)getHandleFromFileDescriptor(pvt->_fd),
				HANDLE_FLAG_INHERIT,
				HANDLE_FLAG_INHERIT)!=0;
	#else
		#error no FD_CLOEXEC or anything like it
	#endif
}

bool filedescriptor::getCloseOnExec() {
	#if defined(RUDIMENTS_HAVE_FD_CLOEXEC)
		return fCntl(F_GETFD,FD_CLOEXEC);
	#elif defined(RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT)
		DWORD	inherit;
		if (GetHandleInformation(
				(HANDLE)getHandleFromFileDescriptor(pvt->_fd),
				&inherit)) {
			return (bool)(inherit&HANDLE_FLAG_INHERIT);
		}
		return false;
	#else
		#error no FD_CLOEXEC or anything like it
	#endif
}

size_t filedescriptor::printf(const char *format, ...) {
	va_list	argp;
	va_start(argp,format);
	size_t	result=printf(format,&argp);
	va_end(argp);
	return result;
}

size_t filedescriptor::printf(const char *format, va_list *argp) {

	ssize_t	size=0;

	// If we're not buffering writes...
	if (!pvt->_writebuffer) {

		#ifdef RUDIMENTS_HAVE_VDPRINTF

			// use vdprintf if it's available
			return vdprintf(pvt->_fd,format,*argp);

		#else

			// otherwise use vfprintf, if we can
			FILE	*f=NULL;
			if (pvt->_fd==0) {
				f=stdin;
			} else if (pvt->_fd==1) {
				f=stdout;
			} else if (pvt->_fd==2) {
				f=stderr;
			}

			// Use fdopen if it's available.  Unfortunately we
			// can't (reliably) on Windows because it won't work
			// if the filedescriptor is a socket.
			#if defined(RUDIMENTS_HAVE_FDOPEN) && \
				defined(FD) && !defined(_WIN32)
			else {
				f=fdopen(pvt->_fd,"a");

				// Some platforms (Unixware) don't like "a"
				// with some types of file descriptors, so if
				// "a" fails, then try "w".
				if (!f) {
					f=fdopen(pvt->_fd,"w");
				}
			}
			#endif

			if (f) {
				size=vfprintf(f,format,*argp);
				fflush(f);

				#if defined(RUDIMENTS_HAVE_FDOPEN) && \
					defined(FD) && !defined(_WIN32)
				if (f!=stdin && f!=stdout && f!=stderr) {

					// We need to free f but we don't want
					// fclose() to close pvt->_fd.  There's
					// no standard way of doing this though.
					//
					// Setting f's file descriptor member
					// to -1 is generally reliable, though
					// that's tricky too...

					// The size and signedness of
					// FD varies a bit.  This
					// is the only way to handle
					// all variations without the
					// compiler throwing errors.
					if (sizeof(FD)==1) {
						int8_t	i8=-1;
						bytestring::copy(&(FD),&i8,1);
					} else if (sizeof(FD)==2) {
						int16_t i16=-1;
						bytestring::copy(&(FD),&i16,2);
					} else if (sizeof(FD)==4) {
						int32_t i32=-1;
						bytestring::copy(&(FD),&i32,4);
					} else if (sizeof(FD)==8) {
						int64_t i64=-1;
						bytestring::copy(&(FD),&i64,8);
					}

					// ok, now close f
					fclose(f);
				}
				#endif

				return size;
			}
		#endif
	}

	// If we are buffering writes though, don't use the above because it
	// would bypass the buffer.

	// Use vasprintf if it is available, otherwise play games with
	// charstring::printf().
	char	*buffer=NULL;
	#ifdef RUDIMENTS_HAVE_VASPRINTF
		size=vasprintf(&buffer,format,*argp);
	#else
		// Some compilers throw a warning if they see "printf(NULL..."
		// at all, whether it's the global function printf() or one
		// that you've defined yourself.  Using buffer here works
		// around that.
		size=charstring::printf(buffer,0,format,argp);
		buffer=new char[size+1];
		size=charstring::printf(buffer,size+1,format,argp);
	#endif
	write(buffer,size);
	delete[] buffer;
	return size;
}

void filedescriptor::safePrint(char c) {
	safePrint((unsigned char)c);
}

void filedescriptor::safePrint(const char *string, int32_t length) {
	safePrint((const unsigned char *)string,length);
}

void filedescriptor::safePrint(const char *string) {
	safePrint((const unsigned char *)string);
}

static char hex[17]="0123456789ABCDEF";

void filedescriptor::safePrint(unsigned char c) {
	if (c=='\r') {
		printf("\\r");
	} else if (c=='\n') {
		printf("\\n");
	} else if (c=='	') {
		printf("\\t");
	} else if (c>=' ' && c<='~') {
		printf("%c",c);
	} else {
		unsigned int	uintc=(unsigned char)c;
		printf("(0x%c%c|%d)",hex[((c>>4)&0x0F)],hex[(c&0x0F)],uintc);
	}
}

void filedescriptor::safePrint(const unsigned char *string, int32_t length) {
	const unsigned char	*ch=string;
	for (int32_t i=0; i<length; i++) {
		safePrint(*ch);
		ch++;
	}
}

void filedescriptor::safePrint(const unsigned char *string) {
	safePrint(string,charstring::length((const char *)string));
}

void filedescriptor::printBits(unsigned char value) {
	printBits(reinterpret_cast<unsigned char *>(&value),sizeof(value));
}

void filedescriptor::printBits(uint16_t value) {
	printBits(reinterpret_cast<unsigned char *>(&value),sizeof(value));
}

void filedescriptor::printBits(uint32_t value) {
	printBits(reinterpret_cast<unsigned char *>(&value),sizeof(value));
}

void filedescriptor::printBits(uint64_t value) {
	printBits(reinterpret_cast<unsigned char *>(&value),sizeof(value));
}

void filedescriptor::printBits(char value) {
	printBits(reinterpret_cast<unsigned char *>(&value),sizeof(value));
}

void filedescriptor::printBits(int16_t value) {
	printBits(reinterpret_cast<unsigned char *>(&value),sizeof(value));
}

void filedescriptor::printBits(int32_t value) {
	printBits(reinterpret_cast<unsigned char *>(&value),sizeof(value));
}

void filedescriptor::printBits(int64_t value) {
	printBits(reinterpret_cast<unsigned char *>(&value),
							sizeof(value));
}

void filedescriptor::printBits(unsigned char *bits, uint64_t size) {
	for (uint64_t i=0; i<size; i++) {
		unsigned char byte=bits[i];
		for (int8_t j=7; j>=0; j--) {
			printf("%d",(byte>>j)&0x01);
		}
	}
}

// FIXME: add a specific test for _set_invalid_parameter_handler
#if defined(_WIN32) && defined(RUDIMENTS_HAVE_LONG_LONG)
static void invalidParameterHandler(const wchar_t *expression,
					const wchar_t *function,
					const wchar_t *file,
					unsigned int line,
					uintptr_t preserved) {
	// don't do anything
}
#endif

void *filedescriptor::getHandleFromFileDescriptor(int32_t fd) {
	#if defined(_WIN32)
		// FIXME: add a specific test for _set_invalid_parameter_handler
		#if defined(RUDIMENTS_HAVE_LONG_LONG)
		_invalid_parameter_handler	oldiph=
			_set_invalid_parameter_handler(invalidParameterHandler);
		intptr_t	handle=_get_osfhandle(fd);
		_set_invalid_parameter_handler(oldiph);
		return (void *)handle;
		#else
		// this is dangerous, and can crash if fd is invalid,
		// but I'm not sure what else to do
		return (void *)_get_osfhandle(fd);
		#endif
	#else
		return NULL;
	#endif
}
