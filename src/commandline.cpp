// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/commandline.h>
#include <rudiments/charstring.h>

#include <stdio.h>

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

	if (!charstring::isNullOrEmpty(arg)) {

		const char	*realarg=arg;

		// arg can be "arg" "-arg" or "--arg"
		if (realarg[0]=='-') {
			realarg++;
		}
		if (realarg[0]=='-') {
			realarg++;
		}
		size_t	realarglen=charstring::length(realarg);

		for (int32_t i=1; i<pvt->_argc; i++) {

			// look for "-arg value"...
			if (i<pvt->_argc-1 &&
				pvt->_argv[i][0]=='-' &&
				!charstring::compare(pvt->_argv[i]+1,realarg)) {

				// return the next arg unless it
				// also starts with a -
				if (pvt->_argv[i+1] &&
					pvt->_argv[i+1][0]!='-') {
					return pvt->_argv[i+1];
				} else {
					return "";
				}
			} else
	
			// ...or "--arg=value"
			if (charstring::length(pvt->_argv[i])>=
							realarglen+3 &&
					!charstring::compare(pvt->_argv[i],
								"--",2) &&
					!charstring::compare(pvt->_argv[i]+2,
								realarg,
								realarglen) &&
					pvt->_argv[i][2+realarglen]=='=') {

				// return the part of this arg after the =
				return pvt->_argv[i]+2+realarglen+1;
			}
		}
	}
	return "";
}

const char *commandline::getValue(const char *arg, const char *abbr) const {
	const char	*value=getValue(arg);
	if (charstring::isNullOrEmpty(value)) {
		value=getValue(abbr);
	}
	return value;
}

bool commandline::found(const char *arg) const {

	if (!charstring::isNullOrEmpty(arg)) {

		const char	*realarg=arg;

		// arg can be "arg" "-arg" or "--arg"
		if (realarg[0]=='-') {
			realarg++;
		}
		if (realarg[0]=='-') {
			realarg++;
		}
		size_t	realarglen=charstring::length(realarg);

		for (int32_t i=1; i<pvt->_argc; i++) {

			// look for "-arg", "--arg", or "--arg=value"
			if (
				(pvt->_argv[i][0]=='-' &&
				!charstring::compare(
						pvt->_argv[i]+1,realarg)) ||

				(!charstring::compare(
						pvt->_argv[i],"--",2) &&
				!charstring::compare(
						pvt->_argv[i]+2,realarg)) ||

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

bool commandline::found(const char *arg, const char *abbr) const {
	return (found(arg) || found(abbr));
}
