include_directories (
  ./include/
  ../TransportManager/include/
  ../protocol_handler/include/
  ../utils/include/
  ../../thirdPartyLibs/jsoncpp/include/
  ../../thirdPartyLibs/MessageBroker/include
  ../../thirdPartyLibs/logger/include
  ../../../src/components/SmartObjects/include
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
)

add_library("JSONHandler" ${SOURCES}
        ${FORMATTER_SOURCES}
)
