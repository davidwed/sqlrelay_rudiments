// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DATETIME_H
#define RUDIMENTS_DATETIME_H

#include <rudiments/private/datetimeincludes.h>

// The datetime class provides methods for converting date/time formats and
// accessing various date/time values.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class datetimeprivate;

class RUDIMENTS_DLLSPEC datetime {
	public:

		// if you need a quick conversion, use one of these methods
		static char	*getString(time_t seconds);
		static char	*getString(const void *tmstruct);
			// returns "mm/dd/yyyy hh:mm:ss TZN"
			//
			// Note that this method allocates a buffer to return
			// the string in which must be deleted by the calling
			// program.
			//
			// "tmstruct" should be a pointer to a platform-specific
			// time structure (struct tm * on unix)
		static time_t	getEpoch(const char *datestring);
			// parses "mm/dd/yyyy hh:mm:ss TZN" and returns the
			// number of seconds since 1970.
			//
			// Note that TZN must be a valid timezone.  Otherwise
			// GMT is assumed.
			//
		static time_t	getEpoch(const void *tmstruct);
			// converts "tmstruct" to the number of seconds
			// since 1970.
			//
			// Note that in "tmstruct", the timezone and GMT offset
			// must be set to valid values.  Otherwise GMT is
			// assumed.
			//
			// "tmstruct" should be a pointer to a platform-specific
			// time structure (struct tm * on unix)


		// if you need anything other than a quick conversion,
		// use these methods
		datetime();
		~datetime();

		bool	initialize(const char *tmstring);
			// Parses "tmstring" and sets the date and time
			// represented in the class to that time.
			// Datestring must be "mm/dd/yyyy hh:mm:ss TZN".
			//
			// Note that TZN must be a valid timezone.  Otherwise
			// GMT is assumed.
			//
			// Returns true on success and false on failure.
		bool	initialize(time_t seconds);
			// Processes "seconds" and sets the date and time
			// represented in the class to that time.
			// "seconds" is the number of seconds since 1970;
			// output by many standard time functions.
			//
			// Returns true on success and false on failure.
		bool	initialize(const void *tmstruct);
			// Processes "tmstruct" and sets the date and time
			// represented in the class to that time.
			//
			// "tmstruct" should be a pointer to a platform-specific
			// time structure (struct tm * on unix)
			//
			// Note that in "tmstruct", the timezone and GMT offset
			// must be set to valid values.  Otherwise GMT is
			// assumed.
			//
			// Returns true on success and false on failure.


		bool	getSystemDateAndTime();
			// Sets the date and time represented in the class to
			// the date and time stored in the system clock.
			//
			// Returns true on success and false on failure.
		bool	getHardwareDateAndTime(const char *hwtz);
			// This method only works if your system has a working
			// real-time clock at /dev/rtc.
			//
			// Sets the date and time represented in the class to
			// the date and time stored in the hardware clock.
			//
			// "hwtz" must be set to the timezone that the hardware
			// clock is using.
			//
			// Returns true on success and false on failure.
		bool	getAdjustedHardwareDateAndTime(const char *hwtz);
			// This method only works if your system has a working
			// real-time clock at /dev/rtc.
			//
			// Gets the date and time from the hardware clock,
			// then adjusts it to the timezone used by the system.
			//
			// Returns true on success and false on failure.
		bool	setSystemDateAndTime();
			// Sets the system clock's date and time to the date
			// and time currently represented in the class.
			//
			// Returns true on success and false on failure.
		bool	setHardwareDateAndTime(const char *hwtz);
			// This method only works if your system has a working
			// real-time clock at /dev/rtc.
			//
			// Sets the hardware clock's date and time to the date
			// and time currently represented in the class.
			//
			// "hwtz" must be set to the timezone that the system
			// clock using.
			//
			// Returns true on success and false on failure.


		// These methods return commonly needed time/date values
		int32_t		getHour() const;
		int32_t		getMinutes() const;
		int32_t		getSeconds() const;
		int32_t		getMonth() const;
		const char	*getMonthName() const;
		const char	*getMonthAbbreviation() const;
		int32_t		getDayOfMonth() const;
		int32_t		getDayOfWeek() const;
		int32_t		getDayOfYear() const;
		int32_t		getYear() const;

		bool	isDaylightSavingsTime() const;
			// returns 1 if daylight savings time is currently
			// in effect and 0 if it isn't

		const char	*getTimeZoneString() const;
			// returns a 3 character string representing the
			// time zone

		int32_t	getTimeZoneOffset() const;
			// returns the offset from GMT (in seconds)
			
		bool	adjustTimeZone(const char *newtz);
			// Recalculates the time currently represented in the
			// class to correspond to the time zone "newtz".
			//
			// If, in the new timezone, daylight savings time is
			// currently in effect, the time zone will be adjusted
			// accordingly.  Ie.  if "newtz" = EST and daylight
			// savings time is in effect in EST5EDT, then EDT
			// will be used instead.
			//
			// Returns true on success and false on failure.

		bool	adjustTimeZone(const char *newtz, bool ignoredst);
			// Recalculates the time currently represented in the
			// class to correspond to the time zone "newtz".
			//
			// If ignoredst is false, this method works like
			// adjustTimezone(const char *newtz).  However if
			// ignoredst is true, this method will not adjust
			// EST to EDT if daylight savings time is in
			// effect in EST5EDT.
			//
			// Returns true on success and false on failure.


		// These methods allow you to change the time currently
		// represented in the class.
		bool	setSeconds(int32_t seconds);
		bool	setMinutes(int32_t minutes);
		bool	setHours(int32_t hours);
		bool	setDays(int32_t days);
		bool	setMonths(int32_t months);
		bool	setYears(int32_t years);


		// These methods allow you to add discrete time intervals to
		// the time currently represented in the class.
		bool	addSeconds(int32_t seconds);
		bool	addMinutes(int32_t minutes);
		bool	addHours(int32_t hours);
		bool	addDays(int32_t days);
		bool	addMonths(int32_t months);
		bool	addYears(int32_t years);


		// These methods output conversions to other date/time
		// formats.
		const char	*getString();
			// returns "mm/dd/yyyy hh:mm:ss TZN"
			// (Note that this method returns a pointer to an
			// internal string which will be deleted if the
			// class instance is deleted.)
		time_t		getEpoch() const;
			// returns the number of seconds since 1970
		const void	*getInternalTimeStructure();
			// returns a pointer to the internal platform-specific
			// time structure (struct tm * on unix)


		// These methods return timezone data
		static const char * const	*getTimeZoneAbbreviations();
				// Returns a NULL terminated array of timezone
				// abbreviations.
		static const int32_t		*getTimeZoneOffsets();
				// Returns an array of timezone offsets from
				// GMT (in seconds).  Each element of this
				// array corresponds to an element of the
				// array returned b getTimeZoneAbbreviations().

		static bool	validDateTime(const char *string);
				// Returns true if string represents a valid
				// date/time and false otherwise.
				//
				// "string" should be formatted like:
				// 00/00/0000 00:00:00
				// and may optionally be followed by a space
				// and a timezone.


		// Many of the functions that the datetime class uses internally
		// are not reentrant and thus not thread-safe.  Indeed, for
		// some functions, there is no thread-safe version.  If your
		// application is multi-threaded, you must use this method to
		// supply a mutex and ensure thread safety.
		//
		// If you don't supply a mutex, the methods in this class
		// will still work, but will not be thread-safe.
		static	void	setTimeMutex(mutex *mtx);
				// Allows you to supply a mutex to regulate
				// access to the non-reentrant functions.

	#include <rudiments/private/datetime.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
