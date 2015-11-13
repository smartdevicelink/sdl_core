

include_directories (
  ./
)

if(CMAKE_SYSTEM_NAME STREQUAL "WindowsCE")
include_directories (
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/pthread-win32-2.9.1
)
endif()

if(NOT CMAKE_SYSTEM_NAME STREQUAL  "Darwin")
  include_directories(./include)
endif()

set (LIB_VR_SOURCES
   ./lib_msp_vr.cpp
   ./msp_passthru.cpp
   ./fa_fir.c
   ./resample.cc
   ./asr.cpp
   ./msp_vr.cpp
)



add_library("lib_msp_vr" ${LIB_VR_SOURCES})

