package com.ford.syncV4.protocol.secure.secureproxy;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.util.DebugTool;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Arrays;

/**
 * Created by Andrew Batutin on 3/20/14.
 */
public class SecureProxyClient implements ISSLComponent {

    private ITransportListener transportListener;
    private ISecureProxyServer sourceStream;
    private IRPCodedDataListener RPCPacketListener;
    private InputStream inputStream;
    private OutputStream outputStream;
    private Socket socket;

    public SecureProxyClient(ISecureProxyServer sourceListener, ITransportListener listener) {
        sourceStream = sourceListener;
        transportListener = listener;
    }

    @Override
    public void setupClient() throws IOException {
        SecureProxyClientReader secureProxyServerReader = new SecureProxyClientReader();
        secureProxyServerReader.start();
    }

    public synchronized void writeData(byte[] data) throws IOException {
        outputStream.write(data);
    }

    public synchronized void setRPCPacketListener(IRPCodedDataListener RPCPacketListener) {
        this.RPCPacketListener = RPCPacketListener;
    }

    public synchronized IRPCodedDataListener getRPCPacketListener() {
        return RPCPacketListener;
    }


    public class SecureProxyClientReader extends Thread {

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
                if (!isConnected()) {
                    startClient();
                }
                readData();
            } catch (IOException e) {
                DebugTool.logError("SecureProxyClientReader fail", e);
                transportListener.onTransportError("SecureProxyClientReader", e);
            }
        }

        private void startClient() throws IOException {
            socket = new Socket("127.0.0.1", 8090);
            inputStream = socket.getInputStream();
            outputStream = socket.getOutputStream();

        }

        private void readData() throws IOException {
            byte[] buffer = new byte[WiProProtocol.MTU_SIZE];
            try {
                int i;

                while ((i = inputStream.read(buffer)) != -1) {
                    if (sourceStream != null) {
                        sourceStream.onDataReceived(Arrays.copyOf(buffer, i));
                    }
                    if (getRPCPacketListener() != null) {
                        getRPCPacketListener().onRPCPayloadCoded(Arrays.copyOf(buffer, i));
                    }
                }
            } catch (IOException e) {
                DebugTool.logError("SecureProxyClientReader fail", e);
                transportListener.onTransportError("SecureProxyClientReader", e);
            }
        }
    }
}
