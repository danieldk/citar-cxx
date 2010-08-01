add_library(citar SHARED
  src/corpus/BrownCorpusReader/BrownCorpusReader.cpp
  src/cwrap/cwrap.cpp
  src/tagger/hmm/HMMTagger/HMMTagger.cpp
  src/tagger/hmm/LinearInterpolationSmoothing/LinearInterpolationSmoothing.cpp
  src/tagger/hmm/Model/Model.cpp
  src/tagger/wordhandler/KnownWordHandler/KnownWordHandler.cpp
  src/tagger/wordhandler/SuffixWordHandler/SuffixWordHandler.cpp
  src/tagger/wordhandler/WordSuffixTree/WordSuffixTree.cpp
)

target_link_libraries(citar ${QT_LIBRARIES})

install(TARGETS citar 
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(DIRECTORY citar
  DESTINATION include)
