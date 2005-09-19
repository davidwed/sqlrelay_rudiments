// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/commandline.h>
#include <rudiments/charstring.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class commandlineprivate {
	friend class commandline;
	private:
		int	_argc;
		char	**_argv;
};

commandline::commandline(int argc, const char **argv) {
	pvt=new commandlineprivate;
	pvt->_argc=argc;
	pvt->_argv=(char **)argv;
}

commandline::~commandline() {
	delete pvt;
}

const char *commandline::value(const char *arg) const {

	if (arg && arg[0]) {
		for (int i=1; i<pvt->_argc-1; i++) {
			if (!charstring::compare(arg,pvt->_argv[i])) {
				return pvt->_argv[i+1];
			}
		}
	}
	return "";
}

bool commandline::found(const char *arg) const {

	if (arg && arg[0]) {
		for (int i=1; i<pvt->_argc; i++) {
			if (!charstring::compare(arg,pvt->_argv[i])) {
				return true;
			}
		}
	}
	return false;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
