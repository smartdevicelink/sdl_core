package com.ford.syncV4.transport.usb;

import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.transport.TransportType;

/**
 * Container of USB transport specific configuration.
 */
public final class USBTransportConfig extends BaseTransportConfig {

    /**
     * Overridden abstract method which returns specific type of this transport configuration.
     *
     * @return Constant value TransportType.USB.
     * @see com.ford.syncV4.transport.TransportType
     */
    public TransportType getTransportType() {
        return TransportType.USB;
    }

}
