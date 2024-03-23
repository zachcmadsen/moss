# Examples

Example C programs that can compile to images which can be executed by `moss`.

## Build

Configure and build the examples with

```bash
$ cmake --preset default
$ cmake --build build
```

Build an example without CMake:

```bash
$ mos-common-clang -o <example> -Os -lexit-loop -linit-stack <example>.c 
```

<!-- TODO: Add a section about running the images with moss. -->
