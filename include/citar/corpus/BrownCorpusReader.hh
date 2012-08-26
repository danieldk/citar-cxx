#ifndef CITAR_BROWNCORPUSREADER_HH
#define CITAR_BROWNCORPUSREADER_HH

#include <istream>
#include <string>
#include <vector>

#include "CorpusReader.hh"
#include "TaggedWord.hh"

namespace citar {
namespace corpus {

class BrownCorpusReaderPrivate;

class BrownCorpusReader : public CorpusReader
{
public:
	BrownCorpusReader(std::vector<TaggedWord> const &startMarkers,
			std::vector<TaggedWord> const &endMarkers,
			bool decapitalizeFirstWord = false);
	~BrownCorpusReader();
	void addSentenceHandler(std::tr1::shared_ptr<SentenceHandler> sentenceHandler);
	void parse(std::istream &in);
private:
	BrownCorpusReaderPrivate *d_private;
};

}
}

#endif // CITAR_BROWNCORPUSREADER_HH
