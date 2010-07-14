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

#include "HMMTagger.ih"

QVector<QString> HMMTagger::tag(QVector<QString> const &sentence) const
{
	QVector<QVector<TagMatrixEntry> > tagMatrix(sentence.size(), QVector<TagMatrixEntry>());

	// We can't estimate the trigram probabilities for the first two tags,
	// so add them to the tag matrix as-is. Normally, these are start markers
	// anyway. XXX - maybe we should at the very least look them up in the
	// lexicon?
	size_t startTag = d_model->tagNumbers().find(sentence[0]).value();
	tagMatrix[0].push_back(TagMatrixEntry(startTag));
	tagMatrix[1].push_back(TagMatrixEntry(startTag));
	tagMatrix[1][0].probs[&tagMatrix[0][0]] = 0.0;
	tagMatrix[1][0].bps[&tagMatrix[0][0]] = 0;

	double beam = 0.0;

	// Loop through the tokens.
	for (int i = 2; i < sentence.size(); ++i)
	{
		double columnHighestProb = -numeric_limits<double>::infinity();
		WordHandler::ProbSet tagProbs = d_wordHandler->tags(sentence[i]);

		// Loop over all possible tags for the current word.
		for (WordHandler::ProbSet::const_iterator tagProbsIter = tagProbs.begin();
			tagProbsIter != tagProbs.end(); ++tagProbsIter)
		{
			TagMatrixEntry newEntry(tagProbsIter->first);

			// Loop over all possible trigrams.
			for (QVector<TagMatrixEntry>::const_iterator t2Iter =
				tagMatrix[i - 1].begin(); t2Iter != tagMatrix[i - 1].end();
				++t2Iter)
			{
				double highestProb = -numeric_limits<double>::infinity();
				TagMatrixEntry const *highestProbBp = 0;

				for (QHash<TagMatrixEntry const *, double>::const_iterator t1Iter =
					t2Iter->probs.begin(); t1Iter != t2Iter->probs.end(); ++t1Iter)
				{
					if (t1Iter.value() < beam)
						continue;

					TriGram curTriGram(t1Iter.key()->tag, t2Iter->tag, tagProbsIter->first);
					double triGramProb = d_smoothing->triGramProb(curTriGram);

					// The probability of the current state is P(w|t) * p(t3|t1,t2) *
					// p(prev_state).
					double prob = triGramProb + tagProbsIter->second + t1Iter.value();

					// Store the path the maximizes the probability.
					if (prob > highestProb)
					{
						highestProb = prob;
						//highestProbBp = t2Iter->bps.find(t1Iter->first)->second;
						highestProbBp = t1Iter.key();
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

	QVector<TagMatrixEntry> &lastColumn = tagMatrix[sentence.size() - 1];

	for (QVector<TagMatrixEntry>::const_iterator iter = lastColumn.begin();
			iter != lastColumn.end(); ++iter)
		for (QHash<TagMatrixEntry const *, double>::const_iterator probIter =
			iter->probs.begin(); probIter != iter->probs.end(); ++probIter)
		{
			if (probIter.value() > highestProb)
			{
				highestProb = probIter.value();
				tail = &(*iter);
				beforeTail = probIter.key();
			}
		}

	// Extract the most probable tag sequence.
	QVector<QString> tagSequence;
	for (int i = tagMatrix.size() - 1; i >= 0; --i)
	{
		QString tagString = d_model->numberTags().find(tail->tag).value();
		tagSequence.push_back(tagString);

		if (beforeTail)
		{
			TagMatrixEntry const *tmp = tail->bps.find(beforeTail).value();
			tail = beforeTail;
			beforeTail = tmp;
		}
	}

	// Since we have extracted the most probable tag sequence from tail to
	// head, we have to reverse it.
	reverse(tagSequence.begin(), tagSequence.end());

	return tagSequence;
}
