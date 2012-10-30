cmake_minimum_required(VERSION 2.8)

set (PROJECT appLinkCore)
project (${PROJECT})

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)
set(CMAKE_CXX_FLAGS "-O0 -g3 -ggdb3")
set(CMAKE_EXE_LINKER_FLAGS "-m")

# Building application

# --- ThirdPartyLibs
add_subdirectory(./src/thirdPartyLibs)

# --- Components
add_subdirectory(./src/components)

# --- Main application
add_subdirectory(./src/appMain)

# Building tests

# Building documentation
