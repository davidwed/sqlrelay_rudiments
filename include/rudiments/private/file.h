// Copyright (c) 2002 David Muse
// See the COPYING file for more information

	private:
		struct	stat	st;
		bool		getcurrentpropertiesonopen;

		bool	lock(int method, short type, short whence,
						off_t start, off_t len);
		bool	checkLock(short type, short whence,
						off_t start, off_t len,
						flock *retlck);
		bool	unlock(short whence, off_t start, off_t len);
