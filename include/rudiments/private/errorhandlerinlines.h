// Copyright (c) 2002 David Muse
// See the COPYING file for more information

INLINE char *errorhandler::getError() {
	return errorstr.getString();
}

INLINE void errorhandler::clearError() {
	errorstr.clear();
}

INLINE void errorhandler::appendError(const char *string) {
	errorstr.append(string);
}

INLINE void errorhandler::appendError(char character) {
	errorstr.append(character);
}

INLINE void errorhandler::appendError(long number) {
	errorstr.append(number);
}

INLINE void errorhandler::appendError(double number) {
	errorstr.append(number);
}
