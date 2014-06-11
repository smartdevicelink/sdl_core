package com.ford.syncV4.android.manager;

import com.ford.syncV4.util.logger.Logger;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11.06.14
 * Time: 17:31
 */
public class RestoreConnectionManager {

    private static final String LOG = RestoreConnectionManager.class.getSimpleName();

    private final ReentrantLock reentrantLock = new ReentrantLock();
    private final Condition condition = reentrantLock.newCondition();
    private volatile boolean mConditionState = false;

    public void acquireLock() {
        reentrantLock.lock();
        try {
            mConditionState = false;
            while (!mConditionState) {
                condition.await();
            }
        } catch (InterruptedException e) {
            Logger.e(LOG + " acquire lock error:" + e.getMessage());
        } finally {
            reentrantLock.unlock();
        }
    }

    public void releaseLock() {
        reentrantLock.lock();
        try {
            if (reentrantLock.getHoldCount() > 0) {
                mConditionState = true;
                condition.signal();
            }
        } finally {
            reentrantLock.unlock();
        }
    }
}