// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_HOSTENTRY_H
#define RUDIMENTS_HOSTENTRY_H

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

		static	int	getAliasList(const char *hostname,
						char ***aliaslist);
		static	int	getAddressType(const char *hostname,
						int *addresstype);
		static	int	getAddressLength(const char *hostname,
						int *addresslength);
		static	int	getAddressList(const char *hostname,
						char ***addresslist);
		static	int	getAddressString(const char *hostname,
						int index,
						char **addressstring);

		static	int	getName(const char *address,
						int len, int type,
						char **name);
		static	int	getAliasList(const char *address,
						int len, int type,
						char ***aliaslist);
		static	int	getAddressList(const char *address,
						int len, int type,
						char ***addresslist);
		static	int	getAddressString(const char *address,
						int len, int type,
						int index,
						char **addressstring);

	#include <rudiments/private/hostentry.h>
};

#ifdef ENABLE_INLINES
	#define INLINE inline
	#include <rudiments/private/hostentryinlines.h>
#endif

#endif
