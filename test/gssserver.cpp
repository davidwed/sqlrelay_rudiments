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
	if (charstring::isNullOrEmpty(keytab)) {
		#ifndef WIN32
		if (process::getUserId()) {
			stdoutput.printf("Warning! Since you're using the "
					"system keytab,\n         "
					"this program should probably be "
					"run as root.\n\n");
		}
		#endif
	}
	bool		verbose=cmdl.found("verbose");
	uint16_t	port=4444;
	if (cmdl.found("port")) {
		port=charstring::toUnsignedInteger(cmdl.getValue("port"));
	}
	const char	*service="gssserver";
	if (cmdl.found("service")) {
		service=cmdl.getValue("service");
	}

	// print available mechs
	if (verbose) {
		displayAvailableMechanisms();
	}

	gsscredentials	gcred;

	// set which keytab to use
	if (!charstring::isNullOrEmpty(keytab) && !gcred.setKeytab(keytab)) {
		stdoutput.printf("failed to configure keytab\n");
		process::exit(1);
	}

	// acquire the credentials for this service
	if (!gcred.acquireForService(service)) {
		stdoutput.printf("acquireForService():\n");
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
	iss.setSecurityContext(&gctx);
	if (iss.listen(NULL,port,0)) {
		for (;;) {

			// accept (will also accept context)
			filedescriptor	*fd=iss.accept();
			if (!fd) {
				stdoutput.printf("accept():\n");
				stdoutput.printf("%s\n",gctx.getStatus());
				continue;
			}
			fd->setWriteBufferSize(65536);
			fd->setReadBufferSize(65536);

			// print information about the context
			if (verbose) {
				displayContext(&gctx);
			}

			stdoutput.printf("clientSession {\n");

			// read messages from the client...
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
						"\n  read() msg failed\n");
					delete[] msg;
					break;
				}

				displayData("Received message...",msg,msgsize);

				stdoutput.printf("\n  Sending response...");

				if (fd->write(msgsize)!=sizeof(msgsize)) {
					stdoutput.printf(
						"\n  write() size failed\n");
					delete[] msg;
					break;
				}

				if (fd->write(msg,msgsize)!=(ssize_t)msgsize) {
					stdoutput.printf(
						"\n  write() msg failed\n");
					delete[] msg;
					break;
				}
				if (!fd->flushWriteBuffer(-1,-1)) {
					stdoutput.printf(
						"\n  flushWriteBuffer() msg "
						"failed\n");
					delete[] msg;
					break;
				}

				delete[] msg;

				stdoutput.printf(" success\n");
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
