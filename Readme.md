# live-server CLI

Live is a command line tool used to start a minimalist web server supporting live reload

Live always handle requests using HTTP/1.1 no matter the version of the incoming http request and so do not support the following features
 - Persistant TCP connections
 - TLS handshake
 - Accurated reponse codes 
 - etc

These features might be implemented later.

## How to install
You can either download prebuilt release for your platform from the release page, add to your path and use live at cli tool

```bash
sudo apt install build-essentials
git clone https://github.com/backslash057/live-server.git
cd live-server
make
./bin/live
```

Hope it's helpful
