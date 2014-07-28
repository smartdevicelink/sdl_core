package com.ford.syncV4.streaming;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;

import java.io.IOException;
import java.io.InputStream;

abstract public class AbstractPacketizer {

    @SuppressWarnings("unused")
    private static final String CLASS_NAME = AbstractPacketizer.class.getSimpleName();

    protected IStreamListener mStreamListener = null;

    protected byte mSessionId = Session.DEFAULT_SESSION_ID;

    protected ServiceType serviceType = ServiceType.Mobile_Nav;

    protected InputStream is = null;

    protected byte[] buffer = new byte[1000];

    public ServiceType getServiceType() {
        return serviceType;
    }

    public byte getSessionID() {
        return mSessionId;
    }

    public AbstractPacketizer(IStreamListener streamListener, InputStream inputStream,
                              byte rpcSessionID) throws IOException {
        mStreamListener = streamListener;
        is = inputStream;
        this.mSessionId = rpcSessionID;
    }

    public void removeListener() {
        mStreamListener = null;
    }

    public abstract void sendMessage(ProtocolMessage protocolMessage);

    public abstract void start() throws IOException;

    public abstract void stop();

    /*public static String printBuffer(byte[] buffer, int start, int end) {
        String str = "";
        for (int i = start; i < end; i++) str += "," + Integer.toHexString(buffer[i] & 0xFF);
        return str;
    }*/
}