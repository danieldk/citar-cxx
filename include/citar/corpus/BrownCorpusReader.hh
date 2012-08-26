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

#ifndef CITAR_BROWNCORPUSREADER_HH
#define CITAR_BROWNCORPUSREADER_HH

#include <istream>
#include <string>
#include <vector>

#include "CorpusReader.hh"
#include "TaggedWord.hh"

namespace citar {
namespace corpus {

class BrownCorpusReader : public CorpusReader
{
public:
	BrownCorpusReader(std::vector<TaggedWord> const &startMarkers,
		std::vector<TaggedWord> const &endMarkers,
		bool decapitalizeFirstWord = false) :
		CorpusReader(startMarkers, endMarkers, decapitalizeFirstWord) {}
	void parse(std::istream &in);
private:
	BrownCorpusReader(BrownCorpusReader const &other);
	BrownCorpusReader &operator=(BrownCorpusReader const &other);
	std::vector<TaggedWord> parseLine(std::string const &line) const;
};

}
}

#endif // CITAR_BROWNCORPUSREADER_HH
