// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

	private:
		#if !defined(HAVE_SETENV) && defined(HAVE_PUTENV)
			namevaluepairs	envstrings;
		#endif
