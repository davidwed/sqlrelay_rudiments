// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

template <class datatype>
inline void listutil<datatype>::print(datatype data) const {
	printf("%x\n",data);
}

template <class datatype>
inline int listutil<datatype>::
			compare(datatype data1, datatype data2) const {
	if (data1<data2) {
		return -1;
	} else if (data1==data2) {
		return 0;
	} else {
		return 1;
	}
}

inline void listutil<char *>::print(char *data) const {
	printf("%s\n",data);
}

inline int listutil<char *>::compare(char *data1, char *data2) const {
	return !strcmp(data1,data2);
}

inline void listutil<char>::print(char data) const {
	printf("%d\n",data);
}

inline void listutil<int>::print(int data) const {
	printf("%d\n",data);
}

inline void listutil<short>::print(short data) const {
	printf("%d\n",data);
}

inline void listutil<long>::print(long data) const {
	printf("%d\n",data);
}

inline void listutil<float>::print(float data) const {
	printf("%f\n",data);
}

inline void listutil<double>::print(double data) const {
	printf("%d\n",data);
}
