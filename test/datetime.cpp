// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#include "test.cpp"

const char	*tz="EST5EDT";
const char	*switchtz="PST8PDT";

int main(int argc, const char **argv) {

	header("datetime");

	// set current time zone
	stdoutput.printf("setting TZ=%s...\n",tz);
	environment	env;
	env.setValue("TZ",tz);

	// get the current date
	datetime	dt;
	dt.getSystemDateAndTime();

	// Write out the different parts of the date.
	stdoutput.printf("Hour       : %d\n",dt.getHour());
	stdoutput.printf("Minutes    : %d\n",dt.getMinutes());
	stdoutput.printf("Seconds    : %d\n",dt.getSeconds());
	stdoutput.printf("uSeconds   : %d\n",dt.getMicroseconds());
	stdoutput.printf("Month      : %d\n",dt.getMonth());
	stdoutput.printf("DayOfMonth : %d\n",dt.getDayOfMonth());
	stdoutput.printf("DayOfWeek  : %d\n",dt.getDayOfWeek());
	stdoutput.printf("DayOfYear  : %d\n",dt.getDayOfYear());
	stdoutput.printf("Year       : %d\n",dt.getYear());
	stdoutput.printf("Daylight Savings Time : %d\n",
						dt.isDaylightSavingsTime());
	stdoutput.printf("Time Zone             : %s\n",dt.getTimeZoneString());
	stdoutput.printf("Offset from GMT       : %d\n",dt.getTimeZoneOffset());
	stdoutput.printf("Seconds since 1970    : %ld\n",dt.getEpoch());
	stdoutput.printf("Date String           : %s\n",dt.getString());
	stdoutput.printf("Date String (uSec)    : %s\n",dt.getString(true));

	// use static methods to translate between formats
	char	*string=datetime::getString(dt.getEpoch());
	stdoutput.printf("String from Epoch     : %s\n",string);

	// use static methods to translate between formats
	time_t	epoch=datetime::getEpoch(string);
	stdoutput.printf("Epoch from String     : %ld\n",epoch);
	delete[] string;

	// use static methods to translate between formats
	string=datetime::getString(dt.getEpoch(),dt.getMicroseconds());
	stdoutput.printf("String from Epoch+uSec: %s\n",string);

	// use static methods to translate between formats
	datetime	dt1;
	dt1.initialize(string);
	stdoutput.printf("Epoch+uSec from String: %ld.%ld\n",
					dt1.getEpoch(),dt1.getMicroseconds());
	delete[] string;

	// get time from hardware clock
	stdoutput.printf("Hardware Clock (assuming GMT): %s\n",
		(dt.getHardwareDateAndTime("GMT"))?
					dt.getString():"failed");

	// get time from hardware clock adjusting for timezone
	stdoutput.printf("Adjusted Hardware Clock      : %s\n",
		(dt.getAdjustedHardwareDateAndTime("GMT"))?
					dt.getString():"failed");

	// switch time zones
	dt.adjustTimeZone(switchtz);
	stdoutput.printf("Adjusting time zone to %s: %s\n",
					switchtz,dt.getString());

	// valid date
	const char	*str="02/20/1974 12:00:00";
	stdoutput.printf("valid date?  %s  : %d\n",
					str,datetime::validDateTime(str));
	str="02/30/1974 12:00:00";
	stdoutput.printf("valid date?  %s  : %d\n",
					str,datetime::validDateTime(str));
	str="02/20/1974 12:00:00 EST5EDT";
	stdoutput.printf("valid date?  %s  : %d\n",
					str,datetime::validDateTime(str));
	str="02/30/1974 12:00:00 EST5EDT";
	stdoutput.printf("valid date?  %s  : %d\n",
					str,datetime::validDateTime(str));
}
