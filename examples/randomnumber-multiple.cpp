#include <rudiments/randomnumber.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create an instance and seed it
	randomnumber	rn;
	rn.setSeed(randomnumber::getSeed());


	// generate some unscaled numbers
	stdoutput.write("generating some random numbers:\n");
	for (uint16_t i=0; i<20; i++) {

		uint32_t	unscalednum;
		rn.generateNumber(&unscalednum);

		stdoutput.printf("%d, ",unscalednum);
	}
	stdoutput.write("\n\n");


	// generate some scaled numbers
	stdoutput.write("generating some scaled random numbers:\n");
	for (uint16_t j=0; j<20; j++) {

		int32_t		scalednum;
		rn.generateScaledNumber(1,10,&scalednum);

		stdoutput.printf("%d, ",scalednum);
	}
	stdoutput.write("\n\n");
}
