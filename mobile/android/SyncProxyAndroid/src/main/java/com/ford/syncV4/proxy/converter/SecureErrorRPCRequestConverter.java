package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.protocol.secure.secureproxy.SecureInternalErrorMessage;
import com.ford.syncV4.proxy.RPCRequest;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Andrew Batutin on 6/24/14.
 */
public class SecureErrorRPCRequestConverter implements IRPCRequestConverter {
    @Override
    public List<ProtocolMessage> getProtocolMessages(RPCRequest request, byte sessionID, IJsonRPCMarshaller marshaller, byte protocolVersion) {
        byte[] msgBytes = marshaller.marshall(request, protocolVersion);
        int securityInternalError = ((SecureInternalErrorMessage)request).getErrorId();
        byte[] payload = new byte[msgBytes.length + 1];
        System.arraycopy(new byte[]{(byte) securityInternalError}, 0, payload, 0, 1);
        System.arraycopy(msgBytes, 0, payload, 1, msgBytes.length);
        ProtocolMessage pm = new ProtocolMessage();
        pm.setVersion(protocolVersion);
        pm.setData(payload);
        pm.setEncrypted(request.isDoEncryption());
        pm.setSessionID(sessionID);
        pm.setServiceType(ServiceType.Heartbeat);
        pm.setRPCType(ProtocolMessage.RPCTYPE_NOTIFICATION);
        pm.setFunctionID(2);
        pm.setCorrID(request.getCorrelationID());
        if (request.getBulkData() != null) {
            pm.setBulkData(request.getBulkData());
        }

        List<ProtocolMessage> res = new ArrayList<ProtocolMessage>(1);
        res.add(pm);
        return res;
    }
}
