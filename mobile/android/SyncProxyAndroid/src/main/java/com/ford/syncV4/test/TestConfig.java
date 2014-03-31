package com.ford.syncV4.test;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/27/14
 * Time: 9:58 AM
 */

/**
 * This class stores all the necessary data for SDK testing
 */
public class TestConfig {

    /**
     * Indicates whether or not to use parameter
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} when initiating
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} instance
     */
    private boolean mUseHashId;

    /**
     * Indicates whether or not to use custom parameter
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} when initiating
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} instance
     */
    private boolean mUseCustomHashId;

    /**
     * Holds a custom value of the
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} when initiating
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} instance
     */
    private String mCustomHashId;

    /**
     * Allow to skip Root Device check in case of, for example, Android emulator
     */
    public boolean mDoRootDeviceCheck;

    /**
     * Indicates whether to keep USB reader thread open when perform session close procedure.
     * This feature is for the such Test Case as: call
     * {@link com.ford.syncV4.proxy.rpc.UnregisterAppInterface} (which will guide to close USB
     * reader)
     */
    private boolean mDoKeepUSBTransportConnected = false;

    /**
     * @return a value of the functionality: whether or not to use parameter
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} when initiating
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} instance, <b>true</b> | <b>false</b>
     */
    public boolean isUseHashId() {
        return mUseHashId;
    }

    /**
     * Set a value of the functionality: whether or not to use parameter
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} when initiating
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} instance, <b>true</b> | <b>false</b>
     *
     * @param mUseHashId <b>true</b> | <b>false</b>
     */
    public void setUseHashId(boolean mUseHashId) {
        this.mUseHashId = mUseHashId;
    }

    /**
     * @return a value of the functionality: whether or not to use custom parameter
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} when initiating
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} instance, <b>true</b> | <b>false</b>
     */
    public boolean isUseCustomHashId() {
        return mUseCustomHashId;
    }

    /**
     * Set a value of the functionality: whether or not to use custom parameter
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} when initiating
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} instance, <b>true</b> | <b>false</b>
     *
     * @param mUseCustomHashId <b>true</b> | <b>false</b>
     */
    public void setUseCustomHashId(boolean mUseCustomHashId) {
        this.mUseCustomHashId = mUseCustomHashId;
    }

    /**
     * @return custom value of the
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} when initiating
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} instance
     */
    public String getCustomHashId() {
        return mCustomHashId;
    }

    /**
     * Set a custom value of the
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} when initiating
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} instance
     *
     * @param mCustomHashId {@link java.lang.String}
     */
    public void setCustomHashId(String mCustomHashId) {
        this.mCustomHashId = mCustomHashId;
    }

    /**
     * @return whether to process Root Device check procedure, boolean value
     */
    public boolean isDoRootDeviceCheck() {
        return mDoRootDeviceCheck;
    }

    /**
     * Set whether or not to process Root Device check procedure
     * @param mDoRootDeviceCheck boolean value
     */
    public void setDoRootDeviceCheck(boolean mDoRootDeviceCheck) {
        this.mDoRootDeviceCheck = mDoRootDeviceCheck;
    }

    /**
     * @return whether to keep USB reader thread open when perform session close procedure.
     */
    public boolean isDoKeepUSBTransportConnected() {
        return mDoKeepUSBTransportConnected;
    }

    /**
     * Set whether to keep USB reader thread open when perform session close procedure.
     * <b> When set this value to true it is important to revert it back to false when Test is
     * complete!</b>
     *
     * @param mDoKeepUSBTransportConnected boolean value
     */
    public void setDoKeepUSBTransportConnected(boolean mDoKeepUSBTransportConnected) {
        this.mDoKeepUSBTransportConnected = mDoKeepUSBTransportConnected;
    }
}