## Native (via Docker)

Prepare docker image for Native compilation:

```bash
make native-image
```

Compile natively:

```bash
make compile-native
```

Compile natively, marian-decoder only with a wasm-incompatible blas library:

```bash
make compile-decoder-only-native
```

Compile natively, marian-decoder only with a wasm-compatible blas library:

```bash
make compile-wasm-compatible-decoder-only-native
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
make compile-wasm-without-pthreads
```

Create a directory called `models/` in the repo root and fill it with relevant model files that should be available to the WASM-compiled runtime, then run:
```bash
make package-files-wasm-without-pthreads
```

Run the wasm-compiled code:

```bash
make run-wasm-without-pthreads
```

Then open up `http://localhost:8001/marian-decoder.html?stdinInput=<stdin-input>&arguments=<command-line-args>` in a browser, eg:

```bash
open "http://localhost:8000/marian-decoder.html?stdinInput=Hola mundo&arguments=-m /repo/models/model.npz -v /repo/models/vocab.esen.spm /repo/models/vocab.esen.spm --cpu-threads 1"
```

Note: To run in Chrome, launch Chrome with `  --js-flags="--experimental-wasm-simd"`, eg:

```bash
/Applications/Google\ Chrome\ Canary.app/Contents/MacOS/Google\ Chrome\ Canary  --js-flags="--experimental-wasm-simd"
```

To compile to WASM with pthreads (to work on making that work), the corresponding commands are:

```bash
make compile-wasm
make package-files
make run-wasm
open "http://localhost:8000/marian-decoder.html?stdinInput=Hola mundo&arguments=-m /repo/models/model.npz -v /repo/models/vocab.esen.spm /repo/models/vocab.esen.spm --cpu-threads 1"
```

## Benchmarking

First, get the relevant models in place:
```bash
cd students/esen/
./download-models.sh
```

Then:

```bash
make benchmark-decoder-only-native
make benchmark-wasm-compatible-decoder-only-native
```

For WASM:
```bash
cp students/esen/esen.student.tiny11/vocab.esen.spm ../models/
cp students/esen/esen.student.tiny11/model.npz ../models/
cp students/esen/esen.student.tiny11/model.intgemm.bin ../models/
cp students/esen/esen.student.tiny11/model.intgemm.alphas.bin ../models/
cp students/esen/esen.student.tiny11/lex.s2t ../models/
make package-files-wasm-without-pthreads
make run-wasm-without-pthreads
```

Then open one of the following links to run a benchmark:
1. float32 `http://localhost:8001/marian-decoder.html?arguments=-m /repo/models/model.npz -v /repo/models/vocab.esen.spm /repo/models/vocab.esen.spm -i /repo/models/newstest2013.es.top300lines --beam-size 1 --mini-batch 32 --maxi-batch 100 --maxi-batch-sort src -w 128 --skip-cost --shortlist /repo/models/lex.s2t 50 50 --cpu-threads 1`
2. intgemm8 `http://localhost:8001/marian-decoder.html?arguments=-m /repo/models/model.npz -v /repo/models/vocab.esen.spm /repo/models/vocab.esen.spm -i /repo/models/newstest2013.es.top300lines --beam-size 1 --mini-batch 32 --maxi-batch 100 --maxi-batch-sort src -w 128 --skip-cost --shortlist /repo/models/lex.s2t 50 50 --cpu-threads 1 --int8shift`
3. intgemm8 with binary model file `http://localhost:8001/marian-decoder.html?arguments=-m /repo/models/model.intgemm.bin -v /repo/models/vocab.esen.spm /repo/models/vocab.esen.spm -i /repo/models/newstest2013.es.top300lines --beam-size 1 --mini-batch 32 --maxi-batch 100 --maxi-batch-sort src -w 128 --skip-cost --shortlist /repo/models/lex.s2t 50 50 --cpu-threads 1 --int8shift`
4. intgemm8alphas `http://localhost:8001/marian-decoder.html?arguments=-m /repo/models/model.npz -v /repo/models/vocab.esen.spm /repo/models/vocab.esen.spm -i /repo/models/newstest2013.es.top300lines --beam-size 1 --mini-batch 32 --maxi-batch 100 --maxi-batch-sort src -w 128 --skip-cost --shortlist /repo/models/lex.s2t 50 50 --cpu-threads 1 --int8shiftAlphaAll`
5. intgemm8alphas with binary model file `http://localhost:8001/marian-decoder.html?arguments=-m /repo/models/model.intgemm.alphas.bin -v /repo/models/vocab.esen.spm /repo/models/vocab.esen.spm -i /repo/models/newstest2013.es.top300lines --beam-size 1 --mini-batch 32 --maxi-batch 100 --maxi-batch-sort src -w 128 --skip-cost --shortlist /repo/models/lex.s2t 50 50 --cpu-threads 1 --int8shiftAlphaAll`

Remember that the Developer Tools must not be open when opening the links or refreshing the page to run the benchmark again.

Note that intgemm options are only available in Firefox Nightly and requires `javascript.options.wasm_baselinejit` to be `false` and `javascript.options.wasm_simd_wormhole` to be `true` in `about:config`, verified by visiting [this link](https://axis-of-eval.org/sandbox/wormhole-test.html).

## Compile and benchmark outside of docker

```bash
make echo-compile-decoder-only-native
make echo-compile-wasm-compatible-decoder-only-native
make echo-benchmark-decoder-only-native
make echo-benchmark-wasm-compatible-decoder-only-native
```

Copy the output of these commands and run those commands in the marian-dev source directory. Check `./native/Dockerfile` for a hint of what dependencies needs to be installed on your system.

## Debugging

Remove the marian-decoder build dir, forcing the next compilation attempt to start from scratch:

```bash
make clean-decoder-only-native
make clean-wasm-compatible-decoder-only-native
make clean-wasm
make clean-wasm-without-pthreads
```

Compile only sentencepiece to wasm:

```bash
make compile-sentencepiece-wasm
```

Compile and run a wasm stdin test:

```bash
make compile-and-run-test-stdin-wasm
open "http://localhost:8009/compile-test-stdin-wasm.html"
```

Enter a docker container shell for manually running commands:

```bash
make wasm-shell
```
