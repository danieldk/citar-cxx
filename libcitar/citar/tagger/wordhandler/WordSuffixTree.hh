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

#ifndef CITAR_TAGGER_WORDSUFFIXTREE
#define CITAR_TAGGER_WORDSUFFIXTREE

#include <map>

#include <QChar>
#include <QHash>
#include <QSharedPointer>
#include <QString>

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
	void addWord(QString word, std::map<size_t, size_t> tagProbs);
	QHash<size_t, double> suffixTagProbs(QString word);
	void calculateTheta();

	double theta() const;
	TreeNode const &root() const;
	UniGramFreqs const &uniGrams() const;
private:
	class TreeNode
	{
	public:
		TreeNode(WordSuffixTree const *tree) : d_tree(tree), d_tagFreq(0) {}
		void addSuffix(QString const &reverseSuffix,
			std::map<size_t, size_t> const &tagFreqs);
		QHash<size_t, double> suffixTagProbs(
			QString const &reverseSuffix,
			QHash<size_t, double> const &tagProbs);
	private:
		TreeNode(TreeNode const &other);
		TreeNode &operator=(TreeNode const &other);
		QHash<size_t, double> bayesianInversion(
			QHash<size_t, double> const &tagProbs) const;

		QHash<size_t, size_t> d_tagFreqs;
		WordSuffixTree const *d_tree;
		std::map<QChar, QSharedPointer<TreeNode> > d_children;
		size_t d_tagFreq;
	};

	QSharedPointer<UniGramFreqs> d_uniGrams;
	QSharedPointer<TreeNode> d_rootNode;
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
