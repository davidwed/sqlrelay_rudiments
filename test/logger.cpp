// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>
#include <rudiments/permissions.h>
#include <rudiments/datetime.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#ifdef RUDIMENTS_HAVE_SYSLOG_H
	#include <syslog.h>
#endif
#include "test.cpp"

int main(int argc, const char **argv) {

	header("logger");

	file::remove("test.log");
	
	logger			lg;

	// initialize the log destinations
	#ifdef RUDIMENTS_HAVE_SYSLOG_H
	syslogdestination	sd;
	sd.open("logtest",LOG_CONS,LOG_USER,LOG_INFO);
	#endif
	filedestination		fd;
	test("file",fd.open("test.log",
			permissions::evalPermString("rw-------")));
	stdoutdestination	sod;
	stderrdestination	sed;

	// add log destinations
	#ifdef RUDIMENTS_HAVE_SYSLOG_H
	lg.addLogDestination(&sd);
	#endif
	lg.addLogDestination(&fd);
	lg.addLogDestination(&sod);
	lg.addLogDestination(&sed);


	// some crash tests...

	// remove log destnations all at once
	lg.removeAllLogDestinations();

	// add them back
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

	// remove them all at once (even though none should currently be added)
	lg.removeAllLogDestinations();


	// get the current date/time so we can verify the header
	// (make sure we're not about to bump over to the next minute)
	datetime	dt;
	dt.getSystemDateAndTime();
	if (dt.getSeconds()>=58) {
		snooze::macrosnooze(3);
	}

	// create and verify the header
	char	*header=logger::logHeader("logtest");
	test("header month",charstring::toInteger(header)==dt.getMonth());
	test("header day",charstring::toInteger(header+3)==dt.getDayOfMonth());
	test("header year",charstring::toInteger(header+6)==dt.getYear());
	test("header hour",charstring::toInteger(header+11)==dt.getHour());
	test("header minute",charstring::toInteger(header+14)==dt.getMinutes());
	test("header program",!charstring::compare(header+24,"logtest ",8));
	test("header pid",charstring::toInteger(header+33)==
					process::getProcessId());

	// write various log messages (even though no destinations exist)
	lg.write(header,0,"test");
	lg.write(header,0,'t');
	lg.write(header,0,(int32_t)12345);
	lg.write(header,0,123.45);
	lg.write("",0,"test");
	lg.write("",0,'t');
	lg.write("",0,(int32_t)12345);
	lg.write("",0,123.45);
	lg.write(NULL,0,"test");
	lg.write(NULL,0,'t');
	lg.write(NULL,0,(int32_t)12345);
	lg.write(NULL,0,123.45);

	// re-add log destinations
	#ifdef RUDIMENTS_HAVE_SYSLOG_H
	lg.addLogDestination(&sd);
	#endif
	lg.addLogDestination(&fd);
	lg.addLogDestination(&sod);
	lg.addLogDestination(&sed);

	// write various log messages, with and without header
	// (now destinations do exist)
	lg.write(header,0,"test");
	lg.write(header,0,'t');
	lg.write(header,0,(int32_t)12345);
	lg.write(header,0,123.45);
	lg.write("",0,"test");
	lg.write("",0,'t');
	lg.write("",0,(int32_t)12345);
	lg.write("",0,123.45);
	lg.write(NULL,0,"test");
	lg.write(NULL,0,'t');
	lg.write(NULL,0,(int32_t)12345);
	lg.write(NULL,0,123.45);

	// verify log file contents
	stringbuffer	testcontents;
	testcontents.append(header)->append(" : test\n");
	testcontents.append(header)->append(" : t\n");
	testcontents.append(header)->append(" : 12345\n");
	testcontents.append(header)->append(" : 123.4500\n");
	testcontents.append("test\n");
	testcontents.append("t\n");
	testcontents.append("12345\n");
	testcontents.append("123.4500\n");
	testcontents.append("test\n");
	testcontents.append("t\n");
	testcontents.append("12345\n");
	testcontents.append("123.4500\n");
	char	*testlog=file::getContents("test.log");
	test("contents",!charstring::compare(testcontents.getString(),testlog));
	delete[] testlog;

	// close the log destinations
	#ifdef RUDIMENTS_HAVE_SYSLOG_H
	sd.close();
	#endif
	fd.close();

	// clean up
	delete[] header;
	file::remove("test.log");
}
