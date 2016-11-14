#set( CMAKE_VERBOSE_MAKEFILE on )

include_directories(
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
	${JSONCPP_INCLUDE_DIRECTORY}
)

if (${EXTENDED_POLICY} STREQUAL "EXTENDED_PROPRIETARY")
  include_directories(${CMAKE_SOURCE_DIR}/src/components/policy/policy_premium/include/)
  message("DEFINED  EXTENDED_PROPRIETARY")
else()
  include_directories(${CMAKE_SOURCE_DIR}/src/components/policy/policy_regular/include/)
  message("DEFAULT PARAMETER")
endif()

link_directories (
  ${CMAKE_BINARY_DIR}/src/components/rpc_base/
)


set(LIBRARIES
  policy_struct
  rpc_base
)

set (SOURCES
  main.cpp 
)

add_executable(policyValidator ${SOURCES})
target_link_libraries(policyValidator ${LIBRARIES})
