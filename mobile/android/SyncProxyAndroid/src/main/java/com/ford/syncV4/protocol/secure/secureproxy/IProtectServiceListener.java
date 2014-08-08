package com.ford.syncV4.protocol.secure.secureproxy;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.secure.SecurityInternalError;

import java.util.Hashtable;

/**
 * Created by Andrew Batutin on 3/12/14.
 */
public interface IProtectServiceListener {

    public void onHandshakeResponse(byte[] data);

    public void onHandshakeError(SecurityInternalError error, Hashtable<String, Object> errorDescription);
}
