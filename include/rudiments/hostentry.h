// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef HOSTENTRY_H
#define HOSTENTRY_H

#include <rudiments/private/config.h>

#include <netdb.h>

class hostentry {
	public:
			hostentry();
			~hostentry();

		int	initialize(const char *hostname);
		int	initialize(const char *address, int len, int type);

		char	*getName() const;
		char	**getAliasList() const;
		int	getAddressType() const;
		int	getAddressLength() const;
		char	**getAddressList() const;
		char	*getAddressString(int index) const;

		void	print() const;

	#include <rudiments/private/hostentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/hostentryinlines.h>
#endif

#endif
