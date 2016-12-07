// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/datetime.h>
#include <rudiments/environment.h>
#include <rudiments/stdio.h>
#include <rudiments/private/config.h>
#include "test.cpp"

const char	*nondstdatestring="02/08/2016 21:54:30 EST";
const char	*nondstdatestringwithzerousec="02/08/2016 21:54:30:000 EST";
time_t		nondstsecsinceepoch=1454986470;

const char	*datestring="04/08/2016 21:54:30 EDT";
const char	*estdatestring="04/08/2016 21:54:30 EDT";
const char	*cstdatestring="04/08/2016 20:54:30 CDT";
const char	*mstdatestring="04/08/2016 19:54:30 MDT";
const char	*pstdatestring="04/08/2016 18:54:30 PDT";
const char	*datestringwithusec="04/08/2016 21:54:30:500 EDT";
const char	*datestringwithzerousec="04/08/2016 21:54:30:000 EDT";
time_t		secsinceepoch=1460166870;

int main(int argc, const char **argv) {

	// set timezone so epoch to string will work correctly
	environment::setValue("TZ","EST5EDT");

	header("datetime");

	datetime	dt;

	// init from string
	stdoutput.printf("non-dst date from string:\n");
	dt.initialize(nondstdatestring);
	test("hour",dt.getHour()==21);
	test("minutes",dt.getMinutes()==54);
	test("seconds",dt.getSeconds()==30);
	test("useconds",dt.getMicroseconds()==0);
	test("month",dt.getMonth()==2);
	test("month name",
		!charstring::compare(dt.getMonthName(),"February"));
	test("month abbr",
		!charstring::compare(dt.getMonthAbbreviation(),"Feb"));
	test("day of month",dt.getDayOfMonth()==8);
	test("day of week",dt.getDayOfWeek()==2);
	test("day of year",dt.getDayOfYear()==39);
	test("year",dt.getYear()==2016);
	test("daylight savings time",!dt.isDaylightSavingsTime());
	test("time zone",
		!charstring::compare(dt.getTimeZoneString(),"EST"));
stdoutput.printf("%d\n",dt.getTimeZoneOffset());
	test("offset from GMT",dt.getTimeZoneOffset()==-18000);
	test("seconds since 1970",dt.getEpoch()==nondstsecsinceepoch);
	test("date string",
		!charstring::compare(dt.getString(),nondstdatestring));
	test("date string (with usec)",
		!charstring::compare(dt.getString(true),
					nondstdatestringwithzerousec));
	stdoutput.printf("\n");


	// init from string
	stdoutput.printf("date from string:\n");
	dt.initialize(datestringwithusec);
	test("hour",dt.getHour()==21);
	test("minutes",dt.getMinutes()==54);
	test("seconds",dt.getSeconds()==30);
	test("useconds",dt.getMicroseconds()==500);
	test("month",dt.getMonth()==4);
	test("month name",
		!charstring::compare(dt.getMonthName(),"April"));
	test("month abbr",
		!charstring::compare(dt.getMonthAbbreviation(),"Apr"));
	test("day of month",dt.getDayOfMonth()==8);
	test("day of week",dt.getDayOfWeek()==6);
	test("day of year",dt.getDayOfYear()==99);
	test("year",dt.getYear()==2016);
	test("daylight savings time",dt.isDaylightSavingsTime());
	test("time zone",
		!charstring::compare(dt.getTimeZoneString(),"EDT"));
	test("offset from GMT",dt.getTimeZoneOffset()==-14400);
	test("seconds since 1970",dt.getEpoch()==secsinceepoch);
	test("date string",
		!charstring::compare(dt.getString(),datestring));
	test("date string (with usec)",
		!charstring::compare(dt.getString(true),datestringwithusec));
	stdoutput.printf("\n");


	// init from epoch
	stdoutput.printf("date from epoch:\n");
	dt.initialize(secsinceepoch);
	test("hour",dt.getHour()==21);
	test("minutes",dt.getMinutes()==54);
	test("seconds",dt.getSeconds()==30);
	test("useconds",!dt.getMicroseconds());
	test("month",dt.getMonth()==4);
	test("month name",
		!charstring::compare(dt.getMonthName(),"April"));
	test("month abbr",
		!charstring::compare(dt.getMonthAbbreviation(),"Apr"));
	test("day of month",dt.getDayOfMonth()==8);
	test("day of week",dt.getDayOfWeek()==6);
	test("day of year",dt.getDayOfYear()==99);
	test("year",dt.getYear()==2016);
	test("daylight savings time",dt.isDaylightSavingsTime());
stdoutput.printf("%s\n",dt.getTimeZoneString());
	test("time zone",
		!charstring::compare(dt.getTimeZoneString(),"EDT"));
	test("offset from GMT",dt.getTimeZoneOffset()==-14400);
	test("seconds since 1970",dt.getEpoch()==secsinceepoch);
	test("date string",
		!charstring::compare(dt.getString(),datestring));
	test("date string (with usec)",
		!charstring::compare(dt.getString(true),
					datestringwithzerousec));
	stdoutput.printf("\n");


	// init from epoch+usec
	stdoutput.printf("date from epoch+usec:\n");
	dt.initialize(secsinceepoch,500);
	test("hour",dt.getHour()==21);
	test("minutes",dt.getMinutes()==54);
	test("seconds",dt.getSeconds()==30);
	test("useconds",dt.getMicroseconds()==500);
	test("month",dt.getMonth()==4);
	test("month name",
		!charstring::compare(dt.getMonthName(),"April"));
	test("month abbr",
		!charstring::compare(dt.getMonthAbbreviation(),"Apr"));
	test("day of month",dt.getDayOfMonth()==8);
	test("day of week",dt.getDayOfWeek()==6);
	test("day of year",dt.getDayOfYear()==99);
	test("year",dt.getYear()==2016);
	test("daylight savings time",dt.isDaylightSavingsTime());
	test("time zone",
		!charstring::compare(dt.getTimeZoneString(),"EDT"));
	test("offset from GMT",dt.getTimeZoneOffset()==-14400);
	test("seconds since 1970",dt.getEpoch()==secsinceepoch);
	test("date string",
		!charstring::compare(dt.getString(),datestring));
	test("date string (with usec)",
		!charstring::compare(dt.getString(true),datestringwithusec));
	stdoutput.printf("\n");


	// add/subtract time
	stdoutput.printf("add/subtract time:");
	dt.addMicroseconds(1);
	test("addSeconds(1) hour",dt.getHour()==21);
	test("addSeconds(1) minutes",dt.getMinutes()==54);
	test("addSeconds(1) seconds",dt.getSeconds()==30);
	test("addSeconds(1) useconds",dt.getMicroseconds()==501);
	test("addSeconds(1) month",dt.getMonth()==4);
	test("addSeconds(1) day of month",dt.getDayOfMonth()==8);
	test("addSeconds(1) year",dt.getYear()==2016);
	dt.addMicroseconds(-1);
	test("addSeconds(-1) hour",dt.getHour()==21);
	test("addSeconds(-1) minutes",dt.getMinutes()==54);
	test("addSeconds(-1) seconds",dt.getSeconds()==30);
	test("addSeconds(-1) useconds",dt.getMicroseconds()==500);
	test("addSeconds(-1) month",dt.getMonth()==4);
	test("addSeconds(-1) day of month",dt.getDayOfMonth()==8);
	test("addSeconds(-1) year",dt.getYear()==2016);
	dt.addSeconds(1);
	test("addSeconds(1) hour",dt.getHour()==21);
	test("addSeconds(1) minutes",dt.getMinutes()==54);
	test("addSeconds(1) seconds",dt.getSeconds()==31);
	test("addSeconds(1) useconds",dt.getMicroseconds()==500);
	test("addSeconds(1) month",dt.getMonth()==4);
	test("addSeconds(1) day of month",dt.getDayOfMonth()==8);
	test("addSeconds(1) year",dt.getYear()==2016);
	dt.addSeconds(-1);
	test("addSeconds(-1) hour",dt.getHour()==21);
	test("addSeconds(-1) minutes",dt.getMinutes()==54);
	test("addSeconds(-1) seconds",dt.getSeconds()==30);
	test("addSeconds(-1) useconds",dt.getMicroseconds()==500);
	test("addSeconds(-1) month",dt.getMonth()==4);
	test("addSeconds(-1) day of month",dt.getDayOfMonth()==8);
	test("addSeconds(-1) year",dt.getYear()==2016);
	dt.addMinutes(1);
	test("addMinutes(1) hour",dt.getHour()==21);
	test("addMinutes(1) minutes",dt.getMinutes()==55);
	test("addMinutes(1) seconds",dt.getSeconds()==30);
	test("addMinutes(1) useconds",dt.getMicroseconds()==500);
	test("addMinutes(1) month",dt.getMonth()==4);
	test("addMinutes(1) day of month",dt.getDayOfMonth()==8);
	test("addMinutes(1) year",dt.getYear()==2016);
	dt.addMinutes(-1);
	test("addMinutes(-1) hour",dt.getHour()==21);
	test("addMinutes(-1) minutes",dt.getMinutes()==54);
	test("addMinutes(-1) seconds",dt.getSeconds()==30);
	test("addMinutes(-1) useconds",dt.getMicroseconds()==500);
	test("addMinutes(-1) month",dt.getMonth()==4);
	test("addMinutes(-1) day of month",dt.getDayOfMonth()==8);
	test("addMinutes(-1) year",dt.getYear()==2016);
	dt.addHours(1);
	test("addHours(1) hour",dt.getHour()==22);
	test("addHours(1) minutes",dt.getMinutes()==54);
	test("addHours(1) seconds",dt.getSeconds()==30);
	test("addHours(1) useconds",dt.getMicroseconds()==500);
	test("addHours(1) month",dt.getMonth()==4);
	test("addHours(1) day of month",dt.getDayOfMonth()==8);
	test("addHours(1) year",dt.getYear()==2016);
	dt.addHours(-1);
	test("addHours(-1) hour",dt.getHour()==21);
	test("addHours(-1) minutes",dt.getMinutes()==54);
	test("addHours(-1) seconds",dt.getSeconds()==30);
	test("addHours(-1) useconds",dt.getMicroseconds()==500);
	test("addHours(-1) month",dt.getMonth()==4);
	test("addHours(-1) day of month",dt.getDayOfMonth()==8);
	test("addHours(-1) year",dt.getYear()==2016);
	dt.addDays(1);
	test("addDays(1) hour",dt.getHour()==21);
	test("addDays(1) minutes",dt.getMinutes()==54);
	test("addDays(1) seconds",dt.getSeconds()==30);
	test("addDays(1) useconds",dt.getMicroseconds()==500);
	test("addDays(1) month",dt.getMonth()==4);
	test("addDays(1) day of month",dt.getDayOfMonth()==9);
	test("addDays(1) year",dt.getYear()==2016);
	dt.addDays(-1);
	test("addDays(-1) hour",dt.getHour()==21);
	test("addDays(-1) minutes",dt.getMinutes()==54);
	test("addDays(-1) seconds",dt.getSeconds()==30);
	test("addDays(-1) useconds",dt.getMicroseconds()==500);
	test("addDays(-1) month",dt.getMonth()==4);
	test("addDays(-1) day of month",dt.getDayOfMonth()==8);
	test("addDays(-1) year",dt.getYear()==2016);
	dt.addMonths(1);
	test("addMonths(1) hour",dt.getHour()==21);
	test("addMonths(1) minutes",dt.getMinutes()==54);
	test("addMonths(1) seconds",dt.getSeconds()==30);
	test("addMonths(1) useconds",dt.getMicroseconds()==500);
	test("addMonths(1) month",dt.getMonth()==5);
	//test("addMonths(1) day of month",dt.getDayOfMonth()==9);
	test("addMonths(1) year",dt.getYear()==2016);
	dt.addMonths(-1);
	test("addMonths(-1) hour",dt.getHour()==21);
	test("addMonths(-1) minutes",dt.getMinutes()==54);
	test("addMonths(-1) seconds",dt.getSeconds()==30);
	test("addMonths(-1) useconds",dt.getMicroseconds()==500);
	test("addMonths(-1) month",dt.getMonth()==4);
	test("addMonths(-1) day of month",dt.getDayOfMonth()==8);
	test("addMonths(-1) year",dt.getYear()==2016);
	dt.addYears(1);
	test("addYears(1) hour",dt.getHour()==21);
	test("addYears(1) minutes",dt.getMinutes()==54);
	test("addYears(1) seconds",dt.getSeconds()==30);
	test("addYears(1) useconds",dt.getMicroseconds()==500);
	test("addYears(1) month",dt.getMonth()==4);
	//test("addYears(1) day of month",dt.getDayOfMonth()==9);
	test("addYears(1) year",dt.getYear()==2017);
	dt.addYears(-1);
	test("addYears(-1) hour",dt.getHour()==21);
	test("addYears(-1) minutes",dt.getMinutes()==54);
	test("addYears(-1) seconds",dt.getSeconds()==30);
	test("addYears(-1) useconds",dt.getMicroseconds()==500);
	test("addYears(-1) month",dt.getMonth()==4);
	test("addYears(-1) day of month",dt.getDayOfMonth()==8);
	test("addYears(-1) year",dt.getYear()==2016);
	stdoutput.printf("\n");


	// conversions
	stdoutput.printf("conversions:\n");
	char	*string=datetime::getString(secsinceepoch);
	test("string from epoch",!charstring::compare(string,datestring));
	delete[] string;
	time_t	epoch=datetime::getEpoch(datestring);
	test("epoch from string",epoch==secsinceepoch);
	string=datetime::getString(secsinceepoch,500);
	test("string+usec from epoch",
			!charstring::compare(string,datestringwithusec));
	delete[] string;
	epoch=datetime::getEpoch(datestringwithusec);
	test("epoch from string+usec",epoch==secsinceepoch);
	stdoutput.printf("\n");


	// get time
	stdoutput.printf("dates from various sources:\n");
	test("system clock",dt.getSystemDateAndTime());
	// getHardwareDateAndTime/getAdjustedHardwareDateAndTime
	// aren't reliable on most systems
	stdoutput.printf("\n");


	// time zones
	stdoutput.printf("time zones:\n");
	const char * const *tz=datetime::getTimeZoneAbbreviations();
	uint32_t	i=0;
	test("",!charstring::compare(tz[i++],"ACST"));
	test("",!charstring::compare(tz[i++],"ACDT"));
	test("",!charstring::compare(tz[i++],"ACST-10:30ACDT"));
	test("",!charstring::compare(tz[i++],"AST"));
	test("",!charstring::compare(tz[i++],"ADT"));
	test("",!charstring::compare(tz[i++],"AST4ADT"));
	test("",!charstring::compare(tz[i++],"AEST"));
	test("",!charstring::compare(tz[i++],"AEDT"));
	test("",!charstring::compare(tz[i++],"AEST10AEDT"));
	test("",!charstring::compare(tz[i++],"AKST"));
	test("",!charstring::compare(tz[i++],"AKDT"));
	test("",!charstring::compare(tz[i++],"AKST-9AKDT"));
	test("",!charstring::compare(tz[i++],"CST"));
	test("",!charstring::compare(tz[i++],"CDT"));
	test("",!charstring::compare(tz[i++],"CST6CDT"));
	test("",!charstring::compare(tz[i++],"CET"));
	test("",!charstring::compare(tz[i++],"CEST"));
	test("",!charstring::compare(tz[i++],"CET-1CST"));
	test("",!charstring::compare(tz[i++],"EST"));
	test("",!charstring::compare(tz[i++],"EDT"));
	test("",!charstring::compare(tz[i++],"EST5EDT"));
	test("",!charstring::compare(tz[i++],"EET"));
	test("",!charstring::compare(tz[i++],"EEST"));
	test("",!charstring::compare(tz[i++],"EET-2EEST"));
	test("",!charstring::compare(tz[i++],"GMT"));
	test("",!charstring::compare(tz[i++],"BST"));
	test("",!charstring::compare(tz[i++],"GMT0BST"));
	test("",!charstring::compare(tz[i++],"HNA"));
	test("",!charstring::compare(tz[i++],"HAA"));
	test("",!charstring::compare(tz[i++],"HNA4HAA"));
	test("",!charstring::compare(tz[i++],"HNC"));
	test("",!charstring::compare(tz[i++],"HAC"));
	test("",!charstring::compare(tz[i++],"HNC6HAC"));
	test("",!charstring::compare(tz[i++],"HAST"));
	test("",!charstring::compare(tz[i++],"HADT"));
	test("",!charstring::compare(tz[i++],"HAST10HADT"));
	test("",!charstring::compare(tz[i++],"HNE"));
	test("",!charstring::compare(tz[i++],"HAE"));
	test("",!charstring::compare(tz[i++],"HNE5HAE"));
	test("",!charstring::compare(tz[i++],"HNP"));
	test("",!charstring::compare(tz[i++],"HAP"));
	test("",!charstring::compare(tz[i++],"HNP8HAP"));
	test("",!charstring::compare(tz[i++],"HNR"));
	test("",!charstring::compare(tz[i++],"HAR"));
	test("",!charstring::compare(tz[i++],"HNR7HAR"));
	test("",!charstring::compare(tz[i++],"HNT"));
	test("",!charstring::compare(tz[i++],"HAT"));
	test("",!charstring::compare(tz[i++],"HNT3:30HAT"));
	test("",!charstring::compare(tz[i++],"HNY"));
	test("",!charstring::compare(tz[i++],"HAY"));
	test("",!charstring::compare(tz[i++],"HNY9HAY"));
	test("",!charstring::compare(tz[i++],"MST"));
	test("",!charstring::compare(tz[i++],"MDT"));
	test("",!charstring::compare(tz[i++],"MST7MDT"));
	test("",!charstring::compare(tz[i++],"MEZ"));
	test("",!charstring::compare(tz[i++],"MESZ"));
	test("",!charstring::compare(tz[i++],"MEZ-1MESZ"));
	test("",!charstring::compare(tz[i++],"NST"));
	test("",!charstring::compare(tz[i++],"NDT"));
	test("",!charstring::compare(tz[i++],"NST3:30NDT"));
	test("",!charstring::compare(tz[i++],"PST"));
	test("",!charstring::compare(tz[i++],"PDT"));
	test("",!charstring::compare(tz[i++],"PST8PDT"));
	test("",!charstring::compare(tz[i++],"WET"));
	test("",!charstring::compare(tz[i++],"WEST"));
	test("",!charstring::compare(tz[i++],"WET-1WEST"));
	stdoutput.printf("\n");

	stdoutput.printf("time zone offsets:\n");
	const int32_t *tzo=datetime::getTimeZoneOffsets();
	i=0;
	test("",tzo[i++]==34200);
	test("",tzo[i++]==37800);
	test("",tzo[i++]==34200);
	test("",tzo[i++]==-14400);
	test("",tzo[i++]==-10800);
	test("",tzo[i++]==-14400);
	test("",tzo[i++]==36000);
	test("",tzo[i++]==39600);
	test("",tzo[i++]==36000);
	test("",tzo[i++]==-32400);
	test("",tzo[i++]==-28800);
	test("",tzo[i++]==-32400);
	test("",tzo[i++]==-21600);
	test("",tzo[i++]==-18000);
	test("",tzo[i++]==-21600);
	test("",tzo[i++]==3600);
	test("",tzo[i++]==7200);
	test("",tzo[i++]==3600);
	test("",tzo[i++]==-18000);
	test("",tzo[i++]==-14400);
	test("",tzo[i++]==-18000);
	test("",tzo[i++]==7200);
	test("",tzo[i++]==10800);
	test("",tzo[i++]==7200);
	test("",tzo[i++]==0);
	test("",tzo[i++]==3600);
	test("",tzo[i++]==0);
	test("",tzo[i++]==-14400);
	test("",tzo[i++]==-10800);
	test("",tzo[i++]==-14400);
	test("",tzo[i++]==-21600);
	test("",tzo[i++]==-18000);
	test("",tzo[i++]==-21600);
	test("",tzo[i++]==-36000);
	test("",tzo[i++]==-32400);
	test("",tzo[i++]==-36000);
	test("",tzo[i++]==-18000);
	test("",tzo[i++]==-14400);
	test("",tzo[i++]==-18000);
	test("",tzo[i++]==-28800);
	test("",tzo[i++]==-25200);
	test("",tzo[i++]==-28800);
	test("",tzo[i++]==-25200);
	test("",tzo[i++]==-21600);
	test("",tzo[i++]==-25200);
	test("",tzo[i++]==-12600);
	test("",tzo[i++]==-9000);
	test("",tzo[i++]==-12600);
	test("",tzo[i++]==-32400);
	test("",tzo[i++]==-28800);
	test("",tzo[i++]==-32400);
	test("",tzo[i++]==-25200);
	test("",tzo[i++]==-21600);
	test("",tzo[i++]==-25200);
	test("",tzo[i++]==3600);
	test("",tzo[i++]==7200);
	test("",tzo[i++]==3600);
	test("",tzo[i++]==-12600);
	test("",tzo[i++]==-9000);
	test("",tzo[i++]==-12600);
	test("",tzo[i++]==-28800);
	test("",tzo[i++]==-25200);
	test("",tzo[i++]==-28800);
	test("",tzo[i++]==0);
	test("",tzo[i++]==3600);
	stdoutput.printf("\n");


// FIXME: doesn't work with linux libc
#ifndef RUDIMENTS_HAVE_G_CONFIG_H
	// switch time zones
	stdoutput.printf("switch time zones:\n");
	dt.initialize(datestring);
	dt.adjustTimeZone("CST6CDT");
	test("CST",!charstring::compare(dt.getString(),cstdatestring));
	dt.adjustTimeZone("MST7MDT");
	test("MST",!charstring::compare(dt.getString(),mstdatestring));
	dt.adjustTimeZone("PST8PDT");
	test("PST",!charstring::compare(dt.getString(),pstdatestring));
	dt.adjustTimeZone("EST5EDT");
	test("EST",!charstring::compare(dt.getString(),estdatestring));
	stdoutput.printf("\n");
#endif


	// valid/invalid dates
	stdoutput.printf("valid/invalid dates:\n");
	const char	*str="02/20/1974 12:00:00";
	test("",datetime::validDateTime(str));
	str="02/30/1974 12:00:00";
	test("",!datetime::validDateTime(str));
	str="02/20/1974 12:00:00 EST5EDT";
	test("",datetime::validDateTime(str));
	str="02/30/1974 12:00:00 EST5EDT";
	test("",!datetime::validDateTime(str));
}
