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

#include <string>

#ifndef CITAR_TAGGEDWORD_HH
#define CITAR_TAGGEDWORD_HH

namespace citar {
namespace corpus {

struct TaggedWord {
	TaggedWord(std::string const &newWord, std::string const &newTag) :
		word(newWord), tag(newTag) {}
	std::string word;
	std::string tag;
};

}
}

#endif // CITAR_TAGGEDWORD_HH
