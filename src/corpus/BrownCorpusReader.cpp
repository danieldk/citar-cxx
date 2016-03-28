#include <algorithm>
#include <cctype>
#include <istream>
#include <sstream>
#include <vector>

#include <citar/corpus/BrownCorpusReader.hh>

#include "BrownCorpusReaderPrivate.hh"

using namespace citar::corpus;

BrownCorpusReader::BrownCorpusReader(std::istream *is)
	: d_private(new BrownCorpusReaderPrivate(is))
{
}

BrownCorpusReader::~BrownCorpusReader()
{
	delete d_private;
}

boost::optional<std::vector<TaggedWord>> BrownCorpusReader::nextSentence()
{
	return d_private->nextSentence();
}
