// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/parameterstring.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/parameterstringinlines.h>
#endif

parameterstring::~parameterstring() {
	// delete each name and value in the list
	for (unsigned int i=0; i<nvp.getList()->getLength(); i++) {
		delete[] nvp.getList()->getNodeByIndex(i)->getData()->getKey();
		delete[] nvp.getList()->getNodeByIndex(i)->getData()->getData();
	}
}

int parameterstring::parse(const char *paramstring) {

	nvp.getList()->clear();

	int	paircount=countPairs(paramstring);

	char	*ptr=(char *)paramstring;
	for (int i=0; i<paircount; i++) {

		char	*namebuffer;
		char	*valuebuffer;

		ptr=parseName(ptr,&namebuffer);

		if (*ptr=='=') {
			ptr++;
		} else {
			nvp.setData(namebuffer,NULL);
			return 0;
		}

		ptr=parseValue(ptr,&valuebuffer);

		nvp.setData(namebuffer,valuebuffer);

		if (*ptr==';') {
			ptr++;
		} else if (!*ptr) {
			break;
		} else {
			return 0;
		}
	}

	return 1;
}

int parameterstring::countPairs(const char *paramstring) {

	// count ;'s that are not inside of quotes
	char	*ptr;
	int	paircount=0;
	int	inquotes=0;
	for (ptr=(char *)paramstring; (*ptr); ptr++) {

		// handle quotes
		if (*ptr=='\'') {
			inquotes=!inquotes;
			continue;
		}

		// handle escaped characters
		if (*ptr=='\\') {
			ptr++;
			continue;
		}

		if (!inquotes && *ptr==';') {
			paircount++;
		}
	}

	// handle case where final character wasn't a ;
	if (*(ptr-1)!=';') {
		paircount++;
	}

	return paircount;
}

char *parameterstring::parsePart(int len, char delimiter,
					const char *data, char **outbuffer,
						int quotes, int escapedchars) {

	char	*ptr=(char *)data;

	char	*buffer=new char[len+1];
	buffer[len]=(char)NULL;

	int	inquotes=0;
	int	index=0;
	while (*ptr && *ptr!=delimiter) {

		// handle quotes
		if (quotes && *ptr=='\'') {
			ptr++;
			if (inquotes) {
				break;
			} else {
				inquotes=1;
				continue;
			}
		}

		// handle escaped characters
		if (escapedchars && *ptr=='\\') {
			ptr++;
			if (!(*ptr)) {
				break;
			}
		}

		buffer[index]=*ptr;
		index++;
		ptr++;
	}

	*outbuffer=buffer;

	return ptr;
}

int parameterstring::parsePartLength(const char *data, char delimiter,
					int quotes, int escapedchars) {

	int	counter=0;
	char	*ptr=(char *)data;

	int	inquotes=0;
	while (*ptr && *ptr!=delimiter) {

		// handle quotes
		if (quotes && *ptr=='\'') {
			ptr++;
			if (inquotes) {
				break;
			} else {
				inquotes=1;
				continue;
			}
		}

		// handle escaped characters
		if (escapedchars && *ptr=='\\') {
			ptr++;
			if (!(*ptr)) {
				break;
			}
		}

		counter++;
		ptr++;
	}
	return counter;
}
