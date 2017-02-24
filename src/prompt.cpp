// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#include <rudiments/prompt.h>
#include <rudiments/charstring.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAVE_LIBEDIT
	// some versions of libedit don't include stdio.h, so FILE is undefined
	#include <stdio.h>
	#include <editline/readline.h>
	#ifndef RUDIMENTS_LIBEDIT_HAS_HISTORY_TRUNCATE_FILE
		static int history_truncate_file(const char * filename, int n);
	#endif
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
	#ifdef RUDIMENTS_HAVE_LIBEDIT

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
		if (retval && retval[0]) {

			add_history(retval);
			pvt->_queue++;

			// flush history if the queue gets too long
			if (pvt->_maxhistoryqueue &&
				pvt->_queue==pvt->_maxhistoryqueue) {
				flushHistory();
			}

		}

	#else

		stdoutput.write(pvt->_prompt);

		size_t	retvalsize=sys::getMaxLineLength();
		char	*retval=new char[retvalsize];
		ssize_t	bytes=stdinput.read(retval,retvalsize-1);
		if (bytes>0) {
			retval[bytes-1]='\0';
			#ifdef ADD_NEWLINE_AFTER_READ_FROM_STDIN
				stdoutput.write('\n');
			#endif
		} else {
			delete[] retval;
			retval=NULL;
		}

	#endif

	if (!retval) {
		stdoutput.write('\n');
	}

	return retval;
}

void prompt::flushHistory() {
	#ifdef RUDIMENTS_HAVE_LIBEDIT
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

#if defined(RUDIMENTS_HAVE_LIBEDIT) && \
	!defined(RUDIMENTS_LIBEDIT_HAS_HISTORY_TRUNCATE_FILE)
/*-
 * Copyright (c) 1997 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jaromir Dolecek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <pwd.h>
#ifdef RUDIMENTS_HAVE_ERRNO_H
	#include <errno.h>
#endif
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif

static const char _history_tmp_template[] = "/tmp/.historyXXXXXX";

static const char *
_default_history_file(void)
{
	struct passwd *p;
	static char *path;
	size_t len;

	if (path)
		return path;

	if ((p = getpwuid(getuid())) == NULL)
		return NULL;

	len = strlen(p->pw_dir) + sizeof("/.history");
	if ((path = (char *)malloc(len)) == NULL)
		return NULL;

	(void)snprintf(path, len, "%s/.history", p->pw_dir);
	return path;
}

int
history_truncate_file (const char *filename, int nlines)
{
	int ret = 0;
	FILE *fp, *tp;
	char ftemplate[sizeof(_history_tmp_template)];
	char buf[4096];
	int fd;
	char *cp;
	off_t off;
	int count = 0;
	ssize_t left = 0;

	if (filename == NULL && (filename = _default_history_file()) == NULL)
		return errno;
	if ((fp = fopen(filename, "r+")) == NULL)
		return errno;
	strcpy(ftemplate, _history_tmp_template);
	if ((fd = mkstemp(ftemplate)) == -1) {
		ret = errno;
		goto out1;
	}

	if ((tp = fdopen(fd, "r+")) == NULL) {
		close(fd);
		ret = errno;
		goto out2;
	}

	for(;;) {
		if (fread(buf, sizeof(buf), (size_t)1, fp) != 1) {
			if (ferror(fp)) {
				ret = errno;
				break;
			}
			if (fseeko(fp, (off_t)sizeof(buf) * count, SEEK_SET) ==
			    (off_t)-1) {
				ret = errno;
				break;
			}
			left = (ssize_t)fread(buf, (size_t)1, sizeof(buf), fp);
			if (ferror(fp)) {
				ret = errno;
				break;
			}
			if (left == 0) {
				count--;
				left = sizeof(buf);
			} else if (fwrite(buf, (size_t)left, (size_t)1, tp)
			    != 1) {
				ret = errno;
				break;
			}
			fflush(tp);
			break;
		}
		if (fwrite(buf, sizeof(buf), (size_t)1, tp) != 1) {
			ret = errno;
			break;
		}
		count++;
	}
	if (ret)
		goto out3;
	cp = buf + left - 1;
	if(*cp != '\n')
		cp++;
	for(;;) {
		while (--cp >= buf) {
			if (*cp == '\n') {
				if (--nlines == 0) {
					if (++cp >= buf + sizeof(buf)) {
						count++;
						cp = buf;
					}
					break;
				}
			}
		}
		if (nlines <= 0 || count == 0)
			break;
		count--;
		if (fseeko(tp, (off_t)sizeof(buf) * count, SEEK_SET) < 0) {
			ret = errno;
			break;
		}
		if (fread(buf, sizeof(buf), (size_t)1, tp) != 1) {
			if (ferror(tp)) {
				ret = errno;
				break;
			}
			ret = EAGAIN;
			break;
		}
		cp = buf + sizeof(buf);
	}

	if (ret || nlines > 0)
		goto out3;

	if (fseeko(fp, (off_t)0, SEEK_SET) == (off_t)-1) {
		ret = errno;
		goto out3;
	}

	if (fseeko(tp, (off_t)sizeof(buf) * count + (cp - buf), SEEK_SET) ==
	    (off_t)-1) {
		ret = errno;
		goto out3;
	}

	for(;;) {
		if ((left = (ssize_t)fread(buf, (size_t)1, sizeof(buf), tp)) == 0) {
			if (ferror(fp))
				ret = errno;
			break;
		}
		if (fwrite(buf, (size_t)left, (size_t)1, fp) != 1) {
			ret = errno;
			break;
		}
	}
	fflush(fp);
	if((off = ftello(fp)) > 0)
		(void)ftruncate(fileno(fp), off);
out3:
	fclose(tp);
out2:
	unlink(ftemplate);
out1:
	fclose(fp);

	return ret;
}
#endif
