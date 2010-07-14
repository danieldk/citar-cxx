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

#ifndef CITAR_BROWNCORPUSREADER_HH
#define CITAR_BROWNCORPUSREADER_HH

#include <QString>
#include <QTextStream>
#include <QVector>

#include "CorpusReader.hh"
#include "TaggedWord.hh"

namespace citar {
namespace corpus {

class BrownCorpusReader : public CorpusReader
{
public:
	BrownCorpusReader(QVector<TaggedWord> const &startMarkers,
		QVector<TaggedWord> const &endMarkers,
		bool decapitalizeFirstWord = false) :
		CorpusReader(startMarkers, endMarkers, decapitalizeFirstWord) {}
	void parse(QTextStream &in);
private:
	BrownCorpusReader(BrownCorpusReader const &other);
	BrownCorpusReader &operator=(BrownCorpusReader const &other);
	QVector<TaggedWord> parseLine(QString const &line) const;
};

}
}

#endif // CITAR_BROWNCORPUSREADER_HH
