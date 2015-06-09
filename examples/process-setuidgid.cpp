#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// display the user and group
	stdoutput.printf("running as user id %d, group id %d\n",
						process::getUserId(),
						process::getGroupId());

	// set the user and group to "nobody"
	process::setUser("nobody");
	process::setGroup("nobody");

	// display the user and group again
	stdoutput.printf("now running as user id %d, group id %d\n",
						process::getUserId(),
						process::getGroupId());
}
