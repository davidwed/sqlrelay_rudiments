#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// open the file
	file	f;
	f.open("testfile",O_RDWR|O_CREAT,
				permissions::evalPermString("rw-rw-rw-"));

	// write 4 fixed length records to the file,
	// each consiting of two 10-character fields
	f.write("                    ");
	f.write("                    ");
	f.write("                    ");
	f.write("                    ");

	// go to the first record
	f.setPositionRelativeToBeginning(0);

	// overwrite the first record
	f.write("goodbye   friends   ");

	// go to the last record
	f.setPositionRelativeToEnd(-20);
	f.write("hello     there     ");

	// go to the third record
	f.setPositionRelativeToBeginning(40);
	f.write("bye       folks     ");

	// go to the second record
	f.setPositionRelativeToCurrent(-40);
	f.write("hi        guys      ");


	// print the records in reverse order
	char	record[20];
	for (off64_t i=1; i<=4; i++) {
		f.setPositionRelativeToEnd(-20*i);
		f.read(record,20);
		stdoutput.write(record,20);
		stdoutput.write('\n');
	}
}
