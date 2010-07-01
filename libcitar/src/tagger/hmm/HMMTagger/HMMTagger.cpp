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

#include "HMMTagger.ih"

vector<string> HMMTagger::tag(vector<string> const &sentence) const
{
	vector<vector<TagMatrixEntry> > tagMatrix(sentence.size(), vector<TagMatrixEntry>());

	// We can't estimate the trigram probabilities for the first two tags,
	// so add them to the tag matrix as-is. Normally, these are start markers
	// anyway. XXX - maybe we should at the very least look them up in the
	// lexicon?
	size_t startTag = d_model->tagNumbers().find(sentence[0])->second;
	tagMatrix[0].push_back(TagMatrixEntry(startTag));
	tagMatrix[1].push_back(TagMatrixEntry(startTag));
	tagMatrix[1][0].probs[&tagMatrix[0][0]] = 0.0;
	tagMatrix[1][0].bps[&tagMatrix[0][0]] = 0;

	double beam = 0.0;

	// Loop through the tokens.
	for (size_t i = 2; i < sentence.size(); ++i)
	{
		double columnHighestProb = -numeric_limits<double>::infinity();
		WordHandler::ProbSet tagProbs = d_wordHandler->tags(sentence[i]);

		// Loop over all possible tags for the current word.
		for (WordHandler::ProbSet::const_iterator tagProbsIter = tagProbs.begin();
			tagProbsIter != tagProbs.end(); ++tagProbsIter)
		{
			TagMatrixEntry newEntry(tagProbsIter->first);

			// Loop over all possible trigrams.
			for (vector<TagMatrixEntry>::const_iterator t2Iter =
				tagMatrix[i - 1].begin(); t2Iter != tagMatrix[i - 1].end();
				++t2Iter)
			{
				double highestProb = -numeric_limits<double>::infinity();
				TagMatrixEntry const *highestProbBp = 0;

				for (unordered_map<TagMatrixEntry const *, double>::const_iterator t1Iter =
					t2Iter->probs.begin(); t1Iter != t2Iter->probs.end(); ++t1Iter)
				{
					if (t1Iter->second < beam)
						continue;

					TriGram curTriGram(t1Iter->first->tag, t2Iter->tag, tagProbsIter->first);
					double triGramProb = d_smoothing->triGramProb(curTriGram);

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


			tagMatrix[i].push_back(newEntry);
		}

		beam = columnHighestProb - d_beamFactor;
	}

	// Find the most probable final state.
	double highestProb = -numeric_limits<double>::infinity();
	TagMatrixEntry const *tail = 0;
	TagMatrixEntry const *beforeTail = 0;

	vector<TagMatrixEntry> &lastColumn = tagMatrix[sentence.size() - 1];

	for (vector<TagMatrixEntry>::const_iterator iter = lastColumn.begin();
			iter != lastColumn.end(); ++iter)
		for (unordered_map<TagMatrixEntry const *, double>::const_iterator probIter =
			iter->probs.begin(); probIter != iter->probs.end(); ++probIter)
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
	for (int i = tagMatrix.size() - 1; i >= 0; --i)
	{
		string tagString = d_model->numberTags().find(tail->tag)->second;
		tagSequence.push_back(tagString);

		if (beforeTail)
		{
			TagMatrixEntry const *tmp = tail->bps.find(beforeTail)->second;
			tail = beforeTail;
			beforeTail = tmp;
		}
	}

	// Since we have extracted the most probable tag sequence from tail to
	// head, we have to reverse it.
	reverse(tagSequence.begin(), tagSequence.end());

	return tagSequence;
}
