The bindings were generated with

```bash
$ bindgen --allowlist-function "cpu_.*" -o src/bindings.rs ../include/cpu.h
```
