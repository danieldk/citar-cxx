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

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>

#include <QCoreApplication>
#include <QFile>
#include <QHash>
#include <QSharedPointer>
#include <QString>
#include <QTextCodec>
#include <QTextStream>
#include <QVector>

#include <citar/corpus/BrownCorpusReader.hh>
#include <citar/corpus/SentenceHandler.hh>
#include <citar/tagger/hmm/Model.hh>

using namespace std;
using namespace citar::corpus;
using namespace citar::tagger;

typedef QHash<QString, map<QString, size_t> > Lexicon;

class TrainHandler : public SentenceHandler
{
public:
	TrainHandler() : d_lexicon(new Lexicon),
		d_uniGrams(new QHash<QString, size_t>),
		d_biGrams(new QHash<QString, size_t>),
		d_triGrams(new QHash<QString, size_t>) {}
	void handleSentence(QVector<TaggedWord> const &sentence);
	Lexicon const &lexicon();
	QHash<QString, size_t> const &biGrams();
	QHash<QString, size_t> const &triGrams();
	QHash<QString, size_t> const &uniGrams();
public:
	QSharedPointer<Lexicon> d_lexicon;
	QSharedPointer<QHash<QString, size_t> > d_uniGrams;
	QSharedPointer<QHash<QString, size_t> > d_biGrams;
	QSharedPointer<QHash<QString, size_t> > d_triGrams;
};

void TrainHandler::handleSentence(QVector<TaggedWord> const &sentence)
{
	for (QVector<TaggedWord>::const_iterator iter = sentence.begin();
		iter != sentence.end(); ++iter)
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

inline Lexicon const &TrainHandler::lexicon()
{
	return *d_lexicon;
}

inline QHash<QString, size_t> const &TrainHandler::biGrams()
{
	return *d_biGrams;
}

inline QHash<QString, size_t> const &TrainHandler::triGrams()
{
	return *d_triGrams;
}

inline QHash<QString, size_t> const &TrainHandler::uniGrams()
{
	return *d_uniGrams;
}

void writeLexicon(QTextStream &out, Lexicon const &lexicon)
{
	for (Lexicon::const_iterator iter = lexicon.begin();
		iter != lexicon.end(); ++iter)
	{
		out << iter.key();

		for (map<QString, size_t>::const_iterator tagIter = iter.value().begin();
			tagIter != iter.value().end(); ++tagIter)
		{
			out << " " << tagIter->first << " " << tagIter->second;
		}

		out << endl;
	}
}

void writeNGrams(QTextStream &out,
		QHash<QString, size_t> const &uniGrams,
		QHash<QString, size_t> const &biGrams,
		QHash<QString, size_t> const &triGrams)
{
	for (QHash<QString, size_t>::const_iterator iter = uniGrams.begin();
			iter != uniGrams.end(); ++iter)
		out << iter.key() << " " << iter.value() << endl;

	for (QHash<QString, size_t>::const_iterator iter = biGrams.begin();
			iter != biGrams.end(); ++iter)
	{
		QString biGram = iter.key();
		out << biGram << " " << iter.value() << endl;
	}

	for (QHash<QString, size_t>::const_iterator iter = triGrams.begin();
			iter != triGrams.end(); ++iter)
	{
		QString triGram = iter.key();
		out << triGram << " " << iter.value() << endl;
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	if (argc != 4)
	{
		cerr << "Usage: " << argv[0] << " corpus lexicon ngrams" << endl;
		return 1;
	}

	QVector<TaggedWord> startTags(2, TaggedWord("<START>", "<START>"));
	QVector<TaggedWord> endTags(1, TaggedWord("<END>", "<END>"));
	BrownCorpusReader brownCorpusReader(startTags, endTags, true);

	QSharedPointer<TrainHandler> trainHandler(new TrainHandler);
	brownCorpusReader.addSentenceHandler(trainHandler);

	QFile corpusFile(argv[1]);
	if (!corpusFile.open(QFile::ReadOnly))
	{
		cerr << "Could not open corpus for reading: " << argv[1] << endl;
		return 1;
	}
	QTextStream corpusStream(&corpusFile);

	brownCorpusReader.parse(corpusStream);

	QFile lexiconFile(argv[2]);
	if (!lexiconFile.open(QFile::WriteOnly))
	{
		cerr << "Could not open lexicon for writing: " << argv[2] << endl;
		return 1;
	}
	QTextStream lexiconStream(&lexiconFile);

	writeLexicon(lexiconStream, trainHandler->lexicon());

	QFile ngramFile(argv[3]);
	if (!ngramFile.open(QFile::WriteOnly))
	{
		cerr << "Could not open ngram list for writing: " << argv[3] << endl;
		return 1;
	}
	QTextStream ngramStream(&ngramFile);

	writeNGrams(ngramStream, trainHandler->uniGrams(), trainHandler->biGrams(),
		trainHandler->triGrams());
}
