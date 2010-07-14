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
#include <sstream>

#include <QCoreApplication>
#include <QFile>
#include <QSharedPointer>
#include <QStringList>
#include <QTextStream>
#include <QVector>

#include <citar/corpus/TaggedWord.hh>
#include <citar/tagger/hmm/HMMTagger.hh>
#include <citar/tagger/hmm/LinearInterpolationSmoothing.hh>
#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/wordhandler/KnownWordHandler.hh>
#include <citar/tagger/wordhandler/SuffixWordHandler.hh>

using namespace std;
using namespace citar::corpus;
using namespace citar::tagger;

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	if (argc != 3)
	{
		cerr << "Syntax: " << argv[0] << " lexicon ngrams" << endl;
		return 1;
	}

	QFile lexiconFile(argv[1]);
	if (!lexiconFile.open(QFile::ReadOnly))
	{
		cerr << "Could not open lexicon: " << argv[1] << endl;
		return 1;
	}

	QTextStream lexiconStream(&lexiconFile);

	QFile nGramFile(argv[2]);
	if (!nGramFile.open(QFile::ReadOnly))
	{
		cerr << "Could not open ngrams: " << argv[2] << endl;
		return 1;
	}

	QTextStream nGramStream(&nGramFile);

	QSharedPointer<Model> model(Model::readModel(lexiconStream, nGramStream));

	SuffixWordHandler suffixWordHandler(model, 2, 2, 8);

	KnownWordHandler knownWordHandler(model, &suffixWordHandler);

	LinearInterpolationSmoothing smoothing(model);

	QSharedPointer<HMMTagger> hmmTagger(new HMMTagger(model,
		&knownWordHandler, &smoothing));

	QTextStream stdinStream(stdin);
	QTextStream stdoutStream(stdout);
	QString line;

	while (true)
	{
		line = stdinStream.readLine();
		if (line.isNull())
			break;

		QStringList splittedLine = line.split(" ", QString::SkipEmptyParts);

		// Get the sequence of words, adding start/end markers.
		QVector<QString> sentence(2, "<START>");
		copy(splittedLine.begin(), splittedLine.end(), back_inserter(sentence));
		sentence.push_back("<END>");

		// Tag it.
		QVector<QString> taggedWords = hmmTagger->tag(sentence);

		// Print word/tags.
		for (int i = 2; i < sentence.size() - 1; ++i)
			stdoutStream << sentence[i] << "/" << taggedWords[i] << " ";

		stdoutStream << endl;
	}
}
