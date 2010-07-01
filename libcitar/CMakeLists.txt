add_library(citar STATIC SHARED
  src/corpus/BrownCorpusReader/BrownCorpusReader.cpp
  src/cwrap/cwrap.cpp
  src/tagger/hmm/HMMTagger/HMMTagger.cpp
  src/tagger/hmm/LinearInterpolationSmoothing/LinearInterpolationSmoothing.cpp
  src/tagger/hmm/Model/Model.cpp
  src/tagger/wordhandler/KnownWordHandler/KnownWordHandler.cpp
  src/tagger/wordhandler/SuffixWordHandler/SuffixWordHandler.cpp
  src/tagger/wordhandler/WordSuffixTree/WordSuffixTree.cpp
)

install(TARGETS citar 
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(DIRECTORY citar
  DESTINATION include)
