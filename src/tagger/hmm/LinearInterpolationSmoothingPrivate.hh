/*
 * Copyright 2008, 2014 Daniel de Kok
 *
 * This file is part of citar.
 *
 * Citar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Citar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Citar.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CITAR_LINEARINTERPOLATIONSMOOTHING_PRIVATE_HH
#define CITAR_LINEARINTERPOLATIONSMOOTHING_PRIVATE_HH

#include <string>

#include <memory>
#include <unordered_map>

#include <citar/config.hh>


#include <citar/tagger/hmm/BiGram.hh>
#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/hmm/Smoothing.hh>
#include <citar/tagger/hmm/TriGram.hh>
#include <citar/tagger/hmm/UniGram.hh>

namespace citar {
namespace tagger {

/**
 * A class that calculates trigram probabilities using Linear interpolation
 * smoothing.
 */
class LinearInterpolationSmoothingPrivate : public Smoothing
{
public:
	typedef std::unordered_map<TriGram, double, TriGramHash> TriGramProbs;

	LinearInterpolationSmoothingPrivate(std::shared_ptr<Model const> model);
	LinearInterpolationSmoothingPrivate(LinearInterpolationSmoothingPrivate const &other);
	LinearInterpolationSmoothingPrivate &operator=(
		LinearInterpolationSmoothingPrivate const &other);

	/**
	 * Calculate the logprob of a trigram.
	 */
	double triGramProb(TriGram const &triGram) const;
private:
	void copy(LinearInterpolationSmoothingPrivate const &other);
	void calculateCorpusSize();
	void calculateLambdas();

	std::unique_ptr<UniGramFreqs> d_uniGrams;
	std::unique_ptr<BiGramFreqs> d_biGrams;
	std::unique_ptr<TriGramFreqs> d_triGrams;
#ifdef WITH_TRIGRAM_CACHE
	std::unique_ptr<TriGramProbs> d_triGramCache;
#endif
	size_t d_corpusSize;
	double d_l1;
	double d_l2;
	double d_l3;
};

inline LinearInterpolationSmoothingPrivate::LinearInterpolationSmoothingPrivate(
	LinearInterpolationSmoothingPrivate const &other)
{
	copy(other);
}

}
}

#endif // CITAR_LINEARINTERPOLATIONSMOOTHING_PRIVATE_HH
