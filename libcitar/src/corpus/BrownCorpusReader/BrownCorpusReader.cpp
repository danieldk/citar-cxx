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

#include "BrownCorpusReader.ih"

void BrownCorpusReader::parse(QTextStream &in)
{
	QString line;
	while (true) {
		line = in.readLine();

		if (line.isNull())
			break;

		if (line.isEmpty())
			continue;

		QVector<TaggedWord> sentence = parseLine(line);
		if (d_decapitalizeFirstWord)
			sentence[0].word[0] = sentence[0].word[0].toLower();

		// Build a sentence with markers.
		QVector<TaggedWord> markerSentence(d_startMarkers);
		copy(sentence.begin(), sentence.end(), back_inserter(markerSentence));
		copy(d_endMarkers.begin(), d_endMarkers.end(), back_inserter(markerSentence));

		// Pass the sentence to all handlers.
		for (QVector<QSharedPointer<SentenceHandler> >::iterator iter = d_sentenceHandlers.begin();
				iter != d_sentenceHandlers.end(); ++iter)
			(*iter)->handleSentence(markerSentence);
	}
}

QVector<TaggedWord> BrownCorpusReader::parseLine(QString const &line) const
{
	QVector<TaggedWord> sentence;

	QStringList lineParts = line.split(" ", QString::SkipEmptyParts);
	for (QStringList::const_iterator iter = lineParts.begin();
		iter != lineParts.end(); ++iter)
	{
		QString wordTag = *iter;

		// Find the word/tag delimiter within the word/tag combination.
		// Characters before the separator denote the word, characters
		// after the separator denote the tag.
		int sepIndex = wordTag.lastIndexOf("/");
		TaggedWord taggedWord(wordTag.left(sepIndex),
			wordTag.mid(sepIndex + 1));
		sentence.push_back(taggedWord);
	}

	return sentence;
}
