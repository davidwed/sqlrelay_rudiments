// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CHAT_H
#define RUDIMENTS_CHAT_H

#include <rudiments/private/chatincludes.h>

// The chat class implements methods for chatting with something over a
// file descriptor.

class chat {
	public:
			chat(filedescriptor *readfd, filedescriptor *writefd);
			~chat();

		int	runScript(const char *script, char **abort);
		int	runScript(const char *script, char **abort,
						namevaluepairs *variables);

	#include <rudiments/private/chat.h>
};

#endif
