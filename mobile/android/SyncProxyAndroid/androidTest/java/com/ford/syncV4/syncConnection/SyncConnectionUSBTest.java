package com.ford.syncV4.syncConnection;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.session.SessionTest;
import com.ford.syncV4.transport.BTTransportConfig;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TransportType;

import junit.framework.TestCase;

import java.io.InputStream;
import java.io.OutputStream;

import static org.mockito.Mockito.mock;

/**
 * Created by u on 2013-09-30
 */
public class SyncConnectionUSBTest extends TestCase {

    private boolean requiredMethodCalled;

    public void testOnProtocolAppUnregisteredStopsTransport() {

        final ITransportListener transportListener = new ITransportListener() {

            @Override
            public void onTransportBytesReceived(byte[] receivedBytes,
                                                 int receivedBytesLength) {
            }

            @Override
            public void onTransportConnected() {
            }

            @Override
            public void onTransportDisconnected(String info) {
            }

            @Override
            public void onTransportError(String info, Exception e) {
            }

            @Override
            public void onServerSocketInit(int serverSocketPort) {

            }
        };

        final SyncTransport fakeTransport = new SyncTransport(transportListener) {

            @Override
            public InputStream getInputStream() {
                return null;
            }

            @Override
            public OutputStream getOutputStream() {
                return null;
            }

            @Override
                    protected boolean sendBytesOverTransport(byte[] msgBytes,
                                                             int offset,
                                                             int length) {
                        return false;
                    }

                    @Override
                    public void openConnection() throws SyncException {
                    }

                    @Override
                    public void disconnect() {
                    }

                    @Override
                    public void stopReading() {
                        requiredMethodCalled = true;
                    }

                    @Override
                    public TransportType getTransportType() {
                        return null;
                    }
                };

        final SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class));
        connection.init(new BTTransportConfig());
        connection._transport = fakeTransport;
        connection.onProtocolServiceEndedAck(ServiceType.RPC, SessionTest.SESSION_ID);
        assertTrue("stopReading() isn't called", requiredMethodCalled);
    }
}
