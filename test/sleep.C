#include <rudiments/sleep.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, char **argv) {

	timespec	timetosleep;
	timetosleep.tv_sec=1;
	timetosleep.tv_nsec=10000;
	timespec	timeremaining;
	bool	result=sleep::nanosleep(&timetosleep,&timeremaining);
	printf("done sleeping: %d: %dsec %dnsec remaining\n",result,
		timeremaining.tv_sec,timeremaining.tv_nsec);
}
