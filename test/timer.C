#include <rudiments/timer.h>
#include <rudiments/signalclasses.h>
#include <stdio.h>
#include <unistd.h>

RETSIGTYPE	alarmhandler() {
	printf("alarm!\n");
}

void	waitForTimer(timer *t) {

	for (;;) {
		itimerval	gtv;
		t->getTimer(&gtv);
		printf("interval left: %dsec %dusec   ",
			gtv.it_interval.tv_sec,gtv.it_interval.tv_usec);
		printf("value left: %dsec %dusec\n",
			gtv.it_value.tv_sec,gtv.it_value.tv_usec);
		if (gtv.it_value.tv_sec==0 && gtv.it_value.tv_usec==0) {
			break;
		}
		sleep(1);
	}
}

int main(int argc, char **argv) {

	signalhandler	ah;
	ah.setHandler((void *)alarmhandler);
	ah.handleSignal(SIGALRM);

	timer	t(ITIMER_REAL);
	t.setTimer(2);
	waitForTimer(&t);
	t.setTimer(2,500000);
	waitForTimer(&t);
	timeval	tv;
	tv.tv_sec=2;
	tv.tv_usec=500000;
	t.setTimer(&tv);
	waitForTimer(&t);
	itimerval	stv;
	stv.it_interval.tv_sec=0;
	stv.it_interval.tv_usec=0;
	stv.it_value.tv_sec=2;
	stv.it_value.tv_usec=0;
	t.setTimer(&stv);
	waitForTimer(&t);
}
