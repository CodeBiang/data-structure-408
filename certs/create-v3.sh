#!/bin/bash

openssl req -newkey rsa:2048 -passout pass:123456 -keyout ca.key -out ca.csr -subj "/C=CN/ST=Hebei/L=Shijiazhuang/O=WHST/OU=EDGE/CN=CA/emailAddress=20055@whst.com"
openssl x509 -req -days 3650 -sha256 -extfile openssl.cnf -set_serial 03 -extensions v3_req -in ca.csr -signkey ca.key -out ca.crt

openssl req -newkey rsa:2048 -passout pass:123456 -keyout server.key -out server.csr -subj "/C=CN/ST=Hebei/L=Shijiazhuang/O=WHST/OU=EDGE/CN=SERVER/emailAddress=20055@whst.com"
openssl x509 -req -days 730 -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -passin pass:123456 -extensions v3_req -set_serial 03 -extfile openssl.cnf -out server.crt

openssl req -newkey rsa:2048 -passout pass:123456 -keyout client.key -out client.csr -subj "/C=CN/ST=Hebei/L=Shijiazhuang/O=WHST/OU=EDGE/CN=CLIENT/emailAddress=20055@whst.com"
openssl x509 -req -days 730 -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -passin pass:123456 -extensions v3_req -set_serial 03 -extfile openssl.cnf -out client.crt


