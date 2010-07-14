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

#ifndef LANGKIT_TAGGER_TRIGRAM
#define LANGKIT_TAGGER_TRIGRAM

#include <QHash>

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

inline uint qHash(TriGram const &triGram)
{
	uint seed = triGram.t1;
	seed ^= triGram.t2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	seed ^= triGram.t3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

typedef QHash<TriGram, size_t> TriGramFreqs;
}
}

#endif // LANGKIT_TAGGER_TRIGRAM
