include_directories (
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/include
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/gtest/include)

set(testSources
  main.cc
  file_system_test.cc
  date_time_test.cc)

set(testLibraries
  gmock
  gtest
  Utils)

add_executable(utils_test ${testSources})
target_link_libraries(utils_test ${testLibraries})

