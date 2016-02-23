include ../../config.mk

all: certificates/ca.pfx certificatechains/client.pfx certificatechains/server.pfx

clean:
	cd keys $(AND) $(RM) *.key
	cd requests $(AND) $(RM) *.csr
	cd certificates $(AND) $(RM) *.pem
	cd certificates $(AND) $(RM) *.pfx
	cd database $(AND) $(RM) index* serial*
	cd database $(AND) cd newcerts $(AND) $(RM) *.pem
	cd certificatechains $(AND) $(RM) *.pem
	cd certificatechains $(AND) $(RM) *.pfx

database/index.txt:
	touch database/index.txt

database/serial: database/index.txt
	echo '01' > database/serial

keys/ca.key: database/serial
	openssl genrsa -out keys/ca.key

certificates/ca.pem: keys/ca.key
	openssl req -config config/ca.cnf -new -x509 -days 3650 -key keys/ca.key -out certificates/ca.pem

certificates/ca.pfx: certificates/ca.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/ca.pem -inkey keys/ca.key -out certificates/ca.pfx

#certificates/ca.der: certificates/ca.pem
	#openssl x509 -in certificates/ca.pem -outform der -out certificates/ca.der

keys/client.key:
	openssl genrsa -out keys/client.key

requests/client.csr: keys/client.key
	openssl req -config config/client.cnf -new -key keys/client.key -out requests/client.csr

certificates/client.pem: requests/client.csr certificates/ca.pem
	openssl ca -batch -config config/ca.cnf -in requests/client.csr -out certificates/client.pem

#certificates/client.der: certificates/client.pem
	#openssl x509 -in certificates/client.pem -outform der -out certificates/client.der

certificatechains/client.pem: certificates/client.pem
	cat certificates/client.pem > certificatechains/client.pem
	cat keys/client.key >> certificatechains/client.pem
	cat certificates/ca.pem >> certificatechains/client.pem

certificatechains/client.pfx: certificatechains/client.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/client.pem -inkey keys/client.key -certfile certificates/ca.pem -out certificatechains/client.pfx

keys/server.key:
	openssl genrsa -out keys/server.key

requests/server.csr: keys/server.key certificates/ca.pem
	openssl req -config config/server.cnf -new -key keys/server.key -out requests/server.csr

certificates/server.pem: requests/server.csr
	openssl ca -batch -config config/ca.cnf -in requests/server.csr -out certificates/server.pem

#certificates/server.der: certificates/server.pem
	#openssl x509 -in certificates/server.pem -outform der -out certificates/server.der

certificatechains/server.pem: certificates/server.pem
	cat certificates/server.pem > certificatechains/server.pem
	cat keys/server.key >> certificatechains/server.pem
	cat certificates/ca.pem >> certificatechains/server.pem

certificatechains/server.pfx: certificatechains/server.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/server.pem -inkey keys/server.key -certfile certificates/ca.pem -out certificatechains/server.pfx
