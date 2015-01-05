// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LINKEDLISTINTERNAL_H
#define RUDIMENTS_LINKEDLISTINTERNAL_H

#define LINKEDLISTINTERNAL

#include <rudiments/private/linkedlistutilinlines.h>
#include <rudiments/private/inttypes.h>

class linkedlistnodeinternalprivate;

class linkedlistnodeinternal {
	public:
				linkedlistnodeinternal(unsigned char *value,
							uint64_t size);
			virtual	~linkedlistnodeinternal();
		void		setValue(unsigned char *value, uint64_t size);
		unsigned char	*getValue() const;
		linkedlistnodeinternal	*getPrevious();
		linkedlistnodeinternal	*getNext();
		void	setNext(linkedlistnodeinternal *next);
		void	setPrevious(linkedlistnodeinternal *previous);
		int32_t	compare(unsigned char *value,
					uint64_t size,
					_linkedlistutil_valuetype_t valuetype)
					const;
		void	print(uint64_t size,
				_linkedlistutil_valuetype_t valuetype) const;

	private:
		linkedlistnodeinternalprivate	*pvt;
};


class linkedlistinternalprivate;

class linkedlistinternal {
	public:
				linkedlistinternal();
			virtual	~linkedlistinternal();

		void	init(uint64_t size,
				_linkedlistutil_valuetype_t valuetype);
		void	prepend(unsigned char *value);
		void	prependNode(linkedlistnodeinternal *node);
		void	append(unsigned char *value);
		void	appendNode(linkedlistnodeinternal *node);
		void	insertBefore(linkedlistnodeinternal *node,
						unsigned char *value);
		void	insertNodeBefore(linkedlistnodeinternal *node,
					linkedlistnodeinternal *newnode);
		void	insertAfter(linkedlistnodeinternal *node,
						unsigned char *value);
		void	insertNodeAfter(linkedlistnodeinternal *node,
					linkedlistnodeinternal *newnode);
		void	moveBefore(linkedlistnodeinternal *node,
					linkedlistnodeinternal *nodetomove);
		void	moveAfter(linkedlistnodeinternal *node,
					linkedlistnodeinternal *nodetomove);
		void	detach(linkedlistnodeinternal *node);
		bool	remove(unsigned char *value);
		bool	removeAll(unsigned char *value);
		bool	removeNode(linkedlistnodeinternal *node);
		uint64_t	getLength() const;
		linkedlistnodeinternal	*getFirst();
		linkedlistnodeinternal	*getLast();
		linkedlistnodeinternal	*getPrevious(
					linkedlistnodeinternal *node);
		linkedlistnodeinternal	*getNext(
					linkedlistnodeinternal *node);
		linkedlistnodeinternal	*find(unsigned char *value);
		linkedlistnodeinternal
			*find(linkedlistnodeinternal *startnode,
							unsigned char *value);
		void	insertionSort();
		void	heapSort();
		void	clear();
		void	print() const;
		void	print(uint64_t count) const;

		void	move(linkedlistnodeinternal *node,
				linkedlistnodeinternal *nodetomove,
				bool before);

	private:
		linkedlistinternalprivate	*pvt;
};


#endif
