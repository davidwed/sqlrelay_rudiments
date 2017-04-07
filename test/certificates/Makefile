include ../../config.mk

all: certificatechains/ca.pfx certificatechains/client.pfx certificatechains/server.pfx certificatechains/db.pfx

clean:
	cd keys $(AND) $(RM) *.key
	cd requests $(AND) $(RM) *.csr
	cd certificates $(AND) $(RM) *.crt *.pfx
	cd database $(AND) $(RM) index* serial*
	cd database $(AND) cd newcerts $(AND) $(RM) *.pem
	cd certificatechains $(AND) $(RM) *.pem
	cd certificatechains $(AND) $(RM) *.pfx

database/index.txt:
	touch database/index.txt

database/serial: database/index.txt
	echo '01' > database/serial

keys/ca.key: database/serial
	openssl genrsa -out keys/ca.key; \
	if ( test "$$?" = "1" ); \
	then \
		openssl genrsa -rand /etc/passwd -out keys/ca.key; \
	fi

certificates/ca.crt: keys/ca.key
	openssl req -config config/ca.cnf -new -x509 -days 3650 -key keys/ca.key -out certificates/ca.crt

certificatechains/ca.pem: certificates/ca.crt
	cat certificates/ca.crt > certificatechains/ca.pem

certificatechains/ca.pfx: certificatechains/ca.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/ca.crt -inkey keys/ca.key -out certificatechains/ca.pfx

keys/client.key:
	openssl genrsa -out keys/client.key || \
	if ( test "$$?" = "1" ); \
	then \
		openssl genrsa -rand /etc/passwd -out keys/client.key; \
	fi

requests/client.csr: keys/client.key
	openssl req -config config/client.cnf -new -key keys/client.key -out requests/client.csr

certificates/client.crt: requests/client.csr certificates/ca.crt
	openssl ca -batch -config config/ca.cnf -in requests/client.csr -out certificates/client.crt -extfile config/client.ca.cnf; \
	if ( test "$$?" = "1" ); \
	then \
		openssl ca -batch -config config/ca.cnf -in requests/client.csr -out certificates/client.crt; \
	fi

certificatechains/client.pem: certificates/client.crt
	cat certificates/client.crt > certificatechains/client.pem
	cat keys/client.key >> certificatechains/client.pem
	cat certificates/ca.crt >> certificatechains/client.pem

certificatechains/client.pfx: certificatechains/client.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/client.crt -inkey keys/client.key -certfile certificates/ca.crt -out certificatechains/client.pfx

keys/server.key:
	openssl genrsa -out keys/server.key || \
	if ( test "$$?" = "1" ); \
	then \
		openssl genrsa -rand /etc/passwd -out keys/server.key; \
	fi

requests/server.csr: keys/server.key certificates/ca.crt
	openssl req -config config/server.cnf -new -key keys/server.key -out requests/server.csr

certificates/server.crt: requests/server.csr
	openssl ca -batch -config config/ca.cnf -in requests/server.csr -out certificates/server.crt -extfile config/server.ca.cnf; \
	if ( test "$$?" = "1" ); \
	then \
		openssl ca -batch -config config/ca.cnf -in requests/server.csr -out certificates/server.crt; \
	fi

certificatechains/server.pem: certificates/server.crt
	cat certificates/server.crt > certificatechains/server.pem
	cat keys/server.key >> certificatechains/server.pem
	cat certificates/ca.crt >> certificatechains/server.pem

certificatechains/server.pfx: certificatechains/server.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/server.crt -inkey keys/server.key -certfile certificates/ca.crt -out certificatechains/server.pfx

keys/db.key:
	openssl genrsa -out keys/db.key || \
	if ( test "$$?" = "1" ); \
	then \
		openssl genrsa -rand /etc/passwd -out keys/db.key; \
	fi

requests/db.csr: keys/db.key certificates/ca.crt
	openssl req -config config/db.cnf -new -key keys/db.key -out requests/db.csr

certificates/db.crt: requests/db.csr
	openssl ca -batch -config config/ca.cnf -in requests/db.csr -out certificates/db.crt -extfile config/db.ca.cnf; \
	if ( test "$$?" = "1" ); \
	then \
		openssl ca -batch -config config/ca.cnf -in requests/db.csr -out certificates/db.crt; \
	fi

certificatechains/db.pem: certificates/db.crt
	cat certificates/db.crt > certificatechains/db.pem
	cat keys/db.key >> certificatechains/db.pem
	cat certificates/ca.crt >> certificatechains/db.pem

certificatechains/db.pfx: certificatechains/db.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/db.crt -inkey keys/db.key -certfile certificates/ca.crt -out certificatechains/db.pfx
