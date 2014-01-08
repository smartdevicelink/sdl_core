-- Put this file into ~/.wireshark/plugins/ directory
-- source: https://delog.wordpress.com/2010/09/27/create-a-wireshark-dissector-in-lua/

-- create sdlproto protocol and its fields
local p_protoname = "sdlproto"
p_sdlproto = Proto(p_protoname, "Ford's Smart Device Link Protocol")
local f_version = ProtoField.uint8(p_protoname..".version", "Protocol Version", base.DEC, nil, 0xF0)
local f_compressionFlag = ProtoField.bool(p_protoname..".compression_flag", "Compression Flag", 8, nil, 0x08)
local f_frameType = ProtoField.uint8(p_protoname..".frame_type", "Frame Type", base.HEX, {
    [0x00] = "Control Frame",
    [0x01] = "Single Frame",
    [0x02] = "First Frame",
    [0x03] = "Consecutive Frame"
    -- TODO check the values not in the list
}, 0x07)
local f_serviceType = ProtoField.uint8(p_protoname..".service_type", "Service Type", base.HEX, {
    [0x07] = "Remote Procedure Call [RPC Service]",
    [0x0F] = "Bulk Data [Hybrid Service]"
    -- TODO check the values not in the list
})
-- TODO add Frame Info depending on Frame Type
local f_sessionID = ProtoField.uint8(p_protoname..".session_id", "Session ID", base.HEX)

p_sdlproto.fields = {
    f_version, f_compressionFlag, f_frameType, f_serviceType, f_sessionID
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
  subtree:add(f_version, l_byte0):append_text(" (Only version 2 is supported)")
  subtree:add(f_compressionFlag, l_byte0)
  subtree:add(f_frameType, l_byte0)

  subtree:add(f_serviceType, buf(1, 1))
  subtree:add(f_sessionID, buf(3, 1))
end

-- Initialization routine
function p_sdlproto.init()
end

-- register a chained dissector for certain port
local tcp_port = 12345
local tcp_dissector_table = DissectorTable.get("tcp.port")
tcp_dissector_table:add(tcp_port, p_sdlproto)
