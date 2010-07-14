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

#include "WordSuffixTree.ih"

void WordSuffixTree::addWord(QString word, map<size_t, size_t> tagFreqs)
{
	// We will traverse the suffix tree by reverse suffix order.
	reverse(word.begin(), word.end());

	// Chop the suffix to the length we are interested in.
	if (static_cast<size_t>(word.size()) > d_maxLength)
		word = word.left(d_maxLength);

	d_rootNode->addSuffix(word, tagFreqs);
}

void WordSuffixTree::calculateTheta()
{
	// Theta is set to the standard deviation of the unconditioned maximum
	// likelihood probabilities of the tags in the corpus.

	double pAvg = 1.0 / d_uniGrams->size();

	size_t freqSum = 0;
	for (UniGramFreqs::const_iterator iter = d_uniGrams->begin();
			iter != d_uniGrams->end(); ++iter)
		freqSum += iter.value();

	double stdDevSum = 0.0;
	for (UniGramFreqs::const_iterator iter = d_uniGrams->begin();
			iter != d_uniGrams->end(); ++iter)
	{
		// P(t)
		double p = iter.value() / static_cast<double>(freqSum);

		stdDevSum += pow(p - pAvg, 2.0);
	}

	d_theta = sqrt(stdDevSum / (d_uniGrams->size() - 1));
}

QHash<size_t, double> WordSuffixTree::suffixTagProbs(QString word)
{
	// We will search the tree by reverse suffix order.
	reverse(word.begin(), word.end());

	// Chop to the length we are interested in.
	if (static_cast<size_t>(word.size()) > d_maxLength)
		word = word.left(d_maxLength);

	return d_rootNode->suffixTagProbs(word, QHash<size_t, double>());
}

void WordSuffixTree::TreeNode::addSuffix(QString const &reverseSuffix,
	map<size_t, size_t> const &tagFreqs)
{
	// Add frequencies for all the tags seen for the word the current
	// suffix is extracted from.
	for (map<size_t, size_t>::const_iterator iter = tagFreqs.begin();
		iter != tagFreqs.end(); ++iter)
	{
		d_tagFreqs[iter->first] += iter->second;
		d_tagFreq += iter->second;
	}

	// If the suffix is fully processed, we have reached the final
	// state for this suffix.
	if (reverseSuffix.size() == 0)
		return;

	// Add a transition.
	QChar transitionChar = reverseSuffix[0];
	if (d_children.find(transitionChar) == d_children.end())
		d_children[transitionChar] = QSharedPointer<TreeNode>(new TreeNode(d_tree));
	d_children[transitionChar]->addSuffix(reverseSuffix.mid(1), tagFreqs);
}

QHash<size_t, double> WordSuffixTree::TreeNode::bayesianInversion(
	QHash<size_t, double> const &tagProbs) const
{
	QHash<size_t, double> inverseTagProbs;

	for (QHash<size_t, double>::const_iterator iter = tagProbs.begin();
			iter != tagProbs.end(); ++iter)
		inverseTagProbs[iter.key()] =
			iter.value() / d_tree->uniGrams().find(UniGram(iter.key())).value();

	return inverseTagProbs;
}

QHash<size_t, double> WordSuffixTree::TreeNode::suffixTagProbs(
	QString const &reverseSuffix, QHash<size_t, double> const &tagProbs)
{
	QHash<size_t, double> newTagProbs;

	for(QHash<size_t, size_t>::const_iterator iter =
		d_tree->root().d_tagFreqs.begin(); iter!= d_tree->root().d_tagFreqs.end();
		++iter)
	{
		size_t const &tag = iter.key();

		double p = 0.0;
		if (d_tagFreqs.find(tag) != d_tagFreqs.end())
			// P(t|reverseSuffix).
			p = d_tagFreqs[tag] / static_cast<double>(d_tagFreq);


		if (&d_tree->root() != this)
		{
			// Add weighted probability of the shorter suffixes.
			p += d_tree->theta() * tagProbs.find(tag).value();

			// Normalize.
			p /= d_tree->theta() + 1.0;
		}

		newTagProbs[tag] = p;
	}

	// If we have processed the full suffix, we are done.
	if (reverseSuffix.size() == 0)
		return bayesianInversion(newTagProbs);

	// If we can't make any further transitions, we are done.
	QChar transitionChar = reverseSuffix[0];
	if (d_children.find(transitionChar) == d_children.end())
		return bayesianInversion(newTagProbs);

	// Transition.
	return d_children[transitionChar]->suffixTagProbs(reverseSuffix.mid(1),
		newTagProbs);
}
