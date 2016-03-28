#include <string>
#include <vector>

#include <citar/corpus/TaggedWord.hh>

namespace citar {

std::string const START_MARKER = "<START>";

std::string const END_MARKER = "<END>";

citar::corpus::TaggedWord START_MARKER_TAGGED(START_MARKER, START_MARKER);

citar::corpus::TaggedWord END_MARKER_TAGGED(END_MARKER, END_MARKER);

std::vector<std::string> START_MARKERS = {START_MARKER, START_MARKER};

std::vector<std::string> END_MARKERS = {END_MARKER};

std::vector<corpus::TaggedWord> const START_MARKERS_TAGGED = {START_MARKER_TAGGED, START_MARKER_TAGGED};

std::vector<corpus::TaggedWord> const END_MARKERS_TAGGED = {END_MARKER_TAGGED};

}
