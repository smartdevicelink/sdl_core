package com.ford.syncV4.protocol;

import com.ford.syncV4.util.logger.Logger;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.04.14
 * Time: 12:43
 */
public class RunnableWithPriority implements Runnable {

    @SuppressWarnings("unused")
    private static final String LOG_TAG = RunnableWithPriority.class.getSimpleName();

    private byte mPriority;
    private int mCorrelationId;

    public RunnableWithPriority(byte priority, int correlationId) {
        Logger.d(LOG_TAG + " TRACE priority: " + priority);
        mPriority = priority;
        mCorrelationId = correlationId;
    }

    @Override
    public void run() {
        Logger.d("TRACE Run");
        try {
            Thread.sleep(50);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public int getCorrelationId() {
        return mCorrelationId;
    }

    public byte getPriority() {
        return mPriority;
    }
}