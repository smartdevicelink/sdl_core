set(
BOOST_QNX_PROJECT_CONFIG_JAM
"using gcc : nto${CMAKE_SYSTEM_PROCESSOR} : ${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-g++${HOST_EXECUTABLE_SUFFIX} : -L${QNX_HOST}/usr/lib -I${QNX_HOST}/usr/include")
set(
  BOOST_GCC_JAM
  sed -Ei "s/case darwin/case *qnx*/g" ./tools/build/src/tools/gcc.jam)
set(
  BOOST_FILESYSTEM_OPERATION
  sed -Ei "s/__SUNPRO_CC/__QNX__/g" ./libs/filesystem/src/operations.cpp)
set(
BOOTSTRAP
./bootstrap.sh --with-toolset=gcc --with-libraries=system,thread,date_time,filesystem --prefix=${3RD_PARTY_INSTALL_PREFIX})

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES ".*aarch64")
  set(ADDRESS_MODEL "64")
else()  
  set(ADDRESS_MODEL "32")
endif ()

set(BOOST_BUILD_COMMAND ./b2 address-model=${ADDRESS_MODEL} cxxflags="-std=gnu++11" linkflags="-stdlib=libstdc++" target-os=qnxnto toolset=gcc-nto${CMAKE_SYSTEM_PROCESSOR} define=__QNXNTO__)
set(BOOST_INSTALL_COMMAND ${BOOST_BUILD_COMMAND} install > boost_install.log)
if (${3RD_PARTY_INSTALL_PREFIX} MATCHES "/usr/local")
  set(BOOST_INSTALL_COMMAND sudo  ${BOOST_BUILD_COMMAND} install > boost_install.log)
endif()
include(ExternalProject)  
ExternalProject_Add(
  Boost
  URL https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
  DOWNLOAD_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
  SOURCE_DIR ${BOOST_LIB_SOURCE_DIRECTORY}  
  CONFIGURE_COMMAND  ${BOOST_GCC_JAM} COMMAND ${BOOST_FILESYSTEM_OPERATION} COMMAND ${BOOTSTRAP}
  BUILD_COMMAND echo ${BOOST_QNX_PROJECT_CONFIG_JAM} $<SEMICOLON> >> ./project-config.jam COMMAND ${BOOST_BUILD_COMMAND}
  INSTALL_COMMAND ${BOOST_INSTALL_COMMAND}
  INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
  BUILD_IN_SOURCE true
)