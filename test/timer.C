#include <rudiments/intervaltimer.h>
#include <rudiments/signalclasses.h>
#include <stdio.h>
#include <unistd.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

RETSIGTYPE	alarmhandler() {
	printf("alarm!\n");
}

void	waitForTimer(intervaltimer *t) {

	itimerval	itv;
	t->getIntervals(&itv);
	printf("initial: %dsec %dusec   ",
		itv.it_value.tv_sec,itv.it_value.tv_usec);
	printf("interval: %dsec %dusec\n",
		itv.it_interval.tv_sec,itv.it_interval.tv_usec);

	for (;;) {
		timeval	gtv;
		t->getTimeRemaining(&gtv);
		printf("time remaining: %dsec %dusec\n",gtv.tv_sec,gtv.tv_usec);
		if (gtv.tv_sec==0 && gtv.tv_usec==0) {
			break;
		}
		sleep(1);
	}
}

int main(int argc, char **argv) {

	signalhandler	ah;
	ah.setHandler((void *)alarmhandler);
	ah.handleSignal(SIGALRM);

	intervaltimer	t(ITIMER_REAL);

	t.setInitialInterval(2,0);
	t.start();
	waitForTimer(&t);

	t.setInitialInterval(2,500000);
	t.start();
	waitForTimer(&t);

	timeval	tv;
	tv.tv_sec=2;
	tv.tv_usec=500000;
	t.setInitialInterval(&tv);
	t.start();
	waitForTimer(&t);

	itimerval	stv;
	stv.it_value.tv_sec=2;
	stv.it_value.tv_usec=0;
	stv.it_interval.tv_sec=0;
	stv.it_interval.tv_usec=0;
	t.setIntervals(&stv);
	t.start();
	waitForTimer(&t);
}
