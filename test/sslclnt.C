// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <errno.h>

int passwdCallback(char *buf, int size, int rwflag, void *userdata) {
	strncpy(buf,(char *)userdata,size);
	buf[size-1]=(char)NULL;
	return strlen(buf);
}

int main(int argc, const char **argv) {

	// initialize the SSL context
	SSL_library_init();
	SSL_CTX	*ctx=SSL_CTX_new(SSLv2_client_method());
	SSL_CTX_set_mode(ctx,SSL_MODE_ENABLE_PARTIAL_WRITE);
	SSL_CTX_use_certificate_chain_file(ctx,"client.pem");
	SSL_CTX_set_default_passwd_cb(ctx,passwdCallback);
	SSL_CTX_set_default_passwd_cb_userdata(ctx,(void *)"password");
	SSL_CTX_use_PrivateKey_file(ctx,"client.pem",SSL_FILETYPE_PEM);
	SSL_CTX_load_verify_locations(ctx,"root.pem",0);
	#if (OPENSSL_VERSION_NUMBER < 0x0090600fL)
		SSL_CTX_set_verify_depth(ctx,1);
	#endif

	// create an inet socket client
	inetclientsocket	clnt;
	clnt.setSSLContext(ctx);

	// connect to a server on localhost, listening on port 8000
	if (clnt.connectToServer("localhost",8000,-1,-1,1,1)<0) {
		printf("connect failed: %s\n",strerror(errno));
		clnt.close();
		exit(1);
	}

	// make sure the certificate was valid
	if (SSL_get_verify_result(clnt.getSSL())!=X509_V_OK) {
		printf("SSL_get_verify_result failed\n");
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

	// Make sure the commonname in the certificate is the one we expect it
	// to be (localhost).
	// (we may also want to check the subject name field or certificate
	// extension for the commonname because sometimes it's there instead
	// of in the commonname field)
	char	commonname[256];
	X509_NAME_get_text_by_NID(X509_get_subject_name(certificate),
					NID_commonName,commonname,256);
	if (strcasecmp(commonname,"localhost")) {
		printf("%s!=localhost\n",commonname);
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
}
