// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	friend class logger;
	public:
		virtual	~logdestination();
	protected:
		virtual void	write(const char *string)=0;
