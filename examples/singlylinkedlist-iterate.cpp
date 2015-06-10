#include <rudiments/singlylinkedlist.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	singlylinkedlist<uint32_t>	sll;

	// apppend values to the list
	for (uint32_t i=0; i<20; i++) {
		sll.append(i);
	}

	// print the list forwards, all on one line
	for (singlylinkedlistnode<uint32_t> *n=sll.getFirst();
						n; n=n->getNext()) {
		stdoutput.printf("%d ",n->getValue());
	}
	stdoutput.write("\n\n");
}
