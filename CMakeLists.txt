cmake_minimum_required(VERSION 2.6)
project(CITAR)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   set (CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

include (${CITAR_SOURCE_DIR}/cmake/CXXFlags.cmake)
include (${CITAR_SOURCE_DIR}/cmake/TR1Test.cmake)

if (NOT TR1_AVAILABLE)
  message (FATAL_ERROR "C++ TR1 headers are required!")
endif (NOT TR1_AVAILABLE)

OPTION (WITH_TRIGRAM_CACHE "Enable trigram caching" OFF)

CONFIGURE_FILE (${CITAR_SOURCE_DIR}/libcitar/citar/config.hh.cmake
  ${CITAR_SOURCE_DIR}/libcitar/citar/config.hh) 

include_directories (${CITAR_SOURCE_DIR}/libcitar)

add_subdirectory(libcitar)

add_executable(citar-evaluate util/evaluate.cpp)
target_link_libraries(citar-evaluate citar)

add_executable(citar-tag util/tag.cpp)
target_link_libraries(citar-tag citar)

add_executable(citar-train util/train.cpp)
target_link_libraries(citar-train citar)

install(TARGETS citar-evaluate citar-tag citar-train
	RUNTIME DESTINATION bin)
