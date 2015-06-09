#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// initialize an instance of datetime to the current system date/time
	datetime	dt;
	dt.getSystemDateAndTime();

	// the components of the date/time are avaialble in various formats...

	// as a string
	stdoutput.printf("date/time: %s\n\n",dt.getString());

	// including microseconds
	stdoutput.printf("date/time: %s\n\n",dt.getString(true));

	// as the number of seconds since 1970
	stdoutput.printf("seconds since 1970: %d\n\n",dt.getEpoch());

	// broken down
	stdoutput.write("date/time: ");
	stdoutput.printf("%02d/%02d/%04d %02d:%02d:%02d.%06d %s (%d)\n\n",
			dt.getMonth(),dt.getDayOfMonth(),dt.getYear(),
			dt.getHour(),dt.getMinutes(),dt.getSeconds(),
			dt.getMicroseconds(),
			dt.getTimeZoneString(),dt.getTimeZoneOffset());

	// additional date information
	stdoutput.write("date: ");
	stdoutput.printf("%s (%s) %d, %04d "
			"(day %d of year) (day %d of week)\n\n",
			dt.getMonthName(),dt.getMonthAbbreviation(),
			dt.getDayOfMonth(),dt.getYear(),dt.getDayOfYear(),
			dt.getDayOfWeek());
}
