message (STATUS "Check for C++ TR1 headers")

try_run (TR1_TEST_EXIT_CODE TR1_TEST_COMPILED
  ${CMAKE_BINARY_DIR}
  ${CITAR_SOURCE_DIR}/cmake/tr1-test.cpp
)

if (TR1_TEST_EXIT_CODE EQUAL 0)
  message (STATUS "Check for C++ TR1 headers -- works")
  set (TR1_AVAILABLE "YES")
else (TR1_TEST_EXIT_CODE EQUAL 0)
  message (STATUS "C++ TR1 headers not found")
  set (TR1_AVAILABLE "NO")
endif (TR1_TEST_EXIT_CODE EQUAL 0)
