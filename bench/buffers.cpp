// Copyright (c) 2015  David Muse
// See the file COPYING for more information

#if defined(USE_STRING)
	#include <string>
#elif defined(USE_STRINGBUFFER)
	#include <rudiments/stringbuffer.h>
#endif
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

#define ITEMS 1024*1024*10
#define ITERS 10
//#define ITERS 160

int main(int argc, char **argv) {

#if defined(USE_STRING)
	std::string	str;
#elif defined(USE_STRINGBUFFER)
	stringbuffer	str;
#endif
	datetime	start;
	datetime	finish;

	char	block[1024];
	for (uint64_t b=0; b<sizeof(block); b++) {
		block[b]='a';
	}

	for (uint64_t i=1; i<=1024; i=i*2) {

		stdoutput.printf("append %d characters "
				"(%lld at a time, %d iterations)\n",
				ITEMS,i,ITERS);

		start.getSystemDateAndTime();
		for (uint64_t j=0; j<ITERS; j++) {

			for (uint64_t k=0; k<ITEMS/i; k++) {
				str.append(block,i);
			}

			str.clear();

			stdoutput.printf(".");
		}
		finish.getSystemDateAndTime();

		stdoutput.printf("\n%lld seconds\n\n",
				finish.getEpoch()-start.getEpoch());
	}
}
