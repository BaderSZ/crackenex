# crackenex
#### A program to crack encrypted notes in Evernote .enex files

## Requirements

This program depends on the [libquentier](https://github.com/d1vanov/libquentier/) library. Make sure this library is install first, before compiling and running the program. You also may need to use qtcreator to add the library path for this library if it is not in `/usr/local/lib`.

## Download
The pre-release for linux is available [here](https://github.com/BaderSZ/crackenex/releases)

## Build
To build, run the following commands:

```
qmake-qt5 -makefile crackenex.pro
make
```

## How-To
The program can be run like so:
```
./crackenex --dict <dictionary file> --source <enex file>
```
you can also provide your dictionary file via stdin instead of a file:

```
cat dict | crackenex --source <enex file>
```

