package com.ford.syncV4.protocol.secure.secureproxy;

/**
 * Created by Andrew Batutin on 3/12/14.
 */
public interface ISecureProxyServer {

    public void onDataReceived(byte[] data);

    public void onHandShakeCompleted();

}
