// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

INLINE xmlsax::xmlsax() : errorhandler() {
	reset();
}

INLINE xmlsax::~xmlsax() {
	close();
}

INLINE void xmlsax::reset() {
	string=NULL;
	line=1;
}

INLINE void xmlsax::parseTagFailed() {
	clearError();
	appendError("error: parseTagFailed() failed at line ");
	appendError(line);
}

INLINE void xmlsax::parseAttributeFailed() {
	clearError();
	appendError("error: parseAttributeFailed() failed at line ");
	appendError(line);
}

INLINE void xmlsax::parseTextFailed() {
	clearError();
	appendError("error: parseText() failed at line ");
	appendError(line);
}

INLINE int xmlsax::tagStart(char *name) {
	// by default, just return success
	return 1;
}

INLINE int xmlsax::attributeName(char *name) {
	// by default, just return success
	return 1;
}

INLINE int xmlsax::attributeValue(char *value) {
	// by default, just return success
	return 1;
}

INLINE int xmlsax::text(char *string) {
	// by default, just return success
	return 1;
}

INLINE int xmlsax::tagEnd(char *name) {
	// by default, just return success
	return 1;
}

INLINE int xmlsax::comment(char *string) {
	// by default, just return success
	return 1;
}

INLINE int xmlsax::cdata(char *string) {
	// by default, just return success
	return 1;
}
