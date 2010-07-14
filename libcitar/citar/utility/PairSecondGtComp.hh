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

#ifndef LANGKIT_UTILITY_PAIRSECONDGTCOMP_HH
#define LANGKIT_UTILITY_PAIRSECONDGTCOMP_HH

namespace citar {
namespace utility {

// This function object class compares two pairs, and returns true when
// the second member of the first pair is greater than the second member
// of the second pair. This can be used for descending ordering on the
// second member.
template <typename T>
struct PairSecondGtComp
{
	bool operator()(T const &p1, T const &p2);
};

template <typename T>
bool PairSecondGtComp<T>::operator()(T const &p1, T const &p2)
{
	if (p1.second != p2.second)
		return p1.second > p2.second;
	else
		return p1.first < p2.first;
}

}
}

#endif // LANGKIT_UTILITY_PAIRSECONDGTCOMP_HH

