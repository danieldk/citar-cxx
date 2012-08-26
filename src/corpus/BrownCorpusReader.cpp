#include <algorithm>
#include <cctype>
#include <istream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <tr1/memory>

#include <citar/corpus/BrownCorpusReader.hh>
#include <citar/corpus/TaggedWord.hh>

#include "BrownCorpusReaderPrivate.hh"

using namespace citar::corpus;

BrownCorpusReader::BrownCorpusReader(std::vector<TaggedWord> const &startMarkers,
		std::vector<TaggedWord> const &endMarkers,
		bool decapitalizeFirstWord)
	: d_private(new BrownCorpusReaderPrivate(startMarkers, endMarkers,
		decapitalizeFirstWord))
{
}

BrownCorpusReader::~BrownCorpusReader()
{
	delete d_private;
}

void BrownCorpusReader::addSentenceHandler(std::tr1::shared_ptr<SentenceHandler> sentenceHandler)
{
	d_private->addSentenceHandler(sentenceHandler);
}


void BrownCorpusReader::parse(std::istream &in)
{
	d_private->parse(in);
}
