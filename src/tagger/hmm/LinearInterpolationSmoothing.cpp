#include <citar/tagger/hmm/LinearInterpolationSmoothing.hh>
#include <citar/tagger/hmm/Model.hh>

#include "LinearInterpolationSmoothingPrivate.hh"

using namespace citar::tagger;

namespace citar {
namespace tagger {

LinearInterpolationSmoothing::LinearInterpolationSmoothing(
		std::tr1::shared_ptr<Model const> model)
	: d_private (new LinearInterpolationSmoothingPrivate(model))
{
}

LinearInterpolationSmoothing::~LinearInterpolationSmoothing()
{
	delete d_private;
}

double LinearInterpolationSmoothing::triGramProb(TriGram const &triGram) const
{
	return d_private->triGramProb(triGram);
}

}
}