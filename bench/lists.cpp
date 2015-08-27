// Copyright (c) 2015  David Muse
// See the file COPYING for more information

#if defined(USE_LIST)
	#include <list>
#elif defined(USE_FORWARD_LIST)
	#include <forward_list>
#elif defined(USE_VECTOR)
	#include <vector>
#elif defined(USE_ARRAY)
	#include <array>
#elif defined(USE_DYNAMICARRAY)
	#include <rudiments/dynamicarray.h>
#elif defined(USE_STATICARRAY)
	#include <rudiments/staticarray.h>
#elif defined(USE_LINKEDLIST)
	#include <rudiments/linkedlist.h>
#elif defined(USE_SINGLYLINKEDLIST)
	#include <rudiments/singlylinkedlist.h>
#endif
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

#define ITEMS 1024*1024
#define ITERS 320

int main(int argc, char **argv) {

#if defined(USE_LIST)
	std::list<uint64_t>		l;
#elif defined(USE_FORWARD_LIST)
	std::forward_list<uint64_t>	l;
#elif defined(USE_VECTOR)
	std::vector<uint64_t>		l;
#elif defined(USE_ARRAY)
	std::array<uint64_t,ITEMS>	l;
#elif defined(USE_DYNAMICARRAY)
	dynamicarray<uint64_t>		l;
#elif defined(USE_STATICARRAY)
	staticarray<uint64_t,ITEMS>	l;
#elif defined(USE_LINKEDLIST)
	linkedlist<uint64_t>		l;
#elif defined(USE_SINGLYLINKEDLIST)
	singlylinkedlist<uint64_t>	l;
#endif
	datetime	start;
	datetime	finish;

	stdoutput.printf("appending %d items (%d times)\n",ITEMS,ITERS);

	start.getSystemDateAndTime();
	for (uint64_t j=0; j<ITERS; j++) {

		for (uint64_t k=0; k<ITEMS; k++) {
			#if defined(USE_LIST) || \
					defined(USE_VECTOR)
				l.push_back(100);
			#elif defined(USE_FORWARD_LIST)
				l.push_front(100);
			#elif defined(USE_ARRAY) || \
					defined(USE_DYNAMICARRAY) || \
					defined(USE_STATICARRAY)
				l[k]=100;
			#else
				l.append(100);
			#endif
		}

		#if !defined(USE_ARRAY)
		l.clear();
		#endif

		stdoutput.printf(".");
	}
	finish.getSystemDateAndTime();

	stdoutput.printf("\n%lld seconds\n\n",
			finish.getEpoch()-start.getEpoch());
}
