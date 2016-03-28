#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#include <unordered_map>

#include <citar/tagger/wordhandler/WordHandler.hh>
#include <citar/util/Markers.hh>

#include "HMMTaggerPrivate.hh"
#include "TrellisEntry.hh"


using namespace std;
using namespace citar::tagger;

vector<string> HMMTaggerPrivate::tag(vector<string> sentence) const
{
	sentence.insert(sentence.begin(), citar::START_MARKERS.begin(), citar::START_MARKERS.end());
	sentence.insert(sentence.end(), citar::END_MARKERS.begin(), citar::END_MARKERS.end());

	vector<vector<TrellisEntry> > trellis(sentence.size(), vector<TrellisEntry>());

	// We can't estimate the trigram probabilities for the first two tags,
	// so add them to the trellis as-is. Normally, these are start markers
	// anyway. XXX - maybe we should at the very least look them up in the
	// lexicon?
	size_t startTag = d_model->tagNumbers().find(sentence[0])->second;
	trellis[0].push_back(TrellisEntry(startTag));
	trellis[1].push_back(TrellisEntry(startTag));
	trellis[1][0].probs[&trellis[0][0]] = 0.0;
	trellis[1][0].bps[&trellis[0][0]] = 0;

	double beam = 0.0;

	// Loop through the tokens.
	for (size_t i = 2; i < sentence.size(); ++i)
	{
		double columnHighestProb = -numeric_limits<double>::infinity();
		WordHandler::ProbSet tagProbs = d_wordHandler->tags(sentence[i]);

		// Loop over all possible tags for the current word.
		for (auto tagProbsIter = tagProbs.begin(); tagProbsIter != tagProbs.end();
        ++tagProbsIter)
		{
			TrellisEntry newEntry(tagProbsIter->first);

			// Loop over all possible trigrams.
			for (auto t2Iter = trellis[i - 1].begin();
          t2Iter != trellis[i - 1].end(); ++t2Iter)
			{
				double highestProb = -numeric_limits<double>::infinity();
				TrellisEntry const *highestProbBp = 0;

				for (auto t1Iter = t2Iter->probs.begin(); t1Iter != t2Iter->probs.end();
            ++t1Iter)
				{
					if (t1Iter->second < beam)
						continue;

					TriGram curTriGram(t1Iter->first->tag, t2Iter->tag, tagProbsIter->first);
					double triGramProb = d_languageModel->triGramProb(curTriGram);

					// The probability of the current state is P(w|t) * p(t3|t1,t2) *
					// p(prev_state).
					double prob = triGramProb + tagProbsIter->second + t1Iter->second;

					// Store the path the maximizes the probability.
					if (prob > highestProb)
					{
						highestProb = prob;
						//highestProbBp = t2Iter->bps.find(t1Iter->first)->second;
						highestProbBp = t1Iter->first;
					}
				}

				newEntry.probs[&(*t2Iter)] = highestProb;
				newEntry.bps[&(*t2Iter)] = highestProbBp;

				if (highestProb > columnHighestProb)
					columnHighestProb = highestProb;
			}


			trellis[i].push_back(newEntry);
		}

		beam = columnHighestProb - d_beamFactor;
	}

	// Find the most probable final state.
	double highestProb = -numeric_limits<double>::infinity();
	TrellisEntry const *tail = 0;
	TrellisEntry const *beforeTail = 0;

	vector<TrellisEntry> &lastColumn = trellis[sentence.size() - 1];

	for (auto iter = lastColumn.begin(); iter != lastColumn.end(); ++iter)
		for (auto probIter = iter->probs.begin(); probIter != iter->probs.end();
        ++probIter)
		{
			if (probIter->second > highestProb)
			{
				highestProb = probIter->second;
				tail = &(*iter);
				beforeTail = probIter->first;
			}
		}

	// Extract the most probable tag sequence.
	vector<string> tagSequence;
	for (int i = trellis.size() - 1; i >= 0; --i)
	{
		string tagString = d_model->numberTags().find(tail->tag)->second;
		tagSequence.push_back(tagString);

		if (beforeTail)
		{
			TrellisEntry const *tmp = tail->bps.find(beforeTail)->second;
			tail = beforeTail;
			beforeTail = tmp;
		}
	}

	// Since we have extracted the most probable tag sequence from tail to
	// head, we have to reverse it.
	reverse(tagSequence.begin(), tagSequence.end());

	// And remove the start/end markers.
	tagSequence.erase(tagSequence.begin(), tagSequence.begin() + citar::START_MARKERS.size());
	tagSequence.erase(tagSequence.end() - citar::END_MARKERS.size(), tagSequence.end());

	return tagSequence;
}
