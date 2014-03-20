package com.ford.syncV4.protocol.secure.secureproxy;

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
    private IRCCodedDataListener RPCPacketListener;
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
    public void setRPCPacketListener(IRCCodedDataListener RPCPacketListener) {
        this.RPCPacketListener = RPCPacketListener;
    }

    @Override
    public IRCCodedDataListener getRPCPacketListener() {
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

        private final String KEYSTORE = "AAAAAgAAABRiFoaozwXbtj+K8HAyyNAksGdwOwAABNAEAAVteWtleQAAAUTfIFnIAAAAAQAFWC41MDkAAAMOMIIDCjCCAsegAwIBAgIEUyrIyjALBgcqhkjOOAQDBQAwaDELMAkGA1UEBhMCVUExEDAOBgNVBAgTB1VrcmFpbmUxDjAMBgNVBAcTBU9kZXNhMQ8wDQYDVQQKEwZMdXhvZnQxDTALBgNVBAsTBEZvcmQxFzAVBgNVBAMTDkFuZHJldyBCYXR1dGluMB4XDTE0MDMyMDEwNTQwMloXDTE0MDYxODEwNTQwMlowaDELMAkGA1UEBhMCVUExEDAOBgNVBAgTB1VrcmFpbmUxDjAMBgNVBAcTBU9kZXNhMQ8wDQYDVQQKEwZMdXhvZnQxDTALBgNVBAsTBEZvcmQxFzAVBgNVBAMTDkFuZHJldyBCYXR1dGluMIIBtzCCASwGByqGSM44BAEwggEfAoGBAP1/U4EddRIpUt9KnC7s5Of2EbdSPO9EAMMeP4C2USZpRV1AIlH7WT2NWPq/xfW6MPbLm1Vs14E7gB00b/JmYLdrmVClpJ+f6AR7ECLCT7up1/63xhv4O1fnxqimFQ8E+4P208UewwI1VBNaFpEy9nXzrith1yrv8iIDGZ3RSAHHAhUAl2BQjxUjC8yykrmCouuEC/BYHPUCgYEA9+GghdabPd7LvKtcNrhXuXmUr7v6OuqC+VdMCz0HgmdRWVeOutRZT+ZxBxCBgLRJFnEj6EwoFhO3zwkyjMim4TwWeotUfI0o4KOuHiuzpnWRbqN/C/ohNWLx+2J6ASQ7zKTxvqhRkImog9/hWuWfBpKLZl6Ae1UlZAFMO/7PSSoDgYQAAoGAWm/ZcMQy7DKwAx8K4MkPvtqYg0Kksh01sZKWO2F+rjWnMn3nN3QmOQmN4MxwOzOdhwwqvnrCNQzkPdmj/LNSQGSpK60TzvIExjIz7PQuOHEYvw4WsPzIzgvm+SiNrgmZFAuYmV6Iv+fF4gDRdww14LFrtPywf1Vxucp3fdg+AOcwCwYHKoZIzjgEAwUAAzAAMC0CFCoucLVHoatL2baST76Nxhta++iKAhUAhmqSAPJqTlrWaeeQ8b3dK1xHHAUAAAGEAAAAFN6oXOBPQsxAvSnwjPQMANHSZMToAAAEgW51b3dyLkwKXDQSGQiWKtGXtxsR+7Vu/SHeyBgVufnsyCUf9HDRxMhcvu9QJyKF+/OTh5dZ0s9mRxp/Fh0YH17AiZiIsZ8s2h2fvS/H2H1CkcYwAJA4E17E6P/6zt1NtRCNzHHSv/1OLlk3U4L2pSfVCC0O0UCkpJOpza0zOobSHdrWFJsVJDQmadsILNbqGjA5FJ8awCg3HjkkpHRmF7UZvz6GiHLVYVX0GZ+9ak91hsPY5A3NazH8IggQuTXbdGzVjX+1YjzAiuH9rpkalSu4oK2EuThnWdjp142f2DBqB3WDbVwL+TzKwniYkzMp499YLQvSU2u4lPyYz4uOkVyL9GXuk7Ho9jSSP3rRJIHIAjkkwAwU4bgoXNDVL7wM5u22qpahrYWYyrOr45HlfjvKhK+iUIgdCXS4W7PzeTzC1Z8vr3MRWksXCaQ7IVR8w+oNkJMZZ18pRQ18d+Hp8tw8SYCfHSEmXACMuIhikF0V2f3eqz04Fk1qb32zoA==";

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
            KeyStore keyStore = KeyStore.getInstance(keyStoreType);
            InputStream is = new ByteArrayInputStream(KEYSTORE.getBytes());
            keyStore.load(is, "android".toCharArray());

            String keyalg = KeyManagerFactory.getDefaultAlgorithm();
            KeyManagerFactory kmf = KeyManagerFactory.getInstance(keyalg);
            kmf.init(keyStore, "android".toCharArray());

            SSLContext context = SSLContext.getInstance("TLS");
            context.init(kmf.getKeyManagers(), null, null);
            SSLServerSocket ss = (SSLServerSocket) context.getServerSocketFactory().createServerSocket(8090);
            socket = ss.accept();
            handshakeCompletedListener.handshakeCompleted(null);
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
    }


}
