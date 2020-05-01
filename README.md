# qt-dtls-client

This is just a DTLS client that to get used to the QT framework.

To get some certificates
```sh
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes
```

To have a server to talk to, for example:
```sh
openssl s_server -cert cert_server/cert.pem  -key cert_server/key.pem -dtls -accept 4444
```
