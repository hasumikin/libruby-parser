# libruby-parser

## Setup

```sh
$ cd [path/to/ruby/ruby]
$ ./configure --enable-shared optflags=-O0 debugflags=-ggdb3 cppflags=-DUNIVERSAL_PARSER
$ make
```

> [!NOTE]
> `optflags=-O0 debugflags=-ggdb3` is optional but recommended for debugging.

```sh
$ cd [path/to/libruby-parser(this repository)]
$ ln -s [/full/path/to/ruby/ruby/] lib/
```

## Build

```sh
$ make
```

## Try it out

```sh
$ build/main fixtures/a.rb
```

## Goals

- Make libruby-parser.a dependent only on `ruby/ruby/rubyparser.h`

