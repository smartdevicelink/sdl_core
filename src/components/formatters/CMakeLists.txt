include_directories (
  ./include/
  ../TransportManager/include/
  ../protocol_handler/include/
  ../utils/include/
  ../../thirdPartyLibs/jsoncpp/include/
  ../../thirdPartyLibs/MessageBroker/include
  ../../thirdPartyLibs/logger/include
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
