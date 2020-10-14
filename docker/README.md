Compile marian-decoder to wasm:

```bash
make marian-decoder-wasm
```

Enter a docker container shell for manually running commands:

```bash
make shell
```

Note: To avoid a sentencepiece header pollution issue (`../src/3rd_party/sentencepiece/version:1:1: error: expected unqualified-id`), run the following once:
```bash
mv ../src/3rd_party/sentencepiece/VERSION ../src/3rd_party/sentencepiece/VERSION-header-pollution-workaround
```
