include_directories (
  ./include
  ../utils/include/
  ../../thirdPartyLibs/logger/include/
  ../../thirdPartyLibs/MessageBroker/include/
)

set (SOURCES
    ./src/audio_manager_impl.cc
)

add_library("AudioManager" ${SOURCES})
target_link_libraries ("AudioManager" Utils)
target_link_libraries ("AudioManager" log4cxx)
target_link_libraries ("AudioManager" pulse-simple)
