// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		memorypoolnode	*first;
		memorypoolnode	*last;

		unsigned long	initialsize;
		unsigned long	increment;

		unsigned long	freecounter;
		unsigned long	resizeinterval;
		unsigned long	totalusedsize;
