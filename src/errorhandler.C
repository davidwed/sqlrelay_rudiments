// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/errorhandler.h>

char *errorhandler::getError() {
	return errorstr.getString();
}

void errorhandler::clearError() {
	errorstr.clear();
}

void errorhandler::appendError(const char *string) {
	errorstr.append(string);
}

void errorhandler::appendError(char character) {
	errorstr.append(character);
}

void errorhandler::appendError(long number) {
	errorstr.append(number);
}

void errorhandler::appendError(double number) {
	errorstr.append(number);
}
