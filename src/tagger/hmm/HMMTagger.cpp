#include <string>
#include <vector>

#include <tr1/unordered_map>

#include <citar/tagger/hmm/HMMTagger.hh>

#include "HMMTaggerPrivate.hh"

using namespace citar::tagger;

HMMTagger::HMMTagger(std::tr1::shared_ptr<Model> model, WordHandler const *wordHandler,
	Smoothing const *smoothing, double beamFactor)
	: d_private(new HMMTaggerPrivate(model, wordHandler, smoothing, beamFactor))
{}

HMMTagger::~HMMTagger()
{
	delete d_private;
}

std::vector<std::string> HMMTagger::tag(std::vector<std::string> const &sentence) const
{
	return d_private->tag(sentence);
}
