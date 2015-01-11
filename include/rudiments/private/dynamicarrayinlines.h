// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <new>

template< class valuetype >
dynamicarrayextent<valuetype>::dynamicarrayextent(int size) {
	this->size=size;
	a=new unsigned char *[size];
	for (int i=0; i<size; i++) {
		a[i]=new unsigned char[sizeof(valuetype)];
		new(a[i]) valuetype;
	}
}

template< class valuetype >
dynamicarrayextent<valuetype>::~dynamicarrayextent() {
	for (int i=0; i<size; i++) {
		((valuetype *)a[i])->~valuetype();
		delete[] a[i];
	}
	delete[] a;
}

template< class valuetype >
valuetype &dynamicarrayextent<valuetype>::operator[](int index) {
	return (valuetype &)*(a[index]);
}




template< class valuetype >
dynamicarray<valuetype>::dynamicarray() {
	init(128,32);
}

template< class valuetype >
dynamicarray<valuetype>::dynamicarray(int initialsize, int increment) {
	init((initialsize>1)?initialsize:128,(increment>1)?increment:32);
}

template< class valuetype >
void dynamicarray<valuetype>::init(int initialsize, int increment) {
	size=0;
	len=0;
	initsize=initialsize;
	extsize=increment;
	extend(initialsize);
	curext=extents.getFirst();
	curind=0;
}

template< class valuetype >
dynamicarray<valuetype>::~dynamicarray() {
	for (linkedlistnode<dynamicarrayextent<valuetype> *>
			*node=extents.getFirst(); node; node=node->getNext()) {
		delete node->getValue();
	}
	extents.clear();
}

template< class valuetype >
valuetype &dynamicarray<valuetype>::operator[](int index) {
	// FIXME: handle index<0
	extend(index+1);
	if (index>=len) {
		len=index+1;
	}
	return find(index);
}

template< class valuetype >
int dynamicarray<valuetype>::getLength() {
	return len;
}

template< class valuetype >
void dynamicarray<valuetype>::extend(int size) {
	while (this->size<size) {
		int	inc=(extents.getLength())?extsize:initsize;
		extents.append(new dynamicarrayextent<valuetype>(inc));
		this->size=this->size+inc;
	}
}

template< class valuetype >
valuetype &dynamicarray<valuetype>::find(int index) {
	
	// move to the extent that contains the specified index
	int	eind=0;
	if (index<initsize) {
		curext=extents.getFirst();
		curind=0;
	} else {
		for (;;) {
			eind=0;
			int	esize=initsize;
			if (curind) {
				eind=initsize+extsize*(curind-1);
				esize=extsize;
			}
			if (eind>index) {
				curext=curext->getPrevious();
				curind--;
			} else if (eind+esize<=index) {
				curext=curext->getNext();
				curind++;
			} else {
				break;
			}
		}
	}

	// return the value
	return (*curext->getValue())[index-eind];
}

template< class valuetype >
void dynamicarray<valuetype>::clear() {
	// FIXME: rather than re-extending, just don't delete the first extent
	for (linkedlistnode<dynamicarrayextent<valuetype> *>
			*node=extents.getFirst(); node; node=node->getNext()) {
		delete node->getValue();
	}
	extents.clear();
	size=0;
	len=0;
	extend(initsize);
	curext=extents.getFirst();
	curind=0;
}
