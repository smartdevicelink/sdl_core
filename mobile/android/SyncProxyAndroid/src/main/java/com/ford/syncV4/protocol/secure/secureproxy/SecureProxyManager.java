package com.ford.syncV4.protocol.secure.secureproxy;

import android.content.Context;
import android.util.Log;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.transport.TCPTransport;
import com.ford.syncV4.transport.TCPTransportConfig;

import java.io.IOException;
import java.util.Arrays;

/**
 * Created by Andrew Batutin on 3/11/14.
 */
public class SecureProxyManager {

    ITransportListener transportListener = new ITransportListener() {
        @Override
        public void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength) {
            byte[] data = Arrays.copyOf(receivedBytes, receivedBytesLength);
            secureProxyServer.writeData(data);
        }

        @Override
        public void onTransportConnected() {
            startSecureProxy();

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
    Context context;
    private TCPTransport transport;
    private SecureProxyServer secureProxyServer;
    private SSLClient sslClient;

    public SecureProxyManager(Context context) {
        this.context = context;
    }

    public void setupSecureConnection() {
        try {
            startTCPConnection();
        } catch (SyncException e) {
            Log.e("SecureProxyManager", "error", e);
        }
    }

    private void startTCPConnection() throws SyncException {
        TCPTransportConfig config = new TCPTransportConfig(443, "gmail.com");
        config.setIsNSD(false);
        config.setApplicationContext(this.context);
        transport = new TCPTransport(config, transportListener);
        transport.openConnection();
    }

    private void startSecureProxy() {
        secureProxyServer = new SecureProxyServer(transport.getOutputStream(), new ITransportListener() {
            @Override
            public void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength) {

            }

            @Override
            public void onTransportConnected() {
                startSSLClient();
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
        });
        try {
            secureProxyServer.setupServer();
        } catch (IOException e) {
            Log.e("SecureProxyManager", "error", e);
        }
    }

    private void startSSLClient() {

        sslClient = new SSLClient(new ITransportListener() {
            @Override
            public void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength) {

            }

            @Override
            public void onTransportConnected() {
                try {
                    writeTestData(sslClient);
                } catch (IOException e) {
                    Log.e("SecureProxyManager", "error", e);
                }
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
        });
        try {
            sslClient.setupClient();

        } catch (IOException e) {
            Log.e("SecureProxyManager", "error", e);
        }
    }

    private void writeTestData(SSLClient sslClient) throws IOException {
        byte[] data = new byte[100];
        for (int i = 0; i< 100; i++){
            data[i] = (byte) i;
        }
       // sslClient.writeData(data);
    }

}
