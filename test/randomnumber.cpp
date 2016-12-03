// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/randomnumber.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#include "test.cpp"

const uint16_t	setcount=10;
const uint16_t	numbercount=1000;
const int32_t	lowerrange=-65536;
const int32_t	upperrange=65536;

void testRange(const char *title, int32_t *numbers, uint16_t count,
						int32_t bottom, int32_t top) {
	bool	success=true;
	for (uint16_t i=0; i<count && success; i++) {
		if (numbers[i]<bottom && numbers[i]>top) {
			success=false;
		}
	}
	test(title,success);
}

int main(int argc, const char **argv) {

	header("randomnumber");

	// get the seed
        uint32_t	seed=randomnumber::getSeed();
        stdoutput.printf("	Seed: %d\n\n",seed);

	randomnumber	r;
	int32_t	*numbers=new int32_t[numbercount];

	for (uint16_t i=0; i<setcount; i++) {

		// generate range
		seed=randomnumber::generateNumber(seed);
		int32_t	bottom=randomnumber::scaleNumber(
						seed,lowerrange,upperrange);
		seed=randomnumber::generateNumber(seed);
		int32_t	top=randomnumber::scaleNumber(
						seed,lowerrange,upperrange);
		if (bottom>top) {
			int32_t	temp=top;
			top=bottom;
			bottom=temp;
		}

        	stdoutput.printf("	%d numbers between %d and %d:\n",
							numbercount,bottom,top);

		// static methods
		for (uint16_t j=0; j<numbercount; j++) {
			numbers[j]=randomnumber::generateScaledNumber(
							seed,bottom,top);
			seed=numbers[j];
		}
		testRange("static methods - in range",
					numbers,numbercount,bottom,top);

		// instance methods
		r.setSeed(seed);
		bool	success=true;
		for (uint16_t j=0; j<numbercount && success; j++) {
			numbers[j]=0;
        		if (!r.generateScaledNumber(bottom,top,&(numbers[j]))) {
				success=false;
			}
		}
		test("instance methods - success",success);
		testRange("instance methods - in range",
					numbers,numbercount,bottom,top);
		stdoutput.printf("\n");
	}
}
