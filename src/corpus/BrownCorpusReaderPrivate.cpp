#include <algorithm>
#include <cctype>
#include <istream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <tr1/memory>

#include "BrownCorpusReaderPrivate.hh"

using namespace std;
using namespace std::tr1;
using namespace citar::corpus;

void BrownCorpusReaderPrivate::parse(std::istream &in)
{
	string line;
	while (getline(in, line)) {
		if (line.empty() || line.size() == 0)
			continue;

		vector<TaggedWord> sentence = parseLine(line);
		if (d_decapitalizeFirstWord)
			sentence[0].word[0] = tolower(sentence[0].word[0]);

		// Build a sentence with markers.
		vector<TaggedWord> markerSentence(d_startMarkers);
		copy(sentence.begin(), sentence.end(), back_inserter(markerSentence));
		copy(d_endMarkers.begin(), d_endMarkers.end(), back_inserter(markerSentence));

		// Pass the sentence to all handlers.
		for (vector<shared_ptr<SentenceHandler> >::iterator iter = d_sentenceHandlers.begin();
				iter != d_sentenceHandlers.end(); ++iter)
			(*iter)->handleSentence(markerSentence);
	}
}

vector<TaggedWord> BrownCorpusReaderPrivate::parseLine(std::string const &line) const
{
	vector<TaggedWord> sentence;

	istringstream lineStream(line);
	for (istream_iterator<string> iter(lineStream);
		iter != istream_iterator<string>(); ++iter)
	{
		string wordTag = *iter;

		// Find the word/tag delimiter within the word/tag combination.
		// Characters before the separator denote the word, characters
		// after the separator denote the tag.
		string::size_type sepIndex = wordTag.rfind("/");
		TaggedWord taggedWord(wordTag.substr(0, sepIndex),
			wordTag.substr(sepIndex + 1));
		sentence.push_back(taggedWord);
	}

	return sentence;
}
