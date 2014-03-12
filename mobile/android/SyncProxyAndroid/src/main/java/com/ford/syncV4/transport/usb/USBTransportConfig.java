package com.ford.syncV4.transport.usb;

import android.content.Context;

import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.transport.TransportType;

/**
 * Container of USB transport specific configuration.
 */
public final class USBTransportConfig extends BaseTransportConfig {
    /**
     * Android context to communicate with the OS (for broadcast receiver).
     */
    private Context mContext;

    /**
     * Constructs the config with all the required parameters.
     *
     * @param mContext Android context to work with broadcast receivers
     */
    public USBTransportConfig(Context mContext) {
        this.mContext = mContext;
    }

    /**
     * Returns the current context.
     *
     * @return current context used by USB transport
     */
    public Context getContext() {
        return mContext;
    }

    /**
     * Returns specific type of this transport configuration.
     *
     * @return Constant value TransportType.USB
     * @see com.ford.syncV4.transport.TransportType
     */
    public TransportType getTransportType() {
        return TransportType.USB;
    }

}
