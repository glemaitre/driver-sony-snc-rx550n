# Driver for Sony SNC-RX550N

This repository offers a C++ driver for Sony SNC-RX550N camera.

## System requirements

* Ubuntu 14.04 LTS

* Qt SDK 4.8

### Qt SDK 4.8

The SDK can be installed through the Ubuntu repository

`sudo apt-get install qt-sdk`

## Sources delivered in this repository

This repository offers the following classes:

* Sony SNC-RX550N: this class allows to manage the Sony SNC-RX550N camera.

## Compilation

* Create a bin directory

`mkdir bin`

* Move to the created directory

`cd bin`
    
* Compile the project using qmake

`qmake-qt4 ../`

* Compile the `MakeFile`

`make`

* Execute

`./driver`

