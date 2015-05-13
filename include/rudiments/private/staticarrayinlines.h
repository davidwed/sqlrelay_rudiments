// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/new.h>

template< class valuetype, uint64_t length >
RUDIMENTS_TEMPLATE_INLINE
staticarray<valuetype,length>::staticarray() {
	len=length;
	data=new valuetype[len];
}

template< class valuetype, uint64_t length >
RUDIMENTS_TEMPLATE_INLINE
staticarray<valuetype,length>::staticarray(const staticarray<valuetype,length> &v) {
	len=v.len;
	data=new valuetype[len];
	for (uint64_t i=0; i<len; i++) {

		// Why not just:
		//	*data[i]=*v.data[i];
		//
		// Some compilers get confused and think that
		//	*data[i]=*v.data[i]
		//		means
		//	*((data[i])->operator=(*v.data[i]))
		// and no carefully placed parentheses help.
		//
		// This silliness sorts out the problem.
		valuetype	*a=&(data[i]);
		valuetype	*b=&(v.data[i]);
		*a=*b;
	}
}

template< class valuetype, uint64_t length >
RUDIMENTS_TEMPLATE_INLINE
staticarray<valuetype,length> &staticarray<valuetype,length>::operator=(
				const staticarray<valuetype,length> &v) {
	if (this!=&v) {
		len=v.len;
		data=new valuetype[len];
		for (uint64_t i=0; i<len; i++) {

			// Why not just:
			//	*data[i]=*v.data[i];
			//
			// Some compilers get confused and think that
			//	*data[i]=*v.data[i]
			//		means
			//	*((data[i])->operator=(*v.data[i]))
			// and no carefully placed parentheses help.
			//
			// This silliness sorts out the problem.
			valuetype	*a=&(data[i]);
			valuetype	*b=&(v.data[i]);
			*a=*b;
		}
	}
	return *this;
}

template< class valuetype, uint64_t length >
RUDIMENTS_TEMPLATE_INLINE
staticarray<valuetype,length>::~staticarray() {
	delete[] data;
}

template< class valuetype, uint64_t length >
RUDIMENTS_TEMPLATE_INLINE
valuetype &staticarray<valuetype,length>::operator[](uint64_t index) {
	return data[index];
}

template< class valuetype, uint64_t length >
RUDIMENTS_TEMPLATE_INLINE
uint64_t staticarray<valuetype,length>::getLength() const {
	return len;
}

template< class valuetype, uint64_t length >
RUDIMENTS_TEMPLATE_INLINE
void staticarray<valuetype,length>::clear() {
	for (uint64_t i=0; i<len; i++) {
		((valuetype *)&data[i])->~valuetype();
		new(&data[i]) valuetype;
	}
}
