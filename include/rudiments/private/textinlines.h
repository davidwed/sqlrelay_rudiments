// Copyright (c) 2002 David Muse
// See the COPYING file for more information

INLINE void text::rightTrim(char *string) {
	rightTrim(string,' ');
}

INLINE void text::leftTrim(char *string) {
	leftTrim(string,' ');
}

INLINE void text::bothTrim(char *string) {
	bothTrim(string,' ');
}

INLINE void text::bothTrim(char *string, char character) {
	leftTrim(string,character);
	rightTrim(string,character);
}

INLINE int text::integerLength(long number) {
	int	length=(number>0)?0:1;
	for (long num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}
