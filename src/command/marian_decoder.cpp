#include "marian.h"
#include "translator/beam_search.h"
#include "translator/translator.h"
#include "common/timer.h"
#ifdef _WIN32
#include <Windows.h>
#endif
#ifdef COMPILE_WASM
#include <bind.h>
#endif

int main(int argc, char** argv) {
  using namespace marian;
  auto options = parseOptions(argc, argv, cli::mode::translation);
  auto task = New<Translate<BeamSearch>>(options);

#ifdef COMPILE_WASM
  fprintf(stderr, "Testing ABORT\n");fflush(stderr);
  ABORT("Testing ABORT");
#endif

  timer::Timer timer;
  fprintf(stderr, "Just before task->run()\n");fflush(stderr);
  task->run();
  LOG(info, "Total time: {:.5f}s wall", timer.elapsed());

  return 0;
}

#ifdef COMPILE_WASM
std::string getExceptionMessage(intptr_t exceptionPtr) {
  return std::string(reinterpret_cast<std::exception *>(exceptionPtr)->what());
}

EMSCRIPTEN_BINDINGS(Bindings) {
  emscripten::function("getExceptionMessage", &getExceptionMessage);
};
#endif
