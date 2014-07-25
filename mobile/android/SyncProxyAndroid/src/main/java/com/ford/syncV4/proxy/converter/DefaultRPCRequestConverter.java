package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCRequest;

import java.util.ArrayList;
import java.util.List;

/**
 * A default converter that maps one RPCRequest into one ProtocolMessage.
 * Extracted from SyncProxyBase.
 *
 * Created by enikolsky on 2014-01-20.
 */
public class DefaultRPCRequestConverter implements IRPCRequestConverter {

    @SuppressWarnings("unused")
    private static final String CLASS_NAME = DefaultRPCRequestConverter.class.getSimpleName();

    @Override
    public List<ProtocolMessage> getProtocolMessages(RPCRequest request,
                                                     byte sessionId,
                                                     IJsonRPCMarshaller marshaller,
                                                     byte protocolVersion) {

        List<ProtocolMessage> messagesList = new ArrayList<ProtocolMessage>(1);
        messagesList.add(createProtocolMessage(request, sessionId, protocolVersion, MessageType.RPC,
                ServiceType.RPC, request.getBulkData(), marshaller));
        return messagesList;
    }

    /**
     * Factory method to convert {@link com.ford.syncV4.proxy.RPCRequest} into
     * {@link com.ford.syncV4.protocol.ProtocolMessage}
     *
     * @param request         {@link com.ford.syncV4.proxy.RPCRequest} object
     * @param sessionId       Session Id
     * @param protocolVersion Version of the used protocol
     * @param messageType     Type of the Message
     * @param serviceType     Type of the Service
     * @param bulkData        Bytes array of the bulk data
     * @param marshaller      Marshaller to convert {@link com.ford.syncV4.proxy.RPCRequest} into
     *                        {@link com.ford.syncV4.protocol.ProtocolMessage}
     *
     * @return converted {@link com.ford.syncV4.protocol.ProtocolMessage}
     */
    public static ProtocolMessage createProtocolMessage(RPCRequest request,
                                                        byte sessionId,
                                                        byte protocolVersion,
                                                        MessageType messageType,
                                                        ServiceType serviceType,
                                                        byte[] bulkData,
                                                        IJsonRPCMarshaller marshaller) {
        byte[] msgBytes = marshaller.marshall(request, protocolVersion);

        ProtocolMessage protocolMessage = new ProtocolMessage();
        protocolMessage.setVersion(protocolVersion);
        protocolMessage.setData(msgBytes);
        protocolMessage.setEncrypted(request.isDoEncryption());
        protocolMessage.setSessionID(sessionId);
        protocolMessage.setMessageType(messageType);
        protocolMessage.setServiceType(serviceType);
        protocolMessage.setFunctionID(FunctionID.getFunctionID(request.getFunctionName()));
        protocolMessage.setCorrID(request.getCorrelationID());
        if (bulkData != null) {
            protocolMessage.setBulkData(bulkData);
        }

        return protocolMessage;
    }
}