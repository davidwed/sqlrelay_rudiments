// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/commandline.h>
#include <rudiments/charstring.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class commandlineprivate {
	friend class commandline;
	private:
		int32_t	_argc;
		char	**_argv;
};

commandline::commandline(int32_t argc, const char **argv) {
	pvt=new commandlineprivate;
	pvt->_argc=argc;
	pvt->_argv=(char **)argv;
}

commandline::~commandline() {
	delete pvt;
}

const char *commandline::getValue(const char *arg) const {

	if (arg && arg[0]) {

		const char	*realarg=arg;

		// arg can be "arg" "-arg" or "--arg"
		if (realarg[0]=='-') {
			realarg=realarg+1;
		}
		if (realarg[0]=='-') {
			realarg=realarg+1;
		}
		size_t	realarglen=charstring::length(realarg);

		for (int32_t i=1; i<pvt->_argc; i++) {

			// look for "-arg value" or "--arg=value"
			if (i<pvt->_argc-1 &&
				pvt->_argv[i][0]=='-' &&
				!charstring::compare(pvt->_argv[i]+1,realarg)) {
				return pvt->_argv[i+1];
			} else if (charstring::length(pvt->_argv[i])>=
							realarglen+3 &&
					!charstring::compare(pvt->_argv[i],
								"--",2) &&
					!charstring::compare(pvt->_argv[i]+2,
								realarg,
								realarglen) &&
					pvt->_argv[i][2+realarglen]=='=') {
				return pvt->_argv[i]+2+realarglen+1;
			}
		}
	}
	return "";
}

bool commandline::found(const char *arg) const {

	if (arg && arg[0]) {

		const char	*realarg=arg;

		// arg can be "arg" "-arg" or "--arg"
		if (realarg[0]=='-') {
			realarg=realarg+1;
		}
		if (realarg[0]=='-') {
			realarg=realarg+1;
		}
		size_t	realarglen=charstring::length(realarg);

		for (int32_t i=1; i<pvt->_argc; i++) {

			// look for "-arg value" or "--arg=value"
			if ((pvt->_argv[i][0]=='-' &&
				!charstring::compare(
						pvt->_argv[i]+1,realarg)) ||
				(charstring::length(pvt->_argv[i])>=
							realarglen+3 &&
					!charstring::compare(pvt->_argv[i],
								"--",2) &&
					!charstring::compare(pvt->_argv[i]+2,
								realarg,
								realarglen) &&
					pvt->_argv[i][2+realarglen]=='=')) {
				return true;
			}
		}
	}
	return false;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
