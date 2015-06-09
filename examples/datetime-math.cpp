#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	datetime	dt;

	// initialize from system date/time
	dt.getSystemDateAndTime();
	stdoutput.printf("current date/time: %s\n\n",dt.getString(true));

	// add three years, months, days,
	// hours, minutes, seconds, and microseconds
	dt.addYears(3);
	dt.addMonths(3);
	dt.addDays(3);
	dt.addHours(3);
	dt.addMinutes(3);
	dt.addSeconds(3);
	dt.addMicroseconds(3);
	stdoutput.printf("new date/time: %s\n\n",dt.getString(true));

	// subtract three years, months, days,
	// hours, minutes, seconds, and microseconds
	dt.addYears(-3);
	dt.addMonths(-3);
	dt.addDays(-3);
	dt.addHours(-3);
	dt.addMinutes(-3);
	dt.addSeconds(-3);
	dt.addMicroseconds(-3);
	stdoutput.printf("original date/time: %s\n\n",dt.getString(true));
}
