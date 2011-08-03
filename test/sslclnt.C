// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAS_SSL
#include <openssl/err.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

#ifdef RUDIMENTS_HAS_SSL
int passwdCallback(char *buf, int size, int rwflag, void *userdata) {
	charstring::copy(buf,(char *)userdata,size);
	buf[size-1]=(char)NULL;
	return charstring::length(buf);
}
#endif

int main(int argc, const char **argv) {

#ifdef RUDIMENTS_HAS_SSL
	// initialize the SSL context
	SSL_library_init();
	SSL_load_error_strings();

	// Create a new client context usng SSLv2.
	SSL_CTX	*ctx=SSL_CTX_new(SSLv2_client_method());

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
	SSL_CTX_use_PrivateKey_file(ctx,"client.pem",SSL_FILETYPE_PEM);

	// load certificates for the signing authorities that we trust
	SSL_CTX_load_verify_locations(ctx,"ca.pem",0);
	#if (OPENSSL_VERSION_NUMBER < 0x0090600fL)
		// server certificates must be directly signed
		// by one of the signing authroities that we trust
		SSL_CTX_set_verify_depth(ctx,1);
	#endif

	// create an inet socket client
	inetclientsocket	clnt;
	clnt.setSSLContext(ctx);

	// connect to a server on localhost, listening on port 8000
	if (clnt.connect("127.0.0.1",8000,-1,-1,1,1)<0) {
		if (errno) {
			printf("connect failed: %s\n",error::getErrorString());
		} else {
			printf("connect failed: ");
			ERR_print_errors_fp(stdout);
			printf("\n");
		}
		clnt.close();
		exit(1);
	}

	// make sure the server sent a certificate
	X509	*certificate=SSL_get_peer_certificate(clnt.getSSL());
	if (!certificate) {
		printf("peer sent no certificate\n");
		clnt.close();
		exit(1);
	}

	// make sure the certificate was valid
	long	result=SSL_get_verify_result(clnt.getSSL());
	if (result!=X509_V_OK) {
		printf("SSL_get_verify_result failed: %ld\n",result);
		clnt.close();
		exit(1);
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
		printf("%s!=server.localdomain\n",commonname);
		clnt.close();
		exit(1);
	}

	// write "hello" to the server
	clnt.write("hello",5);

	// read 10 bytes from the server and display them
	char	buffer[11];
	int	sizeread=clnt.read(buffer,10);
	buffer[sizeread]=(char)NULL;
	printf("%s\n",buffer);

	// close the connection to the server
	clnt.close();
#else
	printf("rudiments built without ssl support\n");
#endif
}
