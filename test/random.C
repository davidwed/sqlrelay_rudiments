// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <stdlib.h>
#include <rudiments/datetime.h>
#include <rudiments/randomnumber.h>
#include <stdio.h>

int main(int argc, const char **argv) {

        // it's common to seed a random number generator with the number of
        // seconds since 1970 (the epoch), so we'll get the current date.
        datetime        dt;

        printf("A random number between 0 and %d: ",RAND_MAX);
        printf("%d",randomnumber::generateNumber(dt.getEpoch()));
        printf("\n");

        printf("A random number between 100 and 1000: ");
        printf("%d",randomnumber::generateScaledNumber(dt.getEpoch(),100,1000));
        printf("\n");

        printf("Another random number between 100 and 1000: ");
        int     basenumber=randomnumber::generateNumber(dt.getEpoch()+1);
        int     scalednumber=randomnumber::scaleNumber(basenumber,100,1000);
        printf("%d",scalednumber);
        printf("\n");

        printf("A random number between -100 and -1000: ");
        printf("%d",randomnumber::generateScaledNumber(dt.getEpoch(),
							-100,-1000));
        printf("\n");

        printf("Another random number between -100 and -1000: ");
        basenumber=randomnumber::generateNumber(dt.getEpoch()+1);
        scalednumber=randomnumber::scaleNumber(basenumber,-100,-1000);
        printf("%d",scalednumber);
        printf("\n");

        printf("A random number between -100 and 100: ");
        printf("%d",randomnumber::generateScaledNumber(dt.getEpoch(),-100,100));
        printf("\n");

        printf("Another random number between -100 and 100: ");
        basenumber=randomnumber::generateNumber(dt.getEpoch()+1);
        scalednumber=randomnumber::scaleNumber(basenumber,-100,100);
        printf("%d",scalednumber);
        printf("\n");
}
