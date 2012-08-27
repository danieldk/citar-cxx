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
#include "ModelPrivate.hh"

using namespace citar::tagger;

std::tr1::shared_ptr<WordTagFreqs> ModelPrivate::readLexicon(
	std::istream &lexiconStream,
	std::tr1::unordered_map<std::string, size_t> const &tagNumbers)
{
	std::tr1::shared_ptr<WordTagFreqs> lexicon(
		new WordTagFreqs);

	std::string line;
	while (std::getline(lexiconStream, line)) {
		std::istringstream lineStream(line);

		// Read line items.
		std::vector<std::string> lineItems;
		copy(std::istream_iterator<std::string>(lineStream),
			std::istream_iterator<std::string>(),
			std::back_inserter(lineItems));

		if (lineItems.size() == 0 || (lineItems.size() - 1) % 2 != 0)
		  throw std::runtime_error(std::string("Invalid lexicon entry: ") + line);

		std::string word = lineItems[0];

		for (size_t i = 1; i < lineItems.size(); i += 2) {
			std::istringstream freqStream(lineItems[i + 1]);
			size_t freq;
			freqStream >> freq;
			size_t tag = tagNumbers.find(lineItems[i])->second;
			(*lexicon)[word][tag] = freq;
		}
	}

	return lexicon;
}

ModelPrivate *ModelPrivate::readModel(std::istream &lexiconStream,
	std::istream &nGramStream)
{
	std::tr1::shared_ptr<NGrams> nGrams = readNGrams(nGramStream);
	std::tr1::shared_ptr<WordTagFreqs> lexicon =
		readLexicon(lexiconStream, nGrams->tagNumbers);

	return new ModelPrivate(lexicon, nGrams);
}

std::tr1::shared_ptr<NGrams> ModelPrivate::readNGrams(std::istream &lexiconStream)
{
	std::tr1::shared_ptr<NGrams> nGrams(new NGrams);

	size_t tagNumber = 0;
	std::string line;
	while (std::getline(lexiconStream, line)) {
		std::istringstream lineStream(line);

		// Read line items.
		std::vector<std::string> lineItems;
		copy(std::istream_iterator<std::string>(lineStream),
			std::istream_iterator<std::string>(), back_inserter(lineItems));

		if (lineItems.size() < 2 || lineItems.size() > 4)
		  throw std::runtime_error(std::string("Invalid ngram model entry: ") + line);

		// Get frequency.
		std::istringstream freqStream(lineItems[lineItems.size() - 1]);
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
			std::cerr << "Encountered a line which does not contain a uni/bi/trigram!" << std::endl;
	}

	return nGrams;
}
