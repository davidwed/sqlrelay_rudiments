// Copyright (c) 1999-2001  David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/loggerinlines.h>
#endif

#include <rudiments/datetime.h>

#include <stdio.h>

char *logger::logHeader(const char *name) {
	datetime	dt;
	dt.getSystemDateAndTime();
	char	*dtstring=dt.getString();
	char	*retval=new char[strlen(dtstring)+strlen(name)+16];
	sprintf(retval,"%s %s [%d]",dtstring,name,getpid());
	return retval;
}

void logger::write(const char *header, int tabs, const char *string) const {
	char	*logentry=new char[strlen(header)+3+tabs+strlen(string)+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%s\n\n",logentry,string);
	write(logentry);
	delete[] logentry;
}

void logger::write(const char *header, int tabs, char character) const {
	char	*logentry=new char[strlen(header)+3+tabs+1+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%c\n\n",logentry,character);
	write(logentry);
	delete[] logentry;
}

void logger::write(const char *header, int tabs, long number) const {
	char	*logentry=new char[strlen(header)+3+tabs+20+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%ld\n\n",logentry,number);
	write(logentry);
	delete[] logentry;
}

void logger::write(const char *header, int tabs, double number) const {
	char	*logentry=new char[strlen(header)+3+tabs+21+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%f\n\n",logentry,number);
	write(logentry);
	delete[] logentry;
}

void logger::write(const char *logentry) const {
	loggerlistnode	*current=logdestlist.getNodeByIndex(0);
	while (current) {
		current->getData()->write(logentry);
		current=(loggerlistnode *)current->getNext();
	}
}
