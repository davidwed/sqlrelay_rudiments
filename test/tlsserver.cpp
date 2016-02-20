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
				const char *ca);
};


void myserver::listen(uint16_t port, const char *cert, const char *ca) {


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

	if (!charstring::isNullOrEmpty(cert)) {
		// load the certificate chain
		ctx.setCertificateChainFile(cert);

		// set password for accesing the private key
		ctx.setPrivateKeyPassword("password");
	}

	if (!charstring::isNullOrEmpty(ca)) {
		// load certificates for the signing authorities that we trust
		ctx.setCertificateAuthority(ca);

		// peer certificates must be directly signed by
		// one of the signing authorities that we trust
		ctx.setValidationDepth(1);
	}

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
/*
			// make sure the client sent a certificate
			tlscertificate	*certificate=ctx.getPeerCertificate();
			if (!certificate) {
				stdoutput.printf("peer sent no "
						"certificate\n%s\n",
						ctx.getErrorString());
				clientsock->close();
				delete clientsock;
				continue;
			}

			// Make sure the commonname in the certificate is the
			// one we expect it to be (client.localdomain).
			// (we may also want to check the subject name field or
			// certificate extension for the commonname because
			// sometimes it's there instead of in the commonname
			// field)
			const char	*commonname=
					certificate->getCommonName();
			if (charstring::compareIgnoringCase(commonname,
							"client.localdomain")) {
				stdoutput.printf("%s!=client.localdomain\n",
								commonname);
				clientsock->close();
				delete clientsock;
				continue;
			}

			stdoutput.printf("Client certificate {\n");
			stdoutput.printf("  common name: %s\n",commonname);
			stdoutput.printf("}\n\n");
*/
			// read 5 bytes from the client and display it
			char	buffer[6];
			buffer[5]=(char)NULL;
			if (clientsock->read((char *)buffer,5)<0) {
				if (error::getErrorNumber()) {
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
			if (clientsock->write("hello",5)<0) {
				if (error::getErrorNumber()) {
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
		stdoutput.printf("tlsserver [-port port] [-cert cert] [-ca ca]\n");
		process::exit(0);
	}

	uint16_t	port=9000;
	if (cmdl.found("port")) {
		port=charstring::toUnsignedInteger(cmdl.getValue("port"));
	}
	const char	*cert="server.pem";
	if (cmdl.found("cert")) {
		cert=cmdl.getValue("cert");
	}
	const char	*ca="server.pem";
	if (cmdl.found("ca")) {
		ca=cmdl.getValue("ca");
	}

	mysvr=new myserver();

	// set up signal handlers for clean shutdown
	process::waitForChildren();
	process::handleShutDown(shutDown);
	process::handleCrash(shutDown);

	mysvr->listen(port,cert,ca);

	file::remove("svr.pid");
	process::exit(1);
}
