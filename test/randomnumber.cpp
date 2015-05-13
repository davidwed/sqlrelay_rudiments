// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/randomnumber.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

        int	seed=randomnumber::getSeed();
        stdoutput.printf("Seed: %d\n\n",seed);

        stdoutput.printf("Random numbers between 0 and %lld:\n",
				(uint64_t)randomnumber::getRandMax());
        stdoutput.printf("%lld (next might be the same as this)\n",
				(uint64_t)randomnumber::generateNumber(seed));
	randomnumber	r;
	r.setSeed(seed);
	for (uint16_t i=0; i<5; i++) {
		uint32_t	number=0;
        	r.generateNumber(&number);
        	stdoutput.printf("%lld\n",(uint64_t)number);
	}
        stdoutput.printf("\n");

	int32_t	lower[]={100,-1000,-100};
	int32_t	upper[]={1000,-100,100};

	for (uint16_t j=0; j<3; j++) {
		int32_t	bottom=lower[j];
		int32_t	top=upper[j];
        	stdoutput.printf("Random numbers between %d and %d:\n",
								bottom,top);
        	stdoutput.printf(" % 4d",
			randomnumber::generateScaledNumber(seed,bottom,top));
        	uint32_t	basenumber=
			randomnumber::generateNumber(seed+1);
        	int32_t	scalednumber=
			randomnumber::scaleNumber(basenumber,bottom,top);
        	stdoutput.printf(" % 4d\n",scalednumber);
		randomnumber	r;
		r.setSeed(seed);
		for (uint16_t k=0; k<100; k++) {
			int32_t	number=0;
        		r.generateScaledNumber(bottom,top,&number);
        		stdoutput.printf(" % 4d",number);
			if (!((k+1)%10)) {
        			stdoutput.printf("\n");
			}
		}
        	stdoutput.printf("\n");
	}

        stdoutput.printf("Generating full range of numbers (max=%lld)...\n",
					(uint64_t)randomnumber::getRandMax());
	datetime	start;
	start.getSystemDateAndTime();
	r.setSeed(0);
	for (int64_t k=0; k<randomnumber::getRandMax(); k++) {
		uint32_t	result;
		if (!r.generateNumber(&result)) {
        		stdoutput.printf("generateNumber failed: %lld\n",k);
			break;
		} else {
			if (k<10) {
        			stdoutput.printf("%lld: %lld\n",
							k,(uint64_t)result);
			} else if (k==10) {
        			stdoutput.printf("...\n");
			}
		}
	}
	datetime	end;
	end.getSystemDateAndTime();
	stdoutput.printf("%d seconds\n",end.getEpoch()-start.getEpoch());
}
