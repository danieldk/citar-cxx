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

#include <ctype.h>
#include <fstream>
#include <iostream>

#include <QCoreApplication>
#include <QFile>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>
#include <QVector>

#include <citar/corpus/BrownCorpusReader.hh>
#include <citar/corpus/SentenceHandler.hh>
#include <citar/corpus/TaggedWord.hh>
#include <citar/tagger/hmm/HMMTagger.hh>
#include <citar/tagger/hmm/LinearInterpolationSmoothing.hh>
#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/wordhandler/KnownWordHandler.hh>
#include <citar/tagger/wordhandler/SuffixWordHandler.hh>

using namespace std;
using namespace citar::corpus;
using namespace citar::tagger;

class EvaluateHandler : public SentenceHandler
{
public:
	EvaluateHandler(QSharedPointer<Model> model,
		QSharedPointer<HMMTagger> hmmTagger) :
		d_model(model), d_hmmTagger(hmmTagger),
		d_knownGood(0), d_knownBad(0), d_unknownGood(0), d_unknownBad(0) {}
	void handleSentence(QVector<TaggedWord> const &sentence);
	size_t knownGood();
	size_t knownBad();
	size_t unknownGood();
	size_t unknownBad();
private:
	QSharedPointer<Model> d_model;
	QSharedPointer<HMMTagger> d_hmmTagger;
	size_t d_knownGood;
	size_t d_knownBad;
	size_t d_unknownGood;
	size_t d_unknownBad;
};

void EvaluateHandler::handleSentence(QVector<TaggedWord> const &sentence)
{
	QVector<QString> words;
	QVector<QString> goldTags;

	for (QVector<TaggedWord>::const_iterator wordIter = sentence.begin();
		wordIter != sentence.end(); ++wordIter)
	{
		words.push_back(wordIter->word);
		goldTags.push_back(wordIter->tag);
	}

	QVector<QString> tags = d_hmmTagger->tag(words);

	for (int i = 2 ; i < tags.size() - 1; ++i)
	{
		bool wordInLexicon = false;
		if (d_model->lexicon().find(words[i]) != d_model->lexicon().end() ||
				d_model->lexicon().find(words[i].toLower()) != d_model->lexicon().end())
			wordInLexicon = true;
		else
		{
			QString wordLower = words[i];
			wordLower[0].toLower();
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
	QCoreApplication app(argc, argv);

	if (argc != 4)
	{
		cerr << "Syntax: " << argv[0] << " lexicon ngrams corpus" << endl;
		return 1;
	}

	QFile nGramFile(argv[2]);
	if (!nGramFile.open(QFile::ReadOnly))
	{
		cerr << "Could not open ngrams: " << argv[2] << endl;
		return 1;
	}

	QTextStream nGramStream(&nGramFile);

	QFile lexiconFile(argv[1]);
	if (!lexiconFile.open(QFile::ReadOnly))
	{
		cerr << "Could not open lexicon: " << argv[1] << endl;
		return 1;
	}

	QTextStream lexiconStream(&lexiconFile);

	QSharedPointer<Model> model(Model::readModel(lexiconStream, nGramStream));

	SuffixWordHandler suffixWordHandler(model, 2, 2, 8);

	KnownWordHandler knownWordHandler(model, &suffixWordHandler);

	LinearInterpolationSmoothing smoothing(model);

	QSharedPointer<HMMTagger> hmmTagger(new HMMTagger(model,
		&knownWordHandler, &smoothing));

	QFile corpusFile(argv[3]);
	if (!corpusFile.open(QFile::ReadOnly))
	{
		cerr << "Could not open corpus: " << argv[3] << endl;
		return 1;
	}

	QTextStream corpusStream(&corpusFile);

	QVector<TaggedWord> startTags(2, TaggedWord("<START>", "<START>"));
	QVector<TaggedWord> endTags(1, TaggedWord("<END>", "<END>"));
	BrownCorpusReader brownCorpusReader(startTags, endTags);

	QSharedPointer<EvaluateHandler> evalHandler(new EvaluateHandler(model, hmmTagger));
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
