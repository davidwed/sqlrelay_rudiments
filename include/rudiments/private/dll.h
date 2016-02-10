// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DLL_H
#define RUDIMENTS_DLL_H

#ifndef RUDIMENTS_DLLSPEC
	#ifdef _WIN32
		#ifdef LIBRUDIMENTS_EXPORTS
			#define RUDIMENTS_DLLSPEC __declspec(dllexport)
		#else
			#define RUDIMENTS_DLLSPEC __declspec(dllimport)
		#endif
	#else
		#define RUDIMENTS_DLLSPEC
	#endif
#endif
	
#endif
