#ifndef CITAR_BROWNCORPUSREADER_HH
#define CITAR_BROWNCORPUSREADER_HH

#include <iosfwd>
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
	BrownCorpusReader(std::istream *is);
	~BrownCorpusReader();
	virtual boost::optional<std::vector<TaggedWord>> nextSentence();
private:
	BrownCorpusReaderPrivate *d_private;
};

}
}

#endif // CITAR_BROWNCORPUSREADER_HH
