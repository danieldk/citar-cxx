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

#ifndef CITAR_TAGGER_SUFFIXWORDHANDLER_HH
#define CITAR_TAGGER_SUFFIXWORDHANDLER_HH

#include <string>

#include <tr1/memory>
#include <tr1/unordered_map>

#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/hmm/UniGram.hh>

#include "WordHandler.hh"
#include "WordSuffixTree.hh"

namespace citar {
namespace tagger {

/**
 * The <i>SuffixWordHandler</i> is a <i>WordHandler</i> that estimates P(w|t)
 * using the suffix of a word.
 */
class SuffixWordHandler : public WordHandler
{
public:
	/**
	 * Construct a <i>SuffixWordHandler</i> using a model.
	 *
	 * @model The model to use for suffix and unigram statistics.
	 * @maxSuffixLength The maximum length of a suffix used for estimating
	 * 	probabilities.
	 * @upperMaxFreq Only train on uppercase words occurring <i>upperMaxFreq</i>
	 * 	fewer times.
	 * @lowerMaxFreq Only train on lowercase words occurring <i>lowerMaxFreq</i>
	 * 	fewer times.
	 * @maxTags The (maximum) number of (most probable) tags to return.
	 */
	SuffixWordHandler(
			std::tr1::shared_ptr<Model const> model, size_t maxSuffixLength,
			size_t upperMaxFreq, size_t lowerMaxFreq, size_t maxTags = 10);

	ProbSet tags(std::string const &word) const;
private:
	SuffixWordHandler(SuffixWordHandler const &other);
	SuffixWordHandler &operator=(SuffixWordHandler const &other);
	std::tr1::shared_ptr<WordSuffixTree> d_upperSuffixTree;
	std::tr1::shared_ptr<WordSuffixTree> d_lowerSuffixTree;
	size_t d_maxTags;
};

}
}

#endif // CITAR_TAGGER_SUFFIXWORDHANDLER_HH
