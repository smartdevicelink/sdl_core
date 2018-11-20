set(BOOST_INSTALL_COMMAND ./b2 install)
if (${3RD_PARTY_INSTALL_PREFIX} MATCHES "/usr/local")
  set(BOOST_INSTALL_COMMAND sudo ./b2 install)
endif()
include(ExternalProject)
ExternalProject_Add(
  Boost
  URL http://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
  DOWNLOAD_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
  SOURCE_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
  CONFIGURE_COMMAND ./bootstrap.sh --with-libraries=system,thread,date_time,filesystem --prefix=${3RD_PARTY_INSTALL_PREFIX}
  BUILD_COMMAND ./b2
  INSTALL_COMMAND ${BOOST_INSTALL_COMMAND} --with-system --with-thread --with-date_time --with-filesystem --prefix=${3RD_PARTY_INSTALL_PREFIX} > boost_install.log
  INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
  BUILD_IN_SOURCE true
)