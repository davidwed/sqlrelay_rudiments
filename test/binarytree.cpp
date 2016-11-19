// Copyright (c) 2016  David Muse
// See the file COPYING for more information

#include <rudiments/binarytree.h>
#include <rudiments/charstring.h>
#include <rudiments/randomnumber.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, char **argv) {

	header("binarytree");

	binarytree<int32_t>	intt;

	randomnumber	r;

	uint32_t	seed=randomnumber::getSeed();
	if (argc==2) {
		seed=charstring::toInteger(argv[1]);
	}
	stdoutput.printf("seed: %d\n",seed);
	r.setSeed(seed);

	for (uint16_t i=0; i<10240; i++) {
		int32_t	number=0;
		r.generateScaledNumber(10000,99999,&number);
		intt.insert(number);
	}

	intt.print();
	stdoutput.printf("node count: %d\n",intt.getLength());
	uint64_t	i=0;
	int32_t		prev=0;
	for (binarytreenode<int32_t> *node=intt.getFirst();
					node; node=node->getNext()) {
		stdoutput.printf("%d: %d\n",i++,node->getValue());
		if (node->getValue()<prev) {
			stdoutput.printf("error!!!!  %d<%d\n",
						node->getValue(),prev);
			break;
		}
		prev=node->getValue();
	}
	if (i!=intt.getLength()) {
		stdoutput.printf("error!!!!  forward traversal didn't return %lld values (only returned %lld)\n",intt.getLength(),i);
	}
	for (binarytreenode<int32_t> *node=intt.getLast();
					node; node=node->getPrevious()) {
		stdoutput.printf("%d: %d\n",--i,node->getValue());
		if (node->getValue()>prev) {
			stdoutput.printf("error!!!!  %d>%d\n",
						node->getValue(),prev);
			break;
		}
		prev=node->getValue();
	}
	if (i!=0) {
		stdoutput.printf("error!!!!  backwards traversal didn't return %lld values (only returned %lld)\n",intt.getLength(),intt.getLength()-i);
	}
}
