// Copyright (c) 2014  David Muse
// See the file COPYING for more information

#include <rudiments/linkedlist.h>
#include <rudiments/singlylinkedlist.h>
#include <rudiments/randomnumber.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

// sort time should scale as square of listsize
const int32_t	listsize=1024*72;

// sort time should scale linearly with iterations
const int32_t	iterations=1;

const char *ordertitles[]={
	"forwards",
	"backwards",
	"scaled",
	"unscaled",
	NULL
};

int32_t	*scaledvalues=NULL;
int32_t	*unscaledvalues=NULL;

int main(int argc, char **argv) {

	datetime	start;
	datetime	finish;

	int32_t	seed=randomnumber::getSeed();
	scaledvalues=new int32_t[listsize];
	unscaledvalues=new int32_t[listsize];
	for (uint32_t i=0; i<listsize; i++) {
		seed=randomnumber::generateNumber(seed);
		scaledvalues[i]=randomnumber::scaleNumber(seed,0,10);
		unscaledvalues[i]=seed;
	}

	linkedlist<int32_t>		uintll;

	stdoutput.printf("linkedlist:\n");

	for (uint16_t j=0; j<4; j++) {

		stdoutput.printf("    %s\t",ordertitles[j]);
		start.getSystemDateAndTime();
		for (uint16_t k=0; k<iterations; k++) {
			for (int32_t i=0; i<listsize; i++) {
				if (j==0) {
					uintll.append(i);
				} else if (j==1) {
					uintll.prepend(i);
				} else if (j==2) {
					uintll.append(scaledvalues[i]);
				} else if (j==3) {
					uintll.append(unscaledvalues[i]);
				}
			}
			uintll.sort();
			uintll.clear();
		}
		finish.getSystemDateAndTime();
		stdoutput.printf("%d seconds\n",
				finish.getEpoch()-start.getEpoch());
	}


	singlylinkedlist<int32_t>		uintsll;

	stdoutput.printf("singlylinkedlist:\n");

	for (uint16_t j=0; j<4; j++) {

		stdoutput.printf("    %s\t",ordertitles[j]);
		start.getSystemDateAndTime();
		for (uint16_t k=0; k<iterations; k++) {
			for (int32_t i=0; i<listsize; i++) {
				if (j==0) {
					uintsll.append(i);
				} else if (j==1) {
					uintsll.prepend(i);
				} else if (j==2) {
					uintsll.append(scaledvalues[i]);
				} else if (j==3) {
					uintsll.append(unscaledvalues[i]);
				}
			}
			uintsll.sort();
			uintsll.clear();
		}
		finish.getSystemDateAndTime();
		stdoutput.printf("%d seconds\n",
				finish.getEpoch()-start.getEpoch());
	}
}
