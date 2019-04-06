find_package(cxx_prettyprint REQUIRED MODULE)
find_package(Catch2 REQUIRED CONFIG)

file(GLOB TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*test.cpp")

add_executable(all_tests ${TEST_SRCS} main.cpp)
target_link_libraries(all_tests PUBLIC CXX_PRETTYPRINT::CXX_PRETTYPRINT pdsalgo Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(all_tests)
