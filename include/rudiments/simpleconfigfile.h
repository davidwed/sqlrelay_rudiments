// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SIMPLECONFIGFILE_H
#define RUDIMENTS_SIMPLECONFIGFILE_H

#include <rudiments/private/config.h>

#include <rudiments/errorhandler.h>
#include <rudiments/stringbuffer.h>

// The simpleconfigfile class provides methods for working with simple config
// files composed of name: value pairs and comments prefaced with a # sign.

class simpleconfigfile : public errorhandler {
	public:

			simpleconfigfile();
			~simpleconfigfile();

		int	parseFile(const char *filename);
			// parse file "filename"
		int	parseString(const char *string);
			// parse string "string"

		char	*getValue(const char *name);
		char	*setValue(const char *name, const char *value);

		int	writeFile(const char *filename, mode_t perms);

	#include <rudiments/private/simpleconfigfile.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/simpleconfigfileinlines.h>
#endif

#endif
