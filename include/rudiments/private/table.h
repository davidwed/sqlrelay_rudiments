// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

	private:

		void	initFields();
		void	initFieldPointers();
		void	initMoreCols();
		void	initMoreRows();
		void	initMore();

		unsigned char	*getPointer(unsigned long row,
						unsigned long col,
						unsigned char dataflag);
		void	clearFieldArray();

		memorypool	*fields;
		unsigned long	optimisticrowcount;
		unsigned long	optimisticcolcount;
		unsigned long	optimisticfieldsize;

		variablebuffer	*fieldpointers;
		variablebuffer	*morecols;
		variablebuffer	*morerows;

		unsigned long	rowcount;
		unsigned long	colcount;

		unsigned char	***fieldarray;
		unsigned char	**rowarray;

		unsigned long	actualrowcount;
		unsigned long	actualcolcount;

		table		*more;
