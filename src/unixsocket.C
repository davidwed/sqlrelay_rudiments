// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/unixsocket.h>

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <limits.h>

//#define DEBUG_UNIXSOCKET 1

#ifdef NEED_XNET_PROTOTYPES
extern ssize_t __xnet_recvmsg (int, struct msghdr *, int);
extern ssize_t __xnet_sendmsg (int, const struct msghdr *, int);
#endif

#ifdef CMSG_SPACE
union control_union {
	struct cmsghdr	cm;
	char		control[CMSG_SPACE(sizeof(int))];
};
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

	// these must be null for stream sockets
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
		// The descriptor is passed in the msg_control
		#ifdef CMSG_SPACE
		union control_union	control;
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
		#ifdef CMSG_LEN
		cmptr->cmsg_len=CMSG_LEN(sizeof(int));
		#else
		cmptr->cmsg_len=sizeof(control);
		#endif
		*((int *)CMSG_DATA(cmptr))=filedesc;

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

bool unixsocket::receiveFileDescriptor(int *filedesc) {

	// have to use recvmsg to receive a file descriptor. 
	// recvmsg can only send a msghdr
	struct msghdr	messageheader;

	// these must be null for stream sockets
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
		// The descriptor is received in the msg_control
		#ifdef CMSG_SPACE
		union control_union	control;
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
				#ifdef HAVE_CMSG_LEN
				if (cmptr->cmsg_len!=CMSG_LEN(sizeof(int))) {
					printf("%d: ",getpid());
					printf("got cmsg_len=");
			       		printf("%d",(long)cmptr->cmsg_len);
			       		printf(" instead of ");
					printf("%d",
						(long)CMSG_LEN(sizeof(int)));
					printf("\n");
				}
				#endif
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
