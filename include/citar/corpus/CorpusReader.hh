/*
 * Copyright 2008, 2016 Daniël de Kok
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

#ifndef CITAR_CORPUSREADER_HH
#define CITAR_CORPUSREADER_HH

#include <iostream>
#include <vector>

#include <memory>

#include <boost/optional.hpp>

#include <citar/util/NonCopyable.hh>
#include <citar/corpus/SentenceHandler.hh>
#include <citar/corpus/TaggedWord.hh>

namespace citar {
namespace corpus {

/**
 * Base class for corpus handlers.
 */
class CorpusReader : public citar::util::NonCopyable
{
public:
	/**
	 * Read a sentence. The value is absent if the end of file is reached.
	 */
	virtual boost::optional<std::vector<TaggedWord>> nextSentence() = 0;

	virtual ~CorpusReader() {}
protected:
};

}
}

#endif // CITAR_CORPUSREADER_HH
