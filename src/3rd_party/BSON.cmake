set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_SOURCE_PREFIX ${CMAKE_SOURCE_PREFIX} "${3RD_PARTY_INSTALL_PREFIX}")
find_package (BSON)
message (STATUS "bson installed in " ${BSON_LIBS_DIRECTORY} " , " ${BSON_INCLUDE_DIRECTORY})
message (STATUS "emhashmap installed in " ${EMHASHMAP_LIBS_DIRECTORY} " , " ${EMHASHMAP_INCLUDE_DIRECTORY})

if (${BSON_LIB} MATCHES "BSON_LIB-NOTFOUND")
  message (STATUS "Building bson required")
  set(BSON_LIB_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bson_c_lib CACHE INTERNAL "Sources of bson library" FORCE)
  set(BSON_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib CACHE INTERNAL "Installation path of bson libraries" FORCE)
  set(BSON_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include CACHE INTERNAL "Installation path of bson headers" FORCE)
  set(EMHASHMAP_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib CACHE INTERNAL "Installation path of emashmap libraries" FORCE)
  set(EMHASHMAP_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include CACHE INTERNAL "Installation path of emashmap headers" FORCE)

  set(BSON_MAKE     MAKE=${QNX_HOST}/usr/bin/make${HOST_EXECUTABLE_SUFFIX})
  set(BSON__STRIP   STRIP=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-strip${HOST_EXECUTABLE_SUFFIX})
  set(BSON_OBJDUMP  OBJDUMP=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-objdump${HOST_EXECUTABLE_SUFFIX})
  set(BSON_OBJCOPY  OBJCOPY=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-objcopy${HOST_EXECUTABLE_SUFFIX})
  set(BSON_LINKER   LINKER=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ld)
  set(BSON_NM       NM=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-nm${HOST_EXECUTABLE_SUFFIX})
  set(BSON_RANLIB   RANLIB=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ranlib${HOST_EXECUTABLE_SUFFIX})
  set(BSON_AR       AR=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ar${HOST_EXECUTABLE_SUFFIX})
  set(BSON_CXX      CXX=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-g++${HOST_EXECUTABLE_SUFFIX})
  set(BSON_CC       CC=${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-gcc${HOST_EXECUTABLE_SUFFIX})

  set(
    BSON_LIB_CONFIG
    --host=${CMAKE_SYSTEM_PROCESSOR}-linux-gnueabi --target=nto${CMAKE_SYSTEM_PROCESSOR} --bindir=${QNX_HOST}/usr/bin/ --prefix=${3RD_PARTY_INSTALL_PREFIX}
  )
  set(
    BSON_LIB_FLAG
    ${BSON_MAKE} ${BSON__STRIP} ${BSON_OBJDUMP} ${BSON_OBJCOPY} ${BSON_LINKER} ${BSON_NM} ${BSON_RANLIB} ${BSON_AR} ${BSON_AR} ${BSON_CXX} ${BSON_CC} 
  )  

  set(BSON_INSTALL_COMMAND make install)
  if (${3RD_PARTY_INSTALL_PREFIX} MATCHES "/usr/local")
    set(BSON_INSTALL_COMMAND sudo make install)
  endif()
  include(ExternalProject)  
  ExternalProject_Add(libbson
      GIT_REPOSITORY "http://github.com/smartdevicelink/bson_c_lib.git"
      GIT_TAG "master"
      BINARY_DIR ${BSON_LIB_SOURCE_DIRECTORY}
      INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
      DOWNLOAD_DIR ${BSON_LIB_SOURCE_DIRECTORY}
      SOURCE_DIR ${BSON_LIB_SOURCE_DIRECTORY}
      CONFIGURE_COMMAND touch aclocal.m4 configure.ac Makefile.am Makefile.in configure config.h.in && ./configure ${BSON_LIB_CONFIG} ${BSON_LIB_FLAG}
      BUILD_COMMAND make
      INSTALL_COMMAND ${BSON_INSTALL_COMMAND})
else()
  get_filename_component(BSON_LIBS_DIRECTORY ${BSON_LIB} DIRECTORY)
  get_filename_component(EMHASHMAP_LIBS_DIRECTORY ${EMHASHMAP_LIB} DIRECTORY)
  set(BSON_LIBS_DIRECTORY ${BSON_LIBS_DIRECTORY} CACHE INTERNAL "Installation path of bson libraries" FORCE)
  set(EMHASHMAP_LIBS_DIRECTORY ${BSON_LIBS_DIRECTORY} CACHE INTERNAL "Installation path of emashmap libraries" FORCE)
  add_custom_target(
      libbson
      DEPENDS ${BSON_LIBS_DIRECTORY}
      DEPENDS ${EMHASHMAP_LIBS_DIRECTORY}
      )
endif()
