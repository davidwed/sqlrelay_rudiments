// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/randomnumber.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

        int	seed=randomnumber::getSeed();
        stdoutput.printf("Seed: %d\n",seed);

        stdoutput.printf("Random numbers between 0 and %d:\n",
					randomnumber::getRandMax());
        stdoutput.printf("%d (next should be the same as this)\n",
				randomnumber::generateNumber(seed));
	randomnumber	r;
	r.setSeed(seed);
	for (uint16_t i=0; i<5; i++) {
		int32_t	number=0;
        	r.generateNumber(&number);
        	stdoutput.printf("%d\n",number);
	}
        stdoutput.printf("\n");

	int32_t	lower[]={100,-1000,-100};
	int32_t	upper[]={1000,-100,100};

	for (uint16_t i=0; i<3; i++) {
		int32_t	bottom=lower[i];
		int32_t	top=upper[i];
        	stdoutput.printf("Random numbers between %d and %d:\n",
								bottom,top);
        	stdoutput.printf("%d\n",
			randomnumber::generateScaledNumber(seed,bottom,top));
        	int32_t	basenumber=
			randomnumber::generateNumber(seed+1);
        	int32_t	scalednumber=
			randomnumber::scaleNumber(basenumber,bottom,top);
        	stdoutput.printf("%d\n",scalednumber);
		randomnumber	r;
		r.setSeed(seed);
		for (uint16_t i=0; i<5; i++) {
			int32_t	number=0;
        		r.generateScaledNumber(bottom,top,&number);
        		stdoutput.printf("%d\n",number);
		}
        	stdoutput.printf("\n");
	}

        stdoutput.printf("Generating full range of numbers (max=%ld)...\n",
						randomnumber::getRandMax());
	datetime	start;
	start.getSystemDateAndTime();
	r.setSeed(0);
	for (int64_t i=0; i<randomnumber::getRandMax(); i++) {
		int32_t	result;
		if (!r.generateNumber(&result)) {
        		stdoutput.printf("generateNumber failed: %lld\n",i);
			break;
		} else {
			if (i<10) {
        			stdoutput.printf("%lld: %ld\n",i,result);
			} else if (i==10) {
        			stdoutput.printf("...\n");
			}
		}
	}
	datetime	end;
	end.getSystemDateAndTime();
	stdoutput.printf("%d seconds\n",end.getEpoch()-start.getEpoch());
}
