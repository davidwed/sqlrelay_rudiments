// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	protected:
		void	move(linkedlistnode<valuetype> *node,
				linkedlistnode<valuetype> *nodetomove,
				bool before);

#ifdef LINKEDLISTINTERNAL
		linkedlistinternal	*l;
#else
		linkedlistnode<valuetype>	*first;
		linkedlistnode<valuetype>	*last;
		uint64_t			length;
#endif
