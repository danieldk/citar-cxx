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

#ifndef CITAR_MODEL_HH
#define CITAR_MODEL_HH

#include <iostream>
#include <map>
#include <string>

#include <memory>
#include <unordered_map>

#include <citar/util/NonCopyable.hh>
#include <citar/tagger/hmm/BiGram.hh>
#include <citar/tagger/hmm/TriGram.hh>
#include <citar/tagger/hmm/UniGram.hh>


namespace citar {
namespace tagger {

typedef std::unordered_map<std::string, std::map<size_t, size_t> >
	WordTagFreqs;

struct NGrams
{
	std::unordered_map<std::string, size_t> tagNumbers;
	std::unordered_map<size_t, std::string> numberTags;
	UniGramFreqs uniGrams;
	BiGramFreqs biGrams;
	TriGramFreqs triGrams;
};

class ModelPrivate;

/**
 * Instances of this class contain a model for a trigram HMM tagger. It
 * consists of the language model (uni/bi/trigram statistics) and a lexicon.
 */
class Model : public citar::util::NonCopyable
{
public:
	virtual ~Model();

	BiGramFreqs const &biGrams() const;
	WordTagFreqs const &lexicon() const;
	std::unordered_map<size_t, std::string> const &numberTags() const;

	/**
	 * Read the model from input streams. An input stream for the lexicon, and
	 * an input stream for n-gram statistics should be provided. The lexicon
	 * should list one word per line, followed by pairs of a tag and its
	 * frequency, for instance:
	 *
	 * <pre>
	 * uncommunicative JJ 1
	 * dormant JJ 3
	 * paranormal JJ 1
	 * plan NN 138 VB 27
	 * accolades NNS 1
	 * </pre>
	 *
	 * The ngram stream should list one ngram per line with its frequency. For
	 * instance:
	 *
	 * <pre>
	 * BED ABX 5
	 * OD , 82
	 * AP DO 1
	 * BEN ABN 1
	 * WPO VBD 5
	 * </pre>
	 */
	static std::shared_ptr<Model> readModel(std::istream &lexiconStream,
		std::istream &nGramStream);

	std::unordered_map<std::string, size_t> const &tagNumbers() const;

	TriGramFreqs const &triGrams() const;

	UniGramFreqs const &uniGrams() const;
private:
	Model(ModelPrivate *model);

	ModelPrivate *d_private;
};

}
}

#endif // CITAR_MODEL_HH
