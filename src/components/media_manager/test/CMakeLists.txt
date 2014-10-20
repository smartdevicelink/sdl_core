include_directories (
${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/include
${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/gtest/include)

set(testSources
  main.cc
  media_adapter_impl_test.cc)

set(testLibraries
  gmock
  gtest)

add_executable(media_manager_test ${testSources})
target_link_libraries(media_manager_test ${testLibraries})
