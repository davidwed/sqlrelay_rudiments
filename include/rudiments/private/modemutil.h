// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMUTIL_H
#define RUDIMENTS_MODEMUTIL_H

#include <rudiments/listener.h>

class modemutil {
	public:
			modemutil();
		virtual	~modemutil();

		void	initialize(const char *device, const char *baud);

	protected:
		char	*devicename;
		char	*baud;

		bool	configureSerialPort(int fd, const char *baud);

};

#endif
