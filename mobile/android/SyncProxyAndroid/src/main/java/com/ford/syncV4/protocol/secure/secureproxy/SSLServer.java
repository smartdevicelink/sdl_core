package com.ford.syncV4.protocol.secure.secureproxy;

import android.util.Base64;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.util.DebugTool;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import java.security.cert.CertificateException;
import java.util.Arrays;

import javax.net.ssl.HandshakeCompletedListener;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLServerSocket;

/**
 * Created by Andrew Batutin on 3/20/14.
 */
public class SSLServer implements ISSLComponent {

    private final HandshakeCompletedListener handshakeCompletedListener;
    ITransportListener transportListener;
    private IRPCodedDataListener RPCPacketListener;
    private SSLServerReader sslServerReader;
    private Socket socket;


    public SSLServer(ITransportListener transportListener, HandshakeCompletedListener handshakeCompletedListener) {
        this.transportListener = transportListener;
        this.handshakeCompletedListener = handshakeCompletedListener;
    }

    @Override
    public void setupClient() throws IOException {
        sslServerReader = new SSLServerReader();
        sslServerReader.start();
    }

    @Override
    public synchronized void writeData(byte[] data) throws IOException {
        socket.getOutputStream().write(data);
    }

    @Override
    public void setRPCPacketListener(IRPCodedDataListener RPCPacketListener) {
        this.RPCPacketListener = RPCPacketListener;
    }

    @Override
    public IRPCodedDataListener getRPCPacketListener() {
        return RPCPacketListener;
    }

    private class SSLServerReader extends Thread {

        public synchronized boolean isConnected() {
            return isConnected;
        }

        public synchronized void setConnected(boolean isConnected) {
            this.isConnected = isConnected;
        }

        private boolean isConnected;

        private final String KEYSTORE = "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";

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
            } catch (UnrecoverableKeyException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (CertificateException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (NoSuchAlgorithmException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (KeyStoreException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (KeyManagementException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            }
        }

        private void startSocket() throws KeyStoreException, CertificateException, NoSuchAlgorithmException, IOException, UnrecoverableKeyException, KeyManagementException {
            String keyStoreType = KeyStore.getDefaultType();
            KeyStore keyStore = KeyStore.getInstance("PKCS12");
            InputStream is = new ByteArrayInputStream(Base64.decode(KEYSTORE, Base64.DEFAULT));
            keyStore.load(is, "android".toCharArray());

            String keyalg = KeyManagerFactory.getDefaultAlgorithm();
            KeyManagerFactory kmf = KeyManagerFactory.getInstance(keyalg);
            kmf.init(keyStore, "android".toCharArray());

            SSLContext context = SSLContext.getInstance("TLS");
            context.init(kmf.getKeyManagers(), null, null);
            SSLServerSocket ss = (SSLServerSocket) context.getServerSocketFactory().createServerSocket(8090);
            transportListener.onTransportConnected();
            socket = ss.accept();
            handshakeCompletedListener.handshakeCompleted(null);
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
    }
}
