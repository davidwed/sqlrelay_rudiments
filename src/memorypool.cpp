// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/memorypool.h>
#include <rudiments/bytestring.h>
#include <rudiments/linkedlist.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

//#define DEBUG_ALLOCATE 1
//#define DEBUG_DEALLOCATE 1

#if defined(DEBUG_ALLOCATE) || defined(DEBUG_DEALLOCATE)
	#include <rudiments/stdio.h>
#endif

class memorypoolnode {
	friend class memorypool;
	private:
			memorypoolnode(size_t size);
			~memorypoolnode();

		unsigned char	*_buffer;
		size_t		_size;
		size_t		_remaining;
		size_t		_position;
};

memorypoolnode::memorypoolnode(size_t size) {
	_buffer=new unsigned char[size];
	_size=size;
	_remaining=size;
	_position=0;
}

memorypoolnode::~memorypoolnode() {
	delete[] _buffer;
}

typedef	linkedlistnode<memorypoolnode *>	memorypoollistnode;
typedef	linkedlist<memorypoolnode *>		memorypoollist;

class memorypoolprivate {
	friend class memorypool;
	private:
		memorypoollist		_nodelist;
		memorypoollistnode	*_first;

		size_t	_initialsize;
		size_t	_increment;
		size_t	_resizeinterval;

		size_t	_deallocations;
		size_t	_total;
		size_t	_average;
};

// Pad to 8-byte boundary.  It's faster to access data aligned on a word
// boundary.  Some architectures (eg. sparc) require it and will throw a bus
// error if you don't.  Arguably we should make this optional for architectures
// that don't require it though, to conserve memory.
#define MEMORYPOOLPAD(a) ((8-(a%8))%8)

memorypool::memorypool(size_t initialsize,
			size_t increment,
			size_t resizeinterval) {
	pvt=new memorypoolprivate;

	pvt->_initialsize=initialsize+MEMORYPOOLPAD(initialsize);
	pvt->_increment=increment+MEMORYPOOLPAD(increment);
	pvt->_resizeinterval=resizeinterval;

	pvt->_deallocations=0;
	pvt->_total=0;
	pvt->_average=0;

	pvt->_nodelist.append(new memorypoolnode(pvt->_initialsize));
	pvt->_first=pvt->_nodelist.getFirst();
}

memorypool::~memorypool() {
	deallocate(false);
	delete pvt;
}

unsigned char *memorypool::allocate(size_t length) { 

	#ifdef DEBUG_ALLOCATE
	stdoutput.printf("allocate {\n");
	stdoutput.printf("	len  : %d",length);
	#endif

	length+=MEMORYPOOLPAD(length);

	#ifdef DEBUG_ALLOCATE
	stdoutput.printf(" (%d)\n",length);
	uint32_t	counter=0;
	#endif

	// look for a node with enough memory remaining
	memorypoollistnode	*node=pvt->_first;
	memorypoolnode		*memnode;
	while (node) {

		memnode=node->getValue();
		if (memnode->_remaining>=length) {
			break;
		}
		node=node->getNext();

		#ifdef DEBUG_ALLOCATE
		counter++;
		#endif
	}

	#ifdef DEBUG_ALLOCATE
	if (node) {
		stdoutput.printf("	node : %d (of %d)\n",
					counter,pvt->_nodelist.getLength());
	}
	#endif

	// if we didn't find a node with enough memory remaining
	// then create a new one at the end of the list
	if (!node) {

		// increase size by increments of at least 10% for better
		// performance and 10% max memory usage penalty - Claudio Freire
		size_t	incr=length;
		if (incr<pvt->_increment) {
			incr=pvt->_increment;
		}
		if (incr<(pvt->_total/10)) {
			incr=(pvt->_total/10);
			incr+=MEMORYPOOLPAD(incr);
		}
		memnode=new memorypoolnode(incr);
		pvt->_nodelist.append(memnode);

		#ifdef DEBUG_ALLOCATE
		stdoutput.printf("\n	allocating new new node "
						"(%d bytes)\n\n",incr);
		#endif
	}

	// add the length to the total
	pvt->_total+=length;

	#ifdef DEBUG_ALLOCATE
	stdoutput.printf("	total: %d\n",pvt->_total);
	#endif

	// get the buffer to return
	unsigned char	*buffer=memnode->_buffer+memnode->_position;

	// adjust position and bytes remaining in
	// the node that the buffer came from
	memnode->_position+=length;
	memnode->_remaining-=length;

	// balance the list...
	if (node!=pvt->_first) {

		memorypoolnode		*firstmemnode=pvt->_first->getValue();

		#ifdef DEBUG_ALLOCATE
		stdoutput.printf("\n	remain in this node : %d\n",
						memnode->_remaining);
		stdoutput.printf("	remain in first node: %d\n",
						firstmemnode->_remaining);
		#endif

		// If this node has 128 bytes more space remaining than the
		// first node has, and that is at least 10% more space
		// remaining than the first node has, then move it to the
		// front.  This tends to reorganize the list such that nodes
		// with more space remaining tend to be are nearer to the front
		// of the list and the next allocate has a better chance of
		// finding space without having to search too hard.  The
		// 128/10% hopes to prevent flip-flopping of nodes.
		if (memnode->_remaining>firstmemnode->_remaining) {
			size_t	difference=memnode->_remaining-
						firstmemnode->_remaining;
			if (!firstmemnode->_remaining ||
				(difference>128 &&
				difference>firstmemnode->_remaining/10)) {

				pvt->_nodelist.moveBefore(pvt->_first,node);
				pvt->_first=pvt->_nodelist.getFirst();

				#ifdef DEBUG_ALLOCATE
				stdoutput.printf("\n	moving node "
							"to front\n");
				#endif
			}
		}
	}

	#ifdef DEBUG_ALLOCATE
	stdoutput.printf("}\n");
	#endif

	// return the buffer
	return buffer;
}

