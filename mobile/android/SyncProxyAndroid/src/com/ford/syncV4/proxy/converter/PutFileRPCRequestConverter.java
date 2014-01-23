package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.rpc.PutFile;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * A converter for the PutFile request which splits it into ProtocolMessage
 * objects that should be small enough to fit into single frames.
 *
 * Created by enikolsky on 2014-01-21.
 */
public class PutFileRPCRequestConverter implements IRPCRequestConverter {
    /**
     * Maximum number of bulk data bytes to fit into each ProtocolMessage.
     */
    private int maxDataSize = 1000;

    @Override
    public List<ProtocolMessage> getProtocolMessages(RPCRequest request,
                                                     byte sessionID,
                                                     IJsonRPCMarshaller marshaller,
                                                     byte protocolVersion) {
        List<ProtocolMessage> res = new ArrayList<ProtocolMessage>();

        if (request instanceof PutFile) {
            final byte[] bulkData = request.getBulkData();
            if (null != bulkData) {
                final int frameCount =
                        (int) Math.ceil(bulkData.length * 1.0 / maxDataSize);

                PutFile tempMsg = copyRequest((PutFile) request);
                for (int i = 0; i < frameCount; ++i) {
                    final int start = i * maxDataSize;
                    final int end =
                            Math.min((i + 1) * maxDataSize, bulkData.length);
                    final byte[] bulkDataRange =
                            Arrays.copyOfRange(bulkData, start, end);
                    tempMsg.setOffset(start);
                    tempMsg.setLength(end - start);
                    ProtocolMessage pm =
                            createProtocolMessage(tempMsg, sessionID,
                                    protocolVersion, bulkDataRange, marshaller);

                    res.add(pm);
                }
            } else {
                // create one message without bulk data
                ProtocolMessage pm = createProtocolMessage(request, sessionID,
                        protocolVersion, null, marshaller);

                res.add(pm);
            }
        }

        return res;
    }

    private PutFile copyRequest(PutFile request) {
        return new PutFile(request);
    }

    private ProtocolMessage createProtocolMessage(RPCRequest request,
                                                  byte sessionID,
                                                  byte protocolVersion,
                                                  byte[] bulkData,
                                                  IJsonRPCMarshaller marshaller) {
        ProtocolMessage pm = new ProtocolMessage();
        pm.setVersion(protocolVersion);
        byte[] msgBytes = marshaller.marshall(request, protocolVersion);
        pm.setData(msgBytes);
        pm.setSessionID(sessionID);
        pm.setMessageType(MessageType.RPC);
        pm.setSessionType(ServiceType.RPC);
        pm.setFunctionID(FunctionID.getFunctionID(request.getFunctionName()));
        pm.setCorrID(request.getCorrelationID());
        if (null != bulkData) {
            pm.setBulkData(bulkData);
        }

        return pm;
    }

    public int getMaxDataSize() {
        return maxDataSize;
    }

    public void setMaxDataSize(int maxDataSize) {
        this.maxDataSize = maxDataSize;
    }
}
