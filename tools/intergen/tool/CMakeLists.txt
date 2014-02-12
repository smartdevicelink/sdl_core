include_directories(
  ${pugixml_SOURCE_DIR}/../src
  ${intergen_SOURCE_DIR}/cppgen/include
  ${intergen_SOURCE_DIR}/model/include
  ${intergen_SOURCE_DIR}/utils/include
)

add_executable(intergen intergen.cc)
target_link_libraries(intergen intergen_cppgen intergen_model intergen_utils pugixml)
install(TARGETS intergen RUNTIME DESTINATION bin)
