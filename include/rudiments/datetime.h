// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DATETIME_H
#define RUDIMENTS_DATETIME_H

#include <rudiments/private/datetimeincludes.h>

class datetimeprivate;

/** The datetime class represents a date/time combination and provides methods
 *  for getting and setting dates and times, converting date/time formats and
 *  accessing various date/time values. */
class RUDIMENTS_DLLSPEC datetime {
	public:

		/** Takes "seconds" since the epoch (the number of seconds
		 *  since 1970) and returns a string in
		 *  "mm/dd/yyyy hh:mm:ss TZN" format.
		 *	
		 *  Note that this method allocates a buffer to return the
		 *  string in which must be deleted by the calling program. */
		static char	*getString(time_t seconds);

		/** Takes "tmstruct", a pointer to a platform-specific time
		 *  structure (struct tm * on unix) and returns a string in
		 *  "mm/dd/yyyy hh:mm:ss TZN" format.
		 *	
		 *  Note that this method allocates a buffer to return the
		 *  string in which must be deleted by the calling program. */
		static char	*getString(const void *tmstruct);

		/** Parses "datestring" which must be of the format:
		 *  "mm/dd/yyyy hh:mm:ss TZN" and returns the number of seconds
		 *  since 1970 (the epoch).
		 *	
		 * Note that TZN must be a valid timezone.  Otherwise
		 * GMT is assumed. */
		static time_t	getEpoch(const char *datestring);

		/** Takes "tmstruct", a pointer to a platform-specific 
		 *  time structure (struct tm * on unix) and returns the number
		 *  of seconds since 1970 (the eopch).
		 *	
		 *  Note that in "tmstruct", the timezone and GMT offset must
		 *  be set to valid values.  Otherwise GMT is assumed. */
		static time_t	getEpoch(const void *tmstruct);


		/** Creates an instance of the datetime class. */
		datetime();

		/** Destroys this instance of the datetime class. */
		~datetime();

		/** Parses "tmstring" and sets the date and time
		 *  represented in the class to that time.
		 *  "tmstring" must be of the format: "mm/dd/yyyy hh:mm:ss TZN".
		 * 
		 *  Note that TZN must be a valid timezone.  Otherwise
		 *  GMT is assumed.
		 * 
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *tmstring);

		/** Processes "seconds" and sets the date and time
		 *  represented in the class to that time.
		 *  "seconds" is the number of seconds since 1970 (the epoch).
		 * 
		 *  Returns true on success and false on failure. */
		bool	initialize(time_t seconds);

		/** Processes "tmstruct" and sets the date and time
		 *  represented in the class to that time.
		 * 
		 *  "tmstruct" should be a pointer to a platform-specific
		 *  time structure (struct tm * on unix)
		 * 
		 *  Note that in "tmstruct", the timezone and GMT offset
		 *  must be set to valid values.  Otherwise GMT is
		 *  assumed.
		 * 
		 *  Returns true on success and false on failure. */
		bool	initialize(const void *tmstruct);


		/** Sets the date and time represented in the class to
		 *  the date and time stored in the system clock.
		 * 
		 *  Returns true on success and false on failure. */
		bool	getSystemDateAndTime();

		/** This method only works if your system has a working
		 *  real-time clock at /dev/rtc.
		 * 
		 *  Sets the date and time represented in the class to
		 *  the date and time stored in the hardware clock.
		 * 
		 *  "hwtz" must be set to the timezone that the hardware
		 *  clock is using.
		 * 
		 *  Returns true on success and false on failure. */
		bool	getHardwareDateAndTime(const char *hwtz);

		/** This method only works if your system has a working
		 *  real-time clock at /dev/rtc.
		 *
		 *  Gets the date and time from the hardware clock,
		 *  then adjusts it to the timezone used by the system.
		 * 
		 *  Returns true on success and false on failure. */
		bool	getAdjustedHardwareDateAndTime(const char *hwtz);

		/** Sets the system clock's date and time to the date
		 *  and time currently represented in the class.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setSystemDateAndTime();

		/** This method only works if your system has a working
		 *  real-time clock at /dev/rtc.
		 * 
		 *  Sets the hardware clock's date and time to the date
		 *  and time currently represented in the class.
		 * 
		 *  "hwtz" must be set to the timezone that the system
		 *  clock using.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setHardwareDateAndTime(const char *hwtz);


		/** Returns the hour component of the date/time currently
		 *  represented in the instance of the class. */
		int32_t	getHour() const;

		/** Returns the minute component of the date/time currently
		 *  represented in the instance of the class. */
		int32_t	getMinutes() const;

		/** Returns the seconds component of the date/time currently
		 *  represented in the instance of the class. */
		int32_t	getSeconds() const;

		/** Returns the month component (1-12) of the date/time
		 *  currently represented in the instance of the class. */
		int32_t	getMonth() const;

		/** Returns the full name of the month component of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		const char	*getMonthName() const;

		/** Returns the abbreviated name of the month component of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		const char	*getMonthAbbreviation() const;

		/** Returns the day of the month component (1-31) of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		int32_t	getDayOfMonth() const;

		/** Returns the day of the week component (1-7) of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		int32_t	getDayOfWeek() const;

		/** Returns the day of the year component (1-365) of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		int32_t	getDayOfYear() const;

		/** Returns the year component (including the century) of
		 *  the date/time currently represented in the instance of the
		 *  class. */
		int32_t	getYear() const;

		/** Returns true if daylight savings time is currently
		 *  in effect and false if it isn't */
		bool	isDaylightSavingsTime() const;

		/** Returns a 3 character string representing the time zone */
		const char	*getTimeZoneString() const;

		/** Returns the offset from GMT in seconds */
		int32_t	getTimeZoneOffset() const;
			
		/** Recalculates the time currently represented in the
		 *  class to correspond to the time zone "newtz".
		 * 
		 *  If, in the new timezone, daylight savings time is
		 *  currently in effect, the time zone will be adjusted
		 *  accordingly.  Ie.  if "newtz" = EST and daylight
		 *  savings time is in effect in EST5EDT, then EDT
		 *  will be used instead.
		 * 
		 *  Returns true on success and false on failure. */
		bool	adjustTimeZone(const char *newtz);

		/** Recalculates the time currently represented in the
		 *  class to correspond to the time zone "newtz".
		 * 
		 *  If ignoredst is false, this method works like
		 *  adjustTimezone(const char *newtz).  However if
		 *  ignoredst is true, this method will not adjust
		 *  EST to EDT if daylight savings time is in
		 *  effect in EST5EDT.
		 * 
		 *  Returns true on success and false on failure. */
		bool	adjustTimeZone(const char *newtz, bool ignoredst);


		/** Sets the seconds component of the date/time represented in
		 *  the instance of the class to "seconds". */
		bool	setSeconds(int32_t seconds);

		/** Sets the minutes component of the date/time represented in
		 *  the instance of the class to "minutes". */
		bool	setMinutes(int32_t minutes);

		/** Sets the hours component of the date/time represented in
		 *  the instance of the class to "hours". */
		bool	setHours(int32_t hours);

		/** Sets the day-of-the-month component (1-31) of the date/time
 		 *  represented in the instance of the class to "days". */
		bool	setDays(int32_t days);

		/** Sets the month component (1-12) of the date/time
 		 *  represented in the instance of the class to "months". */
		bool	setMonths(int32_t months);

		/** Sets the year component of the date/time represented in the
		 *  instance of the class to "years". */
		bool	setYears(int32_t years);

		/** Adds "seconds" to the date/time represented in the
		 *  instance of the class. */
		bool	addSeconds(int32_t seconds);

		/** Adds "minutes" to the date/time represented in the
		 *  instance of the class. */
		bool	addMinutes(int32_t minutes);

		/** Adds "hours" to the date/time represented in the
		 *  instance of the class. */
		bool	addHours(int32_t hours);

		/** Adds "days" to the date/time represented in the
		 *  instance of the class. */
		bool	addDays(int32_t days);

		/** Adds "months" to the date/time represented in the
		 *  instance of the class. */
		bool	addMonths(int32_t months);

		/** Adds "years" to the date/time represented in the
		 *  instance of the class. */
		bool	addYears(int32_t years);


		/** Returns a string of the format: "mm/dd/yyyy hh:mm:ss TZN"
		 *  for the date/time represented in the instance of the class.
		 * 
		 *  (Note that this method returns a pointer to an internal
		 *  string which will be deleted if the class instance is
		 *  deleted.) */
		const char	*getString();

		/** Returns the number of seconds since 1970 (the epoch). */
		time_t		getEpoch() const;


		/** Returns a NULL terminated array of timezone
		 *  abbreviations. */
		static const char * const	*getTimeZoneAbbreviations();

		/** Returns an array of timezone offsets from
		 *  GMT (in seconds).  Each element of this
		 *  array corresponds to an element of the
		 *  array returned b getTimeZoneAbbreviations(). */
		static const int32_t		*getTimeZoneOffsets();

		/** Returns true if string represents a valid date/time and
		 *  false otherwise.
		 * 
		 *  For this methods to return true, "string" must be
		 *  formatted like: "00/00/0000 00:00:00" optionally
		 *  followed by a space and a timezone. */
		static bool	validDateTime(const char *string);


		/** Many of the functions that the datetime class uses
		 *  internally are not reentrant and thus not thread-safe.
		 *  Indeed, for some functions, there is no thread-safe version
		 *  available on any platform.
		 *
		 *  If your application is multi-threaded, you must use this
		 *  method to supply a mutex and ensure thread safety.
		 * 
		 *  If you don't supply a mutex, the methods in the class
		 *  will still work, but will not be thread-safe. */
		static	void	setTimeMutex(threadmutex *mtx);

	#include <rudiments/private/datetime.h>
};

#endif
