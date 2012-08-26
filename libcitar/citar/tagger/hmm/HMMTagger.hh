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

#ifndef LANGKIT_TAGGER_HMMTAGGER
#define LANGKIT_TAGGER_HMMTAGGER

#include <cmath>
#include <functional>
#include <string>
#include <vector>

#include <tr1/memory>

#include <citar/tagger/wordhandler/WordHandler.hh>
#include "Model.hh"
#include "Smoothing.hh"
#include "TagMatrixEntry.hh"

namespace citar {
namespace tagger {

/**
 * This class implements a Hidden Markov Model (HMM) trigram tagger.
 */
class HMMTagger
{
public:
	HMMTagger(std::tr1::shared_ptr<Model> model, WordHandler const *wordHandler,
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

#endif // LANGKIT_TAGGER_HMMTAGGER
