
## Requirements

This program depends on the [libquentier](https://github.com/d1vanov/libquentier/) library. Make sure this library is install first, before compiling and running the program. You also may need to use qtcreator to add the library path for this library if it is not in `/usr/local/lib`.

## Build
To build, run the following commands:

```
qmake-qt5 -makefile crackenex.pro
make
```

## How-To
Firstly, copy-paste the content of the `<en-crypt>` tag form your .enex file into its own file. (autodetect/read will come in the near future)

The program can be run like so:
```
./crackenex --dict <dictionary file> --source <hash file>
```

