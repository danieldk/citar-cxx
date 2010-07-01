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

#ifndef LANGKIT_TAGGER_WORDHANDLER
#define LANGKIT_TAGGER_WORDHANDLER

#include <map>
#include <set>
#include <string>
#include <utility>

#include <tr1/unordered_map>

#include <citar/tagger/hmm/UniGram.hh>
#include <citar/utility/PairSecondGtComp.hh>

namespace citar {
namespace tagger {

class WordHandler
{
public:
	typedef std::set<std::pair<size_t, double>,
		citar::utility::PairSecondGtComp<std::pair<size_t, double> > >
		ProbSet;
	typedef std::map<size_t, size_t> TagFreqs;
	virtual ProbSet tags(std::string const &word) const = 0;
	virtual ~WordHandler() {}
};

}
}

#endif // LANGKIT_TAGGER_WORDHANDLER

