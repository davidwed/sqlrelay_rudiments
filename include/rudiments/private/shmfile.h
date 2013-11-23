// Copyright (c) 2004 David Muse
// See the COPYING file for more information

	private:
		virtual	void	openInternal(const char *name, int32_t flags);
		virtual	void	openInternal(const char *name,
						int32_t flags, mode_t perms);
		shmfileprivate	*pvt;
