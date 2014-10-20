include_directories (
  ./include/
  ../protocol_handler/include/
  ../utils/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
  ../../../src/components/smart_objects/include
)

set (SOURCES
    ./src/CSmartFactory.cpp
)

set (FORMATTER_SOURCES
    ./src/CFormatterJsonBase.cpp
    ./src/CFormatterJsonSDLRPCv1.cpp
    ./src/CFormatterJsonSDLRPCv2.cpp
    ./src/formatter_json_rpc.cc
    ./src/meta_formatter.cc
    ./src/generic_json_formatter.cc
)

add_library("formatters" ${SOURCES}
        ${FORMATTER_SOURCES}
)
