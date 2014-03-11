package com.ford.syncV4.protocol.secure.secureproxy;

/**
 * Created by Andrew Batutin on 3/11/14.
 */
public class SecureProxyServer {

    public void setupServer(){

    }

    public void writeData(byte [] data){

    }

    public class SecureProxyServerReader extends Thread{


        @Override
        public void run() {
            readData();
        }

        private void readData() {

        }
    }

}
