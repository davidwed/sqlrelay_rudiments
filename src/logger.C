// Copyright (c) 1999-2001  David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/loggerinlines.h>
#endif

#include <rudiments/datetime.h>

#include <stdio.h>

void	logger::addLogDestination(logdestination *logdest) {
	if (last) {
		last->next=new logdestnode;
		last->next->logdest=logdest;
		last->next->previous=last;
		last->next->next=NULL;
		last=last->next;
	} else {
		first=new logdestnode;
		first->logdest=logdest;
		first->previous=NULL;
		first->next=NULL;
		last=first;
	}
}

void	logger::removeLogDestination(logdestination *logdest) {
	logdestnode	*current=first;
	while (current) {
		if (current->logdest==logdest) {
			if (current->next) {
				current->next->previous=current->previous;
			}
			if (current->previous) {
				current->previous->next=current->next;
			}
			if (current==first) {
				first=current->next;
			}
			if (current==last) {
				last=current->previous;
			}
			logdestnode	*temp=current;
			current=current->next;
			delete temp;
			return;
		} else {
			current=current->next;
		}
	}
}

void	logger::removeAllLogDestinations() {
	logdestnode	*current=first;
	while (current) {
		last=current->next;
		delete current;
		current=last;
	}
	first=NULL;
	last=NULL;
}

char	*logger::logHeader(const char *name) {
	datetime	*dt=new datetime();
	char	*dtstring=dt->getString();
	char	*retval=new char[strlen(dtstring)+strlen(name)+16];
	sprintf(retval,"%s %s [%d]",dtstring,name,getpid());
	delete dt;
	return retval;
}

void	logger::write(const char *header, int tabs, const char *string) const {
	char	*logentry=new char[strlen(header)+3+tabs+strlen(string)+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%s\n\n",logentry,string);
	write(logentry);
	delete[] logentry;
}

void	logger::write(const char *header, int tabs, char character) const {
	char	*logentry=new char[strlen(header)+3+tabs+1+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%c\n\n",logentry,character);
	write(logentry);
	delete[] logentry;
}

void	logger::write(const char *header, int tabs, long number) const {
	char	*logentry=new char[strlen(header)+3+tabs+20+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%d\n\n",logentry,number);
	write(logentry);
	delete[] logentry;
}

void	logger::write(const char *header, int tabs, double number) const {
	char	*logentry=new char[strlen(header)+3+tabs+21+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%f\n\n",logentry,number);
	write(logentry);
	delete[] logentry;
}

void	logger::write(const char *logentry) const {
	logdestnode	*current=first;
	while (current) {
		current->logdest->write(logentry);
		current=current->next;
	}
}
