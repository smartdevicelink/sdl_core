-- Put this file into ~/.wireshark/plugins/ directory
-- source: https://delog.wordpress.com/2010/09/27/create-a-wireshark-dissector-in-lua/

-- print debug info?
local DEBUG = false

--- SDL RPC protocol

local p_rpc_protoname = "sdlproto.rpc"
p_rpc_sdlproto = Proto(p_rpc_protoname, "SDL RPC Service")

local f_rpc_type_values = {
  [0x0] = "Request",
  [0x1] = "Response",
  [0x2] = "Notification"
  -- TODO check the values not in the list
}
-- according to v3.0_revL
local f_rpc_functionID_values = {
  [0x1] = "RegisterAppInterface",
  [0x2] = "UnregisterAppInterface",
  [0x3] = "SetGlobalProperties",
  [0x4] = "ResetGlobalProperties",
  [0x5] = "AddCommand",
  [0x6] = "DeleteCommand",
  [0x7] = "AddSubMenu",
  [0x8] = "DeleteSubMenu",
  [0x9] = "CreateInteractionChoiceSet",
  [0xA] = "PerformInteraction",
  [0xB] = "DeleteInteractionChoiceSet",
  [0xC] = "Alert",
  [0xD] = "Show",
  [0xE] = "Speak",
  [0xF] = "SetMediaClockTimer",
  [0x10] = "PerformAudioPassThru",
  [0x11] = "EndAudioPassThru",
  [0x12] = "SubscribeButton",
  [0x13] = "UnsubscribeButton",
  [0x14] = "SubscribeVehicleData",
  [0x15] = "UnsubscribeVehicleData",
  [0x16] = "GetVehicleData",
  [0x17] = "ReadDID",
  [0x18] = "GetDTCs",
  [0x19] = "ScrollableMessage",
  [0x1A] = "Slider",
  [0x1B] = "ShowConstantTBT",
  [0x1C] = "AlertManeuver",
  [0x1D] = "UpdateTurnList",
  [0x1E] = "ChangeRegistration",
  [0x1F] = "GenericResponse",
  [0x20] = "PutFile",
  [0x21] = "DeleteFile",
  [0x22] = "ListFiles",
  [0x23] = "SetAppIcon",
  [0x24] = "SetDisplayLayout",
  [0x25] = "DiagnosticMessage",
  [0x26] = "SystemRequest",
  [0x8000] = "OnHMIStatus",
  [0x8001] = "OnAppInterfaceUnregistered",
  [0x8002] = "OnButtonEvent",
  [0x8003] = "OnButtonPress",
  [0x8004] = "OnVehicleData",
  [0x8005] = "OnCommand",
  [0x8006] = "OnTBTClientState",
  [0x8007] = "OnDriverDistraction",
  [0x8008] = "OnPermissionsChange",
  [0x8009] = "OnAudioPassThru",
  [0x800A] = "OnLanguageChange",
  [0x800B] = "OnKeyboardInput",
  [0x800C] = "OnTouchEvent",
  [0x800D] = "OnSystemRequest",
  [0x10000] = "EncodedSyncPData",
  [0x10001] = "SyncPData",
  [0x18000] = "OnEncodedSyncPData",
  [0x18001] = "OnSyncPData"
  -- TODO check the values not in the list
}

local f_rpc_type = ProtoField.uint8(p_rpc_protoname..".type", "RPC Type", base.HEX, f_rpc_type_values, 0xF0)
local f_rpc_functionID = ProtoField.uint32(p_rpc_protoname..".functionID", "RPC Function ID", base.HEX, f_rpc_functionID_values, 0x0FFFFFFF)
local f_rpc_correlationID = ProtoField.uint32(p_rpc_protoname..".correlationID", "RPC Correlation ID", base.HEX)
local f_rpc_jsonSize = ProtoField.uint32(p_rpc_protoname..".jsonSize", "JSON Size", base.DEC)
local f_rpc_json = ProtoField.string(p_rpc_protoname..".json", "JSON")
local f_rpc_data = ProtoField.bytes(p_rpc_protoname..".data", "Data")

p_rpc_sdlproto.fields = {
  f_rpc_type, f_rpc_functionID, f_rpc_correlationID, f_rpc_jsonSize, f_rpc_json,
  f_rpc_data
}

local RPC_HEADER_LENGTH = 12

-- rpc_sdlproto dissector function
function p_rpc_sdlproto.dissector(buf, pkt, root)
  -- validate packet length is adequate, otherwise quit
  if buf:len() == 0 then return end

  --pkt.cols.protocol = p_rpc_protoname.name

  -- create subtree for rpc_sdlproto
  subtree = root:add(p_rpc_sdlproto, buf(0))

  -- add protocol fields to subtree
  subtree:add(f_rpc_type, buf(0, 1))
  subtree:add(f_rpc_functionID, buf(0, 4))
  subtree:add(f_rpc_correlationID, buf(4, 4))

  local l_rpc_jsonSizeBuf = buf(8, 4)
  local l_rpc_jsonSizeValue = l_rpc_jsonSizeBuf:uint()
  subtree:add(f_rpc_jsonSize, l_rpc_jsonSizeBuf)

  -- after the header, we should have some data (JSON and/or binary)
  local l_dataOffset = RPC_HEADER_LENGTH
  local l_bytesLeft = buf:len() - l_dataOffset
  if l_bytesLeft > 0 then
    -- we do have something
    if l_rpc_jsonSizeValue > 0 then
      -- if we expect to have JSON, calculate how much we have
      local l_rpc_jsonSizeAvailable = math.min(l_bytesLeft, l_rpc_jsonSizeValue)
      subtree:add(f_rpc_json, buf(l_dataOffset, l_rpc_jsonSizeAvailable))
      l_dataOffset = l_dataOffset + l_rpc_jsonSizeAvailable
    end

    if l_dataOffset < buf:len() then
      -- we have something after JSON
      subtree:add(f_rpc_data, buf(l_dataOffset))
    end
  end
end


--- SDL protocol

-- create sdlproto protocol and its fields
local p_protoname = "sdlproto"
p_sdlproto = Proto(p_protoname, "Ford's Smart Device Link Protocol")

local f_frameTypeValue_ControlFrame = 0x00
local f_frameTypeValue_SingleFrame = 0x01
local f_frameTypeValue_FirstFrame = 0x02
local f_frameTypeValue_ConsecutiveFrame = 0x03
local f_frameTypeValues = {
  [f_frameTypeValue_ControlFrame] = "Control Frame",
  [f_frameTypeValue_SingleFrame] = "Single Frame",
  [f_frameTypeValue_FirstFrame] = "First Frame",
  [f_frameTypeValue_ConsecutiveFrame] = "Consecutive Frame"
  -- TODO check the values not in the list
}
local f_serviceTypeValue_RPC = 0x07
local f_serviceTypeValue_Bulk = 0x0F
local f_serviceTypeValues = {
  [f_serviceTypeValue_RPC] = "Remote Procedure Call [RPC Service]",
  [f_serviceTypeValue_Bulk] = "Bulk Data [Hybrid Service]"
  -- TODO check the values not in the list
}
local f_frameInfoValue_StartServiceACK = 0x02
local f_frameInfoValue_ServiceDataACK = 0xFE
local f_frameInfoValues_ControlFrame = {
  -- this only applies when f_frameType == ControlFrame
  [0x00] = "Heart Beat",
  [0x01] = "Start Service",
  [f_frameInfoValue_StartServiceACK] = "Start Service ACK",
  [0x03] = "Start Service NACK",
  [0x04] = "End Service",
  [0x05] = "End Service ACK",
  [0x06] = "End Service NACK",
  [f_frameInfoValue_ServiceDataACK] = "Service Data ACK",
  [0xFF] = "Heart Beat ACK"
  -- TODO check the values not in the list
}
local f_frameInfoValues_ConsecutiveFrame = {
  -- this only applies when f_frameType == ConsecutiveFrame
  [0x00] = "Last frame in sequence"
  -- TODO check the values not in the list
}

local f_version = ProtoField.uint8(p_protoname..".version", "Protocol Version", base.DEC, nil, 0xF0)
local f_compressionFlag = ProtoField.bool(p_protoname..".compression_flag", "Compression Flag", 8, nil, 0x08)
local f_frameType = ProtoField.uint8(p_protoname..".frame_type", "Frame Type", base.HEX, f_frameTypeValues, 0x07)
local f_serviceType = ProtoField.uint8(p_protoname..".service_type", "Service Type", base.HEX, f_serviceTypeValues)
local f_frameInfo = ProtoField.uint8(p_protoname..".frame_info", "Frame Info", base.HEX)
local f_frameInfo_ControlFrame = ProtoField.uint8(p_protoname..".frame_info", "Frame Info", base.HEX, f_frameInfoValues_ControlFrame)
local f_frameInfo_ConsecutiveFrame = ProtoField.uint8(p_protoname..".frame_info", "Frame Info", base.HEX, f_frameInfoValues_ConsecutiveFrame)
local f_dataSize = ProtoField.uint32(p_protoname..".data_size", "Data Size", base.DEC)
local f_sessionID = ProtoField.uint8(p_protoname..".session_id", "Session ID", base.HEX)
local f_messageID = ProtoField.uint32(p_protoname..".message_id", "Message ID", base.HEX)
local f_totalDataBytes = ProtoField.uint32(p_protoname..".total_data_bytes", "Total Data Bytes", base.DEC)
local f_numberOfConsecutiveFrames = ProtoField.uint32(p_protoname..".number_of_consecutive_frames", "Number of consecutive frames", base.DEC)
local f_data = ProtoField.bytes(p_protoname..".data", "Data")

