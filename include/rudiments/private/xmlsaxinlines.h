// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

inline xmlsax::xmlsax() : errorhandler() {
	reset();
}

inline xmlsax::~xmlsax() {
	close();
}

inline void xmlsax::reset() {
	string=NULL;
	filedescriptor=-1;
	line=1;
}

inline void xmlsax::parseTagFailed() {
	clearError();
	appendError("error: parseTagFailed() failed at line ");
	appendError(line);
}

inline void xmlsax::parseAttributeFailed() {
	clearError();
	appendError("error: parseAttributeFailed() failed at line ");
	appendError(line);
}

inline void xmlsax::parseTextFailed() {
	clearError();
	appendError("error: parseText() failed at line ");
	appendError(line);
}

inline int xmlsax::tagStart(char *name) {
	// by default, just return success
	return 1;
}

inline int xmlsax::attributeName(char *name) {
	// by default, just return success
	return 1;
}

inline int xmlsax::attributeValue(char *value) {
	// by default, just return success
	return 1;
}

inline int xmlsax::text(char *string) {
	// by default, just return success
	return 1;
}

inline int xmlsax::tagEnd(char *name) {
	// by default, just return success
	return 1;
}

inline int xmlsax::comment(char *string) {
	// by default, just return success
	return 1;
}

inline int xmlsax::cdata(char *string) {
	// by default, just return success
	return 1;
}
