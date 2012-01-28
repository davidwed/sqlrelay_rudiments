// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		// FIXME: it should be possible to copy a sharedmemory object
				sharedmemory(const sharedmemory &s);
		sharedmemory	&operator=(const sharedmemory &s);


		int32_t	shmGet(key_t key, size_t size, int32_t shmflag);
		void	*shmAttach();
		bool	shmControl(int32_t cmd, shmid_ds *buf);

		sharedmemoryprivate	*pvt;
