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

RUDIMENTS_INLINE bool xmlsax::tagStart(char *name) {
	// by default, just return success
	return true;
}

RUDIMENTS_INLINE bool xmlsax::attributeName(char *name) {
	// by default, just return success
	return true;
}

RUDIMENTS_INLINE bool xmlsax::attributeValue(char *value) {
	// by default, just return success
	return true;
}

RUDIMENTS_INLINE bool xmlsax::text(char *string) {
	// by default, just return success
	return true;
}

RUDIMENTS_INLINE bool xmlsax::tagEnd(char *name) {
	// by default, just return success
	return true;
}

RUDIMENTS_INLINE bool xmlsax::comment(char *string) {
	// by default, just return success
	return true;
}

RUDIMENTS_INLINE bool xmlsax::cdata(char *string) {
	// by default, just return success
	return true;
}
