// Copyright (c) 2016  David Muse
// See the file COPYING for more information

#include <rudiments/avltree.h>
#include <rudiments/charstring.h>
#include <rudiments/randomnumber.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>
#include "test.cpp"


const int32_t	randomlow=10000;
const int32_t	randomhigh=99999;
const uint16_t	nodecount=4096;


// generates a random number
template<class type> void generateRandom(randomnumber *r, type *number) {
	int32_t	num=0;
	r->generateScaledNumber(10000,99999,&num);
	*number=(type)num;
}


// walk the tree, looking for errors in order, balance, or length
template<class type> void walk(bool forwards,
				avltree<type> *t,
				bool *ordererror,
				bool *balanceerror,
				bool *lengtherror) {

	*ordererror=false;
	*balanceerror=false;
	*lengtherror=false;

	uint64_t	length=0;
	type		prev;

	avltreenode<type> *node=(forwards)?t->getFirst():t->getLast();
	while (node) {

		if (length &&
			((forwards && node->compare(prev)==-1) ||
			(!forwards && node->compare(prev)==1))) {
			*ordererror=true;
		}
		prev=node->getValue();

		int64_t	factor=(int64_t)node->getLeftHeight()-
				(int64_t)node->getRightHeight();

		if (factor>1 || factor<-1) {
			*balanceerror=true;
		}

		if (forwards) {
			node=node->getNext();
		} else {
			node=node->getPrevious();
		}

		length++;
	}

	*lengtherror=(length!=t->getLength());
}


// walk the tree, doing a find() on each value to verify
// that it's found in the location that it's in
template<class type> void find(avltree<type> *t, bool *finderror) {

	for (avltreenode<type> *n=t->getFirst(); n; n=n->getNext()) {
		if (t->find(n->getValue())!=n) {
			*finderror=true;
			break;
		}
	}
	*finderror=false;
}


int main(int argc, char **argv) {

	header("avltree");

	// initialize random number generator
	randomnumber	r;
	uint32_t	seed=randomnumber::getSeed();
	stdoutput.printf("(seed: %d)\n\n",seed);
	if (argc==2) {
		seed=charstring::toInteger(argv[1]);
	}
	r.setSeed(seed);

	// error flags
	bool		ordererror=false;
	bool		balanceerror=false;
	bool		lengtherror=false;
	bool		finderror=false;


	// integers
	stdoutput.printf("integers: \n");
	avltree<int32_t>	intt;

	// insert
	for (uint16_t i=0; i<nodecount; i++) {

		// generate a random number
		int32_t	number=0;
		generateRandom(&r,&number);

		// insert the number
		intt.insert(number);

		// verify that the tree is still ok
		walk(true,&intt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		walk(false,&intt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		find(&intt,&finderror);
	}
	test("insert/order",!ordererror);
	test("insert/balance",!balanceerror);
	test("insert/length",!lengtherror);
	test("insert/find",!finderror);
	test("insert/ultimate length",intt.getLength()==nodecount);
	stdoutput.printf("\n");

	// remove
	for (avltreenode<int32_t> *node=intt.getTop();
					node; node=intt.getTop()) {

		// go left a random amount
		for (;;) {
			int32_t	yes=0;
			r.generateScaledNumber(0,1,&yes);
			if (yes && node->getLeftChild()) {
				node=node->getLeftChild();
			} else {
				break;
			}
		}

		// go right a random amount
		for (;;) {
			int32_t	yes=0;
			r.generateScaledNumber(0,1,&yes);
			if (yes && node->getRightChild()) {
				node=node->getRightChild();
			} else {
				break;
			}
		}

		// remove the node
		intt.remove(node);

		// verify that the tree is still ok
		walk(true,&intt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		walk(false,&intt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		find(&intt,&finderror);
	}
	test("remove/order",!ordererror);
	test("remove/balance",!balanceerror);
	test("remove/length",!lengtherror);
	test("remove/find",!finderror);
	test("remove/ultimate length",intt.getLength()==0);
	stdoutput.printf("\n");
}
