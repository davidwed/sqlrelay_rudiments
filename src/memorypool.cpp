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
//#define DEBUG_RESIZE 1

#if defined(DEBUG_ALLOCATE) || \
		defined(DEBUG_DEALLOCATE) || \
		defined(DEBUG_RESIZE)
	#include <rudiments/stdio.h>
#endif

class memorypoolbuffer {
	friend class memorypool;
	private:
			memorypoolbuffer(size_t size);
			~memorypoolbuffer();

		unsigned char	*_buffer;
		size_t		_size;
		size_t		_remaining;
		size_t		_position;
};

memorypoolbuffer::memorypoolbuffer(size_t size) {
	_buffer=new unsigned char[size];
	_size=size;
	_remaining=size;
	_position=0;
}

memorypoolbuffer::~memorypoolbuffer() {
	delete[] _buffer;
}

typedef	linkedlistnode<memorypoolbuffer *>	memorypoollistnode;
typedef	linkedlist<memorypoolbuffer *>		memorypoollist;

class memorypoolprivate {
	friend class memorypool;
	private:
		memorypoollist		_bufferlist;
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

	pvt->_bufferlist.append(new memorypoolbuffer(pvt->_initialsize));
	pvt->_first=pvt->_bufferlist.getFirst();
}

memorypool::~memorypool() {
	for (memorypoollistnode	*node=pvt->_first; node; node=node->getNext()) {
		delete node->getValue();
	}
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
	memorypoolbuffer	*membuf;
	while (node) {

		membuf=node->getValue();
		if (membuf->_remaining>=length) {
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
					counter,pvt->_bufferlist.getLength());
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
		membuf=new memorypoolbuffer(incr);
		pvt->_bufferlist.append(membuf);

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
	unsigned char	*buffer=membuf->_buffer+membuf->_position;

	// adjust position and bytes remaining in
	// the node that the buffer came from
	membuf->_position+=length;
	membuf->_remaining-=length;

	// balance the list...
	if (node!=pvt->_first) {

		memorypoolbuffer	*firstmembuf=pvt->_first->getValue();

		#ifdef DEBUG_ALLOCATE
		stdoutput.printf("\n	remain in this node : %d\n",
						membuf->_remaining);
		stdoutput.printf("	remain in first node: %d\n",
						firstmembuf->_remaining);
		#endif

		// If this node has 128 bytes more space remaining than the
		// first node has, and that is at least 10% more space
		// remaining than the first node has, then move it to the
		// front.  This tends to reorganize the list such that nodes
		// with more space remaining tend to be are nearer to the front
		// of the list and the next allocate has a better chance of
		// finding space without having to search too hard.  The
		// 128/10% hopes to prevent flip-flopping of nodes.
		if (membuf->_remaining>firstmembuf->_remaining) {
			size_t	difference=membuf->_remaining-
						firstmembuf->_remaining;
			if (!firstmembuf->_remaining ||
				(difference>128 &&
				difference>firstmembuf->_remaining/10)) {

				pvt->_bufferlist.moveBefore(pvt->_first,node);
				pvt->_first=pvt->_bufferlist.getFirst();

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

	#ifdef DEBUG_DEALLOCATE
	stdoutput.printf("deallocate {\n");
	#endif

	// if the pool was unused during this iteration...
	// (a surprisingly common case)
	if (pvt->_bufferlist.getLength()==1 &&
			!pvt->_first->getValue()->_position) {

		#ifdef DEBUG_DEALLOCATE
		stdoutput.printf("	pool was unused\n");
		stdoutput.printf("}\n");
		#endif

		return;
	}

	// get the first buffer
	// (we'll need it later and pvt->_first is about to become invalid)
	memorypoolbuffer	*firstmembuf=pvt->_first->getValue();

	#ifdef DEBUG_DEALLOCATE
	stdoutput.printf("	clearing %d nodes\n",
					pvt->_bufferlist.getLength());
	#endif

	// delete all buffers except for the first one
	for (memorypoollistnode	*node=pvt->_first->getNext();
					node; node=node->getNext()) {
		delete node->getValue();
	}
	pvt->_bufferlist.clear();

	// bump deallocations counter
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
	// re-size the first buffer then do that
	if (pvt->_deallocations==pvt->_resizeinterval) {

		#ifdef DEBUG_RESIZE
		stdoutput.printf("	resize {\n");
		stdoutput.printf("		current    : %d\n",
							firstmembuf->_size);
		stdoutput.printf("		average    : %d\n",
							pvt->_average);
		#endif

		// calculate the difference between the current size
		// of the first buffer and average total allocation
		// since the last resize
		size_t	difference=
			(pvt->_average>firstmembuf->_size)?
				pvt->_average-firstmembuf->_size:
				firstmembuf->_size-pvt->_average;

		// don't grow or shrink the buffer if the
		// average is within 10% of its current size
		if (difference>firstmembuf->_size-firstmembuf->_size/10) {

			// resize the first buffer to the average
			// total allocation since the last resize
			delete[] firstmembuf->_buffer;
			firstmembuf->_buffer=new unsigned char[pvt->_average];
			firstmembuf->_size=pvt->_average;

			#ifdef DEBUG_RESIZE
			stdoutput.printf("		resizing\n");
			#endif
		}
		#ifdef DEBUG_RESIZE
		else {
			stdoutput.printf("		not resizing\n");
		}
		#endif

		// reset counters
		pvt->_deallocations=0;
		pvt->_average=0;

		#ifdef DEBUG_RESIZE
		stdoutput.printf("	}\n");
		#endif
	}

	// reset position and remaining on the first node
	firstmembuf->_position=0;
	firstmembuf->_remaining=firstmembuf->_size;

	// reset total
	pvt->_total=0;

	// add back the first buffer
	pvt->_bufferlist.append(firstmembuf);
	pvt->_first=pvt->_bufferlist.getFirst();

	#ifdef DEBUG_DEALLOCATE
	stdoutput.printf("	node count: %d\n",pvt->_bufferlist.getLength());
	stdoutput.printf("}\n");
	#endif
}
