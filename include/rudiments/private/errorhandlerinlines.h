// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE char *errorhandler::getError() {
	return errorstr.getString();
}

RUDIMENTS_INLINE void errorhandler::clearError() {
	errorstr.clear();
}

RUDIMENTS_INLINE void errorhandler::appendError(const char *string) {
	errorstr.append(string);
}

RUDIMENTS_INLINE void errorhandler::appendError(char character) {
	errorstr.append(character);
}

RUDIMENTS_INLINE void errorhandler::appendError(long number) {
	errorstr.append(number);
}

RUDIMENTS_INLINE void errorhandler::appendError(double number) {
	errorstr.append(number);
}
