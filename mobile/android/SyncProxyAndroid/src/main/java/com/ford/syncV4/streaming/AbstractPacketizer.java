package com.ford.syncV4.streaming;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.util.logger.Logger;

import java.io.IOException;
import java.io.InputStream;

abstract public class AbstractPacketizer {

    @SuppressWarnings("unused")
    private static final String CLASS_NAME = AbstractPacketizer.class.getSimpleName();

    private IStreamListener mStreamListener = null;

    protected byte _rpcSessionID = Session.DEFAULT_SESSION_ID;

    protected ServiceType _serviceType = ServiceType.Mobile_Nav;

    protected InputStream is = null;

    protected byte[] buffer = new byte[1000];

    protected boolean upts = false;
    //protected long ts = 0, intervalBetweenReports = 5000, delta = 0;
    protected long intervalBetweenReports = 5000, delta = 0;

    public ServiceType getServiceType() {
        return _serviceType;
    }

    public byte getSessionID() {
        return _rpcSessionID;
    }

    public AbstractPacketizer(IStreamListener streamListener, InputStream inputStream,
                              byte rpcSessionID) throws IOException {
        mStreamListener = streamListener;
        is = inputStream;
        _rpcSessionID = rpcSessionID;
    }

    public void removeListener() {
        mStreamListener = null;
    }

    public void sendMessage(ProtocolMessage protocolMessage) {
        if (protocolMessage == null) {
            return;
        }
        mStreamListener.sendH264(protocolMessage);
    }

    public abstract void start() throws IOException;

    public abstract void stop();

    public static String printBuffer(byte[] buffer, int start, int end) {
        String str = "";
        for (int i = start; i < end; i++) str += "," + Integer.toHexString(buffer[i] & 0xFF);
        return str;
    }
}
