// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DATETIME_H
#define RUDIMENTS_DATETIME_H

#include <rudiments/private/config.h>

#include <rudiments/environment.h>

#include <time.h>

// The datetime class provides methods for converting date/time formats and
// accessing various date/time values.

class datetime {
	public:

		// if you need a quick conversion, use one of these methods
		static char	*getString(time_t seconds);
		static char	*getString(const struct tm *tmstruct);
			// returns "mm/dd/yyyy hh:mm:ss TZN"
			// Note that this method allocates a buffer to return
			// the string in which must be deleted by the calling
			// program.
		static time_t	getEpoch(const char *datestring);
			// parses "mm/dd/yyyy hh:mm:ss TZN" and returns the
			// number of seconds since 1970.
			//
			// Note that TZN must be a valid timezone.  Otherwise
			// GMT is assumed.
			//
		static time_t	getEpoch(const struct tm *tmstruct);
			// converts "tmstruct" to the number of seconds
			// since 1970.
			//
			// Note that in "tmstruct", the timezone and GMT offset
			// must be set to valid values.  Otherwise GMT is
			// assumed.


		// if you need anything other than a quick conversion,
		// use these methods
			datetime();
			~datetime();

		int	initialize(const char *tmstring);
			// Parses "tmstring" and sets the date and time
			// represented in the class to that time.
			// Datestring must be "mm/dd/yyyy hh:mm:ss TZN".
			//
			// Note that TZN must be a valid timezone.  Otherwise
			// GMT is assumed.
			//
			// Returns 1 on success and 0 on failure.
		int	initialize(time_t seconds);
			// Processes "seconds" and sets the date and time
			// represented in the class to that time.
			// "seconds" is the number of seconds since 1970;
			// output by many standard time functions.
			//
			// Returns 1 on success and 0 on failure.
		int	initialize(const struct tm *tmstruct);
			// Processes "tmstruct" and sets the date and time
			// represented in the class to that time.
			// "tmstruct" is a (struct tm *); output by 
			// many standard time functions.
			//
			// Note that in "tmstruct", the timezone and GMT offset
			// must be set to valid values.  Otherwise GMT is
			// assumed.
			//
			// Returns 1 on success and 0 on failure.


		int	getSystemDateAndTime();
			// Sets the date and time represented in the class to
			// the date and time stored in the system clock.
			//
			// Returns 1 on success and 0 on failure.
		int	getHardwareDateAndTime(const char *hwtz);
			// This method only works if your system has a working
			// real-time clock at /dev/rtc.
			//
			// Sets the date and time represented in the class to
			// the date and time stored in the hardware clock.
			//
			// "hwtz" must be set to the timezone that the hardware
			// clock is using.
			//
			// Returns 1 on success and 0 on failure.
		int	getAdjustedHardwareDateAndTime(const char *hwtz);
			// This method only works if your system has a working
			// real-time clock at /dev/rtc.
			//
			// Gets the date and time from the hardware clock,
			// then adjusts it to the timezone used by the system.
			//
			// Returns 1 on success and 0 on failure.
		int	setSystemDateAndTime();
			// Sets the system clock's date and time to the date
			// and time currently represented in the class.
			//
			// Returns 1 on success and 0 on failure.
		int	setHardwareDateAndTime(const char *hwtz);
			// This method only works if your system has a working
			// real-time clock at /dev/rtc.
			//
			// Sets the hardware clock's date and time to the date
			// and time currently represented in the class.
			//
			// "hwtz" must be set to the timezone that the system
			// clock using.
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

		char	*getTimeZoneString() const;
			// returns a 3 character string representing the
			// time zone

		long	getTimeZoneOffset() const;
			// returns the offset from GMT (in seconds)

		int	adjustTimeZone(const char *newtz);
			// Recalculates the time currently represented in the
			// class to correspond to the time zone "newtz".
			//
			// Returns 1 on success and 0 on failure.


		// These methods allow you to add discrete time intervals to
		// the time currently represented in the class.
		void	addSeconds(int seconds);
		void	addMinutes(int minutes);
		void	addHours(int hours);
		void	addDays(int days);
		void	addMonths(int months);
		void	addYears(int years);


		// These methods output conversions to other date/time
		// formats.
		char	*getString();
			// returns "mm/dd/yyyy hh:mm:ss TZN"
			// (Note that this method returns a pointer to an
			// internal string which will be deleted if the
			// class instance is deleted.)
		time_t	getEpoch() const;
			// returns the number of seconds since 1970
		struct tm	*getTm();
			// returns a pointer to the internal "struct tm"


		// These methods return timezone data
		static char	**getTimeZoneAbbreviations();
				// Returns a NULL terminated array of timezone
				// abbreviations.
		static long	*getTimeZoneOffsets();
				// Returns an array of timezone offsets from
				// GMT (in seconds).  Each element of this
				// array corresponds to an element of the
				// array returned b getTimeZoneStrings().

	#include <rudiments/private/datetime.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/datetimeinlines.h>
#endif

#endif
