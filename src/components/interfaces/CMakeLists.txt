include_directories (
  ../utils/include/
  ../formatters/include
)

#GenerateInterface("v4_protocol_v1_2_no_extra.xml" "NsSmartDeviceLinkRPC::V1" "alrpcv1")
GenerateInterface("MOBILE_API.xml" "mobile_apis" "sdlrpcv2")
GenerateInterface("HMI_API.xml" "hmi_apis" "jsonrpc")
