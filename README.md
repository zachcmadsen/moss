# moss

I'm writing `moss` to explore different interpreter approaches (cached 
interpreter, continuation-passing style, JIT, etc.). I chose the 6502 since
I've written an interpreter for it before as part of an NES
[emulator][duNES]. I'm still writing the baseline version, so there isn't
much to see yet.

## Build

```bash
$ cmake -S . -B build --preset=debug
$ cmake --build build
```

[duNES]: https://github.com/zachcmadsen/duNES
