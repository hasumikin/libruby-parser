
AR = ar
CC = gcc
RM_F = rm -f
CFLAGS = -Wall -O0 -ggdb3 -DUNIVERSAL_PARSER \
		 -I./include \
		 -I./lib/prism/include \
		 -I./lib/ruby \
		 -I./lib/ruby/include \
		 -I./lib/ruby/.ext/include/x86_64-linux
LDFLAGS = -L./build -L./lib/ruby -lcrypt -lm -lgmp -lz

all: build/main

build/pm_constant_pool.o: lib/prism/src/util/pm_constant_pool.c lib/prism/include/prism/util/pm_constant_pool.h
	$(CC) $(CFLAGS) -c lib/prism/src/util/pm_constant_pool.c -o $@

build/parser_dump.o: src/parser_dump.c include/parser_dump.h
	$(CC) $(CFLAGS) -c src/parser_dump.c -o $@

build/helper.o: src/helper.c include/helper.h
	$(CC) $(CFLAGS) -c src/helper.c -o $@

build/libruby-parser.a: build/helper.o build/parser_dump.o build/pm_constant_pool.o lib/ruby/libruby-static.a
	$(AR) rcs $@ $^

build/main: src/main.c build/libruby-parser.a
	$(CC) \
		$(CFLAGS) \
		-o $@ src/main.c \
		-l:libruby-static.a \
		-l:libruby-parser.a \
		$(LDFLAGS)

phony: all clean

clean:
	$(RM_F) build/main
	$(RM_F) build/*.o
	$(RM_F) build/libruby-parser.a

