// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/datetime.h>
#include <stdio.h>

int	main() {

	// get the current date
	datetime	dt;

	// Write out the different parts of the date.
	printf("Hour		  : %d\n",dt.getHour());
	printf("Minutes	       : %d\n",dt.getMinutes());
	printf("Seconds	       : %d\n",dt.getSeconds());
	printf("Month		 : %d\n",dt.getMonth());
	printf("DayOfMonth	    : %d\n",dt.getDayOfMonth());
	printf("DayOfWeek	     : %d\n",dt.getDayOfWeek());
	printf("DayOfYear	     : %d\n",dt.getDayOfYear());
	printf("Year		  : %d\n",dt.getYear());
	printf("Daylight Savings Time : %d\n",dt.isDaylightSavingsTime());
	printf("Time Zone	     : %d\n",dt.getTimeZone());
	printf("Offset from GMT       : %d\n",dt.getTimeZoneOffset());
	printf("Date String	   : %s\n",dt.getString());
	printf("Seconds since 1970    : %d\n",dt.getEpoch());

	char	*string=datetime::getString(dt.getEpoch());
	time_t	epoch=datetime::getEpoch(string);

	printf("Epoch/String : %d %s\n",epoch,string);
}
