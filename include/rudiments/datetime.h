// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DATETIME_H
#define RUDIMENTS_DATETIME_H

#include <rudiments/private/config.h>

#include <time.h>

// The datetime class provides methods for converting date/time formats and
// accessing various date/time values.

class datetime {
	public:


		// if you need a quick conversion, use one of these methods
		static char	*getString(time_t epoch);
		static char	*getString(const tm *timestruct);
			// returns "mm/dd/yyyy hh:mm:ss"
			// Note that this method allocates a buffer to return
			// the string in which must be deleted by the calling
			// program.
		static time_t	getEpoch(const char *datestring);
			// parses "mm/dd/yyyy hh:mm:ss" and returns the
			// number of seconds since 1970.
		static time_t	getEpoch(const tm *timestruct);
			// converts "timestruct" to the number of seconds
			// since 1970.


		// if you need anything other than a quick conversion,
		// use these methods
			datetime();
			~datetime();

		int	initialize(const char *datestring);
			// Parses "datestring" and sets the date and time
			// represented in the class to that time.
			// Datestring must be "mm/dd/yyyy hh:mm:ss".
			//
			// Returns 1 on success and 0 on failure.
		int	initialize(time_t epoch);
			// Processes "epoch" and sets the date and time
			// represented in the class to that time.
			// Epoch is the number of seconds since 1970;
			// output by many standard time functions.
			//
			// Returns 1 on success and 0 on failure.
		int	initialize(const tm *timestruct);
			// Processes "timestruct" and sets the date and time
			// represented in the class to that time.
			// "timestruct" is a (struct tm *); output by 
			// many standard time functions.
			//
			// Returns 1 on success and 0 on failure.


		int	getSystemDateAndTime();
			// Sets the date and time represented in the class to
			// the date and time stored in the system clock.
			//
			// Returns 1 on success and 0 on failure.
		int	getHardwareDateAndTime();
			// This method only works if your system has a working
			// real-time clock at /dev/rtc.
			//
			// Sets the date and time represented in the class to
			// the date and time stored in the hardware clock.
			//
			// Returns 1 on success and 0 on failure.
		int	setSystemDateAndTime();
			// Sets the system clock's date and time to the date
			// and time currently represented in the class.
			//
			// Returns 1 on success and 0 on failure.
		int	setHardwareDateAndTime();
			// This method only works if your system has a working
			// real-time clock at /dev/rtc.
			//
			// Sets the hardware clock's date and time to the date
			// and time currently represented in the class.
			//
			// Returns 1 on success and 0 on failure.


		// These methods return commonly needed time/date values
		int	getHour() const;
		int	getMinutes() const;
		int	getSeconds() const;
		int	getMonth() const;
		int	getDayOfMonth() const;
		int	getDayOfWeek() const;
		int	getDayOfYear() const;
		int	getYear() const;

		int	isDaylightSavingsTime() const;
			// returns 1 if daylight savings time is currently
			// in effect and 0 if it isn't

		char	*getTimeZone() const;
			// returns a 3 character string representing the
			// time zone

		long	getTimeZoneOffset() const;
			// returns (in seconds) the offset from GMT


		// These methods output conversions to other date/time
		// formats.
		char	*getString();
			// returns "mm/dd/yyyy hh:mm:ss"
			// (Note that this method returns a pointer to an
			// internal string which will be deleted if the
			// class instance is deleted.)
		time_t	getEpoch() const;
			// returns the number of seconds since 1970
		tm	*getTm();
			// returns a pointer to the internal "struct tm"

	#include <rudiments/private/datetime.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/datetimeinlines.h>
#endif

#endif
