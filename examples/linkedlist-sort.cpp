#include <rudiments/linkedlist.h>
#include <rudiments/randomnumber.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	linkedlist<uint32_t>	llis;
	linkedlist<uint32_t>	llhs;

	// generate random numbers and append them to the lists
	randomnumber	rr;
	rr.setSeed(randomnumber::getSeed());

	stdoutput.printf("generating numbers...\n");
	for (uint16_t i=0; i<20000; i++) {

		uint32_t	num;
		rr.generateNumber(&num);

		llis.append(num);
		llhs.append(num);
	}

	// sort one list using insertion sort
	stdoutput.printf("sorting using insertion sort...\n");
	llis.insertionSort();

	// sort one list using heap sort
	stdoutput.printf("sorting using heap sort...\n");
	llhs.heapSort();

	// print the lists
	stdoutput.printf("insertion sorted list\n");
	llis.print(5);
	stdoutput.write("...\n\n");

	// print the list
	stdoutput.printf("heap sorted list\n");
	llhs.print(5);
	stdoutput.write("...\n\n");
}
