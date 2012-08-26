#ifndef LANGKIT_TAGGER_SMOOTHING
#define LANGKIT_TAGGER_SMOOTHING

#include <citar/util/NonCopyable.hh>

#include "TriGram.hh"

namespace citar {
namespace tagger {

class Smoothing : public citar::util::NonCopyable
{
public:
	virtual double triGramProb(TriGram const &triGram) const = 0;
	virtual ~Smoothing() {}
};

}
}

#endif
