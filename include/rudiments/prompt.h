// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROMPT_H
#define RUDIMENTS_PROMPT_H

#include <rudiments/private/promptincludes.h>

class promptprivate;

/** The prompt class provides methods for displaying a prompt and accepting
 *  user input.
 *
 *  Input history and tab-completion are supported if the underlying system
 *  supports them.  On Unix-like systems, this is enabled by the readline
 *  library.  On many Windows systems, this is provided by the OS. */
class RUDIMENTS_DLLSPEC prompt {
	public:

		/** Creates an instance of the prompt class. */
		prompt();

		/** Deletes this instance of the prompt class.
		 *
 		 *  Calls flushHistory(). */
		virtual	~prompt();

		/** Sets the name of the file used to store the input histroy.
		 *  On Unix-like systems, this is traditionally a dot-file in
		 *  the user's home directory.
		 *
		 *  Does nothing if rudiments is compiled without support for
		 *  readline. */
		void	setHistoryFile(const char *filename);

		/** Returns the name of the file used to store the input
 		 *  history. */
		const char	*getHistoryFile();

		/** Sets the maximum number of lines of input history that will
 		 *  be stored in the input history file.  The history file will
 		 *  be truncated to this number of lines when flushHistory() is
 		 *  called, either explicitly or implicitly.
 		 *
 		 *  Defaults to 1024.
 		 *
 		 *  If set to 0 then the input history file will not be
 		 *  truncated during calls to flushHistory().
		 *
		 *  Does nothing if rudiments is compiled without support for
		 *  readline. */
		void	setMaxHistoryLines(uint32_t lines);

		/** Returns the maximum number of lines to store in the input
 		 *  history file. */
		uint32_t	getMaxHistoryLines();

		/** Sets the maximum number of lines of input history that will
		 *  be queued in memory before flushHistory() is called
		 *  automatically.
 		 *
 		 *  Defaults to 1024.
 		 *
 		 *  If set to 0 then an unlimited number of lines will be
 		 *  queued, and will only be flushed when flushHistory() is
 		 *  called manually, or when the instance of the class is
 		 *  deleted.
		 *
		 *  Does nothing if rudiments is compiled without support for
		 *  readline. */
		void	setMaxHistoryQueue(uint32_t queue);

		/** Returns the maximum number of lines of input history that
 		 *  will be queued in memory before flushHistory() will be
 		 *  called automatically. */
		uint32_t	getMaxHistoryQueue();

		/** Sets the prompt that will be displayed during the next
		 *  call to read(). */
		void	setPrompt(const char *prompt);

		/** Returns the prompt that will be displayed during the next
		 *  call to read(). */
		const char	*getPrompt();

		/** Displays the prompt configured by setPrompt(), reads a
		 *  line from standard input, queues the line in memory, and
		 *  calls flushHistory() if the queue has grown beyond the
		 *  number of lines configured by getMaxHistoryQueue().
		 *
		 *  Returns the line that was read from standard input.
		 *
		 *  Note that return value is allocated internally and returned.
		 *  The calling program must free the buffer. */
		char	*read();

		/** Flushes all lines of input history queued in memory to the
		 *  file configured by setHistoryFile(). */
		void	flushHistory();

	#include <rudiments/private/prompt.h>
};

#endif
