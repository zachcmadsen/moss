CC=clang
CFLAGS=-std=c2x -Wall -Wextra -Iinclude

HEADERS = include/cpu.h
OBJECTS = build/cpu.o build/main.o

# What does this command do?
#   - c flag means only preprocess, compile, and assemble (don't link)
#   - $@ refers to the target (the left side of :)
#   - $< refers to the first dependency (the right side of :)
build/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJECTS)
	$(CC) -o $@ $^

run: main
	./main

clean:
	-rm -rf build/*
	-rm -f main
