#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/userentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// open the file
	file	f;
	if (f.open("testfile1",O_RDONLY)) {

		// print out various file properties...

		// permissions
		mode_t	perms=f.getPermissions();
		char	*permstring=permissions::evalPermOctal(perms);
		stdoutput.printf("Permissions:		%s\n",permstring);
		delete[] permstring;

		// owner user/group
		uid_t	user=f.getOwnerUserId();
		userentry	ou;
		ou.initialize(user);
		stdoutput.printf("Owner User:		%s\n",ou.getName());

		gid_t	group=f.getOwnerGroupId();
		groupentry	og;
		og.initialize(group);
		stdoutput.printf("Owner Group:		%s\n",og.getName());

		// sizes
		stdoutput.printf("File Size:		%lld\n",
							f.getSize());
		stdoutput.printf("Block Size:		%lld\n",
							f.getBlockSize());
		stdoutput.printf("Block Count:		%lld\n",
							f.getBlockCount());

		// file type
		stdoutput.printf("Is Socket:		%s\n",
					(f.isSocket())?"yes":"no");
		stdoutput.printf("Is Symbolic Link:	%s\n",
					(f.isSymbolicLink())?"yes":"no");
		stdoutput.printf("Is Regular File:	%s\n",
					(f.isRegularFile())?"yes":"no");
		stdoutput.printf("Is Block Device:	%s\n",
					(f.isBlockDevice())?"yes":"no");
		stdoutput.printf("Is Directory:		%s\n",
					(f.isDirectory())?"yes":"no");
		stdoutput.printf("Is Character Device:	%s\n",
					(f.isCharacterDevice())?"yes":"no");
		stdoutput.printf("Is Fifo:		%s\n",
					(f.isFifo())?"yes":"no");

		// access/modification times
		time_t		access=f.getLastAccessTime();
		datetime	da;
		da.initialize(access);
		stdoutput.printf("Last Access:		%s\n",da.getString());

		time_t	mod=f.getLastModificationTime();
		datetime	dm;
		dm.initialize(mod);
		stdoutput.printf("Last Modification:	%s\n",dm.getString());

	} else {
		stdoutput.write("failed to open testfile1\n");
	}
}
