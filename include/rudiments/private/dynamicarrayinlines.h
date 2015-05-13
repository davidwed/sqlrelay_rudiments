// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/new.h>

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
dynamicarray<valuetype>::dynamicarray() {
	init(128,32);
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
dynamicarray<valuetype>::dynamicarray(uint64_t initialsize,
						uint64_t increment) {
	init((initialsize)?initialsize:128,(increment)?increment:32);
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
dynamicarray<valuetype>::dynamicarray(const dynamicarray<valuetype> &v) {
	dynamicarrayClone(v);
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
dynamicarray<valuetype> &dynamicarray<valuetype>::operator=(
					const dynamicarray<valuetype> &v) {
	if (this!=&v) {
		clearExtentList();
		dynamicarrayClone(v);
	}
	return *this;
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
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
RUDIMENTS_TEMPLATE_INLINE
void dynamicarray<valuetype>::dynamicarrayClone(
				const dynamicarray<valuetype> &v) {

	dynamicarray<valuetype>	*vconst=(dynamicarray<valuetype> *)&v;

	// clone the data
	for (uint64_t i=0; i<v.getLength(); i++) {

		// Why not just:
		//	this[i]=v[i];
		//
		// Some compilers don't allow v[] because the operator[] method
		// isn't const, but v is.
		//
		// Also, some compilers get confused and think that
		//	this[i]=v[i]
		//		means
		//	(this[i])->operator=(v[i])
		// and no carefully placed parentheses help.
		//
		// This silliness sorts both issues out.
		this.find(i)=((dynamicarray<valuetype> *)&v)->find(i);
	}

	// clone sizes and positions
	size=v.size;
	len=v.len;
	initial=v.initial;
	extsize=v.extsize;
	curind=v.curind;
	curext=extents.getFirst();
	for (uint64_t eind=0; eind<curind; eind++) {
		curext=curext->getNext();
	}
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
dynamicarray<valuetype>::~dynamicarray() {
	clearExtentList();
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
valuetype &dynamicarray<valuetype>::operator[](uint64_t index) {
	extend(index+1);
	if (index>=len) {
		len=index+1;
	}
	return find(index);
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
uint64_t dynamicarray<valuetype>::getLength() const {
	return len;
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
void dynamicarray<valuetype>::extend(uint64_t size) {
	uint64_t	inc=(extents.getLength())?extsize:initial;
	while (this->size<size) {
		valuetype	*newext=new valuetype[inc];
		extents.append(newext);
		this->size=this->size+inc;
		inc=extsize;
	}
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
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
	return curext->getValue()[index-eind];
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
void dynamicarray<valuetype>::clearExtentList() {
	curext=extents.getFirst();
	while (curext) {
		linkedlistnode<valuetype *>	*next=curext->getNext();
		valuetype			*ext=curext->getValue();
		delete[] ext;
		extents.remove(curext);
 		curext=next;
	}
}

template< class valuetype >
RUDIMENTS_TEMPLATE_INLINE
void dynamicarray<valuetype>::clear() {

	// remove all but the first extent
	curext=extents.getLast();
	while (curext!=extents.getFirst()) {
		linkedlistnode<valuetype *>	*prev=curext->getPrevious();
		valuetype			*ext=curext->getValue();
		delete[] ext;
		extents.remove(curext);
 		curext=prev;
	}

	// reinit first extent
	valuetype	*ext=curext->getValue();
	for (uint64_t i=0; i<initial; i++) {
		ext[i].~valuetype();
		new(&(ext[i])) valuetype;
	}

	// reset sizes and positions
	size=0;
	len=0;
	curind=0;
}
