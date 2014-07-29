#ifndef CITAR_CONLL_READER_PRIVATE_HH
#define CITAR_CONLL_READER_PRIVATE_HH

#include <istream>
#include <string>
#include <vector>

#include <memory>

#include <citar/corpus/CorpusReader.hh>
#include <citar/corpus/TaggedWord.hh>

namespace citar {
namespace corpus {

class CONLLCorpusReaderPrivate : public CorpusReader
{
public:
  CONLLCorpusReaderPrivate(std::vector<TaggedWord> const &startMarkers,
      std::vector<TaggedWord> const &endMarkers,
      bool decapitalizeFirstWord = false) :
    d_startMarkers(startMarkers), d_endMarkers(endMarkers),
    d_decapitalizeFirstWord(decapitalizeFirstWord) {}
  void addSentenceHandler(std::shared_ptr<SentenceHandler> sentenceHandler);
  void parse(std::istream &in);

private:
  TaggedWord parseLine(std::string const &line);

  std::vector<std::shared_ptr<SentenceHandler> > d_sentenceHandlers;
  std::vector<TaggedWord> d_startMarkers;
  std::vector<TaggedWord> d_endMarkers;
  bool d_decapitalizeFirstWord;
};

inline void CONLLCorpusReaderPrivate::addSentenceHandler(std::shared_ptr<SentenceHandler> sentenceHandler)
{
  d_sentenceHandlers.push_back(sentenceHandler);
}

}
}

#endif // CITAR_CONLL_READER_PRIVATE_HH
