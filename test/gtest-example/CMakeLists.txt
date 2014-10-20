include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
)

set (SOURCES 
    ./example.cpp
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
)

add_executable("gtest-example" ${SOURCES})
target_link_libraries("gtest-example" ${LIBRARIES})
add_test(gtest-example gtest-example)
