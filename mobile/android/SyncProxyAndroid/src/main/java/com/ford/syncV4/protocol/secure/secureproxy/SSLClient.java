package com.ford.syncV4.protocol.secure.secureproxy;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.util.DebugTool;

import java.io.IOException;
import java.io.InputStream;
import java.net.InetAddress;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

import javax.net.SocketFactory;
import javax.net.ssl.HandshakeCompletedListener;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

/**
 * Created by Andrew Batutin on 3/11/14.
 */
public class SSLClient {

    private final HandshakeCompletedListener handshakeCompletedListener;
    private SSLSocket socket;
    ITransportListener transportListener;
    private SSLClientReader sslClientReader;
    private IRCCodedDataListener RPCPacketListener;

    public SSLClient(ITransportListener transportListener, HandshakeCompletedListener handshakeCompletedListener) {
        this.transportListener = transportListener;
        this.handshakeCompletedListener = handshakeCompletedListener;
    }

    public void setupClient() throws IOException {
        sslClientReader = new SSLClientReader();
        sslClientReader.start();
    }

    public void startHandshake(){
        socket.getSession();
    }

    private void startSocket() throws IOException, NoSuchAlgorithmException, KeyManagementException {
        TrustManager[] trustAllCerts = createTrustManagers();
        SSLContext sc = SSLContext.getInstance("TLS");
        sc.init(null, trustAllCerts, new java.security.SecureRandom());
        SocketFactory sf = sc.getSocketFactory();
        InetAddress addr = InetAddress.getByName("127.0.0.1");
        socket = (SSLSocket) sf.createSocket(addr, 8090);
        socket.addHandshakeCompletedListener(handshakeCompletedListener);
        socket.getSession();
        transportListener.onTransportConnected();
    }

    private void readData() throws IOException {
        InputStream inputStream = socket.getInputStream();

        byte[] buffer = new byte[WiProProtocol.MTU_SIZE];
        try {
            int i;
            while ((i = inputStream.read(buffer)) != -1) {
                transportListener.onTransportBytesReceived(buffer, i);

                if (getRPCPacketListener() != null) {
                    getRPCPacketListener().onRPCPayloadCoded(Arrays.copyOf(buffer, i));
                }
            }
        } catch (IOException e) {
            DebugTool.logError("sslClient read data failed ", e);
            transportListener.onTransportError("SSLClient", e);
        }
    }

    private TrustManager[] createTrustManagers() {
        // Create a trust manager that does not validate certificate chains
        return new TrustManager[]{new X509TrustManager() {
            @Override
            public void checkClientTrusted(java.security.cert.X509Certificate[] chain, String authType) throws java.security.cert.CertificateException {
            }

            @Override
            public void checkServerTrusted(java.security.cert.X509Certificate[] chain, String authType) throws java.security.cert.CertificateException {
            }

            @Override
            public java.security.cert.X509Certificate[] getAcceptedIssuers() {
                return new java.security.cert.X509Certificate[]{};
            }
        }};
    }

    public synchronized void writeData(byte[] data) throws IOException {
        socket.getOutputStream().write(data);
    }

    public void setRPCPacketListener(IRCCodedDataListener RPCPacketListener) {
        this.RPCPacketListener = RPCPacketListener;
    }

    public IRCCodedDataListener getRPCPacketListener() {
        return RPCPacketListener;
    }

    public class SSLClientReader extends Thread {

        public synchronized boolean isConnected() {
            return isConnected;
        }

        public synchronized void setConnected(boolean isConnected) {
            this.isConnected = isConnected;
        }

        private boolean isConnected;

        @Override
        public void run() {
            try {
                if (!isConnected) {
                    startSocket();
                }
                readData();

            } catch (IOException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (NoSuchAlgorithmException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (KeyManagementException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            }
        }
    }

}
