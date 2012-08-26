#include <algorithm>
#include <cmath>
#include <map>
#include <string>

#include <tr1/unordered_map>

#include <citar/tagger/hmm/UniGram.hh>
#include <citar/tagger/wordhandler/WordHandler.hh>
#include <citar/tagger/wordhandler/WordSuffixTree.hh>

using namespace std;
using namespace std::tr1;
using namespace citar::tagger;

void WordSuffixTree::addWord(string word, map<size_t, size_t> tagFreqs)
{
	// We will traverse the suffix tree by reverse suffix order.
	reverse(word.begin(), word.end());

	// Chop the suffix to the length we are interested in.
	if (word.size() > d_maxLength)
		word = word.substr(0, d_maxLength);

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
		freqSum += iter->second;

	double stdDevSum = 0.0;
	for (UniGramFreqs::const_iterator iter = d_uniGrams->begin();
			iter != d_uniGrams->end(); ++iter)
	{
		// P(t)
		double p = iter->second / static_cast<double>(freqSum);

		stdDevSum += pow(p - pAvg, 2.0);
	}

	d_theta = sqrt(stdDevSum / (d_uniGrams->size() - 1));
}

unordered_map<size_t, double> WordSuffixTree::suffixTagProbs(string word)
{
	// We will search the tree by reverse suffix order.
	reverse(word.begin(), word.end());

	// Chop to the length we are interested in.
	if (word.size() > d_maxLength)
		word = word.substr(0, d_maxLength);

	return d_rootNode->suffixTagProbs(word, unordered_map<size_t, double>());
}

void WordSuffixTree::TreeNode::addSuffix(string const &reverseSuffix,
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
	char transitionChar = reverseSuffix[0];
	if (d_children.find(transitionChar) == d_children.end())
		d_children[transitionChar].reset(new TreeNode(d_tree));
	d_children[transitionChar]->addSuffix(reverseSuffix.substr(1), tagFreqs);
}

unordered_map<size_t, double> WordSuffixTree::TreeNode::bayesianInversion(
	unordered_map<size_t, double> const &tagProbs) const
{
	unordered_map<size_t, double> inverseTagProbs;

	for (unordered_map<size_t, double>::const_iterator iter = tagProbs.begin();
			iter != tagProbs.end(); ++iter)
		inverseTagProbs[iter->first] =
			iter->second / d_tree->uniGrams().find(UniGram(iter->first))->second;

	return inverseTagProbs;
}

unordered_map<size_t, double> WordSuffixTree::TreeNode::suffixTagProbs(
	string const &reverseSuffix, unordered_map<size_t, double> const &tagProbs)
{
	unordered_map<size_t, double> newTagProbs;

	for(unordered_map<size_t, size_t>::const_iterator iter =
		d_tree->root().d_tagFreqs.begin(); iter!= d_tree->root().d_tagFreqs.end();
		++iter)
	{
		size_t const &tag = iter->first;

		double p = 0.0;
		if (d_tagFreqs.find(tag) != d_tagFreqs.end())
			// P(t|reverseSuffix).
			p = d_tagFreqs[tag] / static_cast<double>(d_tagFreq);


		if (&d_tree->root() != this)
		{
			// Add weighted probability of the shorter suffixes.
			p += d_tree->theta() * tagProbs.find(tag)->second;

			// Normalize.
			p /= d_tree->theta() + 1.0;
		}

		newTagProbs[tag] = p;
	}

	// If we have processed the full suffix, we are done.
	if (reverseSuffix.size() == 0)
		return bayesianInversion(newTagProbs);

	// If we can't make any further transitions, we are done.
	char transitionChar = reverseSuffix[0];
	if (d_children.find(transitionChar) == d_children.end())
		return bayesianInversion(newTagProbs);

	// Transition.
	return d_children[transitionChar]->suffixTagProbs(reverseSuffix.substr(1),
		newTagProbs);
}
