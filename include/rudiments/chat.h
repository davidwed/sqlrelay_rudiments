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

		int	chat::runScript(const char *script);

	#include <rudiments/private/chat.h>
};

#endif
