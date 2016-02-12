// Copyright (c) 2004  David Muse
// See the file COPYING for more information

#include <rudiments/tls.h>
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
		void	listen();
};


void myserver::listen() {


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

	// load the server's certificate chain
	ctx.setCertificateChainFile("server.pem");

	// load the server's private key (which is also stored in client.pem)
	// if the private key requires a password then supply "password"
	ctx.setPrivateKeyFile("server.pem","password");

	// Instruct the server to request the client's certificate.  Servers
	// always send certificates to clients, but in order for a client to
	// send a certificate to a server, the server must request it.
	ctx.setValidatePeer(true);

	// load certificates for the signing authorities that we trust
	ctx.setCertificateAuthorityFile("ca.pem");

	// peer certificates must be directly signed by
	// one of the signing authorities that we trust
	ctx.setValidationDepth(1);

	// Instruct the context to use a dh key for encrypting the session.
	ctx.setKeyExchangeCertificate("dh1024.pem");

	// listen on inet socket port 9000
	if (!inetsocketserver::listen(NULL,9000,15)) {
		stdoutput.printf("couldn't listen on port 9000\n");
		return;
	}

	// loop...
	for (;;) {

		// attach the context to the server
		setSecurityContext(&ctx);

		// accept a client connection
		filedescriptor	*clientsock=accept();

		if (clientsock) {

			// make sure the client sent a certificate
			tlscertificate	*certificate=ctx.getPeerCertificate();
			if (!certificate) {
				stdoutput.printf("peer sent no certificate\n");
				clientsock->close();
				delete clientsock;
				continue;
			}

			// make sure the certificate was valid
			if (!ctx.peerCertificateIsValid()) {
				stdoutput.printf("peer certificate "
						"is valid failed\n%s\n",
						ctx.getErrorString());
				clientsock->close();
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

			// read 5 bytes from the client and display it
			char	buffer[6];
			buffer[5]=(char)NULL;
			clientsock->read((char *)buffer,5);
			stdoutput.printf("%s\n",buffer);

			// write "hello" back to the client
			clientsock->write("hello",5);

		} else {

			// if errno>0 then we got a system error, otherwise we
			// got a tls-specific error
			if (errno) {
				stdoutput.printf("accept failed: %s\n",
						error::getErrorString());
			} else {
				stdoutput.printf("accept failed: %s\n",
						ctx.getErrorString());
			}
		}

		// close the socket and clean up
		delete clientsock;

		//ctx.close();
		//ctx.setCertificateChainFile("server.pem");
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

	mysvr=new myserver();

	// set up signal handlers for clean shutdown
	process::waitForChildren();
	process::handleShutDown(shutDown);
	process::handleCrash(shutDown);

	mysvr->listen();

	file::remove("svr.pid");
	process::exit(1);
}
