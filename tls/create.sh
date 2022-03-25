#!/bin/bash

# 生成根证书密钥
openssl genrsa -aes256 -passout pass:123456 -out ca.key 4096
# 生成根证书请求文件
openssl req -new -key -passin pass:123456 ca.key -out ca.csr -subj "/C=CN/ST=Hebei/L=Shijiazhuang/O=WHST/OU=EDGE/CN=CLIENT/emailAddress=20055@whst.com"
# 自签根证书
openssl x509 -req -passin pass:123456 -days 3650 -sha256 -extfile openssl.cnf -extensions v3_ca -signkey ca.key -in ca.csr -out ca.crt

# 服务端证书
openssl genrsa -aes256 -passout pass:123456 -out server.key 4096
openssl req -new -key -passin pass:123456 server.key -out server.csr -subj "/C=CN/ST=Hebei/L=Shijiazhuang/O=WHST/OU=EDGE/CN=CLIENT/emailAddress=20055@whst.com"
openssl x509 -req -passin pass:123456 -days 3650 -sha256 -extfile openssl.cnf -extensions v3_req -CA ./ca.crt -CAkey ./ca.key -CAserial ca.srl -CAcreateserial -in server.csr -out server.crt
# openssl pkcs8 -topk8 -passin pass:123456 -inform PEM -in server.key -outform PEM -nocrypt > server.pkcs8.key
openssl rsa -in server.key -out server.key.unsecure

# 客户端
openssl genrsa -aes256 -passout pass:123456 -out client.key 4096
openssl req -new -key -passin pass:123456 client.key -out client.csr -subj "/C=CN/ST=Hebei/L=Shijiazhuang/O=WHST/OU=EDGE/CN=CLIENT/emailAddress=20055@whst.com"
openssl x509 -req -passin pass:123456 -days 3650 -sha256 -extfile openssl.cnf -extensions v3_req -CA ./ca.crt -CAkey ./ca.key -CAserial ca.srl -CAcreateserial -in client.csr - -out client.crt
# openssl pkcs8 -topk8 -passin pass:123456 -inform PEM -in client.key -outform PEM -nocrypt > client.pkcs8.key
openssl rsa -in client.key -out client.key.unsecure

