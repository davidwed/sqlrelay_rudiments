// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CONNECTIONDATA_H
#define RUDIMENTS_CONNECTIONDATA_H

#include <rudiments/dictionary.h>

class connectiondata : public stringdictionary<char *> {
	public:
		void	print() const;
};

#endif
