cmake_minimum_required(VERSION 2.8)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/intergen)
execute_process(COMMAND cmake ${CMAKE_CURRENT_SOURCE_DIR}/intergen
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/intergen)
execute_process(COMMAND cmake --build ${CMAKE_CURRENT_BINARY_DIR}/intergen)
