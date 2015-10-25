// Copyright (c) 2015  David Muse
// See the COPYING file for more information.

#include <rudiments/xmldomevents.h>

#if (defined(_MSC_VER) && (_MSC_VER <= 1300)) || \
	(defined(__USLC__) && !defined(__GNUC__)) || \
	defined(__VMS)

	// degenerate debug macros for really incapable compilers
	// (old microsoft compilers, unixware native compiler, openvms compiler)
	static void debugPrintIndent(uint8_t level) {}
	static void debugPrintf(uint8_t level, const char *format, ...) {}
	static void debugSafePrint(uint8_t level, const char *string) {}
	static void debugSafePrintLength(uint8_t level, const char *string,
							uint32_t length) {}
#else

	// working debug macros for more modern compilers
	#define debugPrintIndent(level) if (pvt->_debuglevel>=level) { for (uint32_t i=0; i<pvt->_depth; i++) { stdoutput.printf(" "); } }
	#ifdef _MSC_VER
		#define debugPrintf(level,ARGS,...) if (pvt->_debuglevel>=level) { stdoutput.printf(ARGS,__VA_ARGS__); stdoutput.flush(); }
	#else
		#define debugPrintf(level,ARGS...) if (pvt->_debuglevel>=level) { stdoutput.printf(ARGS); stdoutput.flush(); }
	#endif
	#define debugSafePrint(level,string) if (pvt->_debuglevel>=level) { stdoutput.safePrint(string); }
	#define debugSafePrintLength(level,string,length) if (pvt->_debuglevel>=level) { stdoutput.safePrint(string,length); }

#endif

class xmldomeventsprivate {
	friend class xmldomevents;
	private:
		xmldom		_etree;
		xmldomnode	*_eventsnode;
		void		*_data;
		uint8_t		_debuglevel;
};

xmldomevents::xmldomevents() {
	pvt=new xmldomeventsprivate;
	pvt->_eventsnode=NULL;
	pvt->_data=NULL;
	pvt->_debuglevel=0;
}

xmldomevents::~xmldomevents() {
}

bool xmldomevents::setEvents(const char *events) {

	debugPrintf(1,"\nsetting events... ");

	pvt->_eventsnode=NULL;

	if (!pvt->_etree.parseString(events)) {
		debugPrintf(1,"failed\n");
		return false;
	}

	pvt->_eventsnode=pvt->_etree.getRootNode()->getFirstTagChild("events");

	debugPrintf(1,"success\n");
	return true;
}

bool xmldomevents::setEventHandler(const char *event,
					xmldomeventhandler_t handler) {

	debugPrintf(1,"setting event handler for %s... ",event);

	// walk the event tree...
	uint64_t	count=0;
	for (xmldomnode *node=pvt->_etree.getRootNode();
				!node->isNullNode();
				node=node->getNextTag()) {

		// for each instance where "event" is referenced...
		if (!charstring::compare(
				node->getAttributeValue("event"),event)) {

			// attach the event handler to the node
			node->setData((void *)handler);
			count++;
		}
	}

	debugPrintf(1,"%lld instances\n",count);

	return true;
}

bool xmldomevents::setData(void *data) {
	pvt->_data=data;
	return true;
}

bool xmldomevents::process(xmldomnode *codetreenode) {

	debugPrintf(1,"process {\n");

	// walk the code tree...
	while (!codetreenode->isNullNode()) {

		// find the corresponding node in the event tree
		xmldomnode	*etnode=findEvent(codetreenode);

		// if there is no corresponding node, then move on
		if (etnode->isNullNode()) {
			codetreenode=codetreenode->getNextTag();
			continue;
		}

		// get the event handler
		xmldomeventhandler_t	handler=
					(xmldomeventhandler_t)etnode->getData();

		// call the event handler
		xmldomnode	*next=(handler)?
					handler(codetreenode,etnode,pvt->_data):
					codetreenode;

		// bail if it returned null node
		if (next->isNullNode()) {
			debugPrintf(1,"} failed\n");
			return false;
		}

		// continue walking from the node that the handler returned
		// (or the next tag node if it returned the current node)
		if (next!=codetreenode) {
			codetreenode=next;
		} else {
			codetreenode=codetreenode->getNextTag();
		}
	}

	debugPrintf(1,"} success\n");
	return true;
}

xmldomnode *xmldomevents::findEvent(xmldomnode *codetreenode) {

	debugPrintf(1,"  %s : ",codetreenode->getName());

	// get the name of the current code tree node, we'll need them later...
	const char	*ctnodename=codetreenode->getName();
	const char	*ctnodevalue=codetreenode->getAttributeValue("value");

	// We need to figure out where to start in the event tree.  If the
	// parent of this codetreenode has an event tree node attached to it,
	// then start there.  If it doesn't, then start at the top of the 
	// event node tree.
	xmldomnode	*p=(xmldomnode *)codetreenode->getParent()->getData();
	if (!p || p->isNullNode()) {
		p=pvt->_eventsnode;
	}

	debugPrintf(1,"%s... ",p->getName());

	// walk the children of the parent event tree node...
	xmldomnode *c=p->getFirstTagChild(ctnodename);
	while (!c->isNullNode()) {

		// if the event tree node matches the code tree node...
		if (!charstring::compare(c->getName(),ctnodename)) {

			// test values too, if necessary...
			const char	*v=c->getAttributeValue("value");
			if (charstring::isNullOrEmpty(v)) {
				break;
			}
			if (!charstring::compare(v,ctnodevalue)) {
				break;
			}
		}

		// move on
 		c=c->getNextTagSibling(ctnodename);
	}

	if (pvt->_debuglevel) {
		if (!c->isNullNode() && c->getAttributeValue("event")) {
			debugPrintf(1,"%s\n",c->getAttributeValue("event"));
		} else {
			debugPrintf(1,"\n");
		}
	}

	// attach the corresponding event tree node to the code tree node
	codetreenode->setData(c);

	return c;
}

void xmldomevents::setDebugLevel(uint8_t debuglevel) {
	pvt->_debuglevel=debuglevel;
}
