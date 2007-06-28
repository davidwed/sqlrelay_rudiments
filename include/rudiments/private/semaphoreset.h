// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
			void	createOperations();

		// FIXME: it should be possible to copy a semaphoreset
				semaphoreset(const semaphoreset &s);
		semaphoreset	&operator=(const semaphoreset &s);

		int	semGet(key_t key, int nsems, int semflg);
		bool	semTimedOp(struct sembuf *sops, timespec *ts);
		bool	semOp(struct sembuf *sops);

		semaphoresetprivate	*pvt;
