// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/unixsocket.h>

#include <sys/types.h>
#include <netinet/in.h>
#define _XPG4_2
	#include <sys/socket.h>
#undef _XPG4_2
#include <sys/uio.h>
#include <limits.h>
#include <stdio.h>

//#define DEBUG_UNIXSOCKET 1

unixsocket::unixsocket() : socket() {
	filename=NULL;
}

unixsocket::unixsocket(int filedescriptor) : socket(filedescriptor) {
	filename=NULL;
}

void unixsocket::initialize(const char *filename) {
	this->filename=(char *)filename;
}

bool unixsocket::passFileDescriptor(int descriptor) {

	// have to use sendmsg to pass a file descriptor. 
	// sendmsg can only send a msghdr
	struct	msghdr	messageheader;

	// no need to name the message
	messageheader.msg_name=NULL;
	messageheader.msg_namelen=0;

	// must send at least 1 iovector with 1 byte of real data
	struct iovec	iovector[1];
	iovector[0].iov_base=(IOV_BASE_TYPE)" ";
	iovector[0].iov_len=sizeof(char);
	messageheader.msg_iov=iovector;
	messageheader.msg_iovlen=1;

	// use other parts of the msghdr structure to send the descriptor
	#ifdef HAVE_CMSGHDR

		// new-style:
		// The descriptor is passed in the cmsghdr part of a 
		// control_union.

		#ifdef HAVE_CMSG_SPACE
		// Multiple descriptors could be passed, but we're 
		// just going to send 1.
		union {
			struct cmsghdr	cm;
			char		control[CMSG_SPACE(sizeof(int))];
		} control_union;
		messageheader.msg_control=control_union.control;
		messageheader.msg_controllen=sizeof(control_union.control);

		struct cmsghdr	*cmptr;
		cmptr=CMSG_FIRSTHDR(&messageheader);
		cmptr->cmsg_level=SOL_SOCKET;
		cmptr->cmsg_type=SCM_RIGHTS;
		cmptr->cmsg_len=CMSG_LEN(sizeof(int));
		#else
		int	controllen=sizeof(struct cmsghdr)+sizeof(int);
		struct cmsghdr	*cmptr=(struct cmsghdr *)malloc(controllen);
		cmptr->cmsg_level=SOL_SOCKET;
		cmptr->cmsg_type=SCM_RIGHTS;
		cmptr->cmsg_len=controllen;
		messageheader.msg_control=(caddr_t)cmptr;
		messageheader.msg_controllen=controllen;
		#endif
		*((int *)CMSG_DATA(cmptr))=descriptor;
	#else
		// old-style:
		// the descriptor is passed in the accrights/accrightslen 
		// section of the msghdr
		messageheader.msg_accrights=(caddr_t)&descriptor;
		messageheader.msg_accrightslen=sizeof(int);
	#endif

	// finally, send the msghdr
	return sendmsg(fd,&messageheader,0)!=-1;
}

bool unixsocket::receiveFileDescriptor(int *descriptor) {

	// have to use recvmsg to receive a file descriptor. 
	// recvmsg can only send a msghdr
	struct msghdr	messageheader;

	// no need to name the message
	messageheader.msg_name=NULL;
	messageheader.msg_namelen=0;

	// the process that's going to handoff it's socket will also 
	// send a single iovector with a single byte of data in it, 
	// so we'll receive that too
	struct iovec	iovector[1];
	char		ptr;
	iovector[0].iov_base=(IOV_BASE_TYPE)&ptr;
	iovector[0].iov_len=sizeof(char);
	messageheader.msg_iov=iovector;
	messageheader.msg_iovlen=1;

	#ifdef HAVE_CMSGHDR
		#ifdef HAVE_CMSG_SPACE
		// new-style:
		// The descriptor is received in the cmsghdr part 
		// of a control_union.
		union {
			struct cmsghdr	cm;
			char		control[CMSG_SPACE(sizeof(int))];
		} control_union;
		messageheader.msg_control=control_union.control;
		messageheader.msg_controllen=sizeof(control_union.control);
		#else
		int	controllen=sizeof(struct cmsghdr)+sizeof(int);
		struct cmsghdr	*cmptr=(struct cmsghdr *)malloc(controllen);
		messageheader.msg_control=(caddr_t)cmptr;
		messageheader.msg_controllen=controllen;
		#endif
	#else
		// old-style
		// The descriptor is passed in the accrights
		int	newdescriptor;
		messageheader.msg_accrights=(caddr_t)&newdescriptor;
		messageheader.msg_accrightslen=sizeof(int);
	#endif

	// receive the msghdr
	if (recvmsg(fd,&messageheader,0)==-1) {
		return false;
	}

	// if we got valid data, set the passed-in descriptor to the 
	// descriptor we received and return success
	#ifdef HAVE_CMSGHDR

		#ifdef HAVE_CMSG_SPACE
		struct cmsghdr	*cmptr=CMSG_FIRSTHDR(&messageheader);
		if (cmptr && cmptr->cmsg_len==CMSG_LEN(sizeof(int)) &&
				cmptr->cmsg_level==SOL_SOCKET &&
				cmptr->cmsg_type==SCM_RIGHTS) {

			// if we got good data, set the desctiptor and return
			*descriptor=*((int *)CMSG_DATA(cmptr));
			return true;

		}
#ifdef DEBUG_UNIXSOCKET
		else {

			// if we got bad data, be specific about what was
			// wrong, this will help debug problems with different
			// platforms
			if (!cmptr) {
				printf("error: null cpmtr\n");
			} else {
				if (cmptr->cmsg_len!=CMSG_LEN(sizeof(int))) {
					printf("error: got cmsg_len=");
			       		printf("%d",(long)cmptr->cmsg_len);
			       		printf(" instead of ");
					printf("%d",
						(long)CMSG_LEN(sizeof(int)));
					printf("\n");
				}
				if (cmptr->cmsg_level!=SOL_SOCKET) {
					printf("error: got cmsg_level=");
					printf("%d",(long)cmptr->cmsg_level);
					printf(" instead of ");
					printf("%d",(long)SOL_SOCKET);
					printf("\n");
				}
				if (cmptr->cmsg_type!=SCM_RIGHTS) {
					printf("error: got cmsg_type=");
					printf("%d",(long)cmptr->cmsg_type);
					printf(" instead of ");
					printf("%d",(long)SCM_RIGHTS);
					printf("\n");
				}
			}
		}
#endif
		#else
		if (messageheader.msg_controllen==controllen) {
			*descriptor=*((int *)CMSG_DATA(cmptr));
			return true;
		}
#ifdef DEBUG_UNIXSOCKET
		else {
			printf("error: got msg_controllen=");
			printf("%d",(long)messageheader.msg_controllen);
			printf(" instead of ");
			printf("%d",(long)controllen);
			printf("\n");
		}
#endif
		#endif
	#else
		if (messageheader.msg_accrightslen==sizeof(int)) {
			*descriptor=newdescriptor;
			return true;
		}
	#endif

	// if we're here then we must have received some bad data
	return false;
}
