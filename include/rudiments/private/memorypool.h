// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		memorypoollist	nodelist;

		size_t	initialsize;
		size_t	increment;

		size_t	freecounter;
		size_t	resizeinterval;
		size_t	totalusedsize;
