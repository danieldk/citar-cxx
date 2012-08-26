#include <algorithm>
#include <cctype>
#include <cmath>
#include <iterator>
#include <map>
#include <string>

#include <tr1/memory>
#include <tr1/unordered_map>

#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/wordhandler/SuffixWordHandler.hh>
#include <citar/tagger/wordhandler/WordSuffixTree.hh>

using namespace std;
using namespace std::tr1;
using namespace citar::tagger;

SuffixWordHandler::SuffixWordHandler(
	shared_ptr<Model const> model, size_t maxSuffixLength,
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
		TagFreqs const &tagFreqs = lexiconIter->second;

		// Get the total frequency of the word.
		size_t wordFreq = 0;
		for (TagFreqs::const_iterator freqIter =
				tagFreqs.begin(); freqIter != tagFreqs.end(); ++freqIter)
			wordFreq += freqIter->second;

		string const &word = lexiconIter->first;

		bool uppercase = isupper(word[0]);

		// Select a suffix trie, based on the capitalization of the first
		// letter. Don't add the word to the trie if its frequency is above
		// the threshold.
		WordSuffixTree *suffixTree = 0;
		if (uppercase && wordFreq <= upperMaxFreq)
			suffixTree = d_upperSuffixTree.get();
		else if (!uppercase && wordFreq <= lowerMaxFreq)
			suffixTree = d_lowerSuffixTree.get();
		else
			continue;

		suffixTree->addWord(word, tagFreqs);
	}

	d_upperSuffixTree->calculateTheta();
	d_lowerSuffixTree->calculateTheta();
}

SuffixWordHandler::ProbSet SuffixWordHandler::tags(string const &word) const
{
	// Select a suffix trie, based on the capitalization of the first
	// letter.
	WordSuffixTree *suffixTree = 0;
	if (isupper(word[0]))
		suffixTree = d_upperSuffixTree.get();
	else
		suffixTree = d_lowerSuffixTree.get();

	unordered_map<size_t, double> tagProbs = suffixTree->suffixTagProbs(word);

	// Order resulting tags by decreasing probability.
	ProbSet allTagProbs;
	copy(tagProbs.begin(), tagProbs.end(), inserter(allTagProbs,
		allTagProbs.begin()));

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
