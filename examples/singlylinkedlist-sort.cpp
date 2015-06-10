#include <rudiments/singlylinkedlist.h>
#include <rudiments/randomnumber.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	singlylinkedlist<uint32_t>	sllis;
	singlylinkedlist<uint32_t>	sllhs;

	// generate random numbers and append them to the lists
	randomnumber	rr;
	rr.setSeed(randomnumber::getSeed());

	stdoutput.printf("generating numbers...\n");
	for (uint16_t i=0; i<20000; i++) {

		uint32_t	num;
		rr.generateNumber(&num);

		sllis.append(num);
		sllhs.append(num);
	}

	// sort one list using insertion sort
	stdoutput.printf("sorting using insertion sort...\n");
	sllis.insertionSort();

	// sort one list using heap sort
	stdoutput.printf("sorting using heap sort...\n");
	sllhs.heapSort();

	// print the lists
	stdoutput.printf("insertion sorted list\n");
	sllis.print(5);
	stdoutput.write("...\n\n");

	// print the list
	stdoutput.printf("heap sorted list\n");
	sllhs.print(5);
	stdoutput.write("...\n\n");
}
