// Copyright (c) 2013 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_BASELINKEDLIST_H
#define RUDIMENTS_BASELINKEDLIST_H

#include <rudiments/private/inttypes.h>

class baselinkedlistnodeprivate;

class baselinkedlistnode {
	public:
		baselinkedlistnode();
		virtual	~baselinkedlistnode();

		void		setValue(uint64_t value, uint64_t size);
		uint64_t	getValue() const;

		void	setPrevious(baselinkedlistnode *previous);
		void	setNext(baselinkedlistnode *next);

		baselinkedlistnode	*getPrevious();
		baselinkedlistnode	*getNext();

		virtual	int32_t	compare(uint64_t value) const;
		virtual	void	print() const;
	private:
		baselinkedlistnodeprivate	*pvt;
};

class baselinkedlistprivate;

class baselinkedlist {
	public:
		baselinkedlist();
		virtual	~baselinkedlist();

		virtual	baselinkedlistnode	*newNode();

		void	append(uint64_t value, uint64_t size);
		void	append(baselinkedlistnode *node);

		bool	insert(uint64_t index, uint64_t value, uint64_t size);
		bool	insert(uint64_t index, baselinkedlistnode *node);

		bool	removeByIndex(uint64_t index);
		bool	removeByValue(uint64_t value);
		bool	removeAllByValue(uint64_t value);
		bool	removeNode(baselinkedlistnode *node);

		bool	setValueByIndex(uint64_t index,
					uint64_t value, uint64_t);
		bool	getValueByIndex(uint64_t index, uint64_t *value);

		uint64_t		getLength() const;

		baselinkedlistnode	*getFirstNode();
		baselinkedlistnode	*getLastNode();

		baselinkedlistnode	*getNodeByIndex(uint64_t index);
		baselinkedlistnode	*getNodeByValue(uint64_t value);
		baselinkedlistnode	*getNodeByValue(
						baselinkedlistnode *startnode,
						uint64_t value);

		void	clear();
		void	print() const;
	private:
		baselinkedlistprivate	*pvt;
};

#endif
