// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline void text::rightTrim(char *string) {
	rightTrim(string,' ');
}

inline void text::leftTrim(char *string) {
	leftTrim(string,' ');
}

inline void text::bothTrim(char *string) {
	bothTrim(string,' ');
}

inline void text::bothTrim(char *string, char character) {
	leftTrim(string,character);
	rightTrim(string,character);
}

inline int text::integerLength(long number) {
	int	length=(number>0)?0:1;
	for (long num=number; num>0; num=num/10) {
		length++;
	}
	return length;
}
