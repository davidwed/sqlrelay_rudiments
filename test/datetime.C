// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/datetime.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argv, const char **argc) {

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
	printf("Offset from GMT       : %d\n",dt.getTimeZoneOffset());
	printf("Seconds since 1970    : %d\n",dt.getEpoch());
	printf("Date String	      : %s\n",dt.getString());

	// use static methods to translate between formats
	char	*string=datetime::getString(dt.getEpoch());
	printf("String from Epoch     : %s\n",string);
	delete[] string;
	string=datetime::getString(dt.getTm());
	printf("String from tm        : %s\n",string);

	// use static methods to translate between formats
	time_t	epoch=datetime::getEpoch(string);
	printf("Epoch from String     : %d\n",epoch);
	delete[] string;
	epoch=datetime::getEpoch(dt.getTm());
	printf("Epoch from tm         : %d\n",epoch);

	// get time from hardware clock
	dt.getHardwareDateAndTime("GMT");
	printf("Hardware Clock (assuming GMT): %s\n",dt.getString());

	// get time from hardware clock adjusting for timezone
	dt.getAdjustedHardwareDateAndTime("GMT");
	printf("Adjusted Hardware Clock      : %s\n",dt.getString());

	// switch time zones
	dt.adjustTimeZone("PST");
	printf("Adjusting time zone to Pacific time: %s\n",dt.getString());
}
