package com.ford.syncV4.protocol.secure.secureproxy;

/**
 * Created by Andrew Batutin on 3/15/14.
 */
public interface IHandshakeDataListener {

    public void onHandshakeDataReceived(byte[] data);

    public void onHandShakeCompleted();

    public void onError(Exception e);

}
