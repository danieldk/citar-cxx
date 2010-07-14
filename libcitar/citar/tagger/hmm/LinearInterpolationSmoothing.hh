/*
 * Copyright 2008 Daniel de Kok
 *
 * This file is part of Citar.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef CITAR_LINEARINTERPOLATIONSMOOTHING
#define CITAR_LINEARINTERPOLATIONSMOOTHING

#include <QHash>
#include <QReadWriteLock>
#include <QSharedPointer>

#include "BiGram.hh"
#include "Model.hh"
#include "Smoothing.hh"
#include "TriGram.hh"
#include "UniGram.hh"

namespace citar {
namespace tagger {

/**
 * A class that calculates trigram probabilities using Linear interpolation
 * smoothing.
 */
class LinearInterpolationSmoothing : public Smoothing
{
public:
	typedef QHash<TriGram, double> TriGramProbs;

	LinearInterpolationSmoothing(QSharedPointer<Model const> model);
	LinearInterpolationSmoothing(LinearInterpolationSmoothing const &other);
	LinearInterpolationSmoothing &operator=(
		LinearInterpolationSmoothing const &other);

	/**
	 * Calculate the logprob of a trigram.
	 */
	double triGramProb(TriGram const &triGram) const;
private:
	void copy(LinearInterpolationSmoothing const &other);
	void calculateCorpusSize();
	void calculateLambdas();

	QSharedPointer<UniGramFreqs> d_uniGrams;
	QSharedPointer<BiGramFreqs> d_biGrams;
	QSharedPointer<TriGramFreqs> d_triGrams;
#ifdef WITH_TRIGRAM_CACHE
	QSharedPointer<TriGramProbs> d_triGramCache;
	QSharedPointer<QReadWriteLock> d_trigramCacheLock;
#endif
	size_t d_corpusSize;
	double d_l1;
	double d_l2;
	double d_l3;
};

inline LinearInterpolationSmoothing::LinearInterpolationSmoothing(
	LinearInterpolationSmoothing const &other) : Smoothing()
{
	copy(other);
}

}
}

#endif // CITAR_LINEARINTERPOLATIONSMOOTHING
