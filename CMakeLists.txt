cmake_minimum_required(VERSION 2.6)
project(CITAR)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)


IF ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
    SET(CMAKE_MACOSX_RPATH OFF)
ENDIF ()

include(${CITAR_SOURCE_DIR}/cmake/CXXFlags.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#if (NOT TR1_AVAILABLE)
#  message (FATAL_ERROR "C++ TR1 headers are required!")
#endif (NOT TR1_AVAILABLE)

OPTION(WITH_TRIGRAM_CACHE "Enable trigram caching" OFF)

CONFIGURE_FILE(${CITAR_SOURCE_DIR}/include/citar/config.hh.cmake
        ${CITAR_BINARY_DIR}/include/citar/config.hh)

include_directories(${CITAR_SOURCE_DIR}/include)
include_directories(${CITAR_BINARY_DIR}/include)

set(LIBCITAR_HEADERS
        include/citar/corpus/BrownCorpusReader.hh
        include/citar/corpus/CorpusReader.hh
        include/citar/corpus/SentenceHandler.hh
        include/citar/corpus/TaggedWord.hh

        include/citar/tagger/hmm/BiGram.hh
        include/citar/tagger/hmm/HMMTagger.hh
        include/citar/tagger/hmm/LinearInterpolationSmoothing.hh
        include/citar/tagger/hmm/Model.hh
        include/citar/tagger/hmm/Smoothing.hh
        include/citar/tagger/hmm/TriGram.hh
        include/citar/tagger/hmm/UniGram.hh

        include/citar/tagger/wordhandler/KnownWordHandler.hh
        include/citar/tagger/wordhandler/SuffixWordHandler.hh
        include/citar/tagger/wordhandler/WordHandler.hh
        include/citar/tagger/wordhandler/WordSuffixTree.hh

        include/citar/util/NonCopyable.hh
        include/citar/util/PairSecondGtComp.hh)

set(LIBCITAR_SOURCES
        src/corpus/BrownCorpusReader.cpp
        src/corpus/BrownCorpusReaderPrivate.cpp

        src/cwrap/cwrap.cpp

        src/tagger/hmm/HMMTagger.cpp
        src/tagger/hmm/HMMTaggerPrivate.cpp
        src/tagger/hmm/LinearInterpolationSmoothing.cpp
        src/tagger/hmm/LinearInterpolationSmoothingPrivate.cpp
        src/tagger/hmm/Model.cpp
        src/tagger/hmm/ModelPrivate.cpp
        src/tagger/hmm/TrellisEntry.hh

        src/tagger/wordhandler/KnownWordHandler.cpp
        src/tagger/wordhandler/SuffixWordHandler.cpp
        src/tagger/wordhandler/WordSuffixTree.cpp)

add_library(citar SHARED ${LIBCITAR_HEADERS} ${LIBCITAR_SOURCES})

install(TARGETS citar
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY include/citar
        DESTINATION include)

add_executable(citar-evaluate util/evaluate.cpp)
target_link_libraries(citar-evaluate citar)

add_executable(citar-tag util/tag.cpp)
target_link_libraries(citar-tag citar)

add_executable(citar-train util/train.cpp)
target_link_libraries(citar-train citar)

install(TARGETS citar-evaluate citar-tag citar-train
        RUNTIME DESTINATION bin)
