// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/tls.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	commandline	cmdl(argc,argv);
	if (cmdl.found("help")) {
		stdoutput.printf("tlsclient [-host host] [-port port] [-cert cert] [-validate (yes|no)] [-depth depth] [-ca ca]\n");
		process::exit(0);
	}
	const char	*host="127.0.0.1";
	if (cmdl.found("host")) {
		host=cmdl.getValue("host");
	}
	uint16_t	port=9000;
	if (cmdl.found("port")) {
		port=charstring::toUnsignedInteger(cmdl.getValue("port"));
	}
	const char	*cert=NULL;
	if (cmdl.found("cert")) {
		cert=cmdl.getValue("cert");
	}
	bool	validate=true;
	if (cmdl.found("validate")) {
		validate=charstring::compare(cmdl.getValue("validate"),"no");
	}
	uint16_t	depth=9;
	if (cmdl.found("depth")) {
		depth=charstring::toUnsignedInteger(cmdl.getValue("depth"));
	}
	const char	*ca=NULL;
	if (cmdl.found("ca")) {
		ca=cmdl.getValue("ca");
	}

	tlscontext	ctx;

	if (!charstring::isNullOrEmpty(cert)) {
		// load the certificate chain
		ctx.setCertificateChainFile(cert);

		// set password for accesing the private key
		ctx.setPrivateKeyPassword("password");
	}

	// set whether or not to validate the peer's certificate
	ctx.setValidatePeer(validate);

	// peer certificates must be directly signed by
	// one of the signing authorities that we trust
	ctx.setValidationDepth(depth);

	if (!charstring::isNullOrEmpty(ca)) {
		// load certificates for the signing authorities that we trust
		ctx.setCertificateAuthority(ca);
	}

	// create an inet socket client
	inetsocketclient	clnt;
	clnt.setSecurityContext(&ctx);

	// connect to a server on localhost, listening on port 9000
	if (clnt.connect(host,port,-1,-1,1,1)<0) {
		if (error::getErrorNumber()) {
			stdoutput.printf("connect failed (1): %s\n",
						error::getErrorString());
		} else {
			stdoutput.printf("connect failed (2): %s\n",
						ctx.getErrorString());
		}
		clnt.close();
		process::exit(1);
	}

	// make sure the server sent a certificate
	tlscertificate	*certificate=ctx.getPeerCertificate();
	if (!certificate) {
		stdoutput.printf("peer sent no certificate\n%s\n",
						ctx.getErrorString());
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
	if (clnt.write("hello",5)<0) {
		if (error::getErrorNumber()) {
			stdoutput.printf("write failed (1): %s\n",
						error::getErrorString());
		} else {
			stdoutput.printf("write failed (2): %s\n",
						ctx.getErrorString());
		}
	}

	// read 10 bytes from the server and display them
	char	buffer[11];
	ssize_t	sizeread=clnt.read(buffer,10);
	if (sizeread<0) {
		if (error::getErrorNumber()) {
			stdoutput.printf("read failed (1): %s\n",
						error::getErrorString());
		} else {
			stdoutput.printf("read failed (2): %s\n",
						ctx.getErrorString());
		}
	} else {
		buffer[sizeread]=(char)NULL;
		stdoutput.printf("%s\n",buffer);
	}

	// close the connection to the server
	clnt.close();
}
