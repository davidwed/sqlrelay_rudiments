// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/datetime.h>
#include <stdio.h>

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
	char	*string=dt.getString();
	printf("Date String	      : %s\n",string);
	delete[] string;

	// use static methods to translate between formats
	string=datetime::getString(dt.getEpoch());
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
	dt.getHardwareDateAndTime(datetime::GMT);
	string=dt.getString();
	printf("Hardware Clock (assuming GMT): %s\n",string);
	delete[] string;

	// get time from hardware clock adjusting for timezone
	dt.getAdjustedHardwareDateAndTime(datetime::GMT);
	string=dt.getString();
	printf("Adjusted Hardware Clock      : %s\n",string);
	delete[] string;

	// switch time zones
	dt.adjustTimeZone(datetime::JST);
	string=dt.getString();
	printf("Adjusting time zone to Japanese time: %s\n",string);
	delete[] string;

	// list of time zones
	printf("\nTimezones: \n");
	for (int i=0; datetime::getTimeZoneAbbreviations()[i]; i++) {
		printf("%s: %6d  ",
			datetime::getTimeZoneAbbreviations()[i],
			datetime::getTimeZoneOffsets()[i]);
		if (!((i+1)%6)) {
			printf("\n");
		}
	}
	printf("\n");
}
