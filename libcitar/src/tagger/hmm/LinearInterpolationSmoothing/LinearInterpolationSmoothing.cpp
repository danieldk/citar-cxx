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

#include "LinearInterpolationSmoothing.ih"

LinearInterpolationSmoothing::LinearInterpolationSmoothing(
	QSharedPointer<Model const> model)
:
	d_uniGrams(new UniGramFreqs(model->uniGrams())),
	d_biGrams(new BiGramFreqs(model->biGrams())),
	d_triGrams(new TriGramFreqs(model->triGrams())),
#ifdef WITH_TRIGRAM_CACHE
	d_triGramCache(new TriGramProbs()),
	d_trigramCacheLock(new QReadWriteLock()),
#endif
	d_corpusSize(0)
{
	calculateCorpusSize();
	calculateLambdas();
}

LinearInterpolationSmoothing &LinearInterpolationSmoothing::operator=(
	LinearInterpolationSmoothing const &other)
{
	if (this != &other)
		copy(other);

	return *this;
}

void LinearInterpolationSmoothing::copy(
	LinearInterpolationSmoothing const &other)
{
	d_uniGrams = QSharedPointer<UniGramFreqs>(new UniGramFreqs(*other.d_uniGrams));
	d_biGrams = QSharedPointer<BiGramFreqs>(new BiGramFreqs(*other.d_biGrams));
	d_triGrams = QSharedPointer<TriGramFreqs>(new TriGramFreqs(*other.d_triGrams));
#ifdef WITH_TRIGRAM_CACHE
	d_triGramCache.reset(new TriGramProbs(*other.d_triGramCache));
	d_trigramCacheLock.reset(new QReadWriteLock());
#endif
	d_corpusSize = other.d_corpusSize;
	d_l1 = other.d_l1;
	d_l2 = other.d_l2;
	d_l3 = other.d_l3;
}

void LinearInterpolationSmoothing::calculateCorpusSize()
{
	for (UniGramFreqs::const_iterator iter = d_uniGrams->begin();
		iter != d_uniGrams->end(); ++iter)
		d_corpusSize += iter.value();
}

void LinearInterpolationSmoothing::calculateLambdas()
{
	size_t l1f = 0;
	size_t l2f = 0;
	size_t l3f = 0;

	for (TriGramFreqs::const_iterator iter = d_triGrams->begin();
		iter != d_triGrams->end(); ++iter)
	{
		TriGram t1t2t3(iter.key());

		BiGram t1t2(t1t2t3.t1, t1t2t3.t2);
		double l3p = 0.0;
		BiGramFreqs::const_iterator biGramIter = d_biGrams->find(t1t2);
		if (biGramIter != d_biGrams->end() && biGramIter.value() - 1 != 0)
			l3p = (iter.value() - 1) / (static_cast<double>(biGramIter.value()) - 1);

		BiGram t2t3(t1t2t3.t2, t1t2t3.t3);
		UniGram t2(t1t2t3.t2);
		double l2p = 0.0;
		biGramIter = d_biGrams->find(t2t3);
		UniGramFreqs::const_iterator uniGramIter = d_uniGrams->find(t2);
		if (uniGramIter != d_uniGrams->end() && uniGramIter.value() - 1 != 0)
			l2p = (biGramIter.value() - 1) / (static_cast<double>(uniGramIter.value()) - 1);

		UniGram t3(t1t2t3.t3);
		uniGramIter = d_uniGrams->find(t3);
		double l1p = (uniGramIter.value() - 1) /
			(static_cast<double>(d_corpusSize) - 1);

		if (l1p > l2p && l1p > l3p)
			l1f += iter.value();
		else if (l2p > l1p && l2p > l3p)
			l2f += iter.value();
		else
			l3f += iter.value();
	}

	double totalTriGrams = static_cast<double>(l1f) + l2f + l3f;

	d_l1 = l1f / totalTriGrams;
	d_l2 = l2f / totalTriGrams;
	d_l3 = l3f / totalTriGrams;
}

double LinearInterpolationSmoothing::triGramProb(TriGram const &triGram) const
{
#ifdef WITH_TRIGRAM_CACHE
	{
		// Read-lock the cache.
		QReadLocker locker(d_trigramCacheLock.get());

		// If we have cached the likelyhood for this trigram, return it.
		TriGramProbs::const_iterator iter = d_triGramCache->find(triGram);
		if (iter != d_triGramCache->end())
			return iter.value();
	}
#endif

	// Unigram likelihood P(t3).
	UniGram t3(triGram.t3);
	UniGramFreqs::const_iterator uniGramIter = d_uniGrams->find(t3);
	double uniGramProb = uniGramIter.value() / static_cast<double>(d_corpusSize);

	// Bigram likelihood P(t3|t2).
	BiGram t2t3(triGram.t2, triGram.t3);
	BiGramFreqs::const_iterator biGramIter = d_biGrams->find(t2t3);
	UniGram t2(triGram.t2);
	uniGramIter = d_uniGrams->find(t2);
	double biGramProb = 0.0;
	if (biGramIter != d_biGrams->end() && uniGramIter != d_uniGrams->end() &&
		uniGramIter.value() != 0)
		biGramProb = biGramIter.value() / static_cast<double>(uniGramIter.value());

	// Trigram likelihood P(t3|t1,t2).
	TriGramFreqs::const_iterator triGramIter = d_triGrams->find(triGram);
	BiGram t1t2(triGram.t1, triGram.t2);
	biGramIter = d_biGrams->find(t1t2);
	double triGramProb = 0.0;
	if (triGramIter != d_triGrams->end() && biGramIter != d_biGrams->end() &&
		biGramIter.value() != 0)
		triGramProb = triGramIter.value() / static_cast<double>(biGramIter.value());

	// Calculate the trigram probability.
	double prob = log(d_l1 * uniGramProb + d_l2 * biGramProb + d_l3 * triGramProb);

#ifdef WITH_TRIGRAM_CACHE
	// Write-lock the cache.
	QWriteLocker locker(d_trigramCacheLock.get());

	// Cache the trigram probability.
	(*d_triGramCache)[triGram] = prob;
#endif

	return prob;
}
