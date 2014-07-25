package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.rpc.PutFile;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * A converter for the PutFile request for system files which splits it into
 * ProtocolMessage objects by maximum size. This is done to allow the SDK to
 * continue transferring big files after errors.
 * <p/>
 * Created by enikolsky on 2014-01-21.
 */
public class PutFileRPCRequestConverter implements IRPCRequestConverter {

    /**
     * Maximum number of bulk data bytes to fit into each ProtocolMessage.
     */
    private int maxDataSize = 100000;

    @Override
    public List<ProtocolMessage> getProtocolMessages(RPCRequest request,
                                                     byte sessionID,
                                                     IJsonRPCMarshaller marshaller,
                                                     byte protocolVersion) {
        final List<ProtocolMessage> messagesList = new ArrayList<ProtocolMessage>();

        if (request instanceof PutFile) {
            final byte[] bulkData = request.getBulkData();
            if (bulkData != null) {
                final int frameCount = (int) Math.ceil(bulkData.length * 1.0 / maxDataSize);

                final PutFile putFile = (PutFile) request;
                final Integer offset = putFile.getOffset();
                final int baseOffset = (offset != null) ? offset : 0;

                PutFile copyRequest = copyRequest(putFile);
                for (int i = 0; i < frameCount; ++i) {
                    final int start = i * maxDataSize;
                    final int end = Math.min((i + 1) * maxDataSize, bulkData.length);
                    final byte[] bulkDataRange = Arrays.copyOfRange(bulkData, start, end);
                    copyRequest.setOffset(baseOffset + start);
                    copyRequest.setLength(end - start);
                    copyRequest.setSystemFile(true);

                    messagesList.add(DefaultRPCRequestConverter.createProtocolMessage(copyRequest,
                            sessionID, protocolVersion, MessageType.RPC, ServiceType.RPC,
                            bulkDataRange, marshaller));
                }
            } else {
                // create one message without bulk data
                messagesList.add(DefaultRPCRequestConverter.createProtocolMessage(request,
                        sessionID, protocolVersion, MessageType.RPC, ServiceType.RPC,
                        null, marshaller));
            }
        }

        return messagesList;
    }

    private PutFile copyRequest(PutFile request) {
        return new PutFile(request);
    }

    public int getMaxDataSize() {
        return maxDataSize;
    }

    public void setMaxDataSize(int maxDataSize) {
        this.maxDataSize = maxDataSize;
    }
}