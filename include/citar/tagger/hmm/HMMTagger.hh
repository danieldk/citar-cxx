#ifndef HMM_TAGGER_HH
#define HMM_TAGGER_HH

#include <string>
#include <vector>

#include <tr1/memory>

#include <citar/util/NonCopyable.hh>

namespace citar {
namespace tagger {

class HMMTaggerPrivate;
class Model;
class WordHandler;
class Smoothing;

/**
 * This class implements a Hidden Markov Model (HMM) trigram tagger.
 */
class HMMTagger : citar::util::NonCopyable
{
public:
	HMMTagger(std::tr1::shared_ptr<Model> model, WordHandler const *wordHandler,
		Smoothing const *smoothing,
		double beamFactor = 1000.0);
	~HMMTagger();

	/**
	 * Tag a sequence of words. A <i>vector</i> of words is expected, the
	 * optimal tag sequence is returned. The sentence should be preceded
	 * by two start markers ended by one end marker. The start markers
	 * should have identical words and tags.
	 */
	std::vector<std::string> tag(std::vector<std::string> const &sentence)
		const;
private:
	HMMTaggerPrivate *d_private;
};

}
}

#endif // HMM_TAGGER_HH
