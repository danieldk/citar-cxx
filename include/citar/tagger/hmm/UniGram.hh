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

#ifndef LANGKIT_TAGGER_UNIGRAM
#define LANGKIT_TAGGER_UNIGRAM

#include <functional>
#include <string>

#include <functional>
#include <unordered_map>

namespace citar {
namespace tagger {

struct UniGram
{
	size_t t1;
	UniGram(size_t newT1) : t1(newT1) {}
};

inline bool operator==(UniGram const &x, UniGram const &y)
{
	return x.t1 == y.t1;
}

inline bool operator<(UniGram const &x, UniGram const &y)
{
	return x.t1 < y.t1;
}

struct UniGramHash : public std::unary_function<UniGram, std::size_t>
{
	std::size_t operator()(UniGram const &uniGram) const
	{
		return std::hash<size_t>()(uniGram.t1);
	}
};

typedef std::unordered_map<UniGram, size_t, UniGramHash> UniGramFreqs;

}
}

#endif // LANGKIT_TAGGER_UNIGRAM