p_sdlproto.fields = {
  f_version, f_compressionFlag, f_frameType, f_serviceType,
  f_frameInfo, f_frameInfo_ControlFrame, f_frameInfo_ConsecutiveFrame,
  f_dataSize, f_sessionID, f_messageID,
  f_totalDataBytes, f_numberOfConsecutiveFrames,
  f_data
}

local HEADER_LENGTH = 12
local FIRSTFRAME_DATASIZE = 0x00000008

-- how many bytes left that are data of the latest packet
local g_expectedBytesLeft = 0

-- sdlproto dissector function
function p_sdlproto.dissector(buf, pkt, root)
  -- validate packet length is adequate, otherwise quit
  if buf:len() == 0 then return end

  pkt.cols.protocol = p_sdlproto.name

  -- create subtree for sdlproto
  subtree = root:add(p_sdlproto, buf(0))

  -- add protocol fields to subtree
  local l_byte0 = buf(0, 1)
  local l_versionTreeItem = subtree:add(f_version, l_byte0)

  local l_version = l_byte0:bitfield(0, 4)
  if l_version ~= 2 then
    l_versionTreeItem:add_expert_info(PI_REQUEST_CODE, PI_WARN, "Only version 2 is supported (maybe the message is incorrect?)")
  end

  subtree:add(f_compressionFlag, l_byte0)
  subtree:add(f_frameType, l_byte0)
  local l_frameTypeValue = l_byte0:bitfield(5, 3)

  local l_serviceTypeBuf = buf(1, 1)
  local l_serviceTypeValue = l_serviceTypeBuf:uint()
  subtree:add(f_serviceType, l_serviceTypeBuf)

  -- Frame Info depends on Frame Type
  local l_frameInfoBuf = buf(2, 1)
  local l_frameInfoValue = l_frameInfoBuf:uint()
  local l_isFirstConsecutiveFrame = false
  if f_frameTypeValue_ControlFrame == l_frameTypeValue then
    subtree:add(f_frameInfo_ControlFrame, l_frameInfoBuf)
  elseif f_frameTypeValue_ConsecutiveFrame == l_frameTypeValue then
    subtree:add(f_frameInfo_ConsecutiveFrame, l_frameInfoBuf)

    -- TODO check for value overflow, when the value is also 0x01
    if l_frameInfoValue == 0x01 then
      l_isFirstConsecutiveFrame = true
    end
  else
    local l_frameInfoTreeItem = subtree:add(f_frameInfo, l_frameInfoBuf)

    if l_frameInfoValue ~= 0x00 then
      l_frameInfoTreeItem:add_expert_info(PI_PROTOCOL, PI_WARN, "Reserved, should be 0x00")
    end
  end

  subtree:add(f_sessionID, buf(3, 1))

  -- Data Size depends on Frame Type
  local l_dataSizeBuf = buf(4, 4)
  local l_dataSizeValue = l_dataSizeBuf:uint()
  local l_dataSizeTreeItem = subtree:add(f_dataSize, l_dataSizeBuf)
  local l_isFirstFrame = false
  if f_frameTypeValue_FirstFrame == l_frameTypeValue then
    if l_dataSizeValue == FIRSTFRAME_DATASIZE then
      g_expectedBytesLeft = l_dataSizeValue
      l_isFirstFrame = true
    else
      l_dataSizeTreeItem:add_expert_info(PI_PROTOCOL, PI_ERROR, "Should be " .. FIRSTFRAME_DATASIZE)
    end
  elseif (f_frameTypeValue_ConsecutiveFrame == l_frameTypeValue) or
         (f_frameTypeValue_SingleFrame == l_frameTypeValue) then
    g_expectedBytesLeft = l_dataSizeValue
    if DEBUG then print("set g_expectedBytesLeft to " .. g_expectedBytesLeft) end
    l_dataSizeTreeItem:append_text(" -- Total data bytes in this frame")

    if l_dataSizeValue < 0x00000001 then
      l_dataSizeTreeItem:add_expert_info(PI_PROTOCOL, PI_ERROR, "Should be >= 0x00000001")
    end
  else
    if l_dataSizeValue ~= 0x00000000 then
      l_dataSizeTreeItem:add_expert_info(PI_PROTOCOL, PI_WARN, "Reserved, should be 0x00000000")
    end
  end

  -- Message ID depends on Frame Data (value of Frame Info)
  local l_messageIDBuf = buf(8, 4)
  local l_messageIDValue = l_messageIDBuf:uint()
  local l_frameInfo = l_frameInfoBuf
  local l_messageIDTreeItem = subtree:add(f_messageID, l_messageIDBuf)
  local l_messageIDDescription = ""
  if f_frameInfoValue_StartServiceACK == l_frameInfo then
    l_messageIDDescription = " -- Hash code for End Session"
  elseif f_frameInfoValue_ServiceDataACK == l_frameInfo then
    l_messageIDDescription = " -- A control message for a given service. Please see the spec for more information."
  else
    l_messageIDDescription = " -- Identifier of a message sent using SDL layer"
  end
  l_messageIDTreeItem:append_text(l_messageIDDescription)
  if l_messageIDValue < 0x00000001 then
    l_messageIDTreeItem:add_expert_info(PI_PROTOCOL, PI_WARN, "Should be >= 0x00000001")
  end

  local l_bytesLeft = buf:len() - HEADER_LENGTH
  if g_expectedBytesLeft > 0 then
    -- we expect more bytes for this message
    if g_expectedBytesLeft > l_bytesLeft then
      -- we have fewer bytes that we need; ask wireshark to supply more
      pkt.desegment_len = g_expectedBytesLeft - l_bytesLeft
      if DEBUG then print("want " .. pkt.desegment_len .. " more bytes") end
      return
    else
      -- special case with First Frame
      if l_isFirstFrame then
        local l_totalDataBytesBuf = buf(12, 4)
        subtree:add(f_totalDataBytes, l_totalDataBytesBuf)

        local l_numberOfConsecutiveFramesBuf = buf(16, 4)
        subtree:add(f_numberOfConsecutiveFrames, l_numberOfConsecutiveFramesBuf)

        g_expectedBytesLeft = 0
      else
        local l_dataLengthAvailable = math.min(g_expectedBytesLeft, l_bytesLeft)
        if DEBUG then print("expect ".. g_expectedBytesLeft .. ", and have " .. l_bytesLeft .. ", and use " .. l_dataLengthAvailable) end
        local l_dataBuf = buf(HEADER_LENGTH, l_dataLengthAvailable)

        local l_isServiceTypeRPC =
          (f_serviceTypeValue_RPC == l_serviceTypeValue) or
          (f_serviceTypeValue_Bulk == l_serviceTypeValue)
        local l_isSingleFrame = (f_frameTypeValue_SingleFrame == l_frameTypeValue)
        if l_isServiceTypeRPC and
          (l_isFirstConsecutiveFrame or l_isSingleFrame) then
          Dissector.get(p_rpc_protoname):call(l_dataBuf:tvb(), pkt, subtree)
        else
          subtree:add(f_data, l_dataBuf)
        end

        -- make this packet look shorter in case there unprocessed bytes after it
        subtree:set_len(HEADER_LENGTH + l_dataLengthAvailable)

        g_expectedBytesLeft = g_expectedBytesLeft - l_dataLengthAvailable
        if DEBUG then print("updated g_expectedBytesLeft to " .. g_expectedBytesLeft) end

        -- are there any more unprocessed bytes on wire? dissect them
        l_bytesLeft = l_bytesLeft - l_dataLengthAvailable
        if l_bytesLeft > 0 then
          Dissector.get(p_protoname):call(buf(l_bytesLeft):tvb(), pkt, root)
        end
      end
    end
  end
