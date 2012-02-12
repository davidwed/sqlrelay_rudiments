// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LOGGER_H
#define RUDIMENTS_LOGGER_H

#include <rudiments/private/loggerincludes.h>

// The logger class and associated logdestination classes provide methods for
// logging data from an application.  The following destinations are supported:
// 	file, syslog, stdout and stderr
//
// To use these classes: instantiate one or more logdestinations,
// instantiate a logger, and add the logdestination instance(s) to the
// logger class instance using addLogDestination().  Then, each call to a
// logger class method will output logging information to each of the
// logdestinations in seqence.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif
 
// The logdestination class is a virtual base class that just defines methods
// that the real logdestination classes must implement.
class logdestination {
	#include <rudiments/private/logdestination.h>
};


// The syslogdestination class writes log data to syslog.
class syslogdestinationprivate;
class DLLSPEC syslogdestination : public logdestination {
	public:
				syslogdestination();
				syslogdestination(const syslogdestination &f);
		syslogdestination	&operator=(const syslogdestination &f);
		virtual		~syslogdestination();

		void	open(const char *ident, int32_t option,
					int32_t facility, int32_t priority);
		void	close();
	#include <rudiments/private/syslogdestination.h>
};


// The filedestination class writes log data to a file.
class filedestinationprivate;
class DLLSPEC filedestination : public logdestination {
	public:
				filedestination();
				filedestination(const filedestination &f);
		filedestination	&operator=(const filedestination &f);
		virtual		~filedestination();

		bool	open(const char *filename);
		void	close();
	#include <rudiments/private/filedestination.h>
};


// The stdoutdestination class writes log data to standard output.
class DLLSPEC stdoutdestination : public logdestination {
	#include <rudiments/private/stdoutdestination.h>
};


// The stderrdestination class writes log data to standard error.
class DLLSPEC stderrdestination : public logdestination {
	#include <rudiments/private/stderrdestination.h>
};


typedef linkedlist<logdestination *>		loggerlist;
typedef linkedlistnode<logdestination *>	loggerlistnode;

// The logger class provides methods for managing
// logdestinations and writing log entries.
class loggerprivate;
class DLLSPEC logger {
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
			// Note that this method allocates a string internally
			// and returns it.  This string must be deleted by the
			// calling program.
			//
			// Note that this method uses getSystemDateAndTime()
			// from the datetime class.  If you are using
			// logHeader() in a multi-threaded application, you
			// must supply a mutex to the datetime class using it's
			// setTimeMutex() method.


		// These methods write log entries to each
		// logdestination in sequence.
		void	write(const char *header, int32_t tabs,
							const char *string);
			// Writes "header", followed by "tabs" tabs, followed
			// by "string" as a single log entry.
		void	write(const char *header, int32_t tabs, char character);
			// Writes "header", followed by "tabs" tabs, followed
			// by "character" as a single log entry.
		void	write(const char *header, int32_t tabs, int32_t number);
			// Writes "header", followed by "tabs" tabs, followed
			// by "number" as a single log entry.
		void	write(const char *header, int32_t tabs, double number);
			// Writes "header", followed by "tabs" tabs, followed
			// by "number" as a single log entry.

	#include <rudiments/private/logger.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
