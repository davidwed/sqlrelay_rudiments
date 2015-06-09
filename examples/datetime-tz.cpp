#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	datetime	dt;

	// initialize from system date/time
	dt.getSystemDateAndTime();
	stdoutput.printf("local time: %s\n\n",dt.getString());

	// get time zone abbreviations and offsets
	const char * const *tzabbr=datetime::getTimeZoneAbbreviations();
	const int32_t *tzoff=datetime::getTimeZoneOffsets();

	// show the current time in different timezones around the world...
	while (*tzabbr && *tzabbr[0]) {

		// convert timezone
		dt.adjustTimeZone(*tzabbr);

		stdoutput.printf("% 14s (% 6d sec from GMT): %s\n",
					*tzabbr,*tzoff,dt.getString());

		tzabbr++;
		tzoff++;
	}
}
