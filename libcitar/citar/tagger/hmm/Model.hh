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

#ifndef CITAR_MODEL_HH
#define CITAR_MODEL_HH

#include <map>

#include <QHash>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>

#include "BiGram.hh"
#include "TriGram.hh"
#include "UniGram.hh"


namespace citar {
namespace tagger {

typedef QHash<QString, std::map<size_t, size_t> > WordTagFreqs;

struct NGrams
{
	QHash<QString, size_t> tagNumbers;
	QHash<size_t, QString> numberTags;
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
	QHash<size_t, QString> const &numberTags() const;

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
	static QSharedPointer<Model> readModel(QTextStream &lexiconStream,
		QTextStream &nGramStream);

	QHash<QString, size_t> const &tagNumbers() const;

	TriGramFreqs const &triGrams() const;

	UniGramFreqs const &uniGrams() const;
private:
	Model(QSharedPointer<WordTagFreqs> lexicon,
		QSharedPointer<NGrams> nGrams) :
		d_lexicon(lexicon), d_nGrams(nGrams) {}
	Model(Model const &other);
	Model &operator=(Model const &other);
	static QSharedPointer<WordTagFreqs>
		readLexicon(QTextStream &lexiconStream,
		QHash<QString, size_t> const &tagNumbers);
	static QSharedPointer<NGrams> readNGrams(QTextStream &lexiconStream);

	QSharedPointer<WordTagFreqs> d_lexicon;
	QSharedPointer<NGrams> d_nGrams;
};

inline BiGramFreqs const &Model::biGrams() const
{
	return d_nGrams->biGrams;
}

inline WordTagFreqs const &Model::lexicon() const
{
	return *d_lexicon;
}

inline QHash<size_t, QString> const &Model::numberTags() const
{
	return d_nGrams->numberTags;
}

inline QHash<QString, size_t> const &Model::tagNumbers() const
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
