// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/unixsocket.h>

#include <netinet/in.h>
#include <sys/uio.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#define DEBUG_UNIXSOCKET 1

#ifdef NEED_XNET_PROTOTYPES
extern ssize_t __xnet_recvmsg (int, struct msghdr *, int);
extern ssize_t __xnet_sendmsg (int, const struct msghdr *, int);
#endif

#ifndef CMSG_ALIGN
#define CMSG_ALIGN(len) (((len)+sizeof(size_t)-1)&~(sizeof(size_t)-1))
#endif

#ifndef CMSG_SPACE
#define CMSG_SPACE(len) (CMSG_ALIGN(len)+CMSG_ALIGN(sizeof(struct cmsghdr)))
#endif

#ifndef CMSG_LEN
#define CMSG_LEN(len) (CMSG_ALIGN(sizeof(struct cmsghdr))+(len))
#endif

unixsocket::unixsocket() : filedescriptor(), datatransport(), socket() {
	filename=NULL;
}

unixsocket::unixsocket(int filedesc) :
	filedescriptor(filedesc), datatransport(filedesc), socket(filedesc) {
	filename=NULL;
}

void unixsocket::initialize(const char *filename) {
	this->filename=(char *)filename;
}

bool unixsocket::passFileDescriptor(int filedesc) {

	// have to use sendmsg to pass a file descriptor. 
	// sendmsg can only send a msghdr
	struct	msghdr	messageheader;

	// no need to name the message
	messageheader.msg_name=NULL;
	messageheader.msg_namelen=0;

	// initialize flags to 0
	messageheader.msg_flags=0;

	// must send at least 1 iovector with 1 byte of real data
	struct iovec	iovector[1];
	iovector[0].iov_base=(IOV_BASE_TYPE)" ";
	iovector[0].iov_len=sizeof(char);
	messageheader.msg_iov=iovector;
	messageheader.msg_iovlen=1;

	// use other parts of the msghdr structure to send the descriptor
	#ifdef HAVE_MSGHDR_MSG_CONTROLLEN

		// new-style:
		// The descriptor is passed in the cmsghdr part of a 
		// control_union.

		// Multiple descriptors could be passed, but we're 
		// just going to send 1.
		union {
			struct cmsghdr	cm;
			char		control[CMSG_SPACE(sizeof(int))];
		} control_union;
		messageheader.msg_control=control_union.control;
		// FIXME: which of these should I use???
		//messageheader.msg_controllen=sizeof(control_union.control);
		messageheader.msg_controllen=sizeof(control_union);

		struct cmsghdr	*cmptr;
		cmptr=CMSG_FIRSTHDR(&messageheader);
		cmptr->cmsg_level=SOL_SOCKET;
		cmptr->cmsg_type=SCM_RIGHTS;
		cmptr->cmsg_len=CMSG_LEN(sizeof(int));
		*((int *)CMSG_DATA(cmptr))=filedesc;
	#else
		// old-style:
		// the descriptor is passed in the accrights/accrightslen 
		// section of the msghdr
		messageheader.msg_accrights=(caddr_t)&filedesc;
		messageheader.msg_accrightslen=sizeof(int);
	#endif

	// finally, send the msghdr
	return sendmsg(fd,&messageheader,0)!=-1;
}

bool unixsocket::receiveFileDescriptor(int *filedesc) {

	// have to use recvmsg to receive a file descriptor. 
	// recvmsg can only send a msghdr
	struct msghdr	messageheader;

	// no need to name the message
	messageheader.msg_name=NULL;
	messageheader.msg_namelen=0;

	// initialize flags to 0
	messageheader.msg_flags=0;

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
		// The descriptor is received in the cmsghdr part 
		// of a control_union.
		union {
			struct cmsghdr	cm;
			char		control[CMSG_SPACE(sizeof(int))];
		} control_union;
		messageheader.msg_control=control_union.control;
		// FIXME: which of these should I use???
		//messageheader.msg_controllen=sizeof(control_union.control);
		messageheader.msg_controllen=sizeof(control_union);
	#else
		// old-style
		// The descriptor is passed in the accrights
		int	newfiledesc;
		messageheader.msg_accrights=(caddr_t)&newfiledesc;
		messageheader.msg_accrightslen=sizeof(int);
	#endif

	// receive the msghdr
//int	ret=recvmsg(fd,&messageheader,0);
//printf("recvmsg()=%d\n",ret);
	if (recvmsg(fd,&messageheader,0)==-1) {
//if (ret==-1) {
		return false;
	}


	// if we got valid data, set the passed-in descriptor to the 
	// descriptor we received and return success
	#ifdef HAVE_MSGHDR_MSG_CONTROLLEN

/*messageheader.msg_controllen=sizeof(control_union);
printf("msg_name=%s\n",(char *)messageheader.msg_name);
printf("msg_namelen=%d\n",messageheader.msg_namelen);
printf("msg_iov",msg_iov);
printf("msg_iovlen=%d\n",messageheader.msg_iovlen);
printf("msg_control",msg_control);
printf("msg_controllen=%d\n",messageheader.msg_controllen);
printf("msg_flags=0x%02x\n",messageheader.msg_flags);*/
		struct cmsghdr	*cmptr=CMSG_FIRSTHDR(&messageheader);
/*cmptr->cmsg_len=CMSG_LEN(sizeof(int));
cmptr->cmsg_level=SOL_SOCKET;
cmptr->cmsg_type=SCM_RIGHTS;*/
		if (cmptr && cmptr->cmsg_len==CMSG_LEN(sizeof(int)) &&
				cmptr->cmsg_level==SOL_SOCKET &&
				cmptr->cmsg_type==SCM_RIGHTS) {

			// if we got good data, set the descriptor and return
			*filedesc=*((int *)CMSG_DATA(cmptr));
			return true;
		}
#ifdef DEBUG_UNIXSOCKET
		else {

			// if we got bad data, be specific about what was
			// wrong, this will help debug problems with different
			// platforms
			if (!cmptr) {
				printf("%d: ",getpid());
				printf("null cmptr\n");
			} else {
				if (cmptr->cmsg_len!=CMSG_LEN(sizeof(int))) {
					printf("%d: ",getpid());
					printf("got cmsg_len=");
			       		printf("%d",(long)cmptr->cmsg_len);
			       		printf(" instead of ");
					printf("%d",
						(long)CMSG_LEN(sizeof(int)));
					printf("\n");
				}
				if (cmptr->cmsg_level!=SOL_SOCKET) {
					printf("%d: ",getpid());
					printf("got cmsg_level=");
					printf("%d",(long)cmptr->cmsg_level);
					printf(" instead of ");
					printf("%d",(long)SOL_SOCKET);
					printf("\n");
				}
				if (cmptr->cmsg_type!=SCM_RIGHTS) {
					printf("%d: ",getpid());
					printf("got cmsg_type=");
					printf("%d",(long)cmptr->cmsg_type);
					printf(" instead of ");
					printf("%d",(long)SCM_RIGHTS);
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
