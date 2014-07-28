package com.ford.syncV4.protocol.secure.secureproxy;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.util.BitConverter;
import com.ford.syncV4.util.DebugTool;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.CountDownLatch;

import javax.net.ssl.HandshakeCompletedEvent;
import javax.net.ssl.HandshakeCompletedListener;

/**
 * Created by Andrew Batutin on 3/12/14.
 */
public class ProtocolSecureManager implements IProtocolSecureManager {

    private IHandshakeDataListener handshakeDataListener;
    private ISecureProxyServer listener;
    ISSLComponent secureProxy;
    ISSLComponent sslClient;
    private boolean handshakeFinished = false;

    public boolean isHandshakeFinished() {
        return handshakeFinished;
    }

    public void setHandshakeFinished(boolean handshakeFinished) {
        this.handshakeFinished = handshakeFinished;
    }

    private byte[] cypheredData = null;
    private byte[] deCypheredData = null;
    Set<ServiceType> serviceTypesToEncrypt = new HashSet<ServiceType>();

    public void addServiceToEncrypt(ServiceType serviceType) {
        serviceTypesToEncrypt.add(serviceType);
    }

    public void removeServiceTypeToEncrypt(ServiceType serviceType) {
        serviceTypesToEncrypt.remove(serviceType);
    }

    public boolean containsServiceTypeToEncrypt(ServiceType serviceType) {
        return serviceTypesToEncrypt.contains(serviceType);
    }

    public ProtocolSecureManager(IHandshakeDataListener listener) {
        this.handshakeDataListener = listener;
    }

    public void setupSecureEnvironment() {
        startSSLComponent();
    }

    private void startProxy(int port) {
        secureProxy = new SecureProxyClient(new ISecureProxyServer() {
            @Override
            public void onDataReceived(byte[] data) {
                if (!isHandshakeFinished()) {
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
                        reportAnError(e);
                    }

                    @Override
                    public void onServerSocketInit(int serverSocketPort) {

                    }
                }
        );
        try {
            secureProxy.setupClient(port);
        } catch (IOException e) {
            DebugTool.logError("Failed to setup secureProxy", e);
        }
    }

    private void startSSLComponent() {
        sslClient = new SSLServer(new ITransportListener() {
            @Override
            public void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength) {

            }

            @Override
            public void onTransportConnected() {
                SSLServer server = (SSLServer) sslClient;
                int port =  server.getServerSocket().getLocalPort();
                startProxy(port);
            }

            @Override
            public void onTransportDisconnected(String info) {

            }

            @Override
            public void onTransportError(String info, Exception e) {
                reportAnError(e);
            }

            @Override
            public void onServerSocketInit(int serverSocketPort) {

            }
        }, new HandshakeCompletedListener() {
            @Override
            public void handshakeCompleted(HandshakeCompletedEvent event) {
                //handshakeFinished = true;
                handshakeDataListener.onHandShakeCompleted();
                if (event != null) {
                    DebugTool.logInfo("GREAT SUCCESS" + event.toString());
                } else {
                    DebugTool.logInfo("GREAT SUCCESS");
                }
                //setHandshakeFinished(true);
            }
        });
        try {
            sslClient.setupClient(0);
        } catch (IOException e) {
            DebugTool.logError("Failed to setup sslClient", e);
        }
    }

    @Override
    public byte[] sendDataToProxyServerByChunk(boolean isEncrypted, byte[] data) throws IOException, InterruptedException {
        if (isEncrypted) {
            CountDownLatch countDownLatch = new CountDownLatch(1);
            RPCCodedDataListener listenerOFCodedData = new RPCCodedDataListener(countDownLatch);
            //Logger.i("cypheredData.length < 1000", "data.length" + data.length);
            listenerOFCodedData.setOriginalLength(data.length - WiProProtocol.SSL_OVERHEAD);
            writeDataToProxyServer(data, listenerOFCodedData);
            countDownLatch.await();
            return cypheredData;
        } else {
            return data;
        }
    }

    @Override
    public byte[] sendDataToProxyServerByteByByte(boolean isEncrypted, byte[] data) throws IOException, InterruptedException {
        if (isEncrypted) {
            CountDownLatch countDownLatch = new CountDownLatch(1);
            IRPCodedDataListener listenerOfDeCodedData = new RPCDeCodedDataListener(countDownLatch);
            writeDataToProxyServer(data, listenerOfDeCodedData);
            countDownLatch.await();
            return deCypheredData;
        } else {
            return data;
        }
    }

    public synchronized void writeDataToProxyServer(byte[] data, IRPCodedDataListener rpcCodedDataListener) throws IOException {
        sslClient.setRPCPacketListener(rpcCodedDataListener);
        secureProxy.writeData(data);
    }

