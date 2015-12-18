#include <rudiments/inetsocketserver.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include <rudiments/character.h>
#include <rudiments/commandline.h>
#include <rudiments/environment.h>
#include <rudiments/gss.h>

#include "gssmisc.h"

int main(int argc, const char **argv) {

	// process command line
	commandline	cmdl(argc,argv);
	const char	*keytab=cmdl.getValue("keytab");
	bool		verbose=cmdl.found("verbose");

	// configure keytab
	if (!charstring::isNullOrEmpty(keytab) &&
			!environment::setValue("KRB5_KTNAME",keytab)) {
		stdoutput.printf("failed to configure keytab\n");
		process::exit(1);
	}

	// print available mechs
	if (verbose) {
		displayAvailableMechanisms();
	}

	// get the credentials for this service from the keytab
	gsscredentials	gcred;
	if (!gcred.acquireService("gssserver")) {
		stdoutput.printf("acquireService():\n");
		stdoutput.printf("%s\n",gcred.getStatus());
		process::exit(1);
	}

	// print information about the credentials
	if (verbose) {
		displayCredentials(&gcred);
	}

	// configure security context
	gsscontext	gctx;
	gctx.setCredentials(&gcred);

	// create socket
	inetsocketserver	iss;
	iss.setGSSContext(&gctx);
	if (iss.listen(NULL,4444,0)) {
		for (;;) {

			// accept (will also accept context)
			filedescriptor	*fd=iss.accept();
			if (!fd) {
				stdoutput.printf("accept():\n");
				stdoutput.printf("%s\n",gctx.getStatus());
				continue;
			}
			fd->setWriteBufferSize(8192);
			fd->setReadBufferSize(8192);

			// print information about the context
			if (verbose) {
				displayContext(&gctx);
			}

			stdoutput.printf("clientSession {\n");

			// read messages from client...
			for (;;) {

				uint64_t	msgsize;
				ssize_t	size=fd->read(&msgsize);
				if (size==0) {
					if (verbose) {
						stdoutput.printf(
						"\n  Client disconnected\n");
					}
					break;
				}
				if (size!=sizeof(msgsize)) {
					stdoutput.printf(
						"\n  read() size failed\n");
					break;
				}

				unsigned char	*msg=new unsigned char[msgsize];
				if (fd->read(msg,msgsize)!=(ssize_t)msgsize) {
					stdoutput.printf(
						"\n  read() message failed\n");
					break;
				}

				displayData("Message",msg,msgsize);
			}

			stdoutput.printf("}\n");

			// close and delete the client socket
			delete fd;
		}
	} else {
		stdoutput.printf("error listening on socket\n");
	}

	process::exit(0);
}
