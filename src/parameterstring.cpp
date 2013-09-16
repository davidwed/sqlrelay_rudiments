// Copyright (c) 2002 David Muse
// See the COPYING file for more information

//#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/parameterstring.h>

// for NULL
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

class parameterstringprivate {
	friend class parameterstring;
	private:
		namevaluepairs	_nvp;
		char		_delim;
};

parameterstring::parameterstring() {
	pvt=new parameterstringprivate;
	pvt->_delim=';';
}

parameterstring::~parameterstring() {
	// delete each name and value in the list
	for (dictionarylistnode< char *, char * > *node=
			pvt->_nvp.getList()->getFirstNode(); node;
		node=(dictionarylistnode< char *, char* > *)node->getNext()) {
		delete[] node->getData()->getKey();
		delete[] node->getData()->getData();
	}
	delete pvt;
}

void parameterstring::setDelimiter(char delim) {
	pvt->_delim=delim;
}

bool parameterstring::parse(const char *paramstring) {

	pvt->_nvp.getList()->clear();

	int32_t	paircount=countPairs(paramstring);

	const char	*ptr=paramstring;
	for (int32_t i=0; i<paircount; i++) {

		char	*namebuffer;
		char	*valuebuffer;

		ptr=parseName(ptr,&namebuffer);

		if (*ptr=='=') {
			ptr++;
		} else {
			pvt->_nvp.setData(namebuffer,NULL);
			return false;
		}

		ptr=parseValue(ptr,&valuebuffer);

		pvt->_nvp.setData(namebuffer,valuebuffer);

		if (*ptr==pvt->_delim) {
			ptr++;
		} else if (!*ptr) {
			break;
		} else {
			return false;
		}
	}

	return true;
}

const char *parameterstring::getValue(const char *name) {
	char	*retval;
	return (pvt->_nvp.getData(const_cast<char *>(name),&retval))?
								retval:NULL;
}

void parameterstring::clear() {
	pvt->_nvp.clear();
}

int32_t parameterstring::countPairs(const char *paramstring) {

	// count ;'s that are not inside of quotes
	const char	*ptr;
	int32_t		paircount=0;
	int32_t		inquotes=0;
	for (ptr=paramstring; (*ptr); ptr++) {

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

		if (!inquotes && *ptr==pvt->_delim) {
			paircount++;
		}
	}

	// handle case where final character wasn't a ;
	if (*(ptr-1)!=pvt->_delim) {
		paircount++;
	}

	return paircount;
}

const char *parameterstring::parsePart(int32_t len, char delimiter,
					const char *data,
					char **outbuffer,
					int32_t quotes, int32_t escapedchars) {

	const char	*ptr=data;

	char	*buffer=new char[len+1];
	buffer[len]='\0';

	int32_t	inquotes=0;
	int32_t	index=0;
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

int32_t parameterstring::parsePartLength(const char *data, char delimiter,
					int32_t quotes, int32_t escapedchars) {

	const char	*ptr=data;
	int32_t		counter=0;

	int32_t	inquotes=0;
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

const char *parameterstring::parseName(const char *data, char **outbuffer) {
	return parsePart(parseNameLength(data),'=',data,outbuffer,0,0);
}

const char *parameterstring::parseValue(const char *data, char **outbuffer) {
	return parsePart(parseValueLength(data),pvt->_delim,data,outbuffer,1,1);
}

int32_t parameterstring::parseNameLength(const char *data) {
	return parsePartLength(data,'=',0,0);
}

int32_t parameterstring::parseValueLength(const char *data) {
	return parsePartLength(data,pvt->_delim,1,1);
}
