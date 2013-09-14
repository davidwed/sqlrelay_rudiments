// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/randomnumber.h>
#include <rudiments/stdio.h>
#include <stdlib.h>

int main(int argc, const char **argv) {

        // it's common to seed a random number generator with the number of
        // seconds since 1970 (the epoch), so we'll get the current date.
        int	seed=randomnumber::getSeed();

        stdoutput.printf("A random number between 0 and %d: ",RAND_MAX);
        stdoutput.printf("%d",randomnumber::generateNumber(seed));
        stdoutput.printf("\n");

        stdoutput.printf("A random number between 100 and 1000: ");
        stdoutput.printf("%d",
		randomnumber::generateScaledNumber(seed,100,1000));
        stdoutput.printf("\n");

        stdoutput.printf("Another random number between 100 and 1000: ");
        int     basenumber=randomnumber::generateNumber(seed+1);
        int     scalednumber=randomnumber::scaleNumber(basenumber,100,1000);
        stdoutput.printf("%d",scalednumber);
        stdoutput.printf("\n");

        stdoutput.printf("A random number between -100 and -1000: ");
        stdoutput.printf("%d",
		randomnumber::generateScaledNumber(seed,-100,-1000));
        stdoutput.printf("\n");

        stdoutput.printf("Another random number between -100 and -1000: ");
        basenumber=randomnumber::generateNumber(seed+1);
        scalednumber=randomnumber::scaleNumber(basenumber,-100,-1000);
        stdoutput.printf("%d",scalednumber);
        stdoutput.printf("\n");

        stdoutput.printf("A random number between -100 and 100: ");
        stdoutput.printf("%d",
		randomnumber::generateScaledNumber(seed,-100,100));
        stdoutput.printf("\n");

        stdoutput.printf("Another random number between -100 and 100: ");
        basenumber=randomnumber::generateNumber(seed+1);
        scalednumber=randomnumber::scaleNumber(basenumber,-100,100);
        stdoutput.printf("%d",scalednumber);
        stdoutput.printf("\n");
}
