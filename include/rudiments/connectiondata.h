// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef CONNECTIONDATA_H
#define CONNECTIONDATA_H

#include <rudiments/private/config.h>

#include <rudiments/private/connectiondatanode.h>

class connectiondata {
	public:
			connectiondata();
			~connectiondata();

		void	setValue(const char *key, const char *value);
		char	*getValue(const char *key) const;
		void	removeValue(const char *key);

		int	getKeyCount() const;
		char	*getKey(int index) const;
		char	*getValue(int index) const;

		void	print() const;

	#include <rudiments/private/connectiondata.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/connectiondatainlines.h>
#endif

#endif
