#set( CMAKE_VERBOSE_MAKEFILE on )

include_directories(
	${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/
        ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include/
        ${CMAKE_SOURCE_DIR}/src/components/utils/include/
	${JSONCPP_INCLUDE_DIRECTORY}
)


if (EXTENDED_POLICY_FLAG)
add_definitions(-DEXTENDED_POLICY)
link_directories (
    ${CMAKE_BINARY_DIR}/src/components/policy/src/policy/policy_table/table_struct_ext/
    ${CMAKE_BINARY_DIR}/src/components/rpc_base/
)
else ()
link_directories (
    ${CMAKE_BINARY_DIR}/src/components/policy/src/policy/policy_table/table_struct/
    ${CMAKE_BINARY_DIR}/src/components/rpc_base/
)
endif ()


set(LIBRARIES
  policy_struct
  rpc_base
)

set (SOURCES
  main.cpp 
)

add_executable(policyValidator ${SOURCES})
target_link_libraries(policyValidator ${LIBRARIES})
