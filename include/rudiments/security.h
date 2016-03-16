// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SECURITY_H
#define RUDIMENTS_SECURITY_H

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

class	filedescriptor;

class RUDIMENTS_DLLSPEC securitycontext {
	public:
		securitycontext() {};
		virtual ~securitycontext() {};
		virtual	void	setFileDescriptor(filedescriptor *fd)=0;
		virtual	filedescriptor	*getFileDescriptor()=0;

		virtual	bool	connect()=0;
		virtual	bool	accept()=0;
		virtual	ssize_t	read(void *buf, ssize_t count)=0;
		virtual	ssize_t	write(const void *buf, ssize_t count)=0;
		virtual	ssize_t pending()=0;
		virtual	bool	close()=0;
		virtual ssize_t	getSizeMax()=0;
};

#endif
