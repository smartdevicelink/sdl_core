package com.ford.syncV4.protocol.secure.secureproxy;

import com.ford.syncV4.protocol.enums.ServiceType;

/**
 * Created by Andrew Batutin on 3/12/14.
 */
public interface IProtectServiceListener {

    public void onProtectServiceStarted(byte sessionId, ServiceType serviceType);

    public void onHandshakeResponse(byte[] data);
}
