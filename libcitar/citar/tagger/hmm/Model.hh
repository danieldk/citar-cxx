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

#include <tr1/memory>
#include <tr1/unordered_map>

#include "BiGram.hh"
#include "TriGram.hh"
#include "UniGram.hh"


namespace citar {
namespace tagger {

typedef std::tr1::unordered_map<std::string, std::map<size_t, size_t> >
	WordTagFreqs;

struct NGrams
{
	std::tr1::unordered_map<std::string, size_t> tagNumbers;
	std::tr1::unordered_map<size_t, std::string> numberTags;
	UniGramFreqs uniGrams;
	BiGramFreqs biGrams;
	TriGramFreqs triGrams;
};


/**
 * Instances of this class contain a model for a trigram HMM tagger. It
 * consists of the language model (uni/bi/trigram statistics) and a lexicon.
 */
class Model
{
public:
	BiGramFreqs const &biGrams() const;
	WordTagFreqs const &lexicon() const;
	std::tr1::unordered_map<size_t, std::string> const &numberTags() const;

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
	static std::tr1::shared_ptr<Model> readModel(std::istream &lexiconStream,
		std::istream &nGramStream);

	std::tr1::unordered_map<std::string, size_t> const &tagNumbers() const;

	TriGramFreqs const &triGrams() const;

	UniGramFreqs const &uniGrams() const;
private:
	Model(std::tr1::shared_ptr<WordTagFreqs> lexicon,
		std::tr1::shared_ptr<NGrams> nGrams) :
		d_lexicon(lexicon), d_nGrams(nGrams) {}
	Model(Model const &other);
	Model &operator=(Model const &other);
	static std::tr1::shared_ptr<WordTagFreqs>
		readLexicon(std::istream &lexiconStream,
		std::tr1::unordered_map<std::string, size_t> const &tagNumbers);
	static std::tr1::shared_ptr<NGrams> readNGrams(std::istream &lexiconStream);

	std::tr1::shared_ptr<WordTagFreqs> d_lexicon;
	std::tr1::shared_ptr<NGrams> d_nGrams;
};

inline BiGramFreqs const &Model::biGrams() const
{
	return d_nGrams->biGrams;
}

inline WordTagFreqs const &Model::lexicon() const
{
	return *d_lexicon;
}

inline std::tr1::unordered_map<size_t, std::string> const &Model::numberTags() const
{
	return d_nGrams->numberTags;
}

inline std::tr1::unordered_map<std::string, size_t> const &Model::tagNumbers() const
{
	return d_nGrams->tagNumbers;
}

inline TriGramFreqs const &Model::triGrams() const
{
	return d_nGrams->triGrams;
}

inline UniGramFreqs const &Model::uniGrams() const
{
	return d_nGrams->uniGrams;
}

}
}

#endif // CITAR_MODEL_HH
