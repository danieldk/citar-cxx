#include <algorithm>
#include <cctype>
#include <istream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <memory>

#include <boost/optional.hpp>

#include "BrownCorpusReaderPrivate.hh"

using namespace citar::corpus;

boost::optional<std::vector<TaggedWord>> BrownCorpusReaderPrivate::nextSentence()
{
	std::string line;
	while (getline(*d_is, line)) {
		if (!*d_is)
			return boost::none;


		if (line.empty() || line.size() == 0)
			continue;

		return parseLine(line);
	}

	// We shouldn't end up here.
	return boost::none;
}

std::vector<TaggedWord> BrownCorpusReaderPrivate::parseLine(std::string const &line) const
{
	std::vector<TaggedWord> sentence;

	std::istringstream lineStream(line);
	for (std::istream_iterator<std::string> iter(lineStream);
		iter != std::istream_iterator<std::string>(); ++iter)
	{
		std::string wordTag = *iter;

		// Find the word/tag delimiter within the word/tag combination.
		// Characters before the separator denote the word, characters
		// after the separator denote the tag.
		std::string::size_type sepIndex = wordTag.rfind("/");
		TaggedWord taggedWord(wordTag.substr(0, sepIndex),
			wordTag.substr(sepIndex + 1));
		sentence.push_back(taggedWord);
	}

	return sentence;
}
