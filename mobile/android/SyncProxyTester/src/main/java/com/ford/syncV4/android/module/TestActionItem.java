package com.ford.syncV4.android.module;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 19.03.14
 * Time: 15:33
 */

/**
 * This class describes an action which is provided with "action" tag
 */
public class TestActionItem {

    /**
     * This is a value of the {@link ModuleTest#ACTION_NAME_ATTR} attribute
     */
    public final static String START_RPC_SERVICE = "startRPCService";

    private long mDelay;
    private String mActionName;
    private String mNextAppId;

    public TestActionItem(String mActionName) {
        this.mActionName = mActionName;
    }

    /**
     * @return a time to delay in milliseconds
     */
    public long getDelay() {
        return mDelay;
    }

    /**
     * Set a time to delay in milliseconds
     * @param mDelay a value of the delay in milliseconds
     */
    public void setDelay(long mDelay) {
        this.mDelay = mDelay;
    }

    /**
     * @return a name of action to be performed
     */
    public String getActionName() {
        return mActionName;
    }

    public String getNextAppId() {
        return mNextAppId;
    }

    public void setNextAppId(String value) {
        mNextAppId = value;
    }

    @Override
    public String toString() {
        return TestActionItem.class.getSimpleName() + "{actionName:" + mActionName + ", " +
                "delay:" + mDelay + " ms}";
    }
}