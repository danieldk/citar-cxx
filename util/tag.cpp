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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <citar/corpus/TaggedWord.hh>
#include <citar/tagger/hmm/HMMTagger.hh>
#include <citar/tagger/hmm/LinearInterpolationSmoothing.hh>
#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/wordhandler/KnownWordHandler.hh>
#include <citar/tagger/wordhandler/SuffixWordHandler.hh>

using namespace std;
using namespace citar::corpus;
using namespace citar::tagger;

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cerr << "Syntax: " << argv[0] << " lexicon ngrams" << endl;
		return 1;
	}

	ifstream nGramStream(argv[2]);
	if (!nGramStream.good())
	{
		cerr << "Could not open ngrams: " << argv[2] << endl;
		return 1;
	}

	ifstream lexiconStream(argv[1]);
	if (!lexiconStream.good())
	{
		cerr << "Could not open lexicon: " << argv[1] << endl;
		return 1;
	}

	shared_ptr<Model> model(Model::readModel(lexiconStream, nGramStream));

	SuffixWordHandler suffixWordHandler(model, 2, 2, 8);

	KnownWordHandler knownWordHandler(model, &suffixWordHandler);

	LinearInterpolationSmoothing smoothing(model);

	shared_ptr<HMMTagger> hmmTagger(new HMMTagger(model,
		&knownWordHandler, &smoothing));

	string line;
	while (true)
	{
		if (!getline(cin, line))
			break;

		istringstream lineStream(line);

		// Get the sequence of words, adding start/end markers.
		vector<string> sentence(2, "<START>");
		copy(istream_iterator<string>(lineStream), istream_iterator<string>(),
			back_inserter(sentence));
		sentence.push_back("<END>");

		// Tag it.
		vector<string> taggedWords = hmmTagger->tag(sentence);

		// Print word/tags.
		for (size_t i = 2; i < sentence.size() - 1; ++i)
			cout << sentence[i] << "/" << taggedWords[i] << " ";

		cout << endl;
	}
}
