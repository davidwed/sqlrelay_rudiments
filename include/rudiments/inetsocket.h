// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETSOCKET_H
#define RUDIMENTS_INETSOCKET_H

#include <rudiments/private/inetsocketincludes.h>

class inetsocket : public socket {
	public:
			inetsocket();
			inetsocket(int filedesc);
		virtual	~inetsocket();
		void	initialize(const char *address, unsigned short port);

	#include <rudiments/private/inetsocket.h>
};

#endif
