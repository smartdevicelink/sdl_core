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
    ./src/formatters/CFormatterJsonBase.cpp
    ./src/formatters/CFormatterJsonSDLRPCv1.cpp
    ./src/formatters/CFormatterJsonSDLRPCv2.cpp
    ./src/formatters/formatter_json_rpc.cc
    ./src/formatters/meta_formatter.cc
)

add_library("JSONHandler" ${SOURCES}
        ${FORMATTER_SOURCES}
)
