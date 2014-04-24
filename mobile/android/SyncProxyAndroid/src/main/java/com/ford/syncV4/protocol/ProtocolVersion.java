package com.ford.syncV4.protocol;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 02.04.14
 * Time: 13:07
 */

import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.util.logger.Logger;

/**
 * This class handle protocol version and version negotiation
 */
public class ProtocolVersion {

    @SuppressWarnings("unused")
    private static final String LOG_TAG = ProtocolVersion.class.getSimpleName();

    /**
     * Current actual version for the SDK to operate with SDL
     */
    private byte mCurrentVersion;

    /**
     * Constructor
     */
    public ProtocolVersion() {
        // Set up default value
        mCurrentVersion = ProtocolConstants.PROTOCOL_VERSION_MIN;
    }

    /**
     * @return current actual protocol version,
     * see {@link com.ford.syncV4.proxy.constants.ProtocolConstants} for more information
     */
    public byte getCurrentVersion() {
        return mCurrentVersion;
    }

    /**
     * Set up current actual protocol version,
     * see {@link com.ford.syncV4.proxy.constants.ProtocolConstants} for more information
     * @param value version of protocol
     */
    public void setCurrentVersion(byte value) {
        mCurrentVersion = negotiateVersion(value);
        Logger.d("Negotiated v:" + mCurrentVersion);
    }

    /**
     * Negotiate current protocol version according to AppLink protocol
     * @param value version value to be updated to
     * @return negotiated version value
     */
    private byte negotiateVersion(byte value) {
        Logger.d("Negotiate value:" + mCurrentVersion + " to:" + value +
                " min:" + ProtocolConstants.PROTOCOL_VERSION_MIN +
                " max:" + ProtocolConstants.PROTOCOL_VERSION_MAX);
        // Check the bounds conditions of the
        if (value <= 0 || value > Byte.MAX_VALUE) {
            return mCurrentVersion;
        }
        if (value < ProtocolConstants.PROTOCOL_VERSION_MIN) {
            return value;
        }
        if (value >= ProtocolConstants.PROTOCOL_VERSION_MAX) {
            return ProtocolConstants.PROTOCOL_VERSION_MAX;
        }
        return value;
    }
}