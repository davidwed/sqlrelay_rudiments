// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

// for struct timespec (it includes inttypes.h as well)
#include <rudiments/private/snoozeincludes.h>

#include <sys/types.h>

#ifdef RUDIMENTS_HAVE_SYS_SEM_H
	// for struct sembuf
	#include <sys/sem.h>
#else
	struct sembuf {
		uint16_t	sem_num;
		int16_t		sem_op;
		int16_t		sem_flg;
	};
#endif
