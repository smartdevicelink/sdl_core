package com.ford.syncV4.syncConnection;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.transport.BTTransportConfig;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TransportType;

import junit.framework.TestCase;

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

        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.init(new BTTransportConfig(), fakeTransport);
        connection.onProtocolServiceEnded(ServiceType.RPC, (byte) 0, "");
        assertTrue("stopReading() isn't called", requiredMethodCalled);
    }
}
