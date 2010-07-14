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

#ifndef CITAR_SENTENCEHANDLER_HH
#define CITAR_SENTENCEHANDLER_HH

#include <QVector>

#include "TaggedWord.hh"

namespace citar {
namespace corpus {

/**
 * This is a base class for sentence handlers. All classes derived from
 * the <i>CorpusReader</i> class should provide facilities to register
 * classes that derive from <i>SentenceHandler</i> classes. The reader
 * will then call all registered <i>SentenceHandler</i> instances for
 * every sentence that was read.
 */
class SentenceHandler
{
public:
	/**
	 * Handle a sentence, represented as a <i>vector</i> of <i>TaggedWord</i>
	 * instances.
	 */
	virtual void handleSentence(QVector<TaggedWord> const &sentence) = 0;
	virtual ~SentenceHandler() {}
};

}
}


#endif // CITAR_SENTENCEHANDLER_HH
