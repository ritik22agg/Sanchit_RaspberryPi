#!/bin/bash
cd ./ODELU/libbswabe-0.9/libbswabe-0.9/
chmod +x configure
chmod +x mkinstalldirs
chmod +x install-sh
./configure
make
sudo make install
cd ..
cd ..
cd cpabe-0.11
cd cpabe-0.11
chmod +x configure
chmod +x mkinstalldirs
chmod +x install-sh
./configure
make LDFLAGS="-lgmp -lpbc -lcrypto -L/usr/lib/x86_64-linux-gnu -lglib-2.0 -lbswabe -lgmp -fPIC"
sudo make install

