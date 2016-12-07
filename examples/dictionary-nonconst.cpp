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
	dictionarynode<char *,char *>	*n=nvp.getNode((char *)"color");
	delete[] n->getKey();
	delete[] n->getValue();
	nvp.remove(n);

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.print();
	stdoutput.write('\n');

	// clear the dictionary
	linkedlist< dictionarynode< char *, char * > *> *list=nvp.getList();
	for (linkedlistnode< dictionarynode< char *, char *> *> *ln=
						list->getFirst();
						ln; ln=ln->getNext()) {
		delete[] ln->getValue()->getKey();
		delete[] ln->getValue()->getValue();
	}
	nvp.clear();
}
