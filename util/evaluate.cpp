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

#include <ctype.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <tr1/memory>

#include <citar/corpus/BrownCorpusReader.hh>
#include <citar/corpus/SentenceHandler.hh>
#include <citar/corpus/TaggedWord.hh>
#include <citar/tagger/hmm/HMMTagger.hh>
#include <citar/tagger/hmm/LinearInterpolationSmoothing.hh>
#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/wordhandler/KnownWordHandler.hh>
#include <citar/tagger/wordhandler/SuffixWordHandler.hh>

using namespace std;
using namespace std::tr1;
using namespace citar::corpus;
using namespace citar::tagger;

class EvaluateHandler : public SentenceHandler
{
public:
	EvaluateHandler(shared_ptr<Model> model,
		shared_ptr<HMMTagger> hmmTagger) :
		d_model(model), d_hmmTagger(hmmTagger),
		d_knownGood(0), d_knownBad(0), d_unknownGood(0), d_unknownBad(0) {}
	void handleSentence(std::vector<TaggedWord> const &sentence);
	size_t knownGood();
	size_t knownBad();
	size_t unknownGood();
	size_t unknownBad();
private:
	shared_ptr<Model> d_model;
	shared_ptr<HMMTagger> d_hmmTagger;
	size_t d_knownGood;
	size_t d_knownBad;
	size_t d_unknownGood;
	size_t d_unknownBad;
};

void EvaluateHandler::handleSentence(std::vector<TaggedWord> const &sentence)
{
	vector<string> words;
	vector<string> goldTags;

	for (vector<TaggedWord>::const_iterator wordIter = sentence.begin();
		wordIter != sentence.end(); ++wordIter)
	{
		words.push_back(wordIter->word);
		goldTags.push_back(wordIter->tag);
	}

	vector<string> tags = d_hmmTagger->tag(words);

	for (size_t i = 2 ; i < tags.size() - 1; ++i)
	{
		bool wordInLexicon = false;
		if (d_model->lexicon().find(words[i]) != d_model->lexicon().end())
			wordInLexicon = true;
		else
		{
			string wordLower = static_cast<char>(tolower(words[i][0])) +
				words[i].substr(1);
			if (d_model->lexicon().find(wordLower) != d_model->lexicon().end())
				wordInLexicon = true;
		}

		if (tags[i] == goldTags[i])
		{
			if (wordInLexicon)
				++d_knownGood;
			else
				++d_unknownGood;
		}
		else
		{
			if (wordInLexicon)
				++d_knownBad;
			else
				++d_unknownBad;
		}
	}
}

inline size_t EvaluateHandler::knownGood()
{
	return d_knownGood;
}

inline size_t EvaluateHandler::knownBad()
{
	return d_knownBad;
}

inline size_t EvaluateHandler::unknownGood()
{
	return d_unknownGood;
}

inline size_t EvaluateHandler::unknownBad()
{
	return d_unknownBad;
}

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		cerr << "Syntax: " << argv[0] << " lexicon ngrams corpus" << endl;
		return 1;
	}

	ifstream nGramStream(argv[2]);
	if (!nGramStream.good())
	{
		cerr << "Could not open ngrams: " << argv[2] << endl;
		return 1;
	}

	ifstream lexiconStream(argv[1]);
	if (!lexiconStream.good())
	{
		cerr << "Could not open lexicon: " << argv[1] << endl;
		return 1;
	}

	shared_ptr<Model> model(Model::readModel(lexiconStream, nGramStream));

	SuffixWordHandler suffixWordHandler(model, 2, 2, 8);

	KnownWordHandler knownWordHandler(model, &suffixWordHandler);

	LinearInterpolationSmoothing smoothing(model);

	shared_ptr<HMMTagger> hmmTagger(new HMMTagger(model,
		&knownWordHandler, &smoothing));

	ifstream corpusStream(argv[3]);
	if (!corpusStream.good())
	{
		cerr << "Could not open corpus: " << argv[3] << endl;
		return 1;
	}

	vector<TaggedWord> startTags(2, TaggedWord("<START>", "<START>"));
	vector<TaggedWord> endTags(1, TaggedWord("<END>", "<END>"));
	BrownCorpusReader brownCorpusReader(startTags, endTags);

	shared_ptr<EvaluateHandler> evalHandler(new EvaluateHandler(model, hmmTagger));
	brownCorpusReader.addSentenceHandler(evalHandler);

	brownCorpusReader.parse(corpusStream);

	double knownAccuracy = evalHandler->knownGood() /
		static_cast<double>(evalHandler->knownGood() + evalHandler->knownBad());
	double unknownAccuracy = evalHandler->unknownGood() /
		static_cast<double>(evalHandler->unknownGood() + evalHandler->unknownBad());
	double overallAccuracy = (evalHandler->knownGood() + evalHandler->unknownGood()) /
		static_cast<float>(evalHandler->knownGood() + evalHandler->knownBad() + evalHandler->unknownGood() + evalHandler->unknownBad());

	cout << "Accuracy (known): " <<  knownAccuracy << endl;
	cout << "Accuracy (unknown): " << unknownAccuracy << endl;
	cout << "Accuracy (overall): " <<  overallAccuracy << endl;
}
