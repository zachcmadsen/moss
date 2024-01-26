CC=clang
CFLAGS=-std=c2x -Wall -Wextra -Iinclude

OUTDIR = build

HEADERS = include/cpu.h
OBJECTS = build/cpu.o build/main.o

# What does this command do?
#   - c flag means only preprocess, compile, and assemble (don't link)
#   - $@ refers to the target (the left side of :)
#   - $< refers to the first dependency (the right side of :)
$(OUTDIR)/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

build: $(OBJECTS)
	$(CC) -o $(OUTDIR)/main $^

run: build
	./$(OUTDIR)/main

tidy: build
	clang-tidy --extra-arg=--std=c2x -header-filter=include/* -checks=bugprone-*,clang-analyzer-*,clang-diagnostic-*,misc-*,modernize-*,readability-*,-readability-identifier-length,-readability-implicit-bool-conversion,-readability-magic-numbers src/*.c

test:
	cargo t --manifest-path moss-sys/Cargo.toml --test it

clean:
	-rm -rf $(OUTDIR)/*
	-rm -f main
