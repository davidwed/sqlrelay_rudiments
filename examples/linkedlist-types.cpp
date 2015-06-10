#include <rudiments/linkedlist.h>
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
	linkedlist<int16_t>	i16ll;
	linkedlist<int64_t>	i64ll;
	linkedlist< char * >	sll;
	linkedlist< myclass * >	oll;

	// populate the lists
	for (int64_t i=0; i<20; i++) {
		i16ll.append(i);
		i64ll.append(i);
		sll.append(charstring::parseNumber(i));
		oll.append(new myclass(i));
	}

	// print the lists of primitive types
	stdoutput.printf("list of 16-bit integers:\n");
	i16ll.print();
	stdoutput.write('\n');

	stdoutput.printf("list of 64-bit integers:\n");
	i16ll.print();
	stdoutput.write('\n');

	stdoutput.printf("list of strings:\n");
	i16ll.print();
	stdoutput.write('\n');

	// manually print the list of objects
	stdoutput.printf("list of objects:\n");
	for (linkedlistnode< myclass * > *n=oll.getFirst(); n; n=n->getNext()) {
		n->getValue()->print();
	}
	stdoutput.write('\n');

	// clean up
	for (linkedlistnode< char * > *n=sll.getFirst(); n; n=n->getNext()) {
		delete[] n->getValue();
	}
	for (linkedlistnode< myclass * > *n=oll.getFirst(); n; n=n->getNext()) {
		delete n->getValue();
	}
}
