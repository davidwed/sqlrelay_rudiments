// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROTOCOLENTRY_H
#define RUDIMENTS_PROTOCOLENTRY_H

#include <rudiments/private/config.h>

#include <netdb.h>

class protocolentry {
	public:
			protocolentry();
			~protocolentry();

		int	initialize(const char *protocolname);
		int	initialize(int number);

		char	*getName() const;
		char	**getAliasList() const;
		int	getNumber() const;

		void	print() const;

		static	int	getAliasList(const char *protocolname,
						char ***aliaslist);
		static	int	getNumber(const char *protocolname,
						int *number);

		static	int	getName(int number, char **name);
		static	int	getAliasList(int number, char ***aliaslist);

	#include <rudiments/private/protocolentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/protocolentryinlines.h>
#endif

#endif
