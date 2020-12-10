#include "data/vocab_base.h"

#ifdef USE_SENTENCEPIECE
#include "sentencepiece/src/sentencepiece_processor.h"
#include "sentencepiece/src/sentencepiece_trainer.h"

/* https://github.com/google/googletest/issues/1063#issuecomment-332518392 */
#if __GNUC__ >= 5
// Disable GCC 5's -Wsuggest-override warnings in gtest
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#endif
#include "sentencepiece/src/builtin_pb/sentencepiece.pb.h"
#if __GNUC__ >= 5
#pragma GCC diagnostic pop
#endif

#endif

#include "common/config.h"
#include "common/filesystem.h"
#include "common/logging.h"
#include "common/options.h"
#include "common/regex.h"

#include <random>
#include <sstream>

namespace marian {
#ifdef USE_SENTENCEPIECE

// Wrapper around https://github.com/google/sentencepiece
class SentencePieceVocab : public IVocab {
private:
  // Actual SentencePiece processor object
  UPtr<sentencepiece::SentencePieceProcessor> spm_;

  // Sampling factor for subword regularization, disabled when 0
  float alpha_{0};

  // Allowed suffixes for SentencePiece model
  std::vector<std::string> suffixes_ = {".spm"};

  Ptr<Options> options_;
  size_t batchIndex_{0};

  std::mt19937 generator_;
  std::uniform_int_distribution<int> randInt_;  // from 0 to INT_MAX

  // Keeps sentences segmented into subword units
  bool keepEncoded_{false};

  // Sample from one file, based on first algorithm from:
  // https://en.wikipedia.org/wiki/Reservoir_sampling
  void reservoirSampling(std::vector<std::string>& sample,
                         size_t& seenLines,
                         const std::string& trainPath,
                         size_t maxLines,
                         size_t maxBytes);

  // Iterate over all input files and collect a representative sample via reservoir sampling.
  // The sample will first grow to the desired size and next keep sampling with decreasing
  // probability in the hope to get a uniform sample from the union of all files.
  size_t reservoirSamplingAll(io::TemporaryFile& temp,
                              const std::vector<std::string>& trainPaths,
                              size_t maxLines,
                              size_t maxBytes);

  // Just concatenate all files to a temporary file so SentencePiece can consume it.
  size_t dumpAll(io::TemporaryFile& temp,
                 const std::vector<std::string>& trainPaths,
                 size_t maxBytes);

public:
  SentencePieceVocab(Ptr<Options> options, size_t batchIndex);

  virtual const std::string& canonicalExtension() const override;
  virtual const std::vector<std::string>& suffixes() const override;

  virtual std::string suffix();
  virtual std::string type() const override;
  virtual Word getEosId() const override;
  virtual Word getUnkId() const override;

  void create(const std::string& vocabPath,
              const std::vector<std::string>& trainPaths,
              size_t maxSize) override;
  void createFake() override;
  Word operator[](const std::string& token) const override;
  const std::string& operator[](Word id) const override;
  Words encode(const std::string& line, bool addEOS, bool inference) const override;
  void encodeAsSentencePieceText(const std::string& line,
                                 sentencepiece::SentencePieceText& spt) const;
  std::string decode(const Words& sentence, bool /*ignoreEOS*/) const override;
  std::string surfaceForm(const Words& sentence) const override;
  size_t size() const override;
  size_t load(const std::string& vocabPath, size_t /*maxSize*/) override;
  std::string toUpper(const std::string& line) const override;
  std::string toEnglishTitleCase(const std::string& line) const override;
};
#endif  // USE_SENTENCEPIECE
}  // namespace marian