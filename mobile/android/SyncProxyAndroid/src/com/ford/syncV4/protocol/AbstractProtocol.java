package com.ford.syncV4.protocol;

import android.database.sqlite.SQLiteCantOpenDatabaseException;
import android.os.Environment;
import android.util.Log;

import com.ford.syncV4.protocol.WiProProtocol.MessageFrameAssembler;
import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.streaming.AbstractPacketizer;
import com.ford.syncV4.trace.SyncTrace;
import com.ford.syncV4.trace.enums.InterfaceActivityDirection;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public abstract class AbstractProtocol {
    private static final String SYNC_LIB_TRACE_KEY = "42baba60-eb57-11df-98cf-0800200c9a66";
    protected IProtocolListener _protocolListener = null;
    private FileOutputStream fos;
    //protected IProtocolListener ProtocolListener() { return _protocolListener; }
    // Lock to ensure all frames are sent uninterupted
    private Object _frameLock = new Object();
    private File file;

    // Caller must provide a non-null IProtocolListener interface reference.
    public AbstractProtocol(IProtocolListener protocolListener) {
        if (protocolListener == null) {
            throw new IllegalArgumentException("Provided protocol listener interface reference is null");
        } // end-if


            _protocolListener = protocolListener;
    }// end-ctor



    // This method receives raw bytes as they arrive from transport.  Those bytes
    // are then collected by the protocol and assembled into complete messages and
    // handled internally by the protocol or propagated to the protocol listener.

    public abstract void HandleReceivedBytes(byte[] receivedBytes, int length);

    // This method receives a protocol message (e.g. RPC, BULK, etc.) and processes
    // it for transmission over the transport.  The results of this processing will
    // be sent to the onProtocolMessageBytesToSend() method on protocol listener
    // interface.  Note that the ProtocolMessage itself contains information
    // about the type of message (e.g. RPC, BULK, etc.) and the protocol session
    // over which to send the message, etc.
    public abstract void SendMessage(ProtocolMessage msg);

    // This method starts a protocol session.  A corresponding call to the protocol
    // listener onProtocolSessionStarted() method will be made when the protocol
    // session has been established.
    public abstract void StartProtocolSession(SessionType sessionType);

    // This method ends a protocol session.  A corresponding call to the protocol
    // listener onProtocolSessionEnded() method will be made when the protocol
    // session has ended.
    public abstract void EndProtocolSession(SessionType sessionType, byte sessionID);

    // This method sets the interval at which heartbeat protocol messages will be
    // sent to SYNC.
    public abstract void SetHeartbeatSendInterval(int heartbeatSendInterval_ms);

    // This method sets the interval at which heartbeat protocol messages are
    // expected to be received from SYNC.
    public abstract void SetHeartbeatReceiveInterval(int heartbeatReceiveInterval_ms);

    // This method is called whenever the protocol receives a complete frame
    protected void handleProtocolFrameReceived(ProtocolFrameHeader header, byte[] data, MessageFrameAssembler assembler) {
        SyncTrace.logProtocolEvent(InterfaceActivityDirection.Receive, header, data,
                0, data.length, SYNC_LIB_TRACE_KEY);

        assembler.handleFrame(header, data);
    }

    // This method is called whenever a protocol has an entire frame to send
    protected void handleProtocolFrameToSend(ProtocolFrameHeader header, byte[] data, int offset, int length) {
        SyncTrace.logProtocolEvent(InterfaceActivityDirection.Transmit, header, data,
                offset, length, SYNC_LIB_TRACE_KEY);

        synchronized (_frameLock) {

            byte[] frameHeader = header.assembleHeaderBytes();
            handleProtocolMessageBytesToSend(frameHeader, 0, frameHeader.length);

            if (data != null) {
                handleProtocolMessageBytesToSend(data, offset, length);
            }
            //logMobileNaviMessages(header, data);
        } // end-if
    }

    private void logMobileNaviMessages(ProtocolFrameHeader header, byte[] data){
        if (header.getSessionType().equals(SessionType.Mobile_Nav)) {
            Log.d("MobileNaviSession", "ProtocolFrameHeader: " + header.toString());
            if ( data != null && data.length > 0 ){
                Log.d("MobileNaviSession", "Hex Data frame: " +AbstractPacketizer.printBuffer(data, 0, data.length));
            }
        }
    }

    private void initVideoDumpStream() {
        String filename = "ford_video.mp4";
        file = new File(Environment.getExternalStorageDirectory(), filename);

        try {
            fos = new FileOutputStream(file);
        } catch (FileNotFoundException e) {
            // handle exception
        }
    }

    private void writeToSdCard(ProtocolFrameHeader header, byte[] data) {
        if (header.getSessionType().equals(SessionType.Mobile_Nav)) {
            if (header.getFrameType().equals(FrameType.Single)) {
                try {
                    if (fos != null) {
                        fos.write(data);
                        fos.flush();
                    }
                } catch (IOException e) {
                    Log.w("SyncProxyTester", e.toString());
                }
            } else {
                Log.w("SyncProxyTester", "wrong frame type for video streaming");
            }
        }
    }

    // This method handles protocol message bytes that are ready to send.
    // A callback is sent to the protocol listener.
    protected void handleProtocolMessageBytesToSend(byte[] bytesToSend,
                                                    int offset, int length) {
        _protocolListener.onProtocolMessageBytesToSend(bytesToSend, offset, length);
    }

    // This method handles received protocol messages.
    protected void handleProtocolMessageReceived(ProtocolMessage message) {
        _protocolListener.onProtocolMessageReceived(message);
    }

    // This method handles the end of a protocol session. A callback is
    // sent to the protocol listener.
    protected void handleProtocolSessionEnded(SessionType sessionType,
                                              byte sessionID, String correlationID) {
        _protocolListener.onProtocolSessionEnded(sessionType, sessionID, correlationID);
    }

    // This method handles the startup of a protocol session. A callback is sent
    // to the protocol listener.
    protected void handleProtocolSessionStarted(SessionType sessionType,
                                                byte sessionID, byte version, String correlationID) {
        _protocolListener.onProtocolSessionStarted(sessionType, sessionID, version, correlationID);
    }

    // This method handles protocol errors. A callback is sent to the protocol
    // listener.
    protected void handleProtocolError(String string, Exception ex) {
        _protocolListener.onProtocolError(string, ex);
    }

    protected void handleAppUnregistered() {
        _protocolListener.onProtocolAppUnregistered();
    }

} // end-class
