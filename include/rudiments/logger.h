// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LOGGER_H
#define RUDIMENTS_LOGGER_H

#include <rudiments/list.h>

#include <syslog.h>

// The logger class and associated logdestination classes provide methods for
// logging data from an application.  The following destinations are supported:
// 	file, syslog, stdout and stderr
//
// To use these classes: instantiate one or more logdestinations,
// instantiate a logger, and add the logdestination instance(s) to the
// logger class instance using addLogDestination().  Then, each call to a
// logger class method will output logging information to each of the
// logdestinations in seqence.


 
// The logdestination class is a virtual base class that just defines methods
// that the real logdestination classes must implement.
class logdestination {
	#include <rudiments/private/logdestination.h>
};


// The syslogdestination class writes log data to syslog.
class syslogdestination : public logdestination {
	public:
		void	open(const char *ident, int option,
					int facility, int priority);
		void	close();
	#include <rudiments/private/syslogdestination.h>
};


// The filedestination class writes log data to a file.
class filedestination : public logdestination {
	public:
			filedestination();
		int	open(const char *filename);
		void	close();
	#include <rudiments/private/filedestination.h>
};


// The filedestination class writes log data to standard output.
class stdoutdestination : public logdestination {
	#include <rudiments/private/stdoutdestination.h>
};


// The filedestination class writes log data to standard error.
class stderrdestination : public logdestination {
	#include <rudiments/private/stderrdestination.h>
};


typedef list<logdestination *>		loggerlist;
typedef listnode<logdestination *>	loggerlistnode;

// The logger class provides methods for managing
// logdestinations and writing log entries.
class logger {
	public:
			logger();
			~logger();

		// These methods add or remove logdestinations.
		void	addLogDestination(logdestination *logdest);
			// Add "logdest" to the list of destinations that
			// log entries will be written to.
		void	removeLogDestination(logdestination *logdest);
			// Remove "logdest" from the list of destinations that
			// log entries will be written to.
		void	removeAllLogDestinations();
			// Remove all entries from the list of destinations that
			// log entries will be written to.


		static char	*logHeader(const char *name);
			// Returns a string containing the current date,
			// followed by "name", followed by the process id in
			// brackets.  For example:
			//
			// 01/05/2002 12:54:23 myprogram [2421]
			//
			// (Note that this method allocates a string internally
			// and returns it.  This string must be deleted by the
			// calling program.)


		// These methods write log entries to each
		// logdestination in sequence.
		void	write(const char *header, int tabs,
						const char *string) const;
			// Writes "header", followed by "tabs" tabs, followed
			// by "string" as a single log entry.
		void	write(const char *header, int tabs,
						char character) const;
			// Writes "header", followed by "tabs" tabs, followed
			// by "character" as a single log entry.
		void	write(const char *header, int tabs,
						long number) const;
			// Writes "header", followed by "tabs" tabs, followed
			// by "number" as a single log entry.
		void	write(const char *header, int tabs,
						double number) const;
			// Writes "header", followed by "tabs" tabs, followed
			// by "number" as a single log entry.

	#include <rudiments/private/logger.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/loggerinlines.h>
#endif

#endif
