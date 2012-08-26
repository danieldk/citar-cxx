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

#ifndef LANGKIT_TAGGER_BIGRAM
#define LANGKIT_TAGGER_BIGRAM

#include <functional>
#include <string>

#include <tr1/functional>
#include <tr1/unordered_map>

namespace citar {
namespace tagger {

struct BiGram
{
	size_t t1;
	size_t t2;
	BiGram(size_t const &newT1, size_t const &newT2) :
		t1(newT1), t2(newT2) {}
};

inline bool operator<(BiGram const &x, BiGram const &y)
{
	if (x.t1 != y.t1)
		return x.t1 < y.t1;
	else
		return x.t2 < y.t2;
}

inline bool operator==(BiGram const &x, BiGram const &y)
{
	return x.t1 == y.t1 && x.t2 == y.t2;
}

struct BiGramHash : public std::unary_function<BiGram, std::size_t>
{
	std::size_t operator()(BiGram const &biGram) const
	{
		std::tr1::hash<size_t> numHash;
		int seed = numHash(biGram.t1);
		seed ^= numHash(biGram.t2) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

		return seed;
	}
};

typedef std::tr1::unordered_map<BiGram, size_t, BiGramHash> BiGramFreqs;
}
}

#endif // LANGKIT_TAGGER_BIGRAM
