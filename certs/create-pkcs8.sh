#!/bin/bash

openssl pkcs8 -topk8 -inform PEM -in client.key -passin pass:123456 -outform PEM -nocrypt >> client.pkcs8.key
openssl pkcs8 -topk8 -inform PEM -in server.key -passin pass:123456 -outform PEM -nocrypt >> server.pkcs8.key