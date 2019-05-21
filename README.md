# Building prerequisites
* GNU make

# Building from sources
To build the executable from the sources, run from the root project directory or the *src/* directory:
```
make
```
This will produce the **ompchat** executable in the */src* directory

# Installing
To build and install the executable from the sources, run from the root project directory or the *src/* directory:
```
make install
```

# Building and installing for rpm
Run from the root project directory:
```
make rpm
```

# Usage
**ompchat** [-h] [-s] [-i] ip [-p] port

Argument|Description|
--------|--------|
-h, --help  | show the help message |
-s, --server| start as a server |
-i, --ip    | (as a client) set the ip of the server |
-p, --port  | (as a server) choose the server port<br>(as a client) specify the port on which to connect |