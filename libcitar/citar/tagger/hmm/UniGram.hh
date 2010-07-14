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

#ifndef CITAR_TAGGER_UNIGRAM
#define CITAR_TAGGER_UNIGRAM

#include <QHash>

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

inline uint qHash(UniGram const &uniGram)
{
	return uniGram.t1;
}

typedef QHash<UniGram, size_t> UniGramFreqs;

}
}

#endif // CITAR_TAGGER_UNIGRAM
