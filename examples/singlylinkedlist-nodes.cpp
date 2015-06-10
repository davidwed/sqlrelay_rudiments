#include <rudiments/singlylinkedlist.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	singlylinkedlist<uint32_t>	sll;

	// apppend nodes to the list
	sll.append(new singlylinkedlistnode<uint32_t>(5));
	sll.append(new singlylinkedlistnode<uint32_t>(6));
	sll.append(new singlylinkedlistnode<uint32_t>(7));

	// prepend nodes to the list
	sll.prepend(new singlylinkedlistnode<uint32_t>(2));
	sll.prepend(new singlylinkedlistnode<uint32_t>(1));
	sll.prepend(new singlylinkedlistnode<uint32_t>(0));

	// insert nodes
	sll.insertAfter(sll.find(2),new singlylinkedlistnode<uint32_t>(3));
	sll.insertAfter(sll.find(3),new singlylinkedlistnode<uint32_t>(4));

	// move nodes around
	sll.prepend(new singlylinkedlistnode<uint32_t>(8));
	sll.prepend(new singlylinkedlistnode<uint32_t>(9));
	sll.prepend(new singlylinkedlistnode<uint32_t>(10));
	sll.moveAfter(sll.find(7),sll.find(8));
	sll.moveAfter(sll.find(8),sll.find(9));
	sll.moveAfter(sll.find(9),sll.find(10));

	// length
	stdoutput.printf("The list contains %lld nodes.\n\n",sll.getLength());

	// print the list
	stdoutput.write("Current contents:\n");
	sll.print();
	stdoutput.write('\n');

	// print the first 3 values in the list
	stdoutput.write("First 3 values:\n");
	sll.print(3);
	stdoutput.write('\n');
}
