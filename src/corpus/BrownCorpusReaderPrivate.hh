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

#ifndef CITAR_BROWNCORPUSREADER_PRIVATE_HH
#define CITAR_BROWNCORPUSREADER_PRIVATE_HH

#include <iosfwd>
#include <string>
#include <vector>

#include <memory>

#include <citar/corpus/CorpusReader.hh>
#include <citar/corpus/TaggedWord.hh>

namespace citar {
namespace corpus {

class BrownCorpusReaderPrivate : public CorpusReader
{
public:
	BrownCorpusReaderPrivate(std::istream *is) : d_is(is) {}
	virtual boost::optional<std::vector<TaggedWord>> nextSentence();
private:
	std::vector<TaggedWord> parseLine(std::string const &line) const;

	std::istream *d_is;
};

}
}

#endif // CITAR_BROWNCORPUSREADER_PRIVATE_HH
