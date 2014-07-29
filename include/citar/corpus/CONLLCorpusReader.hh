#ifndef CITAR_CONLL_READER_HH
#define CITAR_CONLL_READER_HH

#include <istream>
#include <vector>

#include <citar/corpus/CorpusReader.hh>
#include <citar/corpus/TaggedWord.hh>

namespace citar {
namespace corpus {

class CONLLCorpusReaderPrivate;

class CONLLCorpusReader : public CorpusReader {
public:
  CONLLCorpusReader(std::vector<TaggedWord> const &startMarkers,
      std::vector<TaggedWord> const &endMarkers,
      bool decapitalizeFirstWord = false);
  ~CONLLCorpusReader();

  void addSentenceHandler(std::shared_ptr<SentenceHandler> sentenceHandler);

  void parse(std::istream &in);

private:
  CONLLCorpusReaderPrivate *d_private;
};

}
}


#endif // CITAR_CONLL_READER_HH
