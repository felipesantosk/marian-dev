## Native (via Docker)

Prepare docker image for Native compilation:

```bash
make native-image
```

Compile natively (Note: currently failing):

```bash
make compile-native
```

Enter a docker container shell for manually running commands:

```bash
make native-shell
```

## WASM

Prepare docker image for WASM compilation:

```bash
make wasm-image
```

Compile to wasm:

```bash
make compile-wasm
```

Create a directory called `models/` in the repo root and fill it with relevant model files that should be available to the WASM-compiled runtime, then run:
```bash
make package-files-wasm
```

Run the wasm-compiled code:

```bash
make run-wasm
```

Then open up `http://localhost:8000/marian-decoder.html?<command-line-args>` in a browser, eg:

```bash
open "http://localhost:8000/marian-decoder.html?-m /repo/models/model.npz -v /repo/models/vocab.esen.spm /repo/models/vocab.esen.spm --cpu-threads 1"
```

Note: To run in Chrome, launch Chrome with `  --js-flags="--experimental-wasm-simd"`, eg:

```bash
/Applications/Google\ Chrome\ Canary.app/Contents/MacOS/Google\ Chrome\ Canary  --js-flags="--experimental-wasm-simd"
```

### Debugging

Remove the marian-decoder WASM build dir, forcing the next compilation attempt to start from scratch:

```bash
make clean-wasm
```

Compile only sentencepiece to wasm:

```bash
make compile-sentencepiece-wasm
```

Enter a docker container shell for manually running commands:

```bash
make wasm-shell
```
