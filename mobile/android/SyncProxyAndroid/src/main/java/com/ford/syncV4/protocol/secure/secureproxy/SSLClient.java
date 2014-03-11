package com.ford.syncV4.protocol.secure.secureproxy;

import java.io.IOException;
import java.net.InetAddress;

import javax.net.SocketFactory;
import javax.net.ssl.SSLSession;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;

/**
 * Created by Andrew Batutin on 3/11/14.
 */
public class SSLClient {

    public void setupClient() throws IOException {
        SocketFactory sf = SSLSocketFactory.getDefault();
        InetAddress addr = InetAddress.getByName("127.0.0.1");
        SSLSocket socket = (SSLSocket) sf.createSocket(addr, 8081);
        SSLSession s = socket.getSession();
    }

    private void writeData(byte[] data){

    }

    public class SSLClientReader extends Thread{

        @Override
        public void run() {

        }
    }

}
