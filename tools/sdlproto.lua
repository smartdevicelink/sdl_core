-- Put this file into ~/.wireshark/plugins/ directory
-- source: https://delog.wordpress.com/2010/09/27/create-a-wireshark-dissector-in-lua/

-- create sdlproto protocol and its fields
p_sdlproto = Proto("sdlproto", "Ford's Smart Device Link Protocol")
local f_command = ProtoField.uint16("sdlproto.command", "Command", base.HEX)
local f_data = ProtoField.string("sdlproto.data", "Data", FT_STRING)

--local f_debug = ProtoField.uint8("sdlproto.debug", "Debug")
p_sdlproto.fields = {f_command}

-- sdlproto dissector function
function p_sdlproto.dissector(buf, pkt, root)
  -- validate packet length is adequate, otherwise quit
  if buf:len() == 0 then return end

  pkt.cols.protocol = p_sdlproto.name

  -- create subtree for sdlproto
  subtree = root:add(p_sdlproto, buf(0))
  -- add protocol fields to subtree
  subtree:add(f_command, buf(0,2)):append_text(" [Command text]")

  -- description of payload
  subtree:append_text(", Command details here or in the tree below")
  -- add debug info if debug field is not nil
  if f_debug then
    -- write debug values
    subtree:add(f_debug, buf:len())
  end
end

-- Initialization routine
function p_sdlproto.init()
end

-- register a chained dissector for certain port
local tcp_port = 12345
local tcp_dissector_table = DissectorTable.get("tcp.port")
--dissector = tcp_dissector_table:get_dissector(tcp_port)
  -- you can call dissector from function p_sdlproto.dissector above
  -- so that the previous dissector gets called
tcp_dissector_table:add(tcp_port, p_sdlproto)
