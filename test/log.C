// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>

class myclass {
	public:
		void	test();
		logger			lg;
		syslogdestination	sd;
		filedestination		fd;
		stdoutdestination	sod;
		stderrdestination	sed;
};

void	myclass::test() {
	sd.open("logtest",LOG_CONS,LOG_USER,LOG_INFO);
	lg.addLogDestination(&sd);
	fd.open("test.log");
	lg.addLogDestination(&fd);
	lg.addLogDestination(&sod);
	lg.addLogDestination(&sed);
	char	*header=logger::logHeader("logtest");
	lg.write(header,0,"test");
	lg.write(header,0,'t');
	lg.write(header,0,(long)12345);
	lg.write(header,0,123.45);
	delete[] header;
	lg.removeAllLogDestinations();
	lg.addLogDestination(&sd);
	lg.addLogDestination(&fd);
	lg.addLogDestination(&sod);
	lg.addLogDestination(&sed);
	lg.removeLogDestination(&sd);
	lg.removeLogDestination(&fd);
	lg.removeLogDestination(&sod);
	lg.removeLogDestination(&sed);
	lg.removeAllLogDestinations();
	sd.close();
	fd.close();
}

int	main() {
	myclass	m;
	m.test();
}
