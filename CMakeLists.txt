cmake_minimum_required(VERSION 2.6)
project(CITAR)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   set (CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

include (${CITAR_SOURCE_DIR}/cmake/CXXFlags.cmake)

find_package(Qt4 COMPONENTS QtCore)
INCLUDE(${QT_USE_FILE})

CONFIGURE_FILE (${CITAR_SOURCE_DIR}/libcitar/citar/config.hh.cmake
  ${CITAR_SOURCE_DIR}/libcitar/citar/config.hh) 

include_directories (${CITAR_SOURCE_DIR}/libcitar)

add_subdirectory(libcitar)

add_executable(citar-evaluate util/evaluate.cpp)
target_link_libraries(citar-evaluate citar)
target_link_libraries(citar-evaluate ${QT_LIBRARIES})

add_executable(citar-tag util/tag.cpp)
target_link_libraries(citar-tag citar)
target_link_libraries(citar-tag ${QT_LIBRARIES})

add_executable(citar-train util/train.cpp)
target_link_libraries(citar-train citar)
target_link_libraries(citar-train ${QT_LIBRARIES})

install(TARGETS citar-evaluate citar-tag citar-train
	RUNTIME DESTINATION bin)
