#include <rudiments/dictionary.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a dictionary of name-value pairs
	dictionary<char *,char *>	nvp;

	// set some name-value pairs
	nvp.setValue(charstring::duplicate("color"),
			charstring::duplicate("blue"));
	nvp.setValue(charstring::duplicate("number"),
			charstring::duplicate("one"));
	nvp.setValue(charstring::duplicate("automobile"),
			charstring::duplicate("car"));
	nvp.setValue(charstring::duplicate("dwelling"),
			charstring::duplicate("house"));
	nvp.setValue(charstring::duplicate("parent"),
			charstring::duplicate("dad"));

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.print();
	stdoutput.write('\n');

	// remove a value
	dictionarynode<char *,char *>	*n=nvp.detach((char *)"color");
	delete[] n->getKey();
	delete[] n->getValue();
	delete n;

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.print();
	stdoutput.write('\n');

	// clear the dictionary
	linkedlist<char *>	*keys=nvp.getKeys();
	for (linkedlistnode<char *> *kn=keys->getFirst();
						kn; kn=kn->getNext()) {
		dictionarynode<char *,char *>	*n=nvp.detach(kn->getValue());
		delete[] n->getKey();
		delete[] n->getValue();
		delete n;
	}
	delete keys;
	nvp.clear();
}
