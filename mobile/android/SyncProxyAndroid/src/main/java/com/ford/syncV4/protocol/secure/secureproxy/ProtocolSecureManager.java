package com.ford.syncV4.protocol.secure.secureproxy;

import android.util.Log;

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
public class ProtocolSecureManager {

    private IHandshakeDataListener handshakeDataListener;
    private ISecureProxyServer listener;
    SecureProxyServer secureProxyServer;
    SSLClient sslClient;
    private boolean handshakeFinished;
    private CountDownLatch countDownLatchInput = new CountDownLatch(1);

    public synchronized CountDownLatch getCountDownLatchOutput() {
        return countDownLatchOutput;
    }

    public synchronized void setCountDownLatchOutput(CountDownLatch countDownLatchOutput) {
        this.countDownLatchOutput = countDownLatchOutput;
    }

    public synchronized CountDownLatch getCountDownLatchInput() {
        return countDownLatchInput;
    }

    public synchronized void setCountDownLatchInput(CountDownLatch countDownLatchInput) {
        this.countDownLatchInput = countDownLatchInput;
    }

    private CountDownLatch countDownLatchOutput = new CountDownLatch(1);
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
                if (!handshakeFinished) {
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
            secureProxyServer.setupServer();
        } catch (IOException e) {
            DebugTool.logError("Failed to setup secureProxyServer", e);
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
                reportAnError(e);
            }

            @Override
            public void onServerSocketInit(int serverSocketPort) {

            }
        }, new HandshakeCompletedListener() {
            @Override
            public void handshakeCompleted(HandshakeCompletedEvent event) {
                setHandshakeFinished(true);
                handshakeDataListener.onHandShakeCompleted();
                DebugTool.logInfo("GREAT SUCCESS" + event.toString());
            }
        });
        try {
            sslClient.setupClient();
        } catch (IOException e) {
            DebugTool.logError("Failed to setup sslClient", e);
        }
    }


    public synchronized byte[] sendDataToProxyServer(ServiceType serviceType, byte[] data) throws IOException, InterruptedException {
        if (serviceTypesToEncrypt.contains(serviceType)) {
            writeDataToProxyServer(data, new IRCCodedDataListener() {
                @Override
                public void onRPCPayloadCoded(byte[] bytes) {
                    if (handshakeFinished) {
                        deCypheredData = Arrays.copyOf(bytes, bytes.length);
                        countDownLatchOutput.countDown();
                    }
                }
            });
            countDownLatchOutput.await();
            return deCypheredData;
        } else {
            return data;
        }
    }


    public byte[] sendDataToProxyServerByteByByte(ServiceType serviceType, byte[] data) throws IOException, InterruptedException {
        if (serviceTypesToEncrypt.contains(serviceType)) {
            IRCCodedDataListener listenerOfDeCodedData = new RPCDeCodedDataListener();
            writeDataToProxyServer(data, listenerOfDeCodedData);
            getCountDownLatchOutput().await();
            return deCypheredData;
        } else {
            return data;
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


    public synchronized byte[] sendDataTOSSLClient(ServiceType serviceType, byte[] data) throws IOException, InterruptedException {
        if (serviceTypesToEncrypt.contains(serviceType)) {
            RPCCodedDataListener listenerOFCodedData = new RPCCodedDataListener();
            Log.i("cypheredData.length < 1000", "data.length" + data.length);
            listenerOFCodedData.setOriginalLength( data.length);
            writeDataToSSLSocket(data, listenerOFCodedData);
            getCountDownLatchInput().await();
            Log.i("cypheredData.length < 1000", "return cypheredData" + cypheredData.length);
            return cypheredData;
        } else {
            return data;
        }
    }

    public void reportAnError(Exception e) {
        handshakeDataListener.onError(e);
    }

    private class RPCDeCodedDataListener implements IRCCodedDataListener {

        ByteArrayOutputStream bOutput = new ByteArrayOutputStream();

        final int bytesToExpectForHeader = 32;
        int bytesReceived = 0;
        int jsonSize = 0;

        private RPCDeCodedDataListener() {
            setCountDownLatchOutput(new CountDownLatch(1));
        }

        @Override
        public void onRPCPayloadCoded(byte[] bytes) {
            if (handshakeFinished) {

                try {
                    bOutput.write(bytes);
                    bytesReceived += bytes.length;
                } catch (IOException e) {
                    DebugTool.logError("Failed to parse frame", e);
                    countDownLatchOutput.countDown();
                }
                if (bytesReceived > bytesToExpectForHeader) {
                    jsonSize = getJsonSizeFromHeader(Arrays.copyOfRange(bOutput.toByteArray(), 0, 32));
                }

                if (jsonSize != 0 && bOutput.size() - 12 == jsonSize) {
                    deCypheredData = bOutput.toByteArray();
                    countDownLatchOutput.countDown();
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

    private class RPCCodedDataListener implements IRCCodedDataListener {

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

        private RPCCodedDataListener() {

            setCountDownLatchInput(new CountDownLatch(1));
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
                    countDownLatchInput.countDown();
                }
                Log.i("cypheredData.length < 1000", " bytesReceived " + bytesReceived + " originalLength " + getOriginalLength() + " thread:" + Thread.currentThread().getName());
                if (bytesReceived > getOriginalLength()) {
                    cypheredData = bOutput.toByteArray();
                    bOutput = new ByteArrayOutputStream();
                    bytesReceived = 0;
                    setOriginalLength(0);
                    Log.i("cypheredData.length < 1000", " countDownLatchInput.countDown()");
                    countDownLatchInput.countDown();
                }

//                cypheredData = bytes;
//                if (cypheredData != null) {
//                    Log.i("cypheredData.length < 1000", " onRPCPayloadCoded " + bytes.length + " cypheredData.l " + cypheredData.length + " thread:" + Thread.currentThread().getName());
//                }
//                countDownLatchInput.countDown();
//                if (cypheredData != null) {
//                    Log.i("cypheredData.length < 1000", " countDownLatchInput.countDown(); " + bytes.length + " cypheredData.l " + cypheredData.length + " thread:" + Thread.currentThread().getName());
//                }
            }
        }
    }
}
