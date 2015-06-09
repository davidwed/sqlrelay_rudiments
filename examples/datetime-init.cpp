#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	datetime	dt;

	// initialize from system date/time
	dt.getSystemDateAndTime();
	stdoutput.printf("system date/time: %s\n\n",dt.getString(true));

	// initialize from a formatted string
	dt.initialize("01/02/2000 03:04:05 EDT");
	stdoutput.printf("formatted string: %s\n\n",dt.getString(true));

	// initialize from the number of seconds since 1970
	dt.initialize(40000000);
	stdoutput.printf("seconds since 1970: %s\n\n",dt.getString(true));

	// initialize from the number of seconds and microseconds since 1970
	dt.initialize(40000000,100);
	stdoutput.printf("sec/usec since 1970: %s\n\n",dt.getString(true));
}
