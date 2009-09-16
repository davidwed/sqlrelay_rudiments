// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/memorypool.h>
#include <rudiments/rawbuffer.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
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

typedef	linkedlist<memorypoolnode *>		memorypoollist;
typedef	linkedlistnode<memorypoolnode *>	memorypoollistnode;

memorypoolnode::memorypoolnode(size_t size) {
	_buffer=new unsigned char[size];
	_size=size;
	_remaining=size;
	_position=0;
}

memorypoolnode::~memorypoolnode() {
	delete[] _buffer;
}

class memorypoolprivate {
	friend class memorypool;
	private:
		memorypoollist	_nodelist;

		size_t	_initialsize;
		size_t	_increment;

		size_t	_freecounter;
		size_t	_resizeinterval;
		size_t	_totalusedsize;
};

memorypool::memorypool(size_t initialsize,
			size_t increment,
			size_t resizeinterval) {
	pvt=new memorypoolprivate;
	pvt->_initialsize=initialsize;
	pvt->_increment=increment;
	pvt->_freecounter=0;
	pvt->_resizeinterval=resizeinterval;
	pvt->_totalusedsize=0;

	pvt->_nodelist.append(new memorypoolnode(pvt->_initialsize));
}

memorypool::~memorypool() {
	free();
	delete pvt->_nodelist.getFirstNode()->getData();
	delete pvt;
}

unsigned char *memorypool::malloc(size_t length) { 

	// add the length to the total size
	pvt->_totalusedsize=pvt->_totalusedsize+length;

	// look for a node with enough memory remaining
	memorypoollistnode	*node=pvt->_nodelist.getFirstNode();
	memorypoolnode		*memnode;
	while (node) {
		memnode=node->getData();
		if (memnode->_remaining>=length) {
			break;
		}
		node=(memorypoollistnode *)node->getNext();
	}

	// if we didn't find a node with enough memory remaining,
	// create a new one at the end of the list
	if (!node) {
		memnode=new memorypoolnode((length>pvt->_increment)?
						length:pvt->_increment);
		pvt->_nodelist.append(memnode);
	}

	// return the buffer
	unsigned char	*buffer=memnode->_buffer+memnode->_position;
	memnode->_position=memnode->_position+length;
	memnode->_remaining=memnode->_remaining-length;
	return buffer;
}

unsigned char *memorypool::calloc(size_t length) {
	unsigned char	*buffer=malloc(length);
	rawbuffer::zero(buffer,length);
	return buffer;
}

void memorypool::free() {

	memorypoollistnode	*firstlistnode=pvt->_nodelist.getFirstNode();
	memorypoolnode		*first=firstlistnode->getData();

	// if it's time to re-evaluate and re-size of the first node, do that
	pvt->_freecounter++;
	if (pvt->_freecounter>pvt->_resizeinterval) {

		// find average amount of memory allocated between free's
		pvt->_initialsize=pvt->_totalusedsize/pvt->_freecounter;

		// reset totalusedsize and freecounter
		pvt->_totalusedsize=0;
		pvt->_freecounter=0;

		// resize the first buffer to this size
		delete[] first->_buffer;
		first->_buffer=new unsigned char[pvt->_initialsize];
		first->_size=pvt->_initialsize;
		first->_remaining=pvt->_initialsize;
	}

	// reset position and remaining on the first node
	first->_position=0;
	first->_remaining=pvt->_initialsize;

	// delete all nodes beyond the first node
	memorypoollistnode	*currentlistnode=
			(memorypoollistnode *)firstlistnode->getNext();
	memorypoollistnode	*nextlistnode;
	while (currentlistnode) {
		nextlistnode=(memorypoollistnode *)currentlistnode->getNext();
		delete currentlistnode->getData();
		currentlistnode=nextlistnode;
	}
	pvt->_nodelist.clear();
	pvt->_nodelist.append(first);
}

void memorypool::print() {

	long		segmentindex=0;
	memorypoollistnode	*listnode=pvt->_nodelist.getFirstNode();
	memorypoolnode		*memnode;

	while (listnode) {
		memnode=listnode->getData();
		printf("segment %ld(%lx): (%d,%d)\n",
				segmentindex,(unsigned long)memnode,
				(int)memnode->_size,(int)memnode->_position);
		for (unsigned long i=0; i<memnode->_position; i++) {
			if (memnode->_buffer[i]>=' ' && 
				memnode->_buffer[i]<='~') {
				printf("'%c'",memnode->_buffer[i]);
			} else {
				printf("%3d",(int)memnode->_buffer[i]);
			}
			if (!((i+1)%20)) {
				printf("\n");
			} else {
				printf(" ");
			}
		}
		listnode=(memorypoollistnode *)listnode->getNext();
		printf("\n\n");
		segmentindex++;
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
