## Native (via Docker)

Prepare docker image for Native compilation:

```bash
make native-image
```

Compile natively:

```bash
make compile-native
```

Compile natively, marian-decoder only:

```bash
make compile-decoder-only-native
```

Enter a docker container shell for manually running commands:

```bash
make native-shell
```

Run the native-compiled code by entering a docker container shell above and manually running commands, eg:

```bash
cd /repo/build-native-docker # OR: cd /repo/build-native-decoder-only-docker
echo "hola mundo" | ./marian-decoder -m /repo/models/model.npz -v /repo/models/vocab.esen.spm /repo/models/vocab.esen.spm --cpu-threads 1
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
