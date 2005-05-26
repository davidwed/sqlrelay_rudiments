// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
			int	shmid;
			bool	created;
			void	*shmptr;

		// FIXME: it should be possible to copy a sharedmemory object
				sharedmemory(const sharedmemory &s);
		sharedmemory	&operator=(const sharedmemory &s);


			int	shmGet(key_t key, size_t size, int shmflag);
			void	*shmAttach();
			bool	shmControl(int cmd, shmid_ds *buf);
