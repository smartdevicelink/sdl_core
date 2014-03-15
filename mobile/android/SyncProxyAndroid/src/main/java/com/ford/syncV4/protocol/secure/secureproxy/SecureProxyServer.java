package com.ford.syncV4.protocol.secure.secureproxy;

import android.util.Log;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.util.DebugTool;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;

/**
 * Created by Andrew Batutin on 3/11/14.
 */
public class SecureProxyServer {

    ITransportListener transportListener;

    private Socket socket;
    private InputStream inputStream;
    private OutputStream outputStream;
    private ISecureProxyServer sourceStream;
    private ServerSocket serverSocket;
    private IRCCodedDataListener RPCPacketListener;

    public SecureProxyServer(ISecureProxyServer sourceListener, ITransportListener listener) {
        sourceStream = sourceListener;
        transportListener = listener;
    }

    public void setupServer() throws IOException {
        SecureProxyServerReader secureProxyServerReader = new SecureProxyServerReader();
        secureProxyServerReader.start();
    }

    private void startServer() throws IOException {
        serverSocket = new ServerSocket(8090);
        transportListener.onTransportConnected();

    }

    public synchronized void writeData(byte[] data) throws IOException {
        outputStream.write(data);
    }

    public synchronized void setRPCPacketListener(IRCCodedDataListener RPCPacketListener) {
        this.RPCPacketListener = RPCPacketListener;
    }

    public synchronized IRCCodedDataListener getRPCPacketListener() {
        return RPCPacketListener;
    }

    public class SecureProxyServerReader extends Thread {

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
                    startServer();
                }
                readData();
            } catch (IOException e) {
                DebugTool.logError("SecureProxyServerReader fail", e);
                transportListener.onTransportError("SecureProxyServer", e);
            }
        }

        private void readData() throws IOException {
            socket = serverSocket.accept();
            inputStream = socket.getInputStream();
            outputStream = socket.getOutputStream();
            byte[] buffer = new byte[WiProProtocol.MTU_SIZE];
            try {
                int i;

                while ((i = inputStream.read(buffer)) != -1) {
                    if (sourceStream != null) {
                        sourceStream.onDataReceived(Arrays.copyOf(buffer, i));
                    }
                    if (getRPCPacketListener() != null) {
                        Log.i("cypheredData.length < 1000", "source bytes " + i);
                        getRPCPacketListener().onRPCPayloadCoded(Arrays.copyOf(buffer, i));
                    }
                }
            } catch (IOException e) {
                DebugTool.logError("SecureProxyServerReader fail", e);
                transportListener.onTransportError("SecureProxyServer", e);
            }
        }
    }

}
