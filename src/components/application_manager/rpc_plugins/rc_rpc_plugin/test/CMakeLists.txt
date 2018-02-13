include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${COMPONENTS_DIR}/application_manager/test/include
  ${COMPONENTS_DIR}/include/test/application_manager/
  )

set (RC_TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/resource_allocation_manager_impl_test.cc
)


set(LIBRARIES
  RCRpcPluginStaticLib
  ApplicationManager
  SmartObjects
  ProtocolHandler
  Utils
  jsoncpp
  gmock_main
  dl
)

if(ENABLE_LOG)
  list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

create_test("resourse_allocaiton_manager_test" "${RC_TEST_SOURCES}" "${LIBRARIES}")

