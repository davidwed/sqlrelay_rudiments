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
