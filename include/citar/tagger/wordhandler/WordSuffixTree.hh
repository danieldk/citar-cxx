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

#ifndef CITAR_TAGGER_WORDSUFFIXTREE
#define CITAR_TAGGER_WORDSUFFIXTREE

#include <map>
#include <string>

#include <tr1/memory>
#include <tr1/unordered_map>

#include <citar/tagger/hmm/UniGram.hh>
#include <citar/tagger/wordhandler/WordHandler.hh>

namespace citar {
namespace tagger {

class WordSuffixTree
{
	class TreeNode;
public:
	WordSuffixTree(UniGramFreqs const &uniGrams, size_t maxLength = 10) :
		d_uniGrams(new UniGramFreqs(uniGrams)),
		d_rootNode(new TreeNode(this)),
		d_maxLength(maxLength) {}
	void addWord(std::string word, std::map<size_t, size_t> tagProbs);
	std::tr1::unordered_map<size_t, double> suffixTagProbs(std::string word);
	void calculateTheta();

	double theta() const;
	TreeNode const &root() const;
	UniGramFreqs const &uniGrams() const;
private:
	class TreeNode
	{
	public:
		TreeNode(WordSuffixTree const *tree) : d_tree(tree), d_tagFreq(0) {}
		void addSuffix(std::string const &reverseSuffix,
			std::map<size_t, size_t> const &tagFreqs);
		std::tr1::unordered_map<size_t, double> suffixTagProbs(
			std::string const &reverseSuffix,
			std::tr1::unordered_map<size_t, double> const &tagProbs);
	private:
		TreeNode(TreeNode const &other);
		TreeNode &operator=(TreeNode const &other);
		std::tr1::unordered_map<size_t, double> bayesianInversion(
			std::tr1::unordered_map<size_t, double> const &tagProbs) const;

		std::tr1::unordered_map<size_t, size_t> d_tagFreqs;
		WordSuffixTree const *d_tree;
		std::map<char, std::tr1::shared_ptr<TreeNode> > d_children;
		size_t d_tagFreq;
	};

	std::tr1::shared_ptr<UniGramFreqs> d_uniGrams;
	std::tr1::shared_ptr<TreeNode> d_rootNode;
	size_t d_maxLength;
	double d_theta;
};

inline WordSuffixTree::TreeNode const &WordSuffixTree::root() const
{
	return *d_rootNode;
}

inline double WordSuffixTree::theta() const
{
	return d_theta;
}

inline UniGramFreqs const &WordSuffixTree::uniGrams() const
{
	return *d_uniGrams;
}

}
}

#endif // CITAR_TAGGER_WORDSUFFIXTREE
