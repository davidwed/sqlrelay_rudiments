// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef TABLE_H
#define TABLE_H

#include <rudiments/private/config.h>

#include <rudiments/memorypool.h>
#include <rudiments/variablebuffer.h>

class table {
	public:
			table();
			~table();

		void	initialize(unsigned long optimisticrowcount,
					unsigned long optimisticcolcount,
					unsigned long optimisticfieldsize);

		unsigned char	*getFieldData(unsigned long row,
						unsigned long col);
		unsigned long	getFieldLength(unsigned long row,
						unsigned long col);

		void	setFieldData(unsigned long row,
					unsigned long col,
					const unsigned char *value,
					unsigned long length);

		unsigned char	*allocateField(unsigned long row,
					unsigned long col,
					unsigned long length);

		unsigned char	***getFields();
		unsigned char	**getRow(unsigned long row);

		unsigned long	getRowCount();
		unsigned long	getColumnCount();

		void	clear();

	#include <rudiments/private/table.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/tableinlines.h>
#endif

#endif
