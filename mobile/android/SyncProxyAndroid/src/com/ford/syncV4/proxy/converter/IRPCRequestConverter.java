package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.proxy.RPCRequest;

import java.util.List;

/**
 * The interface for converters from an RPCRequest object into an appropriate
 * number of ProtocolMessage objects.
 *
 * Created by enikolsky on 2014-01-20.
 */
public interface IRPCRequestConverter {
    /**
     * Converts a given RPCRequest object into a number of ProtocolMessage
     * objects to send. The return list can be empty, and can't be null.
     *
     * @param request         request to convert; must not be null
     * @param sessionID       RPC session ID
     * @param marshaller      is used to serialize the request's parameters;
     *                        must not be null
     * @param protocolVersion protocol version to serialize the parameters
     * @return a list of protocol messages; can't be null
     */
    public List<ProtocolMessage> getProtocolMessages(RPCRequest request,
                                                     byte sessionID,
                                                     IJsonRPCMarshaller marshaller,
                                                     byte protocolVersion);
}
