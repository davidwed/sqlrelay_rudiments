// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/tls.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	tlsclientcontext	ctx;

	// load the client's certificate chain
	if (!ctx.setCertificateChainFile("client.pem")) {
		stdoutput.printf("set cert chain file failed\n%s\n",
						ctx.getErrorString());
		process::exit(0);
	}

	// load the client's private key (which is also stored in client.pem)
	// if the private key requires a password then supply "password"
	if (!ctx.setPrivateKeyFile("client.pem","password")) {
		stdoutput.printf("set private key file failed\n%s\n",
						ctx.getErrorString());
		process::exit(0);
	}

	// load certificates for the signing authorities that we trust
	if (!ctx.setCertificateAuthorityFile("ca.pem")) {
		stdoutput.printf("set cert authority file failed\n%s\n",
						ctx.getErrorString());
		process::exit(0);
	}

	// peer certificates must be directly signed by
	// one of the signing authorities that we trust
	ctx.setVerifyDepth(1);

	// create an inet socket client
	inetsocketclient	clnt;
	clnt.setSecurityContext(&ctx);

	// connect to a server on localhost, listening on port 9000
	if (clnt.connect("127.0.0.1",9000,-1,-1,1,1)<0) {
		if (errno) {
			stdoutput.printf("connect failed: %s\n",
						error::getErrorString());
		} else {
			stdoutput.printf("connect failed: %s\n",
						ctx.getErrorString());
		}
		clnt.close();
		process::exit(1);
	}

	if (!ctx.peerCertificateIsValid()) {
		stdoutput.printf("peer certificate was invalidd\n");
		clnt.close();
		process::exit(1);
	}

	// make sure the server sent a certificate
	tlscertificate	*certificate=ctx.getPeerCertificate();
	if (!certificate) {
		stdoutput.printf("peer sent no certificate\n");
		clnt.close();
		process::exit(1);
	}
	
	// Make sure the commonname in the certificate is the one we expect it
	// to be (server.localdomain).
	// (we may also want to check the subject name field or certificate
	// extension for the commonname because sometimes it's there instead
	// of in the commonname field)
	const char	*commonname=certificate->getCommonName();
	if (charstring::compareIgnoringCase(commonname,"server.localdomain")) {
		stdoutput.printf("%s!=server.localdomain\n",commonname);
		clnt.close();
		process::exit(1);
	}

	stdoutput.printf("Server certificate {\n");
	stdoutput.printf("  common name: %s\n",commonname);
	stdoutput.printf("}\n\n");

	// write "hello" to the server
	clnt.write("hello",5);

	// read 10 bytes from the server and display them
	char	buffer[11];
	int	sizeread=clnt.read(buffer,10);
	buffer[sizeread]=(char)NULL;
	stdoutput.printf("%s\n",buffer);

	// close the connection to the server
	clnt.close();
}
