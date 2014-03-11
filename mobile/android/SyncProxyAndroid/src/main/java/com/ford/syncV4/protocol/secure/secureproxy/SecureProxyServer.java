package com.ford.syncV4.protocol.secure.secureproxy;

import android.util.Log;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by Andrew Batutin on 3/11/14.
 */
public class SecureProxyServer {

    public void setupServer() throws IOException {
        SecureProxyServerReader secureProxyServerReader = new SecureProxyServerReader();
        secureProxyServerReader.start();
    }

    private void startServer() throws IOException {
        ServerSocket serverSocket = new ServerSocket(8081);
        Socket socket = serverSocket.accept();
        socket.getOutputStream();
        socket.getInputStream();
    }

    public void writeData(byte[] data) {

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
            if (!isConnected()){
                try {
                    startServer();
                } catch (IOException e) {
                    Log.e("SecureProxyServer", "error", e);
                }
            }
            readData();
        }

        private void readData() {

        }
    }

}
