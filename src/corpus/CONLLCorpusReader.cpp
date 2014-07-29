#include <istream>
#include <vector>

#include <memory>

#include <citar/corpus/CONLLCorpusReader.hh>
#include <citar/corpus/TaggedWord.hh>

#include "CONLLCorpusReaderPrivate.hh"

using namespace citar::corpus;

CONLLCorpusReader::CONLLCorpusReader(std::vector<TaggedWord> const &startMarkers,
		std::vector<TaggedWord> const &endMarkers,
		bool decapitalizeFirstWord)
	: d_private(new CONLLCorpusReaderPrivate(startMarkers, endMarkers,
		decapitalizeFirstWord))
{
}

CONLLCorpusReader::~CONLLCorpusReader()
{
	delete d_private;
}

void CONLLCorpusReader::addSentenceHandler(std::shared_ptr<SentenceHandler> sentenceHandler)
{
	d_private->addSentenceHandler(sentenceHandler);
}


void CONLLCorpusReader::parse(std::istream &in)
{
	d_private->parse(in);
}
