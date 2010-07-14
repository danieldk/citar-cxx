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

#ifndef CITAR_CORPUSREADER_HH
#define CITAR_CORPUSREADER_HH

#include <QSharedPointer>
#include <QTextStream>
#include <QVector>

#include "SentenceHandler.hh"
#include "TaggedWord.hh"

namespace citar {
namespace corpus {

/**
 * Base class for corpus handlers. Derived classes should use the protected
 * <i>d_startMarkers</i> and <i>d_endMarkers</i> fields to add start/end
 * markers to sentences.
 */
class CorpusReader
{
public:
	CorpusReader (QVector<TaggedWord> const &startMarkers,
		QVector<TaggedWord> const &endMarkers,
		bool decapitalizeFirstWord = false) :
		d_startMarkers(startMarkers), d_endMarkers(endMarkers),
		d_decapitalizeFirstWord(decapitalizeFirstWord) {}

	/**
	 * Register a sentence handler class.
	 */
	void addSentenceHandler(QSharedPointer<SentenceHandler> sentenceHandler);

	/**
	 * Parse a corpus. The registered sentence handlers will be called to handle
	 * each parsed sentence.
	 */
	virtual void parse(QTextStream &in) = 0;

	virtual ~CorpusReader() {}
protected:
	QVector<QSharedPointer<SentenceHandler> > d_sentenceHandlers;
	QVector<TaggedWord> d_startMarkers;
	QVector<TaggedWord> d_endMarkers;
	bool d_decapitalizeFirstWord;
};

inline void CorpusReader::addSentenceHandler(QSharedPointer<SentenceHandler> sentenceHandler)
{
	d_sentenceHandlers.push_back(sentenceHandler);
}

}
}

#endif // CITAR_CORPUSREADER_HH
