add_custom_command(OUTPUT ${APR_BUILD_DIRECTORY}/Makefile
  COMMAND CC=${CMAKE_C_COMPILER} ${APR_SOURCE_DIRECTORY}/configure
  COMMAND ${CMAKE_COMMAND} -E copy include/apr.h ${APR_SOURCE_DIRECTORY}/include  
  WORKING_DIRECTORY ${APR_BUILD_DIRECTORY}
)

add_custom_command(OUTPUT ${APR_BUILD_DIRECTORY}/include/private/apr_escape_test_char.h
  DEPENDS ${APR_BUILD_DIRECTORY}/Makefile
  COMMAND make include/private/apr_escape_test_char.h 
  COMMAND ${CMAKE_COMMAND} -E copy include/private/apr_escape_test_char.h ${APR_SOURCE_DIRECTORY}/include/private
  WORKING_DIRECTORY ${APR_BUILD_DIRECTORY}
)

add_custom_target(libapr-1 
  ALL make
  DEPENDS ${APR_BUILD_DIRECTORY}/include/private/apr_escape_test_char.h
  WORKING_DIRECTORY ${APR_BUILD_DIRECTORY}
)