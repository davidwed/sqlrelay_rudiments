// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CHAT_H
#define RUDIMENTS_CHAT_H

#include <rudiments/private/chatincludes.h>

// The chat class implements methods for chatting with something (such as
// a modem) over a file descriptor or set of file descriptors.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class chatprivate;

class RUDIMENTS_DLLSPEC chat {
	public:
		chat(const filedescriptor *fd);
		// Initializes an instance of chat.  Data will be
		// written to and read from "fd".
		chat(const filedescriptor *rfd, const filedescriptor *wfd);
		// Initializes an instance of chat.  Data will be
		// written to "wfd" and read from "rfd".
		~chat();

		// These methods process "script" which is expected to be
		// a chunk of XML of the following format:
		//
		// <script>
		//	<timeout seconds="2"/>
		//	<abort string="ERROR"/>
		//	<send string="AT Z\p\p\p\r\n"/>
		//	<expect string="OK"/>
		//	<clearabort/>
		//	<abort string="(NO DIALTONE|NO DIAL TONE)"/>
		//	<abort string="NO CARRIER"/>
		//	<abort string="BUSY"/>
		//	<timeout seconds="30"/>
		//	<send string="ATDT18005555555\r\n"/>
		//	<expect string="CONNECT"/>
		// </script>
		//
		// runScript() processes each tag sequentially and may abort
		// before processing all of the tags if a user-initiated abort
		// occurs, if an abort condition is met or if a timeout occurs.
		//
		// The <send> tag specifies data to be written.  The following
		// escape sequences are recognized:
		//	\d - 1 second pause
		//	\p - 1/10th of a second pause
		//	\r - carriage return
		//	\n - new line
		//	\b - backspace
		//	\000 - an octal sequence
		//		(where 000 is replaced with 3 numbers <=8)
		//	\N - null
		// If a filedescriptor-level error occurs writing data, then
		// runScript() will return RESULT_ERROR.
		//	
		// The <timeout> tag specified the number of seconds to wait
		// for a response after a <send> tag has been fully processed
		// before runScript() will fail, returning a RESULT_TIMEOUT.
		// If no <timeout> tag is defined, a default timeout of 45
		// seconds will be used.
		//
		// The <abort> tag specifies a regular expression that defines
		// an abort condition.  If the regular expression is evaluated
		// true in the data read after processing a <send> tag, then
		// runScript() will return a value greater than 1 and set
		// "abort" to the regular expression that caused the abort.
		// Multiple <abort> tags may be used to define multiple abort
		// conditions.
		//
		// The <clearabort> tag clears all previously defined abort
		// conditions.
		//
		// The <expect> tag defines a regular expression that is
		// expected to be evaluated true in the data read after
		// processing a <send> tag.  As soon as the expression is
		// evaluated true, the next tag in the script is processed.
		// If a filedescriptor-level error occurs reading data, then
		// runScript() will return RESULT_ERROR.
		//
		// If runScript() successfully processes all tags then it
		// returns RESULT_SUCCESS.
		int32_t	runScript(const char *script, char **abort);
			// Runs "script", if an abort condition is met, the
			// "abort" is allocated and set to the abort string
			// from the script that caught the condition.
		int32_t	runScript(const char *script, char **abort,
						constnamevaluepairs *variables);
			// Runs "script" as runScript() above does, but also
			// looks through "script" for variables of format:
			//	$(variable)
			// For each variable that it finds, if there is a
			// matching name/value pair in "variables", the value
			// is substituted in for the variable.

	#include <rudiments/private/chat.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
