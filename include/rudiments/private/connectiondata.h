// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		connectiondatanode	*findNode(int index) const;
		connectiondatanode	*findNode(const char *key) const;

		connectiondatanode	*first;
		connectiondatanode	*last;

		int	keycount;
