package com.ford.syncV4.protocol.secure.secureproxy;

import android.test.AndroidTestCase;

import static org.mockito.Mockito.mock;

/**
 * Created by Andrew Batutin on 3/15/14.
 */
public class ProtocolSecureManagerTest extends AndroidTestCase {

    public void testSyncSendingOfDataToSSLSocket() throws Exception {
        ISecureProxyServer secureProxyServerInt = mock(ISecureProxyServer.class);
        ProtocolSecureManager protocolSecureManager = new ProtocolSecureManager(secureProxyServerInt);
        SSLClient sslClient = mock(SSLClient.class);
        SecureProxyServer secureProxyServer = mock(SecureProxyServer.class);
        byte [] data = new byte[10];
        protocolSecureManager.sslClient = sslClient;
        protocolSecureManager.secureProxyServer = secureProxyServer;
        byte [] result = protocolSecureManager.sendDataTOSSLClient(data);
    }
}