end

-- Initialization routine
function p_sdlproto.init()
end

-- register a chained dissector for certain port
local tcp_port = 12345
local tcp_dissector_table = DissectorTable.get("tcp.port")
tcp_dissector_table:add(tcp_port, p_sdlproto)

-- attempt to register the dissector for the standard USB AOA vendor&product IDs
-- (requires wireshark 1.10.0+)
-- this may fail if wireshark doesn't get device descriptors
local success, data = pcall(function() return DissectorTable.get("usb.product") end)
if success then
  local usb_product_dissector_table = data
  for id = 0x18d12d00, 0x18d12d05 do
    usb_product_dissector_table:add(id, p_sdlproto)
  end
else
  -- pre wireshark 1.10.0
  -- register the dissector for USB device class

  -- from packet-usb.h
  IF_CLASS_DEVICE = 0x00
  IF_CLASS_VENDOR_SPECIFIC = 0xFF
  IF_CLASS_UNKNOWN = 0xFFFF

  local usb_bulk_dissector_table = DissectorTable.get("usb.bulk")
  usb_bulk_dissector_table:add(IF_CLASS_DEVICE, p_sdlproto)
  usb_bulk_dissector_table:add(IF_CLASS_VENDOR_SPECIFIC, p_sdlproto)
  usb_bulk_dissector_table:add(IF_CLASS_UNKNOWN, p_sdlproto)
end

-- vim: expandtab softtabstop=2 tabstop=2 shiftwidth=2
