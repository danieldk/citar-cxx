/*
 * Copyright 2008, 2016 Daniel de Kok
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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

#include <memory>
#include <unordered_map>

#include <citar/corpus/BrownCorpusReader.hh>
#include <citar/corpus/SentenceHandler.hh>
#include <citar/tagger/hmm/Model.hh>
#include <citar/util/Markers.hh>

using namespace std;
using namespace citar;
using namespace citar::corpus;
using namespace citar::tagger;

typedef std::unordered_map<string, map<string, size_t> > Lexicon;

class Trainer
{
public:
	Trainer() : d_lexicon(new Lexicon),
		d_uniGrams(new unordered_map<string, size_t>),
		d_biGrams(new unordered_map<string, size_t>),
		d_triGrams(new unordered_map<string, size_t>) {}
	void handleSentence(vector<TaggedWord> sentence);
	Lexicon const &lexicon();
	unordered_map<string, size_t> const &biGrams();
	unordered_map<string, size_t> const &triGrams();
	unordered_map<string, size_t> const &uniGrams();
public:
	unique_ptr<Lexicon> d_lexicon;
	unique_ptr<unordered_map<string, size_t> > d_uniGrams;
	unique_ptr<unordered_map<string, size_t> > d_biGrams;
	unique_ptr<unordered_map<string, size_t> > d_triGrams;
};

void Trainer::handleSentence(vector<TaggedWord> sentence)
{
	sentence.insert(sentence.begin(), citar::START_MARKERS_TAGGED.begin(), citar::START_MARKERS_TAGGED.end());
	sentence.insert(sentence.end(), citar::END_MARKERS_TAGGED.begin(), citar::END_MARKERS_TAGGED.end());

	for (auto iter = sentence.begin(); iter != sentence.end(); ++iter)
	{
		++(*d_uniGrams)[iter->tag];

		size_t beginDistance = distance(sentence.begin(), iter);

		if (beginDistance > 0)
			++(*d_biGrams)[(iter - 1)->tag + " " + iter->tag];

		if (beginDistance > 1)
			++(*d_triGrams)[(iter - 2)->tag + " " + (iter - 1)->tag + " " + iter->tag];

		++(*d_lexicon)[iter->word][iter->tag];
	}
}

inline Lexicon const &Trainer::lexicon()
{
	return *d_lexicon;
}

inline unordered_map<string, size_t> const &Trainer::biGrams()
{
	return *d_biGrams;
}

inline unordered_map<string, size_t> const &Trainer::triGrams()
{
	return *d_triGrams;
}

inline unordered_map<string, size_t> const &Trainer::uniGrams()
{
	return *d_uniGrams;
}

void writeLexicon(ostream &out, Lexicon const &lexicon)
{
	for (auto iter = lexicon.begin(); iter != lexicon.end(); ++iter)
	{
		out << iter->first;

		for (auto tagIter = iter->second.begin(); tagIter != iter->second.end();
        ++tagIter)
		{
			out << " " << tagIter->first << " " << tagIter->second;
		}

		out << endl;
	}
}

void writeNGrams(ostream &out,
		unordered_map<string, size_t> const &uniGrams,
		unordered_map<string, size_t> const &biGrams,
		unordered_map<string, size_t> const &triGrams)
{
	for (auto iter = uniGrams.begin(); iter != uniGrams.end(); ++iter)
		out << iter->first << " " << iter->second << endl;

	for (auto iter = biGrams.begin(); iter != biGrams.end(); ++iter)
	{
		string biGram = iter->first;
		out << biGram << " " << iter->second << endl;
	}

	for (auto iter = triGrams.begin(); iter != triGrams.end(); ++iter)
	{
		string triGram = iter->first;
		out << triGram << " " << iter->second << endl;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		cerr << "Usage: " << argv[0] << " corpus lexicon ngrams" << endl;
		return 1;
	}

	vector<TaggedWord> startTags(2, TaggedWord("<START>", "<START>"));
	vector<TaggedWord> endTags(1, TaggedWord("<END>", "<END>"));

	ifstream corpusStream(argv[1]);
	if (!corpusStream.good())
	{
		cerr << "Could not open corpus for reading: " << argv[1] << endl;
		return 1;
	}

	BrownCorpusReader brownCorpusReader(&corpusStream);

	Trainer trainHandler;

	while (true) {
		auto sentence = brownCorpusReader.nextSentence();
		if (!sentence)
			break;

		trainHandler.handleSentence(sentence.value());
	}

	ofstream lexiconStream(argv[2]);
	if (!lexiconStream.good())
	{
		cerr << "Could not open lexicon for writing: " << argv[2] << endl;
		return 1;
	}

	ofstream ngramStream(argv[3]);
	if (!ngramStream.good())
	{
		cerr << "Could not open ngram list for writing: " << argv[3] << endl;
		return 1;
	}


	writeLexicon(lexiconStream, trainHandler.lexicon());
	writeNGrams(ngramStream, trainHandler.uniGrams(), trainHandler.biGrams(),
		trainHandler.triGrams());
}
