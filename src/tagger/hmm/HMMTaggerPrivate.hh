#ifndef LANGKIT_TAGGER_HMM_TAGGER_PRIVATE
#define LANGKIT_TAGGER_HMM_TAGGER_PRIVATE

#include <cmath>
#include <functional>
#include <string>
#include <vector>

#include <tr1/memory>

#include <citar/tagger/wordhandler/WordHandler.hh>
#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/hmm/Smoothing.hh>

#include "TagMatrixEntry.hh"

namespace citar {
namespace tagger {

/**
 * This class implements a Hidden Markov Model (HMM) trigram tagger.
 */
class HMMTaggerPrivate
{
public:
	HMMTaggerPrivate(std::tr1::shared_ptr<Model> model, WordHandler const *wordHandler,
		Smoothing const *smoothing,
		double beamFactor = 1000.0)
	:
		d_model(model), d_wordHandler(wordHandler), d_smoothing(smoothing),
		d_beamFactor(log(beamFactor)) {}

	/**
	 * Tag a sequence of words. A <i>vector</i> of words is expected, the
	 * optimal tag sequence is returned. The sentence should be preceded
	 * by two start markers ended by one end marker. The start markers
	 * should have identical words and tags.
	 */
	std::vector<std::string> tag(std::vector<std::string> const &sentence)
		const;
private:
	std::tr1::shared_ptr<Model> d_model;
	WordHandler const *d_wordHandler;
	Smoothing const *d_smoothing;
	double d_beamFactor;
};

}
}

#endif // LANGKIT_TAGGER_HMM_TAGGER_PRIVATE
