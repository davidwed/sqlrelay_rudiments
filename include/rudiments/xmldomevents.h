// Copyright (c) 2015  David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLDOMEVENTS_H
#define RUDIMENTS_XMLDOMEVENTS_H

#include <rudiments/xmldom.h>
#include <rudiments/private/xmldomeventsincludes.h>

class xmldomeventsprivate;

typedef xmldomnode *(*xmldomeventhandler_t)(xmldomnode *codetreenode,
						xmldomnode *eventtreenode,
						void *data);

class RUDIMENTS_DLLSPEC xmldomevents {
	public:
		xmldomevents();
		~xmldomevents();

		bool	setEvents(const char *events);

		bool	setEventHandler(const char *event,
					xmldomeventhandler_t handler);

		bool	setData(void *data);

		bool	process(xmldomnode *codetreenode);

		void	setDebugLevel(uint8_t debuglevel);


		static	xmldomnode	*ignore(xmldomnode *codetreenode,
						xmldomnode *eventtreenode,
						void *data);

		static	xmldomnode	*rename(xmldomnode *codetreenode,
						xmldomnode *eventtreenode,
						void *data);

		static	xmldomnode	*revalue(xmldomnode *codetreenode,
						xmldomnode *eventtreenode,
						void *data);

	#include <rudiments/private/xmldomevents.h>
};

#endif
