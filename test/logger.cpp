// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>
#ifdef RUDIMENTS_HAVE_SYSLOG_H
	#include <sys/syslog.h>
#endif

int main(int argc, const char **argv) {
	
	logger			lg;

#ifdef RUDIMENTS_HAVE_SYSLOG_H
	syslogdestination	sd;
#endif
	filedestination		fd;
	stdoutdestination	sod;
	stderrdestination	sed;

#ifdef RUDIMENTS_HAVE_SYSLOG_H
	// initialize and add a syslog log destination
	sd.open("logtest",LOG_CONS,LOG_USER,LOG_INFO);
	lg.addLogDestination(&sd);
#endif

	// initialize and add a file log destination
	fd.open("test.log");
	lg.addLogDestination(&fd);

	// add a standard output log destination
	lg.addLogDestination(&sod);

	// add a standard error log destination
	lg.addLogDestination(&sed);

	// write a string, character, long and float value to the log, along
	// with a standard log header for each
	char	*header=logger::logHeader("logtest");
	lg.write(header,0,"test");
	lg.write(header,0,'t');
	lg.write(header,0,(int32_t)12345);
	lg.write(header,0,123.45);
	delete[] header;

	// remove all log destinations
	lg.removeAllLogDestinations();

	// add each of the log destinations back
#ifdef RUDIMENTS_HAVE_SYSLOG_H
	lg.addLogDestination(&sd);
#endif
	lg.addLogDestination(&fd);
	lg.addLogDestination(&sod);
	lg.addLogDestination(&sed);

	// remove them one by one
#ifdef RUDIMENTS_HAVE_SYSLOG_H
	lg.removeLogDestination(&sd);
#endif
	lg.removeLogDestination(&fd);
	lg.removeLogDestination(&sod);
	lg.removeLogDestination(&sed);
	lg.removeAllLogDestinations();

#ifdef RUDIMENTS_HAVE_SYSLOG_H
	// close the syslog log destination
	sd.close();
#endif

	// close the file log destination
	fd.close();
}
