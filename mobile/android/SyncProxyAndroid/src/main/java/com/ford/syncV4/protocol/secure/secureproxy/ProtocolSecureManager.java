package com.ford.syncV4.protocol.secure.secureproxy;

import android.util.Log;

import com.ford.syncV4.transport.ITransportListener;

import java.io.IOException;
import java.util.Arrays;
import java.util.concurrent.CountDownLatch;

import javax.net.ssl.HandshakeCompletedEvent;
import javax.net.ssl.HandshakeCompletedListener;

/**
 * Created by Andrew Batutin on 3/12/14.
 */
public class ProtocolSecureManager {

    private IHandshakeDataListener handshakeDataListener;
    private ISecureProxyServer listener;
    SecureProxyServer secureProxyServer;
    SSLClient sslClient;
    private boolean handshakeFinished;
    private final CountDownLatch countDownLatch = new CountDownLatch(1);
    private byte[] cypheredData = null;

    public ProtocolSecureManager(IHandshakeDataListener listener) {
        this.handshakeDataListener = listener;
    }

    public void startHandShake() {
        startSSLClient();
    }

    public void setupSecureEnvironment() {
        startSecureProxy();
    }

    private void startSecureProxy() {
        secureProxyServer = new SecureProxyServer(new ISecureProxyServer() {
            @Override
            public void onDataReceived(byte[] data) {
                if (!handshakeFinished){
                    handshakeDataListener.onHandshakeDataReceived(data);
                }
            }
        },
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
                handshakeDataListener.onHandShakeCompleted();
                Log.i("SSLClient", "GREAT SUCCESS" + event.toString());
            }
        });
        try {
            sslClient.setupClient();

        } catch (IOException e) {
            Log.e("ProtocolSecureManager", "error", e);
        }
    }


    public synchronized void writeDataToProxyServer(byte[] data, IRCCodedDataListener ircCodedDataListener) throws IOException {
        sslClient.setRPCPacketListener(ircCodedDataListener);
        secureProxyServer.writeData(data);
    }

    public synchronized void writeDataToSSLSocket(byte[] data, IRCCodedDataListener ircCodedDataListener) throws IOException {
        secureProxyServer.setRPCPacketListener(ircCodedDataListener);
        sslClient.writeData(data);
    }


    private synchronized void setHandshakeFinished(boolean handshakeFinished) {
        this.handshakeFinished = handshakeFinished;
    }


    public synchronized byte[] sendDataTOSSLClient(byte[] data) throws IOException, InterruptedException {
        writeDataToSSLSocket(data, new IRCCodedDataListener() {
            @Override
            public void onRPCPayloadCoded(byte[] bytes) {
                if (handshakeFinished) {
                    cypheredData = Arrays.copyOf(bytes, bytes.length);
                    countDownLatch.countDown();
                }
            }
        });
        countDownLatch.await();
        return cypheredData;
    }
}
