// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/stringbuffer.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/stringbufferinlines.h>
#endif

stringbuffer	*stringbuffer::append(const char *string) {
	char	*oldbuffer=buffer;
	buffer=new char[((oldbuffer)?strlen(oldbuffer):0)+
				strlen(string)+1];
	sprintf(buffer,"%s%s",(oldbuffer)?oldbuffer:"",string);
	delete[] oldbuffer;
	return this;
}

stringbuffer	*stringbuffer::append(char character) {
	char	*oldbuffer=buffer;
	buffer=new char[((oldbuffer)?strlen(oldbuffer):0)+sizeof(char)+1];
	sprintf(buffer,"%s%c",(oldbuffer)?oldbuffer:"",character);
	delete[] oldbuffer;
	return this;
}

stringbuffer	*stringbuffer::append(long number) {
	char	*oldbuffer=buffer;
	buffer=new char[((oldbuffer)?strlen(oldbuffer):0)+21];
	sprintf(buffer,"%s%d",(oldbuffer)?oldbuffer:"",number);
	delete[] oldbuffer;
	return this;
}

stringbuffer	*stringbuffer::append(double number) {
	char	*oldbuffer=buffer;
	buffer=new char[((oldbuffer)?strlen(oldbuffer):0)+22];
	sprintf(buffer,"%s%f",(oldbuffer)?oldbuffer:"",number);
	delete[] oldbuffer;
	return this;
}

stringbuffer	*stringbuffer::append(double number,
						unsigned short precision,
						unsigned short scale) {
	char	*oldbuffer=buffer;
	buffer=new char[((oldbuffer)?strlen(oldbuffer):0)+22];
	sprintf(buffer,"%s%.*f",(oldbuffer)?oldbuffer:"",precision,number);
	delete[] oldbuffer;
	return this;
}
