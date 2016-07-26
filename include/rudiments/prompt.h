// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROMPT_H
#define RUDIMENTS_PROMPT_H

#include <rudiments/private/promptincludes.h>

class promptprivate;

/** The prompt class provides ... */
class RUDIMENTS_DLLSPEC prompt {
	public:
		prompt();
		virtual	~prompt();

		void	setHistoryFile(const char *filename);
		const char	*getHistoryFile();

		void	setMaxHistoryLines(uint32_t lines);
		uint32_t	getMaxHistoryLines();

		void	setMaxHistoryQueue(uint32_t queue);
		uint32_t	getMaxHistoryQueue();

		void	setPrompt(const char *prompt);
		const char	*getPrompt();

		char	*read();

		void	flushHistory();

	#include <rudiments/private/prompt.h>
};

#endif
