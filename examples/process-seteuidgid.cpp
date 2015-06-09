#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// display the effective user and group id
	stdoutput.printf("running as effetive user id %d, group id %d\n",
					process::getEffectiveUserId(),
					process::getEffectiveGroupId());

	// set the effective user and group to "nobody"
	process::setEffectiveUser("nobody");
	process::setEffectiveGroup("nobody");

	// display the effective user and group id again
	stdoutput.printf("now running as effetive user id %d, group id %d\n",
					process::getEffectiveUserId(),
					process::getEffectiveGroupId());
}
