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
cmake_cmd += -DBUILD_ARCH=native
cmake_cmd += -DCMAKE_BUILD_TYPE=Release
cmake_cmd += -DGIT_REVISION="f00f00f0 2020-11-13 09:51:29 +0200"
cmake_cmd += -DPROJECT_VERSION_GIT_SHA="f00f00f0"
cmake_cmd += -DUSE_STATIC_LIBS=on
cmake_cmd += -DUSE_SENTENCEPIECE=on
cmake_cmd += -DCOMPILE_CUDA=off
cmake_cmd += -DUSE_DOXYGEN=off
cmake_cmd += -DUSE_FBGEMM=off
cmake_cmd += -DCOMPILE_LIBRARY_ONLY=on
native_cmake_cmd = ${cmake_cmd}
native_cmake_cmd += -DMKL=on
native_cmake_cmd += -DCOMPILE_CPU=on
native_cmake_cmd += -DCOMPILE_WASM=off
wasm_cmake_cmd = ${cmake_cmd}
wasm_cmake_cmd += -DMKL=off
wasm_cmake_cmd += -DCOMPILE_CPU=off
wasm_cmake_cmd += -DCOMPILE_WASM=on
wasm_cmake_cmd += -DProtobuf_INCLUDE_DIR=/root/protobuf-wasm-lib/dist/include
wasm_cmake_cmd += -DProtobuf_LIBRARY=/root/protobuf-wasm-lib/dist/lib/libprotobuf.a

# we use MAKE_OPTIONS for default make options (but the can still be overriden
# via
#      make MAKE_OPTIONS=''
# and EXTRA_MAKE_OPTIONS for additional make options, keeping the default options
MAKE_OPTIONS = -j4
make_cmd  = make ${MAKE_OPTIONS} ${EXTRA_MAKE_OPTIONS}


# ... and running things on Docker
docker_mounts  = ${PWD}/..:/repo
docker_mounts += ${HOME}/.ccache:/.ccache
docker_mounts += ${PWD}/../.git:${PWD}/.git
docker_mounts += ${PWD}/../.gitmodules:${PWD}/.gitmodules
run_on_docker  = docker run --rm
run_on_docker += $(addprefix -v, ${docker_mounts})
run_on_docker += --user $$(id -u):$$(id -g)
run_on_docker += ${INTERACTIVE_DOCKER_SESSION}

${HOME}/.ccache:
	mkdir -p $@

# Compile native version
compile-native: ${HOME}/.ccache
	${run_on_docker} ${NATIVE_IMAGE} bash -c 'mkdir -p /repo/build-native-docker &&\
cd /repo/build-native-docker &&\
(${native_cmake_cmd} .. &&\
${make_cmd} || rm CMakeCache.txt)'

# Compile marian-decoder to WASM
compile-wasm: ${HOME}/.ccache
	${run_on_docker} ${WASM_IMAGE} bash -c 'mkdir -p /repo/build-wasm-docker && \
cd /repo/build-wasm-docker && \
(emcmake ${wasm_cmake_cmd} .. && \
(emmake ${make_cmd}) || \
rm CMakeCache.txt)'

# Compile sentencepiece to WASM
sentencepiece_wasm_cmake_cmd = cmake --debug-output -Wno-dev
sentencepiece_wasm_cmake_cmd += -DCOMPILE_WASM=on
sentencepiece_wasm_cmake_cmd += -DSPM_ENABLE_SHARED=off
sentencepiece_wasm_cmake_cmd += -DProtobuf_INCLUDE_DIR=/root/protobuf-wasm-lib/dist/include
sentencepiece_wasm_cmake_cmd += -DProtobuf_LIBRARY=/root/protobuf-wasm-lib/dist/lib/libprotobuf.a

compile-sentencepiece-wasm: ${HOME}/.ccache
	${run_on_docker} ${WASM_IMAGE} bash -c 'mkdir -p /repo/src/3rd_party/sentencepiece/build-wasm-docker && \
cd /repo/src/3rd_party/sentencepiece/build-wasm-docker && \
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
