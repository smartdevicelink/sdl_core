package com.ford.syncV4.proxy.session;

import com.ford.syncV4.protocol.AbstractProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;

/**
 * Created by Andrew Batutin on 8/20/13.
 * <p/>
 * Abstraction Over Mobile Navigation Session Concept
 */

public class MobileNavSession {

    private AbstractProtocol mAbstractProtocol;

    private MobileNavSession() {

    }

    public MobileNavSession(AbstractProtocol protocol) {
        mAbstractProtocol = protocol;
    }

    public void startService(byte sessionId) {
        mAbstractProtocol.StartProtocolService(ServiceType.Mobile_Nav, sessionId, false);
    }
}
