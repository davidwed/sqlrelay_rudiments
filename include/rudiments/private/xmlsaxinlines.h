// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE xmlsax::xmlsax() : errorhandler() {
	reset();
}

RUDIMENTS_INLINE xmlsax::~xmlsax() {
	close();
}

RUDIMENTS_INLINE void xmlsax::reset() {
	string=NULL;
	line=1;
}

RUDIMENTS_INLINE void xmlsax::parseTagFailed() {
	clearError();
	appendError("error: parseTagFailed() failed at line ");
	appendError(line);
}

RUDIMENTS_INLINE void xmlsax::parseAttributeFailed() {
	clearError();
	appendError("error: parseAttributeFailed() failed at line ");
	appendError(line);
}

RUDIMENTS_INLINE void xmlsax::parseTextFailed() {
	clearError();
	appendError("error: parseText() failed at line ");
	appendError(line);
}

RUDIMENTS_INLINE int xmlsax::tagStart(char *name) {
	// by default, just return success
	return 1;
}

RUDIMENTS_INLINE int xmlsax::attributeName(char *name) {
	// by default, just return success
	return 1;
}

RUDIMENTS_INLINE int xmlsax::attributeValue(char *value) {
	// by default, just return success
	return 1;
}

RUDIMENTS_INLINE int xmlsax::text(char *string) {
	// by default, just return success
	return 1;
}

RUDIMENTS_INLINE int xmlsax::tagEnd(char *name) {
	// by default, just return success
	return 1;
}

RUDIMENTS_INLINE int xmlsax::comment(char *string) {
	// by default, just return success
	return 1;
}

RUDIMENTS_INLINE int xmlsax::cdata(char *string) {
	// by default, just return success
	return 1;
}
