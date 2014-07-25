package com.ford.syncV4.service.secure.mutations;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * Created by admin on 7/2/14.
 */
public interface ISecureServiceMutation {

    public ProtocolMessage mutate(ProtocolMessage protocolMessage);

    public void mutate(ProtocolFrameHeader header, byte[] dataChunk);
}
