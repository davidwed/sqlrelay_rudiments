// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/datetime.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	// set current time zone to America/New_York
	printf("setting TZ=America/New_York...\n");
	environment	env;
	env.setValue("TZ","America/New_York");

	// get the current date
	datetime	dt;
	dt.getSystemDateAndTime();

	// Write out the different parts of the date.
	printf("Hour       : %d\n",dt.getHour());
	printf("Minutes    : %d\n",dt.getMinutes());
	printf("Seconds	   : %d\n",dt.getSeconds());
	printf("Month      : %d\n",dt.getMonth());
	printf("DayOfMonth : %d\n",dt.getDayOfMonth());
	printf("DayOfWeek  : %d\n",dt.getDayOfWeek());
	printf("DayOfYear  : %d\n",dt.getDayOfYear());
	printf("Year       : %d\n",dt.getYear());
	printf("Daylight Savings Time : %d\n",dt.isDaylightSavingsTime());
	printf("Time Zone	      : %s\n",dt.getTimeZoneString());
	printf("Offset from GMT       : %ld\n",dt.getTimeZoneOffset());
	printf("Seconds since 1970    : %ld\n",dt.getEpoch());
	printf("Date String	      : %s\n",dt.getString());

	// use static methods to translate between formats
	char	*string=datetime::getString(dt.getEpoch());
	printf("String from Epoch     : %s\n",string);
	delete[] string;
	string=datetime::getString(dt.getTm());
	printf("String from tm        : %s\n",string);

	// use static methods to translate between formats
	time_t	epoch=datetime::getEpoch(string);
	printf("Epoch from String     : %ld\n",epoch);
	delete[] string;
	epoch=datetime::getEpoch(dt.getTm());
	printf("Epoch from tm         : %ld\n",epoch);

	// get time from hardware clock
	printf("Hardware Clock (assuming GMT): %s\n",
		(dt.getHardwareDateAndTime("GMT"))?
					dt.getString():"failed");

	// get time from hardware clock adjusting for timezone
	printf("Adjusted Hardware Clock      : %s\n",
		(dt.getAdjustedHardwareDateAndTime("GMT"))?
					dt.getString():"failed");

	// switch time zones
	dt.adjustTimeZone("MST");
	printf("Adjusting time zone to Mountain time: %s\n",dt.getString());

	// valid date
	const char	*str="02/20/1974 12:00:00";
	printf("valid date?  %s  : %d\n",str,datetime::validDateTime(str));
	str="02/30/1974 12:00:00";
	printf("valid date?  %s  : %d\n",str,datetime::validDateTime(str));
	str="02/20/1974 12:00:00 EST5EDT";
	printf("valid date?  %s  : %d\n",str,datetime::validDateTime(str));
	str="02/30/1974 12:00:00 EST5EDT";
	printf("valid date?  %s  : %d\n",str,datetime::validDateTime(str));
}
