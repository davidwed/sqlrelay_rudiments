#include <rudiments/linkedlist.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	linkedlist<uint32_t>	ll;

	// apppend values to the list
	for (uint32_t i=0; i<20; i++) {
		ll.append(i);
	}

	// print the list forwards, all on one line
	stdoutput.write("forwards:\n");
	for (linkedlistnode<uint32_t> *n=ll.getFirst(); n; n=n->getNext()) {
		stdoutput.printf("%d ",n->getValue());
	}
	stdoutput.write("\n\n");

	// print the list backwards, all on one line
	stdoutput.write("backwards:\n");
	for (linkedlistnode<uint32_t> *n=ll.getLast(); n; n=n->getPrevious()) {
		stdoutput.printf("%d ",n->getValue());
	}
	stdoutput.write("\n\n");
}
