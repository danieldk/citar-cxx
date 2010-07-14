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

#ifndef LANGKIT_TAGGER_KNOWNWORDHANDLER
#define LANGKIT_TAGGER_KNOWNWORDHANDLER

#include <map>

#include <QHash>
#include <QSharedPointer>
#include <QString>

#include "citar/tagger/hmm/Model.hh"
#include "citar/tagger/hmm/UniGram.hh"
#include "WordHandler.hh"

namespace citar {
namespace tagger {

/**
 * <i>KnownWordHandler</i> is a <i>WordHandler</i> for words that occur
 * in the lexicon.
 */
class KnownWordHandler : public WordHandler
{
public:
	typedef QHash<QString, std::map<size_t, double> >
		WordTagProbLexicon;

	/**
	 * Construct a <i>KnownWordHandler</i> using a model.
	 *
	 * @model The model to retrieve the lexicon from.
	 * @fallbackWordHandler A pointer to the <i>WordHandler</i> that should
	 * 	be used when the word could not be found in the lexicon.
	 */
	KnownWordHandler(QSharedPointer<Model const> model,
		WordHandler const *fallbackWordHandler);
	KnownWordHandler(KnownWordHandler const &other) :
		WordHandler(),
		d_lexicon(new WordTagProbLexicon(*other.d_lexicon)),
		d_fallbackWordHandler(other.d_fallbackWordHandler)
		{}
	virtual ~KnownWordHandler();
	KnownWordHandler &operator=(KnownWordHandler const &other);
	ProbSet tags(QString const &word) const;
private:
	void copy(KnownWordHandler const &other);
	void calcWordTagProbs(WordTagFreqs const &wordTagFreqs,
		UniGramFreqs const &uniGramFreqs);

	QSharedPointer<WordTagProbLexicon> d_lexicon;
	WordHandler const *d_fallbackWordHandler;
};

}
}

#endif // LANGKIT_TAGGER_KNOWNWORDHANDLER

