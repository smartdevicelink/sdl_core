package com.ford.syncV4.service.secure.mutations;

import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * Created by admin on 7/2/14.
 */
public class InvalidSslDataMutation extends  AbstractMutation{

    @Override
    protected ProtocolMessage mutateMessage(ProtocolMessage protocolMessage) {
         byte[] result = mutateHandshakeData(protocolMessage.getData());
        protocolMessage.setData(result);
        return null;
    }

    private byte[] mutateHandshakeData(byte[] data){
        for ( int i =0; i < data.length; i++ ){
            data[i]= (byte) (data[i] << 0xb);
        }
        return data;
    }
}
