package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.util.logger.Logger;

import java.util.ArrayList;
import java.util.List;

/**
 * A default converter that maps one RPCRequest into one ProtocolMessage.
 * Extracted from SyncProxyBase.
 *
 * Created by enikolsky on 2014-01-20.
 */
public class DefaultRPCRequestConverter implements IRPCRequestConverter {

    private static final String CLASS_NAME = DefaultRPCRequestConverter.class.getSimpleName();

    @Override
    public List<ProtocolMessage> getProtocolMessages(RPCRequest request,
                                                     byte sessionID,
                                                     IJsonRPCMarshaller marshaller,
                                                     byte protocolVersion) {
        Logger.d(CLASS_NAME + " DefaultMarshaller converter: " + marshaller);

        byte[] msgBytes = marshaller.marshall(request, protocolVersion);

        ProtocolMessage pm = new ProtocolMessage();
        pm.setVersion(protocolVersion);
        pm.setData(msgBytes);
        pm.setSessionID(sessionID);
        pm.setMessageType(MessageType.RPC);
        pm.setSessionType(ServiceType.RPC);
        pm.setFunctionID(FunctionID.getFunctionID(request.getFunctionName()));
        pm.setCorrID(request.getCorrelationID());
        if (request.getBulkData() != null) {
            pm.setBulkData(request.getBulkData());
        }

        List<ProtocolMessage> res = new ArrayList<ProtocolMessage>(1);
        res.add(pm);
        return res;
    }


}
