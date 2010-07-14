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

#ifndef LANGKIT_TAGGER_BIGRAM
#define LANGKIT_TAGGER_BIGRAM

#include <QHash>

namespace citar {
namespace tagger {

struct BiGram
{
	size_t t1;
	size_t t2;
	BiGram(size_t newT1, size_t newT2) :
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

inline uint qHash(BiGram const &biGram)
{
	uint seed = biGram.t1;
	seed ^= biGram.t2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

typedef QHash<BiGram, size_t> BiGramFreqs;
}
}

#endif // LANGKIT_TAGGER_BIGRAM
