// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#include <rudiments/prompt.h>
#include <rudiments/charstring.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>

#ifdef HAVE_READLINE
	// This is an interesting story...
	// readline 2's include files don't list any parameters for any of
	// the functions.  This is fine for C, but not C++, at least not with
	// the compiler I'm using, even with the extern "C" {} bit.
	// This is fixed in readline 4, but, to maintain compatibility with 
	// readline 2, I define the functions myself.
	extern "C" {
		extern char *readline(char *prompt);
		extern void add_history(char *line);
		extern void read_history(char *file);
		extern void write_history(char *file);
		extern void history_truncate_file(char *file, int line);
	}
#endif

class promptprivate {
	friend class prompt;
	private:
		char		*_historyfilename;
		bool		_historyread;
		uint32_t	_maxhistorylines;
		uint32_t	_maxhistoryqueue;
		uint32_t	_queue;
		char		*_prompt;
};

prompt::prompt() {
	pvt=new promptprivate;
	pvt->_historyfilename=NULL;
	pvt->_historyread=false;
	pvt->_maxhistorylines=1024;
	pvt->_maxhistoryqueue=1024;
	pvt->_queue=0;
	pvt->_prompt=NULL;
}

prompt::~prompt() {
	flushHistory();
	delete[] pvt->_historyfilename;
	delete[] pvt->_prompt;
	delete pvt;
}

void prompt::setHistoryFile(const char *filename) {
	delete[] pvt->_historyfilename;
	pvt->_historyfilename=charstring::duplicate(filename);
}

const char *prompt::getHistoryFile() {
	return pvt->_historyfilename;
}

void prompt::setMaxHistoryLines(uint32_t lines) {
	pvt->_maxhistorylines=lines;
}

uint32_t prompt::getMaxHistoryLines() {
	return pvt->_maxhistorylines;
}

void prompt::setMaxHistoryQueue(uint32_t queue) {
	pvt->_maxhistoryqueue=queue;
}

uint32_t prompt::getMaxHistoryQueue() {
	return pvt->_maxhistoryqueue;
}

void prompt::setPrompt(const char *prompt) {
	delete[] pvt->_prompt;
	pvt->_prompt=charstring::duplicate(prompt);
}

const char *prompt::getPrompt() {
	return pvt->_prompt;
}

char *prompt::read() {
	#ifdef HAVE_READLINE

		// read the history file if we haven't already
		if (!charstring::isNullOrEmpty(pvt->_historyfilename) &&
							!pvt->_historyread) {

			// open and read the history file,
			// creating it if it doesn't exist
			file	historyfile;
			if (historyfile.open(
				pvt->_historyfilename,
				O_WRONLY|O_CREAT|O_APPEND,
				permissions::evalPermString("rw-rw-r--"))) {
				historyfile.close();
				read_history(pvt->_historyfilename);
			}
			pvt->_historyread=true;
		}

		// read a line
		char	*retval=readline(pvt->_prompt);

		// queue the line to be flushed to the history
		if (!charstring::isNullOrEmpty(retval)) {

			add_history(retval);
			pvt->_queue++;

			// flush history if the queue gets too long
			if (pvt->_maxhistoryqueue &&
				pvt->_queue==pvt->_maxhistoryqueue) {
				flushHistory();
			}
		} else {
			stdoutput.write('\n');
		}

	#else

		stdoutput.write(pvt->_prompt);

		size_t	retvalsize=sys::getMaxLineLength();
		char	*retval=new char[retvalsize];
		ssize_t	bytes=stdinput.read(retval,retvalsize-1);
		retval[bytes-1]='\0';
		#ifdef ADD_NEWLINE_AFTER_READ_FROM_STDIN
			stdoutput.write('\n');
		#endif

	#endif

	return retval;
}

void prompt::flushHistory() {
	#ifdef HAVE_READLINE
		if (!charstring::isNullOrEmpty(pvt->_historyfilename)) {
			write_history(pvt->_historyfilename);
			if (pvt->_maxhistorylines) {
				history_truncate_file(
						pvt->_historyfilename,
						pvt->_maxhistorylines);
			}
		}
	#endif
}
