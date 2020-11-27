# -*- mode: makefile-gmake; indent-tabs-mode: true; tab-width: 4 -*-
SHELL   		= bash
PWD     		= $(shell pwd)
NATIVE_IMAGE    = local/marian-build-native
WASM_IMAGE	    = local/marian-build-wasm

all: native-image wasm-image

# Build the Docker image for native builds
native-image:
	docker build -t local/marian-build-native ./native/

# Build the Docker image for WASM builds
wasm-image:
	docker build -t local/marian-build-wasm ./wasm/

# Commands for compilation:
cmake_cmd  = cmake --debug-output -Wno-dev
cmake_cmd += -DCMAKE_BUILD_TYPE=Release
cmake_cmd += -DGIT_REVISION="f00f00f0 2020-11-13 09:51:29 +0200"
cmake_cmd += -DPROJECT_VERSION_GIT_SHA="f00f00f0"
cmake_cmd += -DUSE_STATIC_LIBS=on
cmake_cmd += -DUSE_SENTENCEPIECE=on
cmake_cmd += -DCOMPILE_CUDA=off
cmake_cmd += -DUSE_DOXYGEN=off
cmake_cmd += -DUSE_FBGEMM=off
cmake_cmd += -DCOMPILE_LIBRARY_ONLY=off
native_cmake_cmd = ${cmake_cmd}
native_cmake_cmd += -DMKL=on
native_cmake_cmd += -DCOMPILE_CPU=on
native_cmake_cmd += -DCOMPILE_WASM=off

native_decoder_only_cmake_cmd = ${cmake_cmd}
native_decoder_only_cmake_cmd += -DMKL=off
native_decoder_only_cmake_cmd += -DCOMPILE_CPU=off
native_decoder_only_cmake_cmd += -DCOMPILE_DECODER_ONLY=on
native_decoder_only_cmake_cmd += -DCOMPILE_WASM=off

wasm_cmake_cmd = ${cmake_cmd}
wasm_cmake_cmd += -DMKL=off
wasm_cmake_cmd += -DCOMPILE_CPU=off
wasm_cmake_cmd += -DCOMPILE_DECODER_ONLY=on
wasm_cmake_cmd += -DCOMPILE_WASM=on
wasm_cmake_cmd += -DProtobuf_INCLUDE_DIR=/usr/opt/protobuf-wasm-lib/dist/include
wasm_cmake_cmd += -DProtobuf_LIBRARY=/usr/opt/protobuf-wasm-lib/dist/lib/libprotobuf.a
make_cmd  = make
make_cmd += VERBOSE=1

# ... and running things on Docker
docker_mounts  = ${PWD}/..:/repo
docker_mounts += ${HOME}/.ccache:/.ccache
run_on_docker  = docker run --rm
run_on_docker += $(addprefix -v, ${docker_mounts})
run_on_docker += ${INTERACTIVE_DOCKER_SESSION}

${HOME}/.ccache:
	mkdir -p $@

# Compile native version
compile-native: BUILD_DIR = /repo/build-native-docker
compile-native: ${HOME}/.ccache
	${run_on_docker} ${NATIVE_IMAGE} bash -c '(rm -rf ${BUILD_DIR} || true) && \
mkdir -p ${BUILD_DIR} &&\
cd ${BUILD_DIR} &&\
(${native_cmake_cmd} .. &&\
${make_cmd} || rm CMakeCache.txt)'

# Remove the build directory of native marian-decoder, forcing a clean compilation attempt
clean-decoder-only-native: BUILD_DIR = /repo/build-native-decoder-only-docker
clean-decoder-only-native: ${HOME}/.ccache
	${run_on_docker} ${NATIVE_IMAGE} bash -c '(rm -rf ${BUILD_DIR} || true)'

# Compile native version of marian-decoder only
compile-decoder-only-native: BUILD_DIR = /repo/build-native-decoder-only-docker
compile-decoder-only-native: ${HOME}/.ccache
	${run_on_docker} ${NATIVE_IMAGE} bash -c 'mkdir -p ${BUILD_DIR} &&\
cd ${BUILD_DIR} &&\
(${native_decoder_only_cmake_cmd} .. &&\
${make_cmd} || rm CMakeCache.txt)'

# Remove the marian-decoder WASM build dir, forcing a clean compilation attempt
clean-wasm: BUILD_DIR = /repo/build-wasm-docker
clean-wasm: ${HOME}/.ccache
	${run_on_docker} ${WASM_IMAGE} bash -c '(rm -rf ${BUILD_DIR} || true)'

# Compile marian-decoder to WASM
compile-wasm: BUILD_DIR = /repo/build-wasm-docker
compile-wasm: ${HOME}/.ccache
	${run_on_docker} ${WASM_IMAGE} bash -c 'mkdir -p ${BUILD_DIR} && \
cd ${BUILD_DIR} && \
(emcmake ${wasm_cmake_cmd} .. && \
(emmake ${make_cmd}) || \
rm CMakeCache.txt)'

# Prepare files to be used with WASM-compiled marian-decoder
package-files-wasm: BUILD_DIR = /repo/build-wasm-docker
package-files-wasm: MODELS_DIR = /repo/models
package-files-wasm:
	${run_on_docker} ${WASM_IMAGE} bash -c 'python3 /emsdk/upstream/emscripten/tools/file_packager.py ${BUILD_DIR}/model-files.data --preload ${MODELS_DIR} --js-output=${BUILD_DIR}/model-files.js'

# Run WASM-compiled marian-decoder
run-wasm: BUILD_DIR = /repo/build-wasm-docker
run-wasm:
	${run_on_docker} -p 8000:8000 ${WASM_IMAGE} bash -c 'emrun --no_browser --port 8000 ${BUILD_DIR}'

# Compile sentencepiece to WASM
sentencepiece_wasm_cmake_cmd = cmake --debug-output -Wno-dev
sentencepiece_wasm_cmake_cmd += -DCOMPILE_WASM=on
sentencepiece_wasm_cmake_cmd += -DSPM_ENABLE_SHARED=off
sentencepiece_wasm_cmake_cmd += -DProtobuf_INCLUDE_DIR=/usr/opt/protobuf-wasm-lib/dist/include
sentencepiece_wasm_cmake_cmd += -DProtobuf_LIBRARY=/usr/opt/protobuf-wasm-lib/dist/lib/libprotobuf.a

compile-sentencepiece-wasm: BUILD_DIR = /repo/src/3rd_party/sentencepiece/build-wasm-docker
compile-sentencepiece-wasm: ${HOME}/.ccache
	${run_on_docker} ${WASM_IMAGE} bash -c '(rm -rf ${BUILD_DIR} || true) && \
mkdir -p ${BUILD_DIR} && \
cd ${BUILD_DIR} && \
(emcmake ${sentencepiece_wasm_cmake_cmd} .. && \
(emmake ${make_cmd}) || \
rm CMakeCache.txt)'

# Start interactive shells for development / debugging purposes
native-shell: INTERACTIVE_DOCKER_SESSION = -it
native-shell:
	${run_on_docker} ${NATIVE_IMAGE} bash

wasm-shell: INTERACTIVE_DOCKER_SESSION = -it
wasm-shell:
	${run_on_docker} ${WASM_IMAGE} bash
