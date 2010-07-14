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

#ifndef LANGKIT_TAGGER_WORDHANDLER
#define LANGKIT_TAGGER_WORDHANDLER

#include <map>
#include <set>
#include <utility>

#include <QHash>
#include <QString>

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
	virtual ProbSet tags(QString const &word) const = 0;
	virtual ~WordHandler() {}
};

}
}

#endif // LANGKIT_TAGGER_WORDHANDLER

