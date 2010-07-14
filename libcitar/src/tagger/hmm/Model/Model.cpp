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

#include "Model.ih"

QSharedPointer<WordTagFreqs> Model::readLexicon(
	QTextStream &lexiconStream,
	QHash<QString, size_t> const &tagNumbers)
{
	QSharedPointer<WordTagFreqs> lexicon(new WordTagFreqs);

	QString line;
	while (true) {
		QString line = lexiconStream.readLine();

		if (line.isNull())
		    break;

		QStringList lineItems = line.split(" ");

		QString word = lineItems[0];

		for (int i = 1; i < lineItems.size(); i += 2) {
			size_t freq = lineItems[i + 1].toUInt();
			size_t tag = tagNumbers.find(lineItems[i]).value();
			(*lexicon)[word][tag] = freq;
		}
	}

	return lexicon;
}

QSharedPointer<Model> Model::readModel(QTextStream &lexiconStream, QTextStream &nGramStream)
{
	QSharedPointer<NGrams> nGrams = readNGrams(nGramStream);
	QSharedPointer<WordTagFreqs> lexicon =
		readLexicon(lexiconStream, nGrams->tagNumbers);

	QSharedPointer<Model> model(new Model(lexicon, nGrams));

	return model;
}

QSharedPointer<NGrams> Model::readNGrams(QTextStream &lexiconStream)
{
	QSharedPointer<NGrams> nGrams(new NGrams);

	size_t tagNumber = 0;
	QString line;
	while (true) {
		line = lexiconStream.readLine();

		if (line.isNull())
			break;

		// Read line items.
		QStringList lineItems = line.split(" ");

		// Get frequency.
		size_t freq = lineItems[lineItems.size() - 1].toUInt();

		// Get uni-/bi-/trigram
		if (lineItems.size() == 2)
		{
			nGrams->tagNumbers[lineItems[0]] = tagNumber;
			nGrams->numberTags[tagNumber] = lineItems[0];
			nGrams->uniGrams[UniGram(tagNumber)] += freq;
			++tagNumber;
		}
		else if (lineItems.size() == 3) {
			BiGram biGram(nGrams->tagNumbers[lineItems[0]],
				nGrams->tagNumbers[lineItems[1]]);
			nGrams->biGrams[biGram] += freq;
		}
		else if (lineItems.size() == 4)
			nGrams->triGrams[TriGram(nGrams->tagNumbers[lineItems[0]],
				nGrams->tagNumbers[lineItems[1]],
				nGrams->tagNumbers[lineItems[2]])] +=
				freq;
		else
			cout << "Encountered a line which does not contain a uni/bi/trigram!" << endl;
	}

	return nGrams;
}
