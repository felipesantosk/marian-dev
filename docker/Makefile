# -*- mode: makefile-gmake; indent-tabs-mode: true; tab-width: 4 -*-
SHELL   = bash
PWD     = $(shell pwd)
IMAGE   = emscripten/emsdk

all: marian-decoder-wasm

# Commands for compilation:
marian_decoder_compile_cmd  = em++ -O2 -msse -msse2 -mssse3 -msse4.1 -mavx -msimd128
marian_decoder_compile_cmd += -I../src/
marian_decoder_compile_cmd += -I../src/3rd_party/
marian_decoder_compile_cmd += -I../src/3rd_party/SQLiteCpp/include
marian_decoder_compile_cmd += -I../src/3rd_party/sentencepiece/src
marian_decoder_compile_cmd += -I../src/3rd_party/fbgemm/include
marian_decoder_compile_cmd += -I../src/3rd_party/intgemm
marian_decoder_compile_cmd += -I../src/3rd_party/
marian_decoder_compile_cmd += ../src/command/marian_decoder.cpp
marian_decoder_compile_cmd += -ferror-limit=100

# ... and running things on Docker
docker_mounts  = ${PWD}/..:/repo
run_on_docker  = docker run --rm
run_on_docker += $(addprefix -v, ${docker_mounts})
run_on_docker += ${INTERACTIVE_DOCKER_SESSION} ${IMAGE}

# Compile to WASM
marian-decoder-wasm:
	${run_on_docker} bash -c 'mkdir -p /repo/build-marian-decoder-wasm && cd /repo/build-marian-decoder-wasm && ${marian_decoder_compile_cmd}'

# Start an interactive shell for development / debugging purposes
shell: INTERACTIVE_DOCKER_SESSION = -it
shell:
	${run_on_docker} bash
