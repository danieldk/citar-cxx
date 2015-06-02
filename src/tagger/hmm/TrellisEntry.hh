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

#ifndef CITAR_TRELLIS_ENTRY
#define CITAR_TRELLIS_ENTRY

#include <string>

#include <unordered_map>

namespace citar {
namespace tagger {

struct TrellisEntry {
	TrellisEntry(size_t const &newTag) : tag(newTag) {}
	size_t tag;
	std::unordered_map<TrellisEntry const *, double> probs;
	std::unordered_map<TrellisEntry const *, TrellisEntry const *> bps;
};

}
}

#endif // CITAR_TRELLIS_ENTRY
