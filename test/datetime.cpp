// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/datetime.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	// set current time zone to America/New_York
	stdoutput.printf("setting TZ=America/New_York...\n");
	environment	env;
	env.setValue("TZ","America/New_York");

	// get the current date
	datetime	dt;
	dt.getSystemDateAndTime();

	// Write out the different parts of the date.
	stdoutput.printf("Hour       : %d\n",dt.getHour());
	stdoutput.printf("Minutes    : %d\n",dt.getMinutes());
	stdoutput.printf("Seconds	   : %d\n",dt.getSeconds());
	stdoutput.printf("Month      : %d\n",dt.getMonth());
	stdoutput.printf("DayOfMonth : %d\n",dt.getDayOfMonth());
	stdoutput.printf("DayOfWeek  : %d\n",dt.getDayOfWeek());
	stdoutput.printf("DayOfYear  : %d\n",dt.getDayOfYear());
	stdoutput.printf("Year       : %d\n",dt.getYear());
	stdoutput.printf("Daylight Savings Time : %d\n",
						dt.isDaylightSavingsTime());
	stdoutput.printf("Time Zone	      : %s\n",dt.getTimeZoneString());
	stdoutput.printf("Offset from GMT       : %d\n",dt.getTimeZoneOffset());
	stdoutput.printf("Seconds since 1970    : %ld\n",dt.getEpoch());
	stdoutput.printf("Date String	      : %s\n",dt.getString());

	// use static methods to translate between formats
	char	*string=datetime::getString(dt.getEpoch());
	stdoutput.printf("String from Epoch     : %s\n",string);
	delete[] string;
	string=datetime::getString(dt.getInternalTimeStructure());
	stdoutput.printf("String from tm        : %s\n",string);

	// use static methods to translate between formats
	time_t	epoch=datetime::getEpoch(string);
	stdoutput.printf("Epoch from String     : %ld\n",epoch);
	delete[] string;
	epoch=datetime::getEpoch(dt.getInternalTimeStructure());
	stdoutput.printf("Epoch from tm         : %ld\n",epoch);

	// get time from hardware clock
	stdoutput.printf("Hardware Clock (assuming GMT): %s\n",
		(dt.getHardwareDateAndTime("GMT"))?
					dt.getString():"failed");

	// get time from hardware clock adjusting for timezone
	stdoutput.printf("Adjusted Hardware Clock      : %s\n",
		(dt.getAdjustedHardwareDateAndTime("GMT"))?
					dt.getString():"failed");

	// switch time zones
	dt.adjustTimeZone("MST");
	stdoutput.printf("Adjusting time zone to Mountain time: %s\n",
							dt.getString());

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
