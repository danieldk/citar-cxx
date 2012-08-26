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

#ifndef CITAR_CORPUSREADER_HH
#define CITAR_CORPUSREADER_HH

#include <iostream>
#include <vector>

#include <tr1/memory>

#include "SentenceHandler.hh"
#include "TaggedWord.hh"

namespace citar {
namespace corpus {

/**
 * Base class for corpus handlers. Derived classes should use the protected
 * <i>d_startMarkers</i> and <i>d_endMarkers</i> fields to add start/end
 * markers to sentences.
 */
class CorpusReader
{
public:
	CorpusReader (std::vector<TaggedWord> const &startMarkers,
		std::vector<TaggedWord> const &endMarkers,
		bool decapitalizeFirstWord = false) :
		d_startMarkers(startMarkers), d_endMarkers(endMarkers),
		d_decapitalizeFirstWord(decapitalizeFirstWord) {}

	/**
	 * Register a sentence handler class.
	 */
	void addSentenceHandler(std::tr1::shared_ptr<SentenceHandler> sentenceHandler);

	/**
	 * Parse a corpus. The registered sentence handlers will be called to handle
	 * each parsed sentence.
	 */
	virtual void parse(std::istream &in) = 0;

	virtual ~CorpusReader() {}
protected:
	std::vector<std::tr1::shared_ptr<SentenceHandler> > d_sentenceHandlers;
	std::vector<TaggedWord> d_startMarkers;
	std::vector<TaggedWord> d_endMarkers;
	bool d_decapitalizeFirstWord;
};

inline void CorpusReader::addSentenceHandler(std::tr1::shared_ptr<SentenceHandler> sentenceHandler)
{
	d_sentenceHandlers.push_back(sentenceHandler);
}

}
}

#endif // CITAR_CORPUSREADER_HH
