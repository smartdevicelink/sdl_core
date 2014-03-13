package com.ford.syncV4.protocol.secure.secureproxy;

import android.util.Log;

import com.ford.syncV4.transport.ITransportListener;

import java.io.IOException;

import javax.net.ssl.HandshakeCompletedEvent;
import javax.net.ssl.HandshakeCompletedListener;

/**
 * Created by Andrew Batutin on 3/12/14.
 */
public class ProtocolSecureManager {

    private ISecureProxyServer listener;
    private SecureProxyServer secureProxyServer;
    private SSLClient sslClient;
    private boolean handshakeFinished;

    public ProtocolSecureManager(ISecureProxyServer listener) {
        this.listener = listener;
    }

    public void startHandShake(){
        startSSLClient();
    }

    public void setupSecureEnvironment() {
        startSecureProxy();
    }

    private void startSecureProxy() {
        secureProxyServer = new SecureProxyServer(listener,
                new ITransportListener() {
                    @Override
                    public void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength) {

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
                }
        );
        try {
            secureProxyServer.setupServer();
        } catch (IOException e) {
            Log.e("ProtocolSecureManager", "error", e);
        }
    }

    private void startSSLClient() {
        sslClient = new SSLClient(new ITransportListener() {
            @Override
            public void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength) {

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
        }, new HandshakeCompletedListener() {
            @Override
            public void handshakeCompleted(HandshakeCompletedEvent event) {
                setHandshakeFinished(true);
                Log.i("SSLClient", "GREAT SUCCESS" + event.toString());
            }
        });
        try {
            sslClient.setupClient();

        } catch (IOException e) {
            Log.e("SecureProxyManager", "error", e);
        }
    }


    public synchronized void writeDataToProxyServer(byte[] data){
        secureProxyServer.writeData(data);
    }

    public synchronized void writeDataToSSLSocket(byte[] data) throws IOException {
        sslClient.writeData(data);
    }

    public synchronized boolean isHandshakeFinished() {
        return handshakeFinished;
    }

    private synchronized void setHandshakeFinished(boolean handshakeFinished) {
        this.handshakeFinished =handshakeFinished;
    }
}
