// Copyright (c) 2004 David Muse
// See the COPYING file for more information

	private:
		virtual	int32_t	openInternal(const char *name, int32_t flags);
		virtual	int32_t	openInternal(const char *name,
						int32_t flags, mode_t perms);
		shmfileprivate	*pvt;
