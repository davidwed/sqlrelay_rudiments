// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline char *errorhandler::getError() {
	return errorstr.getString();
}

inline void errorhandler::clearError() {
	errorstr.clear();
}

inline void errorhandler::appendError(const char *string) {
	errorstr.append(string);
}

inline void errorhandler::appendError(char character) {
	errorstr.append(character);
}

inline void errorhandler::appendError(long number) {
	errorstr.append(number);
}

inline void errorhandler::appendError(double number) {
	errorstr.append(number);
}
