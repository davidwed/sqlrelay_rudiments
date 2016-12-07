// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/memorypool.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
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

typedef	singlylinkedlistnode<memorypoolnode *>	memorypoollistnode;
typedef	singlylinkedlist<memorypoolnode *>	memorypoollist;

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
	deallocate();
	delete pvt->_nodelist.getFirst()->getValue();
	delete pvt;
}

unsigned char *memorypool::allocate(size_t length) { 

	// look for a node with enough memory remaining
	memorypoollistnode	*node=pvt->_nodelist.getFirst();
	memorypoolnode		*memnode;
	while (node) {
		memnode=node->getValue();
		if (memnode->_remaining>=length) {
			break;
		}
		node=node->getNext();
	}

	// if we didn't find a node with enough memory remaining,
	// create a new one at the end of the list
	if (!node) {
		// increase size by increments of at least 10% for better
		// performance and 10% max memory usage penalty - Claudio Freire
		size_t	incr=length;
		if (incr<pvt->_increment) {
			incr=pvt->_increment;
		}
		if (incr<(pvt->_totalusedsize/10)) {
			incr=(pvt->_totalusedsize/10);
		}
		memnode=new memorypoolnode(incr);
		pvt->_nodelist.append(memnode);
	}

	// add the length to the total size
	pvt->_totalusedsize=pvt->_totalusedsize+length;

	// return the buffer
	unsigned char	*buffer=memnode->_buffer+memnode->_position;
	memnode->_position=memnode->_position+length;
	memnode->_remaining=memnode->_remaining-length;
	return buffer;
}

unsigned char *memorypool::allocateAndClear(size_t length) {
	unsigned char	*buffer=allocate(length);
	bytestring::zero(buffer,length);
	return buffer;
}

void memorypool::deallocate() {

	memorypoollistnode	*firstlistnode=pvt->_nodelist.getFirst();
	memorypoolnode		*first=firstlistnode->getValue();

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
	}

	// reset position and remaining on the first node
	first->_position=0;
	first->_remaining=first->_size;

	// delete all nodes beyond the first node
	memorypoollistnode	*currentlistnode=firstlistnode->getNext();
	memorypoollistnode	*nextlistnode;
	while (currentlistnode) {
		nextlistnode=currentlistnode->getNext();
		delete currentlistnode->getValue();
		currentlistnode=nextlistnode;
	}
	pvt->_nodelist.clear();
	pvt->_nodelist.append(first);
}

void memorypool::print() {

	uint64_t		segmentindex=0;
	memorypoollistnode	*listnode=pvt->_nodelist.getFirst();
	memorypoolnode		*memnode;

	while (listnode) {
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
		listnode=listnode->getNext();
		stdoutput.printf("\n\n");
		segmentindex++;
	}
}



/*int32_t  _containerutil_compare(memorypoolnode *value1,
					memorypoolnode *value2) {
	return _containerutil_compare((void *)value1,(void *)value2);
}

void _containerutil_print(memorypoolnode *value) {
	_containerutil_print((void *)value);
}*/
