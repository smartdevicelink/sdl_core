package com.ford.syncV4.service.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/5/14
 * Time: 5:08 PM
 */

import android.util.Log;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.protocol.secure.secureproxy.IProtectServiceListener;

/**
 * This class is implement {@link ISecureServiceMessageCallback}
 */
public class SecureServiceMessageCallback implements ISecureServiceMessageCallback {

    private static final String TAG = "SecureServiceMessageCallback";
    private IProtectServiceListener protectServiceListener;
    private byte mSessionId = 0;

    public void setSyncConnection(IProtectServiceListener protectServiceListener) {
        this.protectServiceListener = protectServiceListener;
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

        if (protectServiceListener == null) {
            Log.w(TAG, SecureServiceMessageCallback.class.getSimpleName() +
                    " protectServiceListener is NULL");
            return;
        }

        switch (result) {
            case SUCCESS:
                protectServiceListener.onProtectServiceStarted(mSessionId, serviceType);
                break;
        }
    }

    @Override
    public void onHandshakeResponse(byte[] data) {
        protectServiceListener.onHandshakeResponse(data);
    }
}