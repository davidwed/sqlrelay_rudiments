include ../../config.mk

all: certificates/ca.pfx certificatechains/client.pfx certificatechains/server.pfx certificatechains/db.pfx

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
	openssl genrsa -out keys/ca.key; \
	if ( test "$$?" = "1" ); \
	then \
		openssl genrsa -rand /etc/passwd -out keys/ca.key; \
	fi

certificates/ca.pem: keys/ca.key
	openssl req -config config/ca.cnf -new -x509 -days 3650 -key keys/ca.key -out certificates/ca.pem

certificates/ca.pfx: certificates/ca.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/ca.pem -inkey keys/ca.key -out certificates/ca.pfx

keys/client.key:
	openssl genrsa -out keys/client.key || \
	if ( test "$$?" = "1" ); \
	then \
		openssl genrsa -rand /etc/passwd -out keys/client.key; \
	fi

requests/client.csr: keys/client.key
	openssl req -config config/client.cnf -new -key keys/client.key -out requests/client.csr

certificates/client.pem: requests/client.csr certificates/ca.pem
	openssl ca -batch -config config/ca.cnf -in requests/client.csr -out certificates/client.pem -extfile config/client.ca.cnf; \
	if ( test "$$?" = "1" ); \
	then \
		openssl ca -batch -config config/ca.cnf -in requests/client.csr -out certificates/client.pem; \
	fi

certificatechains/client.pem: certificates/client.pem
	cat certificates/client.pem > certificatechains/client.pem
	cat keys/client.key >> certificatechains/client.pem
	cat certificates/ca.pem >> certificatechains/client.pem

certificatechains/client.pfx: certificatechains/client.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/client.pem -inkey keys/client.key -certfile certificates/ca.pem -out certificatechains/client.pfx

keys/server.key:
	openssl genrsa -out keys/server.key || \
	if ( test "$$?" = "1" ); \
	then \
		openssl genrsa -rand /etc/passwd -out keys/server.key; \
	fi

requests/server.csr: keys/server.key certificates/ca.pem
	openssl req -config config/server.cnf -new -key keys/server.key -out requests/server.csr

certificates/server.pem: requests/server.csr
	openssl ca -batch -config config/ca.cnf -in requests/server.csr -out certificates/server.pem -extfile config/server.ca.cnf; \
	if ( test "$$?" = "1" ); \
	then \
		openssl ca -batch -config config/ca.cnf -in requests/server.csr -out certificates/server.pem; \
	fi

certificatechains/server.pem: certificates/server.pem
	cat certificates/server.pem > certificatechains/server.pem
	cat keys/server.key >> certificatechains/server.pem
	cat certificates/ca.pem >> certificatechains/server.pem

certificatechains/server.pfx: certificatechains/server.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/server.pem -inkey keys/server.key -certfile certificates/ca.pem -out certificatechains/server.pfx

keys/db.key:
	openssl genrsa -out keys/db.key || \
	if ( test "$$?" = "1" ); \
	then \
		openssl genrsa -rand /etc/passwd -out keys/db.key; \
	fi

requests/db.csr: keys/db.key certificates/ca.pem
	openssl req -config config/db.cnf -new -key keys/db.key -out requests/db.csr

certificates/db.pem: requests/db.csr
	openssl ca -batch -config config/ca.cnf -in requests/db.csr -out certificates/db.pem -extfile config/db.ca.cnf; \
	if ( test "$$?" = "1" ); \
	then \
		openssl ca -batch -config config/ca.cnf -in requests/db.csr -out certificates/db.pem; \
	fi

certificatechains/db.pem: certificates/db.pem
	cat certificates/db.pem > certificatechains/db.pem
	cat keys/db.key >> certificatechains/db.pem
	cat certificates/ca.pem >> certificatechains/db.pem

certificatechains/db.pfx: certificatechains/db.pem
	PASSWORD="" openssl pkcs12 -export -passout env:PASSWORD -in certificates/db.pem -inkey keys/db.key -certfile certificates/ca.pem -out certificatechains/db.pfx
