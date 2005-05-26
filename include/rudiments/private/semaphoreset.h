// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
			void	createOperations();

			int	semid;
			bool	created;
			int	semcount;
		struct	sembuf	**waitop;
		struct	sembuf	**waitwithundoop;
		struct	sembuf	**signalop;
		struct	sembuf	**signalwithundoop;

			key_t	key;

		#ifndef HAVE_SEMUN
		union semun {
			int	val;
			struct	semid_ds	*buf;
			ushort	*array;
		};
		#endif

		// FIXME: it should be possible to copy a semaphoreset
				semaphoreset(const semaphoreset &s);
		semaphoreset	&operator=(const semaphoreset &s);

		int	semGet(key_t key, int nsems, int semflg);
		int	semControl(int semnum, int cmd, semun semctlun);
		bool	semTimedOp(struct sembuf *sops, timespec *ts);
		bool	semOp(struct sembuf *sops);
