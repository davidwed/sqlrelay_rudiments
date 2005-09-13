// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		void	write(const char *logentry);

		// FIXME: should be possible to copy a logger
			logger(const logger &l);
		logger	&operator=(const logger &l);

		loggerprivate	*pvt;
