// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	friend class logger;
		virtual	~stderrdestination();
	private:
		void	write(const char *string);
