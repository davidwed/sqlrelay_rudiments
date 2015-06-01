#include <rudiments/intervaltimer.h>
#include <rudiments/signalclasses.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif

#ifndef ITIMER_REAL
	#define	ITIMER_REAL 0
#endif

// for SIGALRM
#ifdef RUDIMENTS_HAVE_SIGNAL_H
	#include <signal.h>
#endif

void alarmhandler(int32_t sig) {
	stdoutput.printf("alarm!\n");
}

void waitForTimer(intervaltimer *t) {

	int32_t	isec;
	int32_t	iusec;
	int32_t	psec;
	int32_t	pusec;
	t->getInitialInterval(&isec,&iusec);
	t->getPeriodicInterval(&psec,&pusec);
	stdoutput.printf("initial: %ldsec %ldusec   ",isec,iusec);
	stdoutput.printf("periodic: %ldsec %ldusec\n",psec,pusec);
		

	for (;;) {
		int32_t	sec;
		int32_t	usec;
		t->getTimeRemaining(&sec,&usec);
		stdoutput.printf("time remaining: %ldsec %ldusec\n",sec,usec);
		if (sec==0 && usec==0) {
			break;
		}
		snooze::macrosnooze(1);
	}
}

int main(int argc, char **argv) {

	signalhandler	ah;
	ah.setHandler(alarmhandler);
#ifdef SIGALRM
	ah.handleSignal(SIGALRM);
#endif

	intervaltimer	t(ITIMER_REAL);

	t.setInitialInterval(2,0);
	t.start();
	waitForTimer(&t);

	t.setInitialInterval(2,500000);
	t.start();
	waitForTimer(&t);
}
