// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>

int main(int argc, const char **argv) {
	
	logger			lg;

	syslogdestination	sd;
	filedestination		fd;
	stdoutdestination	sod;
	stderrdestination	sed;

	// initialize and add a syslog log destination
	sd.open("logtest",LOG_CONS,LOG_USER,LOG_INFO);
	lg.addLogDestination(&sd);

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
	lg.write(header,0,(long)12345);
	lg.write(header,0,123.45);
	delete[] header;

	// remove all log destinations
	lg.removeAllLogDestinations();

	// add each of the log destinations back
	lg.addLogDestination(&sd);
	lg.addLogDestination(&fd);
	lg.addLogDestination(&sod);
	lg.addLogDestination(&sed);

	// remove them one by one
	lg.removeLogDestination(&sd);
	lg.removeLogDestination(&fd);
	lg.removeLogDestination(&sod);
	lg.removeLogDestination(&sed);
	lg.removeAllLogDestinations();

	// close the syslog log destination
	sd.close();

	// close the file log destination
	fd.close();
}
