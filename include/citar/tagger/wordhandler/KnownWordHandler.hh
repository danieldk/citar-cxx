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

#ifndef LANGKIT_TAGGER_KNOWNWORDHANDLER
#define LANGKIT_TAGGER_KNOWNWORDHANDLER

#include <map>
#include <string>

#include <tr1/memory>
#include <tr1/unordered_map>

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
	typedef std::tr1::unordered_map<std::string, std::map<size_t, double> >
		WordTagProbLexicon;

	/**
	 * Construct a <i>KnownWordHandler</i> using a model.
	 *
	 * @model The model to retrieve the lexicon from.
	 * @fallbackWordHandler A pointer to the <i>WordHandler</i> that should
	 * 	be used when the word could not be found in the lexicon.
	 */
	KnownWordHandler(std::tr1::shared_ptr<Model const> model,
		WordHandler const *fallbackWordHandler);
	KnownWordHandler(KnownWordHandler const &other) :
		d_lexicon(new WordTagProbLexicon(*other.d_lexicon)),
		d_fallbackWordHandler(other.d_fallbackWordHandler)
		{}
	virtual ~KnownWordHandler();
	KnownWordHandler &operator=(KnownWordHandler const &other);
	ProbSet tags(std::string const &word) const;
private:
	void copy(KnownWordHandler const &other);
	void calcWordTagProbs(WordTagFreqs const &wordTagFreqs,
		UniGramFreqs const &uniGramFreqs);

	std::tr1::shared_ptr<WordTagProbLexicon> d_lexicon;
	WordHandler const *d_fallbackWordHandler;
};

}
}

#endif // LANGKIT_TAGGER_KNOWNWORDHANDLER