    public synchronized void writeDataToSSLSocket(byte[] data, IRPCodedDataListener IRPCodedDataListener) throws IOException {
        secureProxy.setRPCPacketListener(IRPCodedDataListener);
        sslClient.writeData(data);
    }

    public synchronized byte[] sendDataTOSSLClient(boolean isEncrypted, byte[] data) throws IOException, InterruptedException {
        if (!isEncrypted) {
            return data;
        }
        CountDownLatch countDownLatch = new CountDownLatch(1);
        RPCCodedDataListener listenerOFCodedData = new RPCCodedDataListener(countDownLatch);
        //Logger.i("cypheredData.length < 1000", "data.length" + data.length);
        listenerOFCodedData.setOriginalLength(data.length);
        writeDataToSSLSocket(data, listenerOFCodedData);
        countDownLatch.await();
        //Logger.i("cypheredData.length < 1000", "return cypheredData" + cypheredData.length);
        return cypheredData;
    }

    public void reportAnError(Exception e) {
        handshakeDataListener.onError(e);
    }

    private class RPCDeCodedDataListener implements IRPCodedDataListener {

        ByteArrayOutputStream bOutput = new ByteArrayOutputStream();

        final int bytesToExpectForHeader = 32;
        int bytesReceived = 0;
        int jsonSize = 0;
        private CountDownLatch mCountDownLatch;

        private RPCDeCodedDataListener(CountDownLatch countDownLatch) {
            mCountDownLatch = countDownLatch;
        }

        @Override
        public void onRPCPayloadCoded(byte[] bytes) {
            if (handshakeFinished) {

                try {
                    bOutput.write(bytes);
                    bytesReceived += bytes.length;
                } catch (IOException e) {
                    DebugTool.logError("Failed to parse frame", e);
                    mCountDownLatch.countDown();
                }
                if (bytesReceived > bytesToExpectForHeader) {
                    jsonSize = getJsonSizeFromHeader(Arrays.copyOfRange(bOutput.toByteArray(), 0, 32));
                }

                if (jsonSize != 0 && bOutput.size() - 12 == jsonSize) {
                    deCypheredData = bOutput.toByteArray();
                    mCountDownLatch.countDown();
                    bytesReceived = 0;
                    jsonSize = 0;
                    bOutput = new ByteArrayOutputStream();

                }
            }
        }

        private int getJsonSizeFromHeader(byte[] bytes) {
            int jsonSize = BitConverter.intFromByteArray(bytes, 8);
            return jsonSize;
        }
    }

    private class RPCCodedDataListener implements IRPCodedDataListener {

        ByteArrayOutputStream bOutput = new ByteArrayOutputStream();

        public synchronized int getOriginalLength() {
            return originalLength;
        }

        public synchronized void setOriginalLength(int originalLength) {
            this.originalLength = originalLength;
        }

        int originalLength;
        int bytesReceived;
        int count = 0;
        private CountDownLatch mCountDownLatch;

        private RPCCodedDataListener(CountDownLatch countDownLatch) {
            mCountDownLatch = countDownLatch;
        }

        @Override
        public void onRPCPayloadCoded(byte[] bytes) {
            if (handshakeFinished) {
                try {
                    bOutput.write(bytes);
                    count++;
                    bytesReceived += bytes.length;
                } catch (IOException e) {
                    DebugTool.logError("Failed to code frame", e);
                    mCountDownLatch.countDown();
                }
                //Logger.i("cypheredData.length < 1000", " bytesReceived " + bytesReceived + " originalLength " + getOriginalLength() + " thread:" + Thread.currentThread().getName());
                if (bytesReceived > getOriginalLength()) {
                    cypheredData = bOutput.toByteArray();
                    bOutput = new ByteArrayOutputStream();
                    bytesReceived = 0;
                    setOriginalLength(0);
                    //Logger.i("cypheredData.length < 1000", " countDownLatchInput.countDown()");
                    mCountDownLatch.countDown();
                }
            }
        }
    }

    @Override
    public String toString() {
        return "ProtocolSecureManager{" +
                "handshakeDataListener=" + handshakeDataListener +
                ", listener=" + listener +
                ", secureProxy=" + secureProxy +
                ", sslClient=" + sslClient +
                ", handshakeFinished=" + handshakeFinished +
                ", cypheredData=" + Arrays.toString(cypheredData) +
                ", deCypheredData=" + Arrays.toString(deCypheredData) +
                ", serviceTypesToEncrypt=" + serviceTypesToEncrypt +
                '}';
    }
}
