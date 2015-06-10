#include <rudiments/singlylinkedlist.h>
#include <rudiments/stdio.h>


// Define a simple class.  Instances of it will be stored in a list later.
class myclass {
	public:
			myclass(int64_t v) { value=v; }
		void	print() { stdoutput.printf("value: %lld\n",value); }
	private:
		int64_t	value;
};


int main(int argc, const char **argv) {

	// lists of various types
	singlylinkedlist<int16_t>	i16sll;
	singlylinkedlist<int64_t>	i64sll;
	singlylinkedlist< char * >	ssll;
	singlylinkedlist< myclass * >	osll;

	// populate the lists
	for (int64_t i=0; i<20; i++) {
		i16sll.append(i);
		i64sll.append(i);
		ssll.append(charstring::parseNumber(i));
		osll.append(new myclass(i));
	}

	// print the lists of primitive types
	stdoutput.printf("list of 16-bit integers:\n");
	i16sll.print();
	stdoutput.write('\n');

	stdoutput.printf("list of 64-bit integers:\n");
	i16sll.print();
	stdoutput.write('\n');

	stdoutput.printf("list of strings:\n");
	i16sll.print();
	stdoutput.write('\n');

	// manually print the list of objects
	stdoutput.printf("list of objects:\n");
	for (singlylinkedlistnode< myclass * > *n=osll.getFirst();
						n; n=n->getNext()) {
		n->getValue()->print();
	}
	stdoutput.write('\n');

	// clean up
	for (singlylinkedlistnode< char * > *n=ssll.getFirst();
						n; n=n->getNext()) {
		delete[] n->getValue();
	}
	for (singlylinkedlistnode< myclass * > *n=osll.getFirst();
						n; n=n->getNext()) {
		delete n->getValue();
	}
}
