package com.ford.syncV4.protocol.secure.secureproxy;

import android.util.Log;

import com.ford.syncV4.transport.ITransportListener;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by Andrew Batutin on 3/11/14.
 */
public class SecureProxyServer {

    ITransportListener transportListener;

    private Socket socket;
    private InputStream inputStream;
    private OutputStream outputStream;
    private OutputStream sourceStream;
    private ServerSocket serverSocket;

    public SecureProxyServer(OutputStream outputStream, ITransportListener listener) {
        sourceStream = outputStream;
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

    public synchronized void writeData(byte[] data) {
        try {
            outputStream.write(data);
        } catch (IOException e) {
            Log.e("SecureProxyServer", "error", e);
        }
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
                Log.e("SecureProxyServer", "error", e);
            }
        }

        private void readData() throws IOException {
            socket = serverSocket.accept();
            inputStream = socket.getInputStream();
            outputStream = socket.getOutputStream();
            byte[] buffer = new byte[1024];
            try {
                int i;
                while ((i = inputStream.read(buffer)) != -1) {

                    sourceStream.write(buffer, 0, i);

                }
            } catch (IOException e) {
                Log.e("SecureProxyServer", "error", e);
            }
        }
    }

}
