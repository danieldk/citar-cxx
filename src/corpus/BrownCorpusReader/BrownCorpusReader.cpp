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

#include "BrownCorpusReader.ih"

void BrownCorpusReader::parse(std::istream &in)
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

vector<TaggedWord> BrownCorpusReader::parseLine(std::string const &line) const
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
