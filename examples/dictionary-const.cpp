#include <rudiments/dictionary.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a dictionary of name-value pairs
	dictionary<const char *,const char *>	nvp;

	// set some name-value pairs
	nvp.setValue("color","blue");
	nvp.setValue("number","one");
	nvp.setValue("automobile","car");
	nvp.setValue("dwelling","house");
	nvp.setValue("parent","dad");

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.print();
	stdoutput.write('\n');

	// replace some values
	nvp.setValue("number","two");
	nvp.setValue("dwelling","apartment");
	nvp.setValue("parent","mom");

	// remove a value
	nvp.remove("color");

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.print();
	stdoutput.write('\n');

	// clear the dictionary
	nvp.clear();

	// set some new values
	nvp.setValue("road","highway");
	nvp.setValue("furniture","chair");
	nvp.setValue("tree","elm");

	// print out the dictionary a different way
	stdoutput.write("Current contents:\n");
	linkedlist<const char *>	*keys=nvp.getKeys();
	for (linkedlistnode<const char *> *kn=keys->getFirst();
						kn; kn=kn->getNext()) {
		stdoutput.printf("%s=%s ",
			kn->getValue(),nvp.getValue(kn->getValue()));
	}
	delete keys;
	stdoutput.write('\n');
}
