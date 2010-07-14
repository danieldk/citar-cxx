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

#include "KnownWordHandler.ih"

KnownWordHandler::KnownWordHandler(QSharedPointer<Model const> model,
	WordHandler const *fallbackWordHandler)
:
	d_lexicon(new WordTagProbLexicon),
	d_fallbackWordHandler(fallbackWordHandler)
{
	calcWordTagProbs(model->lexicon(), model->uniGrams());
}

KnownWordHandler &KnownWordHandler::operator=(KnownWordHandler const &other)
{
	if (this != &other)
		copy(other);

	return *this;
}

void KnownWordHandler::copy(KnownWordHandler const &other)
{
	d_lexicon = QSharedPointer<WordTagProbLexicon>(new WordTagProbLexicon(*other.d_lexicon));
	d_fallbackWordHandler = other.d_fallbackWordHandler;
}

void KnownWordHandler::calcWordTagProbs(WordTagFreqs const &wordTagFreqs,
	UniGramFreqs const &uniGramFreqs)
{
	for (WordTagFreqs::const_iterator wordIter = wordTagFreqs.begin();
		wordIter != wordTagFreqs.end(); ++wordIter)
	{
		for (std::map<size_t, size_t>::const_iterator tagIter =
			wordIter.value().begin(); tagIter != wordIter.value().end();
			++tagIter)
		{
			// Calculate the maximum likelihood probability P(w|t).
			double p = log(static_cast<double>(tagIter->second) /
				uniGramFreqs.find(tagIter->first).value());
			(*d_lexicon)[wordIter.key()][tagIter->first] = p;
		}
	}
}

KnownWordHandler::ProbSet KnownWordHandler::tags(QString const &word) const
{
	// Find the maximum likelyhood probabilities, and add them to a set
	// ordered by decreasing probability.
	WordTagProbLexicon::const_iterator match = d_lexicon->find(word);
	if (match != d_lexicon->end())
	{
		ProbSet result;
		for (map<size_t, double>::const_iterator iter = match.value().begin();
				iter != match.value().end(); ++iter)
			result.insert(make_pair(iter->first, iter->second));
		return result;
	}
	// The word was not found, try once more with the lowercase variant.
	else if (word[0].isUpper())
	{
		QString wordLower = word;
		wordLower[0] = wordLower[0].toLower();

		match = d_lexicon->find(wordLower);
		if (match != d_lexicon->end())
		{
			ProbSet result;
			for (map<size_t, double>::const_iterator iter = match.value().begin();
				iter != match.value().end(); ++iter)
			result.insert(make_pair(iter->first, iter->second));
			return result;
		}
	}

	if (d_fallbackWordHandler)
		return d_fallbackWordHandler->tags(word);

	return ProbSet();
}

KnownWordHandler::~KnownWordHandler() {}

