#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <tr1/memory>
#include <tr1/unordered_map>

#include <citar/tagger/hmm/Model.hh>

using namespace std;
using namespace std::tr1;
using namespace citar::tagger;

shared_ptr<WordTagFreqs> Model::readLexicon(
	std::istream &lexiconStream,
	unordered_map<string, size_t> const &tagNumbers)
{
	shared_ptr<WordTagFreqs> lexicon(
		new WordTagFreqs);

	string line;
	while (getline(lexiconStream, line)) {
		istringstream lineStream(line);

		// Read line items.
		vector<string> lineItems;
		copy(istream_iterator<string>(lineStream), istream_iterator<string>(),
			back_inserter(lineItems));

		if (lineItems.size() == 0 || (lineItems.size() - 1) % 2 != 0)
		  throw runtime_error(string("Invalid lexicon entry: ") + line);

		string word = lineItems[0];

		for (size_t i = 1; i < lineItems.size(); i += 2) {
			istringstream freqStream(lineItems[i + 1]);
			size_t freq;
			freqStream >> freq;
			size_t tag = tagNumbers.find(lineItems[i])->second;
			(*lexicon)[word][tag] = freq;
		}
	}

	return lexicon;
}

shared_ptr<Model> Model::readModel(istream &lexiconStream, istream &nGramStream)
{
	shared_ptr<NGrams> nGrams = readNGrams(nGramStream);
	shared_ptr<WordTagFreqs> lexicon =
		readLexicon(lexiconStream, nGrams->tagNumbers);

	shared_ptr<Model> model(new Model(lexicon, nGrams));

	return model;
}

shared_ptr<NGrams> Model::readNGrams(std::istream &lexiconStream)
{
	shared_ptr<NGrams> nGrams(new NGrams);

	size_t tagNumber = 0;
	string line;
	while (getline(lexiconStream, line)) {
		istringstream lineStream(line);

		// Read line items.
		vector<string> lineItems;
		copy(istream_iterator<string>(lineStream), istream_iterator<string>(), back_inserter(lineItems));

		if (lineItems.size() < 2 || lineItems.size() > 4)
		  throw runtime_error(string("Invalid ngram model entry: ") + line);

		// Get frequency.
		istringstream freqStream(lineItems[lineItems.size() - 1]);
		size_t freq;
		freqStream >> freq;

		// Get uni-/bi-/trigram
		if (lineItems.size() == 2)
		{
			nGrams->tagNumbers[lineItems[0]] = tagNumber;
			nGrams->numberTags[tagNumber] = lineItems[0];
			nGrams->uniGrams[UniGram(tagNumber)] += freq;
			++tagNumber;
		}
		else if (lineItems.size() == 3) {
			BiGram biGram(nGrams->tagNumbers[lineItems[0]],
				nGrams->tagNumbers[lineItems[1]]);
			nGrams->biGrams[biGram] += freq;
		}
		else if (lineItems.size() == 4)
			nGrams->triGrams[TriGram(nGrams->tagNumbers[lineItems[0]],
				nGrams->tagNumbers[lineItems[1]],
				nGrams->tagNumbers[lineItems[2]])] +=
				freq;
		else
			cerr << "Encountered a line which does not contain a uni/bi/trigram!" << endl;
	}

	return nGrams;
}
