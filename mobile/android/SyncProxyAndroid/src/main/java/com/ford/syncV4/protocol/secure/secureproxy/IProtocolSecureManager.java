package com.ford.syncV4.protocol.secure.secureproxy;

import java.io.IOException;

/**
 * Created by Andrew Batutin on 6/23/14.
 */
public interface IProtocolSecureManager {

    public byte[] sendDataToProxyServerByChunk(boolean isEncrypted, byte[] data) throws IOException, InterruptedException;

    byte[] sendDataToProxyServerByteByByte(boolean isEncrypted, byte[] data) throws IOException, InterruptedException;
}
