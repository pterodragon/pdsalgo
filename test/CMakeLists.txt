find_package(cxx_prettyprint REQUIRED MODULE)

file(GLOB TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(TEST_SRC ${TEST_SRCS})
    get_filename_component(test_name ${TEST_SRC} NAME_WE)
    add_executable(${test_name} ${TEST_SRC})
    target_link_libraries(${test_name} PUBLIC CXX_PRETTYPRINT::CXX_PRETTYPRINT pdsalgo)
    add_test(${test_name} ${test_name})
endforeach(TEST_SRC)
