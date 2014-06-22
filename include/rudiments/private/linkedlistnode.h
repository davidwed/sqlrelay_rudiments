// Copyright (c) 2003 David Muse
// See the COPYING file for more information

		void	setNext(linkedlistnode<valuetype> *next);
		void	setPrevious(linkedlistnode<valuetype> *previous);

	protected:
		valuetype			value;
		linkedlistnode<valuetype>	*next;
		linkedlistnode<valuetype>	*previous;