unsigned char *memorypool::allocateAndClear(size_t length) {
	unsigned char	*buffer=allocate(length);
	bytestring::zero(buffer,length);
	return buffer;
}

void memorypool::deallocate() {
	deallocate(true);
}

void memorypool::deallocate(bool reinit) {

	#ifdef DEBUG_DEALLOCATE
	stdoutput.printf("deallocate {\n");
	#endif

	memorypoolnode		*first=NULL;

	if (reinit) {

		first=pvt->_first->getValue();

		pvt->_deallocations++;

		// update the running average
		pvt->_average-=pvt->_average/pvt->_deallocations;
		pvt->_average+=pvt->_total/pvt->_deallocations;

		#ifdef DEBUG_DEALLOCATE
		stdoutput.printf("	num: %d\n",pvt->_deallocations);
		stdoutput.printf("	avg: %d\n",pvt->_average);
		stdoutput.printf("	tot: %d\n",pvt->_total);
		#endif

		// if it's time to re-evaluate and
		// re-size the first node then do that
		if (pvt->_deallocations==pvt->_resizeinterval) {

			// reset initial size to the average amount
			// of memory allocated between each deallocation
			pvt->_initialsize=pvt->_average;

			// reset counters
			pvt->_deallocations=0;
			pvt->_average=0;

			// resize the first buffer to this size
			delete[] first->_buffer;
			first->_buffer=new unsigned char[pvt->_initialsize];
			first->_size=pvt->_initialsize;

			#ifdef DEBUG_DEALLOCATE
			stdoutput.printf("	resize {\n");
			stdoutput.printf("		initialsize: %d\n",
							pvt->_initialsize);
			stdoutput.printf("	}\n");
			#endif
		}

		// reset position and remaining on the first node
		first->_position=0;
		first->_remaining=first->_size;

		// reset total
		pvt->_total=0;
	}

	#ifdef DEBUG_DEALLOCATE
	stdoutput.printf("	clearing %d nodes\n",
					pvt->_nodelist.getLength());
	#endif

	// delete all nodes
	// (except for the first node, if we're reinit'ing)
	memorypoollistnode	*currentlistnode=
				(reinit)?pvt->_first->getNext():pvt->_first;
	memorypoollistnode	*nextlistnode;
	while (currentlistnode) {
		nextlistnode=currentlistnode->getNext();
		delete currentlistnode->getValue();
		currentlistnode=nextlistnode;
	}
	pvt->_nodelist.clear();

	// add back the new first node
	if (reinit) {
		pvt->_nodelist.append(first);
		pvt->_first=pvt->_nodelist.getFirst();
		#ifdef DEBUG_DEALLOCATE
		stdoutput.printf("	node count: %d\n",
					pvt->_nodelist.getLength());
		#endif
	}

	#ifdef DEBUG_DEALLOCATE
	if (!reinit) {
		stdoutput.printf("	final\n");
	}
	stdoutput.printf("}\n");
	#endif
}

void memorypool::print() {

	uint64_t		segmentindex=0;
	memorypoolnode		*memnode;

	for (memorypoollistnode *listnode=pvt->_first;
				listnode; listnode=listnode->getNext()) {
		memnode=listnode->getValue();
		stdoutput.printf("segment %lld(%x): (%d,%d)\n",
				segmentindex,memnode,
				(int)memnode->_size,(int)memnode->_position);
		for (size_t i=0; i<memnode->_position; i++) {
			if (memnode->_buffer[i]>=' ' && 
				memnode->_buffer[i]<='~') {
				stdoutput.printf("'%c'",memnode->_buffer[i]);
			} else {
				stdoutput.printf("%3d",
						(int)memnode->_buffer[i]);
			}
			if (!((i+1)%20)) {
				stdoutput.printf("\n");
			} else {
				stdoutput.printf(" ");
			}
		}
		stdoutput.printf("\n\n");
		segmentindex++;
	}
}
