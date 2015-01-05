// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	protected:
		void	move(linkedlistnode<valuetype> *node,
				linkedlistnode<valuetype> *nodetomove,
				bool before);

		linkedlistinternal	*l;
		static	valuetype	v;
