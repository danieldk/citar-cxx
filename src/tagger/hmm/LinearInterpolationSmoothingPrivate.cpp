#include <cmath>
#include <memory>

#include <tr1/memory>
#include <tr1/unordered_map>

#include <citar/config.hh>
#include <citar/tagger/hmm/BiGram.hh>
#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/hmm/TriGram.hh>
#include <citar/tagger/hmm/UniGram.hh>

#include "LinearInterpolationSmoothingPrivate.hh"


using namespace std;
using namespace std::tr1;
using namespace citar::tagger;

LinearInterpolationSmoothingPrivate::LinearInterpolationSmoothingPrivate(
	std::tr1::shared_ptr<Model const> model)
:
	d_uniGrams(new UniGramFreqs(model->uniGrams())),
	d_biGrams(new BiGramFreqs(model->biGrams())),
	d_triGrams(new TriGramFreqs(model->triGrams())),
#ifdef WITH_TRIGRAM_CACHE
	d_triGramCache(new TriGramProbs()),
#endif
	d_corpusSize(0)
{
	calculateCorpusSize();
	calculateLambdas();
}

LinearInterpolationSmoothingPrivate &LinearInterpolationSmoothingPrivate::operator=(
	LinearInterpolationSmoothingPrivate const &other)
{
	if (this != &other)
		copy(other);

	return *this;
}

void LinearInterpolationSmoothingPrivate::copy(
	LinearInterpolationSmoothingPrivate const &other)
{
	d_uniGrams.reset(new UniGramFreqs(*other.d_uniGrams));
	d_biGrams.reset(new BiGramFreqs(*other.d_biGrams));
	d_triGrams.reset(new TriGramFreqs(*other.d_triGrams));
#ifdef WITH_TRIGRAM_CACHE
	d_triGramCache.reset(new TriGramProbs(*other.d_triGramCache));
#endif
	d_corpusSize = other.d_corpusSize;
	d_l1 = other.d_l1;
	d_l2 = other.d_l2;
	d_l3 = other.d_l3;
}

void LinearInterpolationSmoothingPrivate::calculateCorpusSize()
{
	for (UniGramFreqs::const_iterator iter = d_uniGrams->begin();
		iter != d_uniGrams->end(); ++iter)
		d_corpusSize += iter->second;
}

void LinearInterpolationSmoothingPrivate::calculateLambdas()
{
	size_t l1f = 0;
	size_t l2f = 0;
	size_t l3f = 0;

	for (TriGramFreqs::const_iterator iter = d_triGrams->begin();
		iter != d_triGrams->end(); ++iter)
	{
		TriGram t1t2t3(iter->first);

		BiGram t1t2(t1t2t3.t1, t1t2t3.t2);
		double l3p = 0.0;
		BiGramFreqs::const_iterator biGramIter = d_biGrams->find(t1t2);
		if (biGramIter != d_biGrams->end() && biGramIter->second - 1 != 0)
			l3p = (iter->second - 1) / (static_cast<double>(biGramIter->second) - 1);

		BiGram t2t3(t1t2t3.t2, t1t2t3.t3);
		UniGram t2(t1t2t3.t2);
		double l2p = 0.0;
		biGramIter = d_biGrams->find(t2t3);
		UniGramFreqs::const_iterator uniGramIter = d_uniGrams->find(t2);
		if (uniGramIter != d_uniGrams->end() && uniGramIter->second - 1 != 0)
			l2p = (biGramIter->second - 1) / (static_cast<double>(uniGramIter->second) - 1);

		UniGram t3(t1t2t3.t3);
		uniGramIter = d_uniGrams->find(t3);
		double l1p = (uniGramIter->second - 1) /
			(static_cast<double>(d_corpusSize) - 1);

		if (l1p > l2p && l1p > l3p)
			l1f += iter->second;
		else if (l2p > l1p && l2p > l3p)
			l2f += iter->second;
		else
			l3f += iter->second;
	}

	double totalTriGrams = static_cast<double>(l1f) + l2f + l3f;

	d_l1 = l1f / totalTriGrams;
	d_l2 = l2f / totalTriGrams;
	d_l3 = l3f / totalTriGrams;
}

double LinearInterpolationSmoothingPrivate::triGramProb(TriGram const &triGram) const
{
#ifdef WITH_TRIGRAM_CACHE
	// If we have cached the likelyhood for this trigram, return it.
	TriGramProbs::const_iterator triGramCacheIter = d_triGramCache->find(triGram);
	if (triGramCacheIter != d_triGramCache->end())
		return triGramCacheIter->second;
#endif

	// Unigram likelihood P(t3).
	UniGram t3(triGram.t3);
	UniGramFreqs::const_iterator uniGramIter = d_uniGrams->find(t3);
	double uniGramProb = uniGramIter->second / static_cast<double>(d_corpusSize);

	// Bigram likelihood P(t3|t2).
	BiGram t2t3(triGram.t2, triGram.t3);
	BiGramFreqs::const_iterator biGramIter = d_biGrams->find(t2t3);
	UniGram t2(triGram.t2);
	uniGramIter = d_uniGrams->find(t2);
	double biGramProb = 0.0;
	if (biGramIter != d_biGrams->end() && uniGramIter != d_uniGrams->end() &&
		uniGramIter->second != 0)
		biGramProb = biGramIter->second / static_cast<double>(uniGramIter->second);

	// Trigram likelihood P(t3|t1,t2).
	TriGramFreqs::const_iterator triGramIter = d_triGrams->find(triGram);
	BiGram t1t2(triGram.t1, triGram.t2);
	biGramIter = d_biGrams->find(t1t2);
	double triGramProb = 0.0;
	if (triGramIter != d_triGrams->end() && biGramIter != d_biGrams->end() &&
		biGramIter->second != 0)
		triGramProb = triGramIter->second / static_cast<double>(biGramIter->second);

	// Calculate the trigram probability.
	double prob = log(d_l1 * uniGramProb + d_l2 * biGramProb + d_l3 * triGramProb);

#ifdef WITH_TRIGRAM_CACHE
	// Cache the trigram probability.
	(*d_triGramCache)[triGram] = prob;
#endif

	return prob;
}
