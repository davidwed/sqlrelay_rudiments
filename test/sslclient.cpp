// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAS_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int passwdCallback(char *buf, int size, int rwflag, void *userdata) {
	charstring::copy(buf,(char *)userdata,size);
	buf[size-1]=(char)NULL;
	return charstring::length(buf);
}

int main(int argc, const char **argv) {

	// initialize the SSL context
	SSL_library_init();
	SSL_load_error_strings();

	// Create a new client context usng SSLv2/3
	SSL_CTX	*ctx=SSL_CTX_new(SSLv23_client_method());

	// in cases where a re-negotiation must take place during an SSL_read
	// or SSL_write, automatically re-negotiate, then retry the read/write
	// instead of causing the read/write to fail
	SSL_CTX_set_mode(ctx,SSL_MODE_AUTO_RETRY);

	// load the client's certificate chain
	SSL_CTX_use_certificate_chain_file(ctx,"client.pem");

	// if the private key requires a password in order to read it,
	// supply "password"
	SSL_CTX_set_default_passwd_cb(ctx,passwdCallback);
	SSL_CTX_set_default_passwd_cb_userdata(ctx,(void *)"password");

	// load the client's private key
	// (which is also stored in client.pem)
	SSL_CTX_use_PrivateKey_file(ctx,"client.pem",SSL_FILETYPE_PEM);
	if (!SSL_CTX_check_private_key(ctx)) {
		stdoutput.printf("private key check failed\n");
		ERR_print_errors_fp(stdout);
		process::exit(0);
	}

	// load certificates for the signing authorities that we trust
	SSL_CTX_load_verify_locations(ctx,"ca.pem",0);
	#if (OPENSSL_VERSION_NUMBER < 0x0090600fL)
		// server certificates must be directly signed
		// by one of the signing authroities that we trust
		SSL_CTX_set_verify_depth(ctx,1);
	#endif

	// create an inet socket client
	inetsocketclient	clnt;
	clnt.setSSLContext((void *)ctx);

	// connect to a server on localhost, listening on port 8000
	if (clnt.connect("127.0.0.1",8000,-1,-1,1,1)<0) {
		if (errno) {
			stdoutput.printf("connect failed: %s\n",
						error::getErrorString());
		} else {
			stdoutput.printf("connect failed: ");
			ERR_print_errors_fp(stdout);
			stdoutput.printf("\n");
		}
		clnt.close();
		process::exit(1);
	}

	SSL	*ssl=(SSL *)clnt.getSSL();

	// make sure the server sent a certificate
	X509	*certificate=SSL_get_peer_certificate(ssl);
	if (!certificate) {
		stdoutput.printf("peer sent no certificate\n");
		clnt.close();
		process::exit(1);
	}
/*
	// make sure the certificate was valid
	long	result=SSL_get_verify_result(ssl);
	if (result!=X509_V_OK) {
		stdoutput.printf("SSL_get_verify_result failed: %ld\n",result);
		clnt.close();
		process::exit(1);
	}

	// Make sure the commonname in the certificate is the one we expect it
	// to be (server.localdomain).
	// (we may also want to check the subject name field or certificate
	// extension for the commonname because sometimes it's there instead
	// of in the commonname field)
	char	commonname[256];
	X509_NAME_get_text_by_NID(X509_get_subject_name(certificate),
					NID_commonName,commonname,256);
	if (charstring::compareIgnoringCase(commonname,"server.localdomain")) {
		stdoutput.printf("%s!=server.localdomain\n",commonname);
		clnt.close();
		process::exit(1);
	}
*/
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

#else

int main(int argc, const char **argv) {
	stdoutput.printf("rudiments built without ssl support\n");
}

#endif