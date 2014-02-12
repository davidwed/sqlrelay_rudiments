// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LOGGER_H
#define RUDIMENTS_LOGGER_H

#include <rudiments/private/loggerincludes.h>

/** The logdestination class is a virtual base class that just defines methods
 *  that the real logdestination classes must implement. */
class RUDIMENTS_DLLSPEC logdestination {
	#include <rudiments/private/logdestination.h>
};

class syslogdestinationprivate;

/** The syslogdestination class writes log data to syslog. */
class RUDIMENTS_DLLSPEC syslogdestination : public logdestination {
	public:

		/** Creates an instance of the syslogdestination class. */
		syslogdestination();

		/** Creates an instance of the syslogdestination class
		 *  that is a copy of "f". */
		syslogdestination(const syslogdestination &f);

		/** Makes this instance of the syslogdestination class
		 *  identical to "f". */
		syslogdestination	&operator=(const syslogdestination &f);

		/** Deletes this instance of the syslogdestination class. */
		virtual		~syslogdestination();

		/** Opens the syslog as a logging destination using "option",
		 *  "facility" and "priority".  The string "ident" will be
		 *  prepended to each message. */
		void	open(const char *ident, int32_t option,
					int32_t facility, int32_t priority);

		/** Closes the syslog as a loggin destination. */
		void	close();
	#include <rudiments/private/syslogdestination.h>
};


class filedestinationprivate;

/** The filedestination class writes log data to a file. */
class RUDIMENTS_DLLSPEC filedestination : public logdestination {
	public:

		/** Creates an instance of the filedestination class. */
		filedestination();

		/** Creates an instance of the filedestination class
		 *  that is a copy of "f". */
		filedestination(const filedestination &f);

		/** Makes this instance of the filedestination class
		 *  identical to "f". */
		filedestination	&operator=(const filedestination &f);

		/** Deletes this instance of the filedestination class. */
		virtual		~filedestination();

		/** Opens file "filename" as a logging destination with
		 *  "perms" permissions. */
		bool	open(const char *filename, mode_t perms);

		/** Closes the file as a logging destination. */
		void	close();
	#include <rudiments/private/filedestination.h>
};


/** The stdoutdestination class writes log data to standard output. */
class RUDIMENTS_DLLSPEC stdoutdestination : public logdestination {
	#include <rudiments/private/stdoutdestination.h>
};


/** The stderrdestination class writes log data to standard error. */
class RUDIMENTS_DLLSPEC stderrdestination : public logdestination {
	#include <rudiments/private/stderrdestination.h>
};


typedef linkedlistnode<logdestination *>	loggerlistnode;
typedef linkedlist<logdestination *>		loggerlist;

class loggerprivate;

/** The logger class and associated logdestination classes provide methods for
 *  logging data from an application.  The following destinations are supported:
 *  	file, syslog, stdout and stderr
 * 
 *  To use these classes: instantiate one or more logdestinations,
 *  instantiate a logger, and add the logdestination instance(s) to the
 *  logger class instance using addLogDestination().  Then, each call to a
 *  logger class method will output logging information to each of the
 *  logdestinations in seqence. */
class RUDIMENTS_DLLSPEC logger {
	public:

		/** Creates an instance of the logger class. */
		logger();

		/** Deletes this instance of the logger class. */
		~logger();

		/** Add "logdest" to the list of destinations that
		 *  log entries will be written to. */
		void	addLogDestination(logdestination *logdest);

		/** Remove "logdest" from the list of destinations that
		 *  log entries will be written to. */
		void	removeLogDestination(logdestination *logdest);

		/** Remove all entries from the list of destinations that
		 *  log entries will be written to. */
		void	removeAllLogDestinations();


		/** Returns a string containing the current date,
		 *  followed by "name", followed by the process id in
		 *  brackets.  For example:
		 * 
		 *  01/05/2002 12:54:23 myprogram [2421]
		 * 
		 *  Note that this method allocates a string internally
		 *  and returns it.  This string must be deleted by the
		 *  calling program.
		 * 
		 *  Note that this method uses getSystemDateAndTime()
		 *  from the datetime class.  If you are using
		 *  logHeader() in a multi-threaded application, you
		 *  must supply a mutex to the datetime class using it's
		 *  setTimeMutex() method. */
		static char	*logHeader(const char *name);


		/** Writes "header", followed by "tabs" tabs, followed
		 *  by "string" as a single log entry. */
		void	write(const char *header, int32_t tabs,
							const char *string);

		/** Writes "header", followed by "tabs" tabs, followed
		 *  by "character" as a single log entry. */
		void	write(const char *header, int32_t tabs, char character);

		/** Writes "header", followed by "tabs" tabs, followed
		 *  by "number" as a single log entry. */
		void	write(const char *header, int32_t tabs, int32_t number);

		/** Writes "header", followed by "tabs" tabs, followed
		 *  by "number" as a single log entry. */
		void	write(const char *header, int32_t tabs, double number);

	#include <rudiments/private/logger.h>
};

#endif
