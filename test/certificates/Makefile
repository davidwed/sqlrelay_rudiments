all: database/index.txt database/serial certificates/ca.pem certificatechains/client.pem certificatechains/server.pem

clean:
	rm -f keys/* requests/* certificates/* database/index* database/serial* database/newcerts/* certificatechains/*


database/index.txt:
	touch database/index.txt

database/serial:
	echo '01' > database/serial


keys/ca.key:
	openssl genrsa -out keys/ca.key

certificates/ca.pem: keys/ca.key
	openssl req -config config/ca.cnf -new -x509 -days 3650 -key keys/ca.key -out certificates/ca.pem



keys/client.key:
	openssl genrsa -out keys/client.key

requests/client.csr: keys/client.key
	openssl req -config config/client.cnf -new -key keys/client.key -out requests/client.csr

certificates/client.pem: requests/client.csr
	openssl ca -config config/ca.cnf -in requests/client.csr -out certificates/client.pem

certificatechains/client.pem: certificates/client.pem certificates/ca.pem
	cat certificates/client.pem > certificatechains/client.pem
	cat certificates/ca.pem >> certificatechains/client.pem



keys/server.key:
	openssl genrsa -out keys/server.key

requests/server.csr: keys/server.key
	openssl req -config config/server.cnf -new -key keys/server.key -out requests/server.csr

certificates/server.pem: requests/server.csr
	openssl ca -config config/ca.cnf -in requests/server.csr -out certificates/server.pem

certificatechains/server.pem: certificates/server.pem certificates/ca.pem
	cat certificates/server.pem > certificatechains/server.pem
	cat certificates/ca.pem >> certificatechains/server.pem
