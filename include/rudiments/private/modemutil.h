// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMUTIL_H
#define RUDIMENTS_MODEMUTIL_H

#include <rudiments/listener.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class modemutilprivate;

class modemutil {
	public:
			modemutil();
		virtual	~modemutil();

		void	initialize(const char *device, const char *baud);

	protected:
		const char	*_devicename();
		const char	*_baud();

		bool	configureSerialPort(int fd, const char *baud);

	private:
		modemutilprivate	*pvt;

};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
