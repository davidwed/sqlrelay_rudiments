// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <stdio.h>
#include <rudiments/snooze.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	uint32_t	timeleft;
	bool		success=snooze::macrosnooze(5,&timeleft);
	printf("success=%d timeleft=%d\n",success,timeleft);
}
