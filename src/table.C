#include <rudiments/table.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

table::table() {
	fieldpointers=NULL;
	morerows=NULL;
	morecols=NULL;
	more=NULL;
	fields=NULL;
	fieldarray=NULL;
	this->optimisticrowcount=0;
	this->optimisticcolcount=0;
	this->optimisticfieldsize=0;
	actualrowcount=0;
	actualcolcount=0;
}

table::~table() {
	delete fields;
	delete fieldpointers;
	delete morerows;
	delete morecols;
	delete more;
	clearFieldArray();
}

void table::initialize(unsigned long optimisticrowcount,
				unsigned long optimisticcolcount,
					unsigned long optimisticfieldsize) {
	clear();

	this->optimisticrowcount=optimisticrowcount;
	this->optimisticcolcount=optimisticcolcount;
	this->optimisticfieldsize=optimisticfieldsize;
}

void table::initFields() {
	fields=new memorypool(optimisticrowcount*optimisticcolcount*
							optimisticfieldsize,
				optimisticcolcount*optimisticfieldsize,5);
}

void table::initFieldPointers() {
	if (!fieldpointers) {
		fieldpointers=new variablebuffer(
				(optimisticrowcount*optimisticcolcount*
					(sizeof(unsigned long *)+
						sizeof(unsigned char *))),0);
	}
}

void table::initMoreCols() {
	if (!morecols) {
		morecols=new variablebuffer(50,25);
	}
}

void table::initMoreRows() {
	if (!morerows) {
		morerows=new variablebuffer(50,25);
	}
}

void table::initMore() {
	if (!more) {
		more=new table();
		more->initialize(optimisticrowcount,optimisticcolcount,
							optimisticfieldsize);
	}
}

unsigned char *table::getPointer(unsigned long row, unsigned long col,
						unsigned char dataflag) {

	if (row>=actualrowcount || col>=actualcolcount) {
		return NULL;
	}

	unsigned long	step=sizeof(unsigned long *)+sizeof(unsigned char *);
	unsigned long	extra=(dataflag)?sizeof(unsigned long *):0;

	variablebuffer	*vb;
	if (row<optimisticrowcount) {
		if (col<optimisticcolcount) {
			// get from fieldpointers
			initFieldPointers();
			vb=fieldpointers;
			vb->setPosition((step*(row*optimisticcolcount+col))+
					extra);
		} else {
			// get from morecols
			initMoreCols();
			vb=fieldpointers;
			vb->setPosition((step*((col-optimisticcolcount)*
						optimisticrowcount+row))+
						extra);
		}
	} else {
		if (col<optimisticcolcount) {
			// get from morerows
			initMoreRows();
			vb=fieldpointers;
			vb->setPosition((step*((row-optimisticrowcount)*
						optimisticcolcount+col))+
						extra);
		} else {
			// get from more
			initMore();
			return more->getPointer(row-optimisticrowcount,
							col-optimisticcolcount,
							dataflag);
		}
	}

	unsigned char	*ptr;
	vb->read((unsigned char *)&ptr,sizeof(unsigned char *));
	return ptr;
}

unsigned char *table::getFieldData(unsigned long row, unsigned long col) {
	return getPointer(row,col,1);
}

unsigned long table::getFieldLength(unsigned long row, unsigned long col) {
	unsigned char *lenptr=getPointer(row,col,0);
	return (lenptr)?(*((unsigned long *)lenptr)):0;
}

void table::setFieldData(unsigned long row, unsigned long col,
					const unsigned char *value,
					unsigned long length) {
	unsigned char	*field=allocateField(row,col,length);
	memcpy((void *)field,(void *)value,length);
	field[length]=(unsigned char)NULL;
printf("%d,%d=%d\n",row,col,field);
}

unsigned char *table::allocateField(unsigned long row, unsigned long col,
						unsigned long length) {

	// increment actual row/col counts
	if (col>=actualcolcount) {
		actualcolcount=col+1;
	}
	if (row>=actualrowcount) {
		actualrowcount=row+1;
	}

	if (row>=optimisticrowcount && col>=optimisticcolcount) {
		initMore();
		return more->allocateField(row-optimisticrowcount,
						col-optimisticcolcount,
						length);
	}

	initFields();
	unsigned long	*fieldlength=
			(unsigned long *)fields->malloc(sizeof(unsigned long));
	unsigned char	*field=
			(unsigned char *)fields->malloc(length+1);

	memcpy((void *)fieldlength,(void *)&length,sizeof(unsigned long));

	unsigned long	step=sizeof(unsigned long *)+sizeof(unsigned char *);
	unsigned long	lenpos;
	variablebuffer	*vb;
	if (row<optimisticrowcount) {
		if (col<optimisticcolcount) {
			// store in fieldpointers
			initFieldPointers();
			lenpos=(row*optimisticcolcount+col)*step;
			vb=fieldpointers;
		} else {
			// store in morecols
			initMoreCols();
			lenpos=((col-optimisticcolcount)*
						optimisticrowcount+row)*step;
			vb=morecols;
		}
	} else {
		// store in morerows
		initMoreRows();
		lenpos=((row-optimisticrowcount)*optimisticcolcount+col)*step;
		vb=morerows;
	}

	unsigned long	datapos=lenpos+sizeof(unsigned long *);
	vb->setPosition(lenpos);
	vb->write((unsigned char *)fieldlength,sizeof(unsigned long *));
	vb->setPosition(datapos);
	vb->write((unsigned char *)field,sizeof(unsigned char *));

	return field;
}

unsigned char ***table::getFields() {

	if (!fieldarray) {
		fieldarray=new unsigned char **[actualrowcount+1];
		fieldarray[actualrowcount]=(unsigned char **)NULL;
		for (int i=0; i<actualrowcount; i++) {
			fieldarray[i]=new unsigned char *[actualcolcount+1];
			fieldarray[i][actualcolcount]=(unsigned char *)NULL;
			for (int j=0; j<actualcolcount; j++) {
				fieldarray[i][j]=getFieldData(i,j);
			}
		}
	}

	return fieldarray;
}

unsigned char **table::getRow(unsigned long row) {
	return getFields()[row];
}

unsigned long table::getRowCount() {
	return actualrowcount;
}

unsigned long table::getColumnCount() {
	return actualcolcount;
}

void table::clearFieldArray() {
	if (!fieldarray) {
		return;
	}
	for (int i=0; i<actualrowcount; i++) {
		delete[] fieldarray[i];
	}
	delete[] fieldarray;
	fieldarray=NULL;
}

void table::clear() {
	if (fields) {
		fields->free();
	}
	if (fieldpointers) {
		fieldpointers->clear();
	}
	delete morecols;
	delete morerows;
	delete more;
	clearFieldArray();
	actualrowcount=0;
	actualcolcount=0;
}
