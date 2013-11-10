include_directories (
  ./include
  ../utils/include/
  ../protocol_handler/include/
)

set (SOURCES
    ./src/media_adapter_impl.cc
    ./src/a2dp_source_player_adapter.cc
    ./src/media_manager_impl.cc
)

add_library("MediaManagerNew" ${SOURCES} )
