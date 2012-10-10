cmake_minimum_required(VERSION 2.8)

set (PROJECT appMain)
project (${PROJECT}) 

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)
set(LIBRARIES
        bluetooth
        asound)

include_directories (./)
set (SOURCES
        appMain.cpp
        CBTAdapter.cpp)

add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})

install (TARGETS ${PROJECT} DESTINATION bin)
