#include <algorithm>
#include <istream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <memory>

#include "CONLLCorpusReaderPrivate.hh"

#include <iostream>

using namespace std;
using namespace citar::corpus;

TaggedWord parseLine(std::string const &line);

void CONLLCorpusReaderPrivate::parse(std::istream &in)
{
  std::vector<TaggedWord> sentence;

  string line;
  while (getline(in, line)) {
    if (line.empty() && sentence.size() != 0) {
      // Build a sentence with markers.
      vector<TaggedWord> markerSentence(d_startMarkers);
      copy(sentence.begin(), sentence.end(), back_inserter(markerSentence));
      copy(d_endMarkers.begin(), d_endMarkers.end(), back_inserter(markerSentence));

      // Pass the sentence to all handlers.
      for (auto iter = d_sentenceHandlers.begin(); iter != d_sentenceHandlers.end();
          ++iter)
        (*iter)->handleSentence(markerSentence);

      sentence.clear();
      //cerr << ".";
    }
    else {
      sentence.push_back(parseLine(line));
    }
  }
}

TaggedWord CONLLCorpusReaderPrivate::parseLine(std::string const &line)
{
  vector<string> columns;
  istringstream iss(line);
  copy(istream_iterator<std::string>(iss), istream_iterator<std::string>(),
      back_inserter(columns));

  if (columns.size() < 5)
    throw runtime_error(std::string("Line contains fewer than five columns: " + line));

  return TaggedWord(columns[1], columns[4]);
}
