package com.ford.syncV4.service.secure.mutations;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * Created by admin on 7/2/14.
 */
public class InvalidSslDataMutation extends AbstractMutation {

    @Override
    protected void mutateMessage(ProtocolFrameHeader header, byte[] dataChunk) {

    }

    @Override
    protected ProtocolMessage mutateMessage(ProtocolMessage protocolMessage) {
        byte[] result = mutateHandshakeData(protocolMessage.getData());
        protocolMessage.setData(result);
        return protocolMessage;
    }

    private byte[] mutateHandshakeData(byte[] data) {
        for (int i = 0; i < data.length; i++) {
            data[i] = 0xb;
        }
        return data;
    }
}
