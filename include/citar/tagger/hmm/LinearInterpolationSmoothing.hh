/*
 * Copyright 2008 Daniel de Kok
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

#ifndef CITAR_LINEARINTERPOLATIONSMOOTHING
#define CITAR_LINEARINTERPOLATIONSMOOTHING

#include <string>

#include <tr1/memory>
#include <tr1/unordered_map>

#include <citar/config.hh>

#include "BiGram.hh"
#include "Model.hh"
#include "Smoothing.hh"
#include "TriGram.hh"
#include "UniGram.hh"

namespace citar {
namespace tagger {

class LinearInterpolationSmoothingPrivate;

/**
 * A class that calculates trigram probabilities using Linear interpolation
 * smoothing.
 */
class LinearInterpolationSmoothing : public Smoothing
{
public:
	typedef std::tr1::unordered_map<TriGram, double, TriGramHash> TriGramProbs;

	LinearInterpolationSmoothing(std::tr1::shared_ptr<Model const> model);
	~LinearInterpolationSmoothing();

	/**
	 * Calculate the logprob of a trigram.
	 */
	double triGramProb(TriGram const &triGram) const;
private:
	LinearInterpolationSmoothingPrivate *d_private;
};

}
}

#endif // CITAR_LINEARINTERPOLATIONSMOOTHING
