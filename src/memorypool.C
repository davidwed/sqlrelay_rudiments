// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/memorypool.h>

#include <stdio.h>
#include <stdlib.h>

// need these for memset...
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

memorypoolnode::memorypoolnode(size_t size) {
	buffer=new unsigned char[size];
	position=0;
	remaining=size;
	this->size=size;
}

memorypoolnode::~memorypoolnode() {
	delete[] buffer;
}

memorypool::memorypool(size_t initialsize,
			size_t increment,
			size_t resizeinterval) {
	this->initialsize=initialsize;
	this->increment=increment;
	this->resizeinterval=resizeinterval;
	totalusedsize=0;
	freecounter=0;

	nodelist.append(new memorypoolnode(initialsize));
}

memorypool::~memorypool() {
	free();
	delete nodelist.getNodeByIndex(0)->getData();
}

unsigned char *memorypool::malloc(size_t length) { 
	// add the length to the total size
	totalusedsize=totalusedsize+length;

	// look for a node with enough memory remaining
	memorypoollistnode	*node=nodelist.getNodeByIndex(0);
	memorypoolnode		*memnode;
	while (node) {
		memnode=node->getData();
		if (memnode->remaining>=length) {
			break;
		}
		node=(memorypoollistnode *)node->getNext();
	}

	// if we didn't find a node with enough memory remaining,
	// create a new one at the end of the list
	if (!node) {
		memnode=new memorypoolnode((length>increment)?length:increment);
		nodelist.append(memnode);
	}

	// return the buffer
	unsigned char	*buffer=memnode->buffer+memnode->position;
	memnode->position=memnode->position+length;
	memnode->remaining=memnode->remaining-length;
	return buffer;
}

unsigned char *memorypool::calloc(size_t length) {
	unsigned char	*buffer=malloc(length);
	memset((void *)buffer,0,length);
	return buffer;
}

void memorypool::free() {

	memorypoollistnode	*firstlistnode=nodelist.getNodeByIndex(0);
	memorypoolnode		*first=firstlistnode->getData();

	// if it's time to re-evaluate and re-size of the first node, do that
	freecounter++;
	if (freecounter>resizeinterval) {

		// find average amount of memory allocated between free's
		initialsize=totalusedsize/freecounter;

		// reset totalusedsize and freecounter
		totalusedsize=0;
		freecounter=0;

		// resize the first buffer to this size
		delete[] first->buffer;
		first->buffer=new unsigned char[initialsize];
		first->size=initialsize;
		first->remaining=initialsize;
	}

	// reset position and remaining on the first node
	first->position=0;
	first->remaining=initialsize;

	// delete all nodes beyond the first node
	memorypoollistnode	*currentlistnode=
			(memorypoollistnode *)firstlistnode->getNext();
	memorypoollistnode	*nextlistnode;
	while (currentlistnode) {
		nextlistnode=(memorypoollistnode *)currentlistnode->getNext();
		delete currentlistnode->getData();
		currentlistnode=nextlistnode;
	}
	nodelist.clear();
	nodelist.append(first);
}

void memorypool::print() {

	long		segmentindex=0;
	memorypoollistnode	*listnode=nodelist.getNodeByIndex(0);
	memorypoolnode		*memnode;

	while (listnode) {
		memnode=listnode->getData();
		printf("segment %ld(%lx): (%d,%d)\n",
				segmentindex,(unsigned long)memnode,
				(int)memnode->size,(int)memnode->position);
		for (unsigned long i=0; i<memnode->position; i++) {
			if (memnode->buffer[i]>=' ' && 
				memnode->buffer[i]<='~') {
				printf("'%c'",memnode->buffer[i]);
			} else {
				printf("%3d",(int)memnode->buffer[i]);
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
