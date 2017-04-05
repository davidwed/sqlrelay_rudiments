#!/bin/sh

if ( test -z "$1" )
then
	echo "usage: sign.sh request.csr"
	exit 1
fi

export OPENSSL_ENABLE_MD5_VERIFY=1
openssl ca -batch -config config/ca.cnf -in $1 -out `basename $1 .csr`.pem
