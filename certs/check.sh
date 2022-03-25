openssl pkey -in server.key -pubout -outform pem | sha256sum 
openssl x509 -in server.crt -pubkey -noout -outform pem | sha256sum 
openssl req -in server.csr -pubkey -noout -outform pem | sha256sum