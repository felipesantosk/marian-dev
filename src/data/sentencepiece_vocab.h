

#ifdef USE_SENTENCEPIECE

namespace sentencepiece { class SentencePieceText; }
#define _USE_INTERNAL_STRING_VIEW  
#include "sentencepiece/src/sentencepiece_processor.h"
#include "sentencepiece/src/sentencepiece_trainer.h"
/* https://github.com/google/googletest/issues/1063#issuecomment-332518392 */	
#if __GNUC__ >= 5	
// Disable GCC 5's -Wsuggest-override warnings in gtest	
# pragma GCC diagnostic push	
# pragma GCC diagnostic ignored "-Wsuggest-override"	
#endif	
#include "sentencepiece/src/builtin_pb/sentencepiece.pb.h"	
#if __GNUC__ >= 5	
# pragma GCC diagnostic pop	
#endif	

#endif
