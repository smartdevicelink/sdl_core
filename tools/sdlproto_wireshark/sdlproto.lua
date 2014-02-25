-- Put this file into ~/.wireshark/plugins/ directory
-- source: https://delog.wordpress.com/2010/09/27/create-a-wireshark-dissector-in-lua/

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

p_sdlproto.fields = {
  f_version, f_compressionFlag, f_frameType, f_serviceType,
  f_frameInfo, f_frameInfo_ControlFrame, f_frameInfo_ConsecutiveFrame,
  f_dataSize, f_sessionID, f_messageID
}

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
  if f_frameTypeValue_FirstFrame == l_frameType then
    if l_dataSizeValue ~= 0x00000008 then
      l_dataSizeTreeItem:add_expert_info(PI_PROTOCOL, PI_ERROR, "Should be 0x00000008")
    end
  elseif (f_frameTypeValue_ConsecutiveFrame == l_frameType) or
         (f_frameTypeValue_SingleFrame == l_frameType) then
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

  -- TODO subdissector for RPC messages
  -- TODO test for 2+ messages in the same TCP packet
end

-- Initialization routine
function p_sdlproto.init()
end

-- register a chained dissector for certain port
local tcp_port = 12345
local tcp_dissector_table = DissectorTable.get("tcp.port")
tcp_dissector_table:add(tcp_port, p_sdlproto)

-- vim: expandtab softtabstop=2 tabstop=2 shiftwidth=2
