#include <algorithm>
#include <cmath>
#include <iterator>
#include <string>

#include <tr1/memory>

#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/wordhandler/KnownWordHandler.hh>

using namespace std;
using namespace std::tr1;
using namespace citar::tagger;

KnownWordHandler::KnownWordHandler(shared_ptr<Model const> model,
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
	d_lexicon.reset(new WordTagProbLexicon(*other.d_lexicon));
	d_fallbackWordHandler = other.d_fallbackWordHandler;
}

void KnownWordHandler::calcWordTagProbs(WordTagFreqs const &wordTagFreqs,
	UniGramFreqs const &uniGramFreqs)
{
	for (WordTagFreqs::const_iterator wordIter = wordTagFreqs.begin();
		wordIter != wordTagFreqs.end(); ++wordIter)
	{
		for (std::map<size_t, size_t>::const_iterator tagIter =
			wordIter->second.begin(); tagIter != wordIter->second.end();
			++tagIter)
		{
			// Calculate the maximum likelihood probability P(w|t).
			double p = log(static_cast<double>(tagIter->second) /
				uniGramFreqs.find(tagIter->first)->second);
			(*d_lexicon)[wordIter->first][tagIter->first] = p;
		}
	}
}

KnownWordHandler::ProbSet KnownWordHandler::tags(string const &word) const
{
	// Find the maximum likelyhood probabilities, and add them to a set
	// ordered by decreasing probability.
	WordTagProbLexicon::const_iterator match = d_lexicon->find(word);
	if (match != d_lexicon->end())
	{
		ProbSet result;
		std::copy(match->second.begin(), match->second.end(),
			inserter(result, result.begin()));
		return result;
	}
	// The word was not found, try once more with the lowercase variant.
	else if (isupper(word[0]))
	{
		string wordLower = static_cast<char>(tolower(word[0])) +
			word.substr(1);

		match = d_lexicon->find(wordLower);
		if (match != d_lexicon->end())
		{
			ProbSet result;
			std::copy(match->second.begin(), match->second.end(),
					inserter(result, result.begin()));
			return result;
		}
	}

	if (d_fallbackWordHandler)
		return d_fallbackWordHandler->tags(word);

	return ProbSet();
}

KnownWordHandler::~KnownWordHandler() {}

