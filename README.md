
CobuildConv is a tool for converting data of a CD-ROM titled
'Collins COBUILD Advanced Leaner's English Dictionary NEW
EDITION' to data for Dictionay.app. The original source of
CobuildConv.cpp is available at
http://www31.ocn.ne.jp/~h_ishida/CobuildConv/index.html (in Japanese).

## Prerequisition

* COBUILD5 CD-ROM

    mounted at /Volumes/COBULD5

* Xcode
    * Dictionary Development Kit

          please make sure the directory '/Developer/Extras/Dictionary Development Kit' exists

## How to install

    > make
    > make xml
    > make dic
    > make install

And then you launch Dictionary.app to test the new dictionary.
