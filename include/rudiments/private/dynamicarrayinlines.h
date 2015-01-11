// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <new>

template< class valuetype >
dynamicarray<valuetype>::dynamicarray() {
	init(128,32);
}

template< class valuetype >
dynamicarray<valuetype>::dynamicarray(uint64_t initialsize,
						uint64_t increment) {
	init((initialsize)?initialsize:128,(increment)?increment:32);
}

template< class valuetype >
void dynamicarray<valuetype>::init(uint64_t initialsize,
						uint64_t increment) {
	size=0;
	len=0;
	initial=initialsize;
	extsize=increment;
	extend(initialsize);
	curext=extents.getFirst();
	curind=0;
}

template< class valuetype >
dynamicarray<valuetype>::~dynamicarray() {
	clearExtentList();
}

template< class valuetype >
valuetype &dynamicarray<valuetype>::operator[](uint64_t index) {
	extend(index+1);
	if (index>=len) {
		len=index+1;
	}
	return find(index);
}

template< class valuetype >
uint64_t dynamicarray<valuetype>::getLength() {
	return len;
}

template< class valuetype >
void dynamicarray<valuetype>::extend(uint64_t size) {
	uint64_t	inc=(extents.getLength())?extsize:initial;
	while (this->size<size) {
		unsigned char	**newext=new unsigned char *[inc];
		for (uint64_t i=0; i<inc; i++) {
			newext[i]=new unsigned char[sizeof(valuetype)];
			new(newext[i]) valuetype;
		}
		extents.append(newext);
		this->size=this->size+inc;
		inc=extsize;
	}
}

template< class valuetype >
valuetype &dynamicarray<valuetype>::find(uint64_t index) {

	// move to the extent that contains the specified index
	// (also calculate the index of the first element of the extent)
	size_t	eind;
	if (index<initial) {
		curext=extents.getFirst();
		curind=0;
		eind=0;
	} else {
		uint64_t	targetind=(index-initial+extsize)/extsize;
		while (curind!=targetind) {
			if (curind>targetind) {
				curext=curext->getPrevious();
				curind--;
			} else {
				curext=curext->getNext();
				curind++;
			}
		}
		eind=initial+extsize*(curind-1);
	}

	// return the value
	return (valuetype &)(*curext->getValue()[index-eind]);
}

template< class valuetype >
void dynamicarray<valuetype>::clearExtentList() {
	uint64_t	inc=initial;
	for (linkedlistnode<unsigned char **> *node=extents.getFirst();
						node; node=node->getNext()) {
		unsigned char	**ext=node->getValue();
		for (uint64_t i=0; i<inc; i++) {
			((valuetype *)ext[i])->~valuetype();
			delete[] ext[i];
		}
		delete[] ext;
		inc=extsize;
	}
	extents.clear();
}

template< class valuetype >
void dynamicarray<valuetype>::clear() {
	// FIXME: rather than re-extending, just don't delete the first extent
	clearExtentList();
	size=0;
	len=0;
	extend(initial);
	curext=extents.getFirst();
	curind=0;
}
