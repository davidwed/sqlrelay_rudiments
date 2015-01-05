// Copyright (c) 2003 David Muse
// See the COPYING file for more information

		void	setNext(linkedlistnode<valuetype> *next);
		void	setPrevious(linkedlistnode<valuetype> *previous);

	protected:
#ifdef LINKEDLISTINTERNAL
		linkedlistnodeinternal	*n;
#else
		valuetype			value;
		linkedlistnode<valuetype>	*next;
		linkedlistnode<valuetype>	*previous;
#endif
