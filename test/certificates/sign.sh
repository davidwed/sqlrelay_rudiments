#!/bin/sh

if ( test -z "$1" )
then
	echo "usage: sign.sh request.csr"
	exit 1
fi

PEM=`basename $1 .csr`.pem

if ( test -r "$PEM" )
then
	openssl ca -config config/ca.cnf -revoke $PEM
	if ( test "$?" != "0" )
	then
		echo "revoke failed"
		exit 1
	fi
fi
export OPENSSL_ENABLE_MD5_VERIFY=1
openssl ca -batch -config config/ca.cnf -in $1 -out $PEM
