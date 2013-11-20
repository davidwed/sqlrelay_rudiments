// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
			void	createOperations();

		// FIXME: it should be possible to copy a semaphoreset
				semaphoreset(const semaphoreset &s);
		semaphoreset	&operator=(const semaphoreset &s);

		int32_t	semGet(key_t key, int32_t nsems,
					int32_t semflg, const int32_t *values);
		int32_t	semControl(semaphoresetprivate *pvt, int32_t semnum,
						int32_t cmd, semun *semctlun);
		bool	semTimedOp(struct sembuf *sops,
						int32_t seconds,
						int32_t nanoseconds);
		bool	semOp(struct sembuf *sops);

		semaphoresetprivate	*pvt;
