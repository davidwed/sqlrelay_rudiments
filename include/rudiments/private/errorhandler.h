// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

			errorhandler();
			~errorhandler();

	protected:
		void	clearError();
		void	appendError(const char *string);
		void	appendError(char character);
		void	appendError(long number);
		void	appendError(double number);

		stringbuffer	*errorstr;
