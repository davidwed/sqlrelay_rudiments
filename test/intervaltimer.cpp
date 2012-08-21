#include <rudiments/intervaltimer.h>
#include <rudiments/signalclasses.h>
#include <rudiments/snooze.h>
#include <sys/time.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

void alarmhandler(int sig) {
	printf("alarm!\n");
}

void waitForTimer(intervaltimer *t) {

	long	isec;
	long	iusec;
	long	psec;
	long	pusec;
	t->getInitialInterval(&isec,&iusec);
	t->getPeriodicInterval(&psec,&pusec);
	printf("initial: %ldsec %ldusec   ",isec,iusec);
	printf("periodic: %ldsec %ldusec\n",psec,pusec);
		

	for (;;) {
		long	sec;
		long	usec;
		t->getTimeRemaining(&sec,&usec);
		printf("time remaining: %ldsec %ldusec\n",sec,usec);
		if (sec==0 && usec==0) {
			break;
		}
		snooze::macrosnooze(1);
	}
}

int main(int argc, char **argv) {

	signalhandler	ah;
	ah.setHandler(alarmhandler);
	ah.handleSignal(SIGALRM);

	intervaltimer	t(ITIMER_REAL);

	t.setInitialInterval(2,0);
	t.start();
	waitForTimer(&t);

	t.setInitialInterval(2,500000);
	t.start();
	waitForTimer(&t);
}
