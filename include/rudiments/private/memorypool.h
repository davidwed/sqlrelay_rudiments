// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		memorypoollist	nodelist;

		unsigned long	initialsize;
		unsigned long	increment;

		unsigned long	freecounter;
		unsigned long	resizeinterval;
		unsigned long	totalusedsize;
