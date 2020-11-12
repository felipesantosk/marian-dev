## Native (via Docker)

Compile natively:

```bash
make native-image
make compile-native
```

Enter a docker container shell for manually running commands:

```bash
make native-shell
```

## WASM

Compile to wasm:

```bash
make wasm-image
make compile-wasm
```

Compile only sentencepiece to wasm:

```bash
make compile-sentencepiece-wasm
```

Enter a docker container shell for manually running commands:

```bash
make wasm-shell
```
