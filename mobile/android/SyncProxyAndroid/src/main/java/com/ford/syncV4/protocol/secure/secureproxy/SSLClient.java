package com.ford.syncV4.protocol.secure.secureproxy;

import android.util.Log;

import com.ford.syncV4.transport.ITransportListener;

import java.io.IOException;
import java.net.InetAddress;

import javax.net.SocketFactory;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;

/**
 * Created by Andrew Batutin on 3/11/14.
 */
public class SSLClient {

    private SSLSocket socket;
    ITransportListener transportListener;

    public SSLClient(ITransportListener transportListener) {
        this.transportListener = transportListener;
    }

    public void setupClient() throws IOException {

        SSLClientReader sslClientReader = new SSLClientReader();
        sslClientReader.start();
    }




    private void startSocket() throws IOException {
        SocketFactory sf = SSLSocketFactory.getDefault();
        InetAddress addr = InetAddress.getByName("127.0.0.1");
        socket = (SSLSocket) sf.createSocket(addr, 8091);
        socket.getSession();
        transportListener.onTransportConnected();

    }

    public synchronized void writeData(byte[] data) throws IOException {
        socket.getOutputStream().write(data);
    }

    public class SSLClientReader extends Thread {

        @Override
        public void run() {
            try {
                startSocket();

            } catch (IOException e) {
                Log.e("SecureProxyServer", "error", e);
            }
        }
    }

}
