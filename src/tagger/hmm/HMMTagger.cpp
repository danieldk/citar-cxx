#include <string>
#include <vector>

#include <unordered_map>

#include <citar/tagger/hmm/HMMTagger.hh>

#include "HMMTaggerPrivate.hh"

using namespace citar::tagger;

HMMTagger::HMMTagger(std::shared_ptr<Model> model, WordHandler const *wordHandler,
	LanguageModel const *languageModel, double beamFactor)
	: d_private(new HMMTaggerPrivate(model, wordHandler, languageModel, beamFactor))
{}

HMMTagger::~HMMTagger()
{
	delete d_private;
}

std::vector<std::string> HMMTagger::tag(std::vector<std::string> const &sentence) const
{
	return d_private->tag(sentence);
}
