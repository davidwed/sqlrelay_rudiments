// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DLL_H
#define RUDIMENTS_DLL_H

#ifdef _WIN32
	#ifdef BUILDING_RUDIMENTS
		#define DLLSPEC __declspec(dllexport)
	#else
		#define DLLSPEC __declspec(dllimport)
	#endif
#else
	#define DLLSPEC
#endif

#endif
