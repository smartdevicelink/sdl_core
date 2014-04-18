package com.ford.syncV4.protocol;

import com.ford.syncV4.util.logger.Logger;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.04.14
 * Time: 12:43
 */
public class RunnableWithPriority implements Runnable {

    private byte mPriority;
    private int mCorrelationId;

    public RunnableWithPriority(byte priority, int correlationId) {
        //Logger.d("TRACE: " + correlationId);
        mPriority = priority;
        mCorrelationId = correlationId;
    }

    @Override
    public void run() {

    }

    public int getCorrelationId() {
        return mCorrelationId;
    }

    public byte getPriority() {
        return mPriority;
    }
}