include_directories (
  ../utils/include/
)

#GenerateInterface("v4_protocol_v1_2_no_extra.xml" "NsSmartDeviceLinkRPC::V1" "alrpcv1")
GenerateInterface("v4_protocol_v2_0_revT.xml" "NsSmartDeviceLinkRPC::V2" "sdlrpcv2")
GenerateInterface("HMI_API.xml" "hmi_apis" "jsonrpc")
