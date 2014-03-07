package com.ford.syncV4.service.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/5/14
 * Time: 5:08 PM
 */

import android.util.Log;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.syncConnection.SyncConnection;

/**
 * This class is implement {@link ISecureServiceMessageCallback}
 */
public class SecureServiceMessageCallback implements ISecureServiceMessageCallback {

    private static final String TAG = "SecureServiceMessageCallback";
    private SyncConnection mSyncConnection;
    private byte mSessionId = 0;

    /**
     * Set an instance of the {@link com.ford.syncV4.syncConnection.SyncConnection} object
     *
     * @param mSyncConnection {@link com.ford.syncV4.syncConnection.SyncConnection}
     */
    public void setSyncConnection(SyncConnection mSyncConnection) {
        this.mSyncConnection = mSyncConnection;
    }

    /**
     * Set current session Id
     *
     * @param mSessionId {@link java.lang.Byte}
     */
    public void setSessionId(byte mSessionId) {
        this.mSessionId = mSessionId;
    }

    @Override
    public void onProtectServiceResponse(ProtectServiceResponse result, ServiceType serviceType) {
        Log.d(TAG, "ProtectServiceResponse:" + result);

        if (mSyncConnection == null) {
            Log.w(TAG, SecureServiceMessageCallback.class.getSimpleName() +
                    " SyncConnection is NULL");
            return;
        }

        if (mSyncConnection.getWiProProtocol() == null) {
            Log.w(TAG, SecureServiceMessageCallback.class.getSimpleName() +
                    " WiProProtocol is NULL");
            return;
        }

        switch (result) {
            case SUCCESS:
                mSyncConnection.getWiProProtocol().startSecureHandshake(mSessionId, serviceType);
                break;
        }
    }

    @Override
    public void onHandshakeResponse(byte[] data) {
        Log.d(TAG, "HandshakeResponse:" + data);
    }
}