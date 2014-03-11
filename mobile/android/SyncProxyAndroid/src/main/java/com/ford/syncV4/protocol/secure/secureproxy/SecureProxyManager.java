package com.ford.syncV4.protocol.secure.secureproxy;

import android.content.Context;
import android.util.Log;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.transport.TCPTransport;
import com.ford.syncV4.transport.TCPTransportConfig;

import java.io.IOException;

/**
 * Created by Andrew Batutin on 3/11/14.
 */
public class SecureProxyManager {

    ITransportListener transportListener = new ITransportListener() {
        @Override
        public void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength) {

        }

        @Override
        public void onTransportConnected() {
            startSecureProxy();
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
    };
    Context context;

    public SecureProxyManager(Context context) {
        this.context = context;
    }

    public void setupSecureConnection(){
        try {
            startTCPConnection();
        } catch (SyncException e) {
            Log.e("SecureProxyManager", "error", e);
        }
    }

    private void startTCPConnection() throws SyncException {
        TCPTransportConfig config = new TCPTransportConfig(8080, "172.30.222.74");
        config.setIsNSD(false);
        config.setApplicationContext(this.context);
        TCPTransport transport = new TCPTransport(config, transportListener);
        transport.openConnection();
    }

    private void startSecureProxy() {
        SecureProxyServer secureProxyServer = new SecureProxyServer();
        try {
            secureProxyServer.setupServer();
        } catch (IOException e) {
            Log.e("SecureProxyManager", "error", e);
        }
    }

    private void startSSLClient() {
        SSLClient sslClient = new SSLClient();
        try {
            sslClient.setupClient();
        } catch (IOException e) {
            Log.e("SecureProxyManager", "error", e);
        }
    }

}
