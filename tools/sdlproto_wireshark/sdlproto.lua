-- Put this file into ~/.wireshark/plugins/ directory
-- source: https://delog.wordpress.com/2010/09/27/create-a-wireshark-dissector-in-lua/

-- print debug info?
local DEBUG = false

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
local f_serviceTypeValues = {
  [0x07] = "Remote Procedure Call [RPC Service]",
  [0x0F] = "Bulk Data [Hybrid Service]"
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

  subtree:add(f_serviceType, buf(1, 1))

  -- Frame Info depends on Frame Type
  local l_frameType = l_byte0:bitfield(5, 3)
  local l_frameInfoBytes = buf(2, 1)
  if f_frameTypeValue_ControlFrame == l_frameType then
    subtree:add(f_frameInfo_ControlFrame, l_frameInfoBytes)
  elseif f_frameTypeValue_ConsecutiveFrame == l_frameType then
    subtree:add(f_frameInfo_ConsecutiveFrame, l_frameInfoBytes)
  else
    local l_frameInfoTreeItem = subtree:add(f_frameInfo, l_frameInfoBytes)

    if l_frameInfoBytes:uint() ~= 0x00 then
      l_frameInfoTreeItem:add_expert_info(PI_PROTOCOL, PI_WARN, "Reserved, should be 0x00")
    end
  end

  subtree:add(f_sessionID, buf(3, 1))

  -- Data Size depends on Frame Type
  local l_dataSizeBytes = buf(4, 4)
  local l_dataSizeValue = l_dataSizeBytes:uint()
  local l_dataSizeTreeItem = subtree:add(f_dataSize, l_dataSizeBytes)
  local l_isFirstFrame = false
  if f_frameTypeValue_FirstFrame == l_frameType then
    if l_dataSizeValue == FIRSTFRAME_DATASIZE then
      g_expectedBytesLeft = l_dataSizeValue
      l_isFirstFrame = true
    else
      l_dataSizeTreeItem:add_expert_info(PI_PROTOCOL, PI_ERROR, "Should be " .. FIRSTFRAME_DATASIZE)
    end
  elseif (f_frameTypeValue_ConsecutiveFrame == l_frameType) or
         (f_frameTypeValue_SingleFrame == l_frameType) then
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
  local l_messageIDBytes = buf(8, 4)
  local l_messageIDValue = l_messageIDBytes:uint()
  local l_frameInfo = l_frameInfoBytes
  local l_messageIDTreeItem = subtree:add(f_messageID, l_messageIDBytes)
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
        local l_totalDataBytesBytes = buf(12, 4)
        subtree:add(f_totalDataBytes, l_totalDataBytesBytes)

        local l_numberOfConsecutiveFramesBytes = buf(16, 4)
        subtree:add(f_numberOfConsecutiveFrames, l_numberOfConsecutiveFramesBytes)

        g_expectedBytesLeft = 0
      else
        local l_dataLengthAvailable = math.min(g_expectedBytesLeft, l_bytesLeft)
        if DEBUG then print("expect ".. g_expectedBytesLeft .. ", and have " .. l_bytesLeft .. ", and use " .. l_dataLengthAvailable) end
        local l_dataBytes = buf(HEADER_LENGTH, l_dataLengthAvailable)
        subtree:add(f_data, l_dataBytes)

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

  -- TODO subdissector for RPC messages
end

-- Initialization routine
function p_sdlproto.init()
end

-- register a chained dissector for certain port
local tcp_port = 12345
local tcp_dissector_table = DissectorTable.get("tcp.port")
tcp_dissector_table:add(tcp_port, p_sdlproto)

-- attempt to register the dissector for the standard USB AOA vendor&product
-- (requires wireshark 1.10.0+)
-- this may fail if wireshark doesn't get device descriptors
local success, data = pcall(function() return DissectorTable.get("usb.product") end)
if success then
  local usb_product_dissector_table = data
  usb_product_dissector_table:add(0x18d12d01, p_sdlproto)
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
