// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/memorypool.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/memorypoolinlines.h>
#endif

#include <stdio.h>
#include <stdlib.h>

memorypoolnode::memorypoolnode(unsigned long size) {
	buffer=new char[size];
	position=0;
	remaining=size;
	this->size=size;
	next=NULL;
}

memorypool::memorypool(unsigned long initialsize,
			unsigned long increment,
			unsigned long resizeinterval) {
	this->initialsize=initialsize;
	this->increment=increment;
	this->resizeinterval=resizeinterval;
	totalusedsize=0;
	freecounter=0;
	first=new memorypoolnode(initialsize);
	first->next=NULL;
	last=first;
}

char	*memorypool::malloc(unsigned long length) {

	// add the length to the total size
	totalusedsize=totalusedsize+length;

	// look for a node with enough memory remaining
	memorypoolnode	*current=first;
	while (current) {
		if (current->remaining>=length) {
			break;
		}
		current=current->next;
	}

	// if we didn't find a node with enough memory remaining,
	// create a new one at the end of the list
	if (!current) {
		if (length>increment) {
			last->next=new memorypoolnode(length);
		} else {
			last->next=new memorypoolnode(increment);
		}
		last->next->next=NULL;
		last=last->next;
		current=last;
	}

	// return the buffer
	char	*retval=current->buffer+current->position;
	current->position=current->position+length;
	current->remaining=current->remaining-length;
	return retval;
}

char	*memorypool::calloc(unsigned long length) {
	char	*retval=malloc(length);
	for (int i=0; i<length; i++) {
		retval[i]=(char)NULL;
	}
	return retval;
}

void	memorypool::free() {

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
		first->buffer=new char[initialsize];
		first->size=initialsize;
		first->remaining=initialsize;
	}

	// reset position and remaining on the first node
	first->position=0;
	first->remaining=initialsize;

	// delete all nodes beyond the first node
	memorypoolnode	*current=first->next;
	while (current) {
		last=current->next;
		delete current;
		current=last;
	}
	last=first;
	first->next=NULL;
}

void	memorypool::print() const {

	long		segmentindex=0;
	memorypoolnode	*current=first;

	while (current) {
		printf("segment %d: (%d,%d)\n",
				segmentindex,current->size,current->position);
		for (long i=0; i<current->position; i++) {
			if (current->buffer[i]>=' ' && 
				current->buffer[i]<='~') {
				printf("'%c'",current->buffer[i]);
			} else {
				printf("%3d",(int)current->buffer[i]);
			}
			if (!((i+1)%20)) {
				printf("\n");
			} else {
				printf(" ");
			}
		}
		current=current->next;
		printf("\n\n");
		segmentindex++;
	}
}
