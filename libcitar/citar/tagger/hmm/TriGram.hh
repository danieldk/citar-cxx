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

#ifndef LANGKIT_TAGGER_TRIGRAM
#define LANGKIT_TAGGER_TRIGRAM

#include <functional>
#include <string>

#include <tr1/functional>
#include <tr1/unordered_map>

namespace citar {
namespace tagger {

struct TriGram
{
	size_t t1;
	size_t t2;
	size_t t3;
	TriGram(size_t newT1, size_t newT2, size_t newT3)
		: t1(newT1), t2(newT2), t3(newT3) {}
};

inline bool operator==(TriGram const &x, TriGram const &y)
{
	return x.t1 == y.t1 && x.t2 == y.t2 && x.t3 == y.t3;
}

inline bool operator<(TriGram const &x, TriGram const &y)
{
	if (x.t1 != y.t1)
		return x.t1 < y.t1;
	else if (x.t2 != y.t2)
		return x.t2 < y.t2;
	else
		return x.t3 < y.t3;
}

struct TriGramHash : public std::unary_function<TriGram, std::size_t>
{
	std::size_t operator()(TriGram const &triGram) const
	{
		std::tr1::hash<size_t> numHash;
		int seed = numHash(triGram.t1);
		seed ^= numHash(triGram.t2) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= numHash(triGram.t3) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

		return seed;
	}
};

typedef std::tr1::unordered_map<TriGram, size_t, TriGramHash> TriGramFreqs;
}
}

#endif // LANGKIT_TAGGER_TRIGRAM
