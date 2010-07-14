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

#include "SuffixWordHandler.ih"

SuffixWordHandler::SuffixWordHandler(
	QSharedPointer<Model const> model, size_t maxSuffixLength,
	size_t upperMaxFreq, size_t lowerMaxFreq, size_t maxTags)
:
	d_upperSuffixTree(new WordSuffixTree(model->uniGrams(), maxSuffixLength)),
	d_lowerSuffixTree(new WordSuffixTree(model->uniGrams(), maxSuffixLength)),
	d_maxTags(maxTags)
{
	for (WordTagFreqs::const_iterator lexiconIter =
		model->lexicon().begin(); lexiconIter != model->lexicon().end();
		++lexiconIter)
	{
		// If the corpus is malformatted, the lexicon sometimes contains
		// frequencies for empty strings.
		if (lexiconIter.value().size() == 0)
			continue;

		TagFreqs const &tagFreqs = lexiconIter.value();

		// Get the total frequency of the word.
		size_t wordFreq = 0;
		for (TagFreqs::const_iterator freqIter =
				tagFreqs.begin(); freqIter != tagFreqs.end(); ++freqIter)
			wordFreq += freqIter->second;

		QString const &word = lexiconIter.key();

		bool uppercase = word[0].isUpper();

		// Select a suffix trie, based on the capitalization of the first
		// letter. Don't add the word to the trie if its frequency is above
		// the threshold.
		WordSuffixTree *suffixTree = 0;
		if (uppercase && wordFreq <= upperMaxFreq)
			suffixTree = d_upperSuffixTree.data();
		else if (!uppercase && wordFreq <= lowerMaxFreq)
			suffixTree = d_lowerSuffixTree.data();
		else
			continue;

		suffixTree->addWord(word, tagFreqs);
	}

	d_upperSuffixTree->calculateTheta();
	d_lowerSuffixTree->calculateTheta();
}

SuffixWordHandler::ProbSet SuffixWordHandler::tags(QString const &word) const
{
	// Select a suffix trie, based on the capitalization of the first
	// letter.
	WordSuffixTree *suffixTree = 0;
	if (word[0].isUpper())
		suffixTree = d_upperSuffixTree.data();
	else
		suffixTree = d_lowerSuffixTree.data();

	QHash<size_t, double> tagProbs = suffixTree->suffixTagProbs(word);

	// Order resulting tags by decreasing probability.
	ProbSet allTagProbs;
	for (QHash<size_t, double>::const_iterator iter = tagProbs.begin();
		iter != tagProbs.end(); ++iter)
	    allTagProbs.insert(make_pair(iter.key(), iter.value()));

	// Get the n most probable tags.
	ProbSet highestTagProbs;
	ProbSet::const_iterator iter = allTagProbs.begin();
	size_t i = 0;
	while (i < d_maxTags && iter != allTagProbs.end())
	{
		highestTagProbs.insert(make_pair(iter->first, log(iter->second)));
		++iter;
		++i;
	}

	return highestTagProbs;
}
