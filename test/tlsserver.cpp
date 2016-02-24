// Copyright (c) 2004  David Muse
// See the file COPYING for more information

#include <rudiments/tls.h>
#include <rudiments/commandline.h>
#include <rudiments/permissions.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

class myserver : public inetsocketserver {
	public:
			myserver() : inetsocketserver() {}
		void	listen(uint16_t port,
				const char *cert,
				const char *ciphers,
				bool validate,
				uint16_t depth,
				const char *ca,
				const char *commonname);
};


void myserver::listen(uint16_t port, const char *cert, const char *ciphers,
				bool validate, uint16_t depth, const char *ca,
				const char *commonname) {


	// make sure that only one instance is running
	int	pid=process::checkForPidFile("svr.pid");
	if (pid>-1) {
		stdoutput.printf("Sorry, an instance of this server is already running with process id: %d\n",pid);
		return;
	}


	// detach from the controlling terminal
	//detach();


	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	process::createPidFile("svr.pid",permissions::ownerReadWrite());

	tlscontext	ctx;
	ctx.setCertificateChainFile(cert);
	ctx.setPrivateKeyPassword("password");
	ctx.setCiphers(ciphers);
	ctx.setValidatePeer(validate);
	ctx.setValidationDepth(depth);
	ctx.setCertificateAuthority(ca);

	// listen on inet socket
	if (!inetsocketserver::listen(NULL,port,15)) {
		stdoutput.printf("couldn't listen on port %d\n",port);
		return;
	}

	// loop...
	for (;;) {

		// attach the context to the server
		setSecurityContext(&ctx);

		// accept a client connection
		filedescriptor	*clientsock=accept();

		if (clientsock) {

			if (validate) {

				// make sure the client sent a certificate
				tlscertificate	*certificate=
						ctx.getPeerCertificate();
				if (!certificate) {
					stdoutput.printf("peer sent no "
							"certificate\n%s\n",
							ctx.getErrorString());
					clientsock->close();
					delete clientsock;
					continue;
				}

				// Make sure the commonname in the certificate
				// is the one we expect it to be
				const char	*cn=
						certificate->getCommonName();
				if (charstring::compareIgnoringCase(
							cn,commonname)) {
					stdoutput.printf("%s!=%s\n",
							cn,commonname);
					clientsock->close();
					delete clientsock;
					continue;
				}

				stdoutput.printf("client certificate {\n");
				stdoutput.printf("  common name: %s\n",cn);
				stdoutput.printf("}\n\n");
			}

			// read 5 bytes from the client and display it
			char	buffer[6];
			buffer[5]=(char)NULL;
			ssize_t	sizeread=clientsock->read((char *)buffer,5);
			if (sizeread<=0) {
				if (sizeread==0) {
					stdoutput.printf(
						"read failed (0): eof\n");
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"read failed (1): %s\n",
						error::getErrorString());
				} else {
					stdoutput.printf(
						"read failed (2): %s\n",
						ctx.getErrorString());
				}
			} else {
				stdoutput.printf("%s\n",buffer);
			}

			// write "hello" back to the client
			ssize_t	sizewritten=clientsock->write("hello",5);
			if (sizewritten<=0) {
				if (sizeread==0) {
					stdoutput.printf(
						"write failed (0): eof\n");
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"write failed (1): %s\n",
						error::getErrorString());
				} else {
					stdoutput.printf(
						"write failed (2): %s\n",
						ctx.getErrorString());
				}
			}

		} else {
			if (error::getErrorNumber()) {
				stdoutput.printf("accept failed (1): %s\n",
						error::getErrorString());
			} else {
				stdoutput.printf("accept failed (2): %s\n",
						ctx.getErrorString());
			}
		}

		// close the socket and clean up
		delete clientsock;

		ctx.close();
	}
}


myserver	*mysvr;

// define a function to shut down the process cleanly
void shutDown(int32_t sig) {
	stdoutput.printf("shutting down\n");
	mysvr->close();
	delete mysvr;
	file::remove("svr.pid");
	process::exit(0);
}

int main(int argc, const char **argv) {

	commandline	cmdl(argc,argv);

	if (cmdl.found("help")) {
		stdoutput.printf("tlsserver [-port port] [-cert cert] [-ciphers ciphers] [-validate (yes|no)] [-depth depth] [-ca ca] [-commonname name]\n");
		process::exit(0);
	}

	uint16_t	port=9000;
	if (cmdl.found("port")) {
		port=charstring::toUnsignedInteger(cmdl.getValue("port"));
	}
	const char	*cert=NULL;
	if (cmdl.found("cert")) {
		cert=cmdl.getValue("cert");
	}
	const char	*ciphers=NULL;
	if (cmdl.found("ciphers")) {
		ciphers=cmdl.getValue("ciphers");
	}
	bool	validate=false;
	if (cmdl.found("validate")) {
		validate=!charstring::compare(cmdl.getValue("validate"),"yes");
	}
	uint16_t	depth=9;
	if (cmdl.found("depth")) {
		depth=charstring::toUnsignedInteger(cmdl.getValue("depth"));
	}
	const char	*ca=NULL;
	if (cmdl.found("ca")) {
		ca=cmdl.getValue("ca");
	}
	const char	*commonname="client.localdomain";
	if (cmdl.found("commonname")) {
		commonname=cmdl.getValue("commonname");
	}

	mysvr=new myserver();

	// set up signal handlers for clean shutdown
	process::waitForChildren();
	process::handleShutDown(shutDown);
	process::handleCrash(shutDown);

	mysvr->listen(port,cert,ciphers,validate,depth,ca,commonname);

	file::remove("svr.pid");
	process::exit(1);
}
