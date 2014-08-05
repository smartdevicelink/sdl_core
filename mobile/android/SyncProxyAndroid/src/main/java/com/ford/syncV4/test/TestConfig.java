package com.ford.syncV4.test;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/27/14
 * Time: 9:58 AM
 */

import com.ford.syncV4.protocol.heartbeat.HeartbeatMonitor;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.service.secure.SecurityInternalError;
import com.ford.syncV4.service.secure.mutations.MessageFilter;
import com.ford.syncV4.service.secure.mutations.MutationManager;

import java.util.List;

/**
 * This class stores all the necessary data for SDK testing
 */
public class TestConfig {

    private static MessageFilter messageFilter;

    public static void setEncrypt(boolean encrypt){
        messageFilter.setEncrypt(encrypt);
    }

    public static boolean isEncrypt(){
        return messageFilter.isEncrypt();
    }

    public static RPCRequest filter(RPCRequest msg) {
        return messageFilter.filter(msg);
    }

    /**
     * Allow to skip Root Device check in case of, for example, Android emulator
     */
    public boolean mDoRootDeviceCheck;

    /*
     * Mutation manager for handshake procedure
     */
    private MutationManager handshakeMutationManager = new MutationManager();

    {
        handshakeMutationManager.addMutationForError(SecurityInternalError.UNKNOWN);
    }

    /*
     * Mutation manager for secure service
     */
    private MutationManager secureServiceMutationManager = new MutationManager();

    {
        secureServiceMutationManager.addMutationForError(SecurityInternalError.UNKNOWN);
    }

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
     * Indicates whether or not it is necessary to call
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} just right after RPC Session is
     * established
     */
    private boolean mDoCallRegisterAppInterface = true;
    /**
     * Indicates whether to keep USB reader thread open when perform session close procedure.
     * This feature is for the such Test Case as: call
     * {@link com.ford.syncV4.proxy.rpc.UnregisterAppInterface} (which will guide to close USB
     * reader)
     */
    private boolean mDoKeepUSBTransportConnected = false;
    /**
     * Indicates whether to start new Session as secured
     */
    private boolean mDoStartSecureSession = false;
    /**
     * Min version of the protocol
     */
    private byte mProtocolMinVersion = ProtocolConstants.PROTOCOL_VERSION_MIN;
    /**
     * Max version of the protocol
     */
    private byte mProtocolMaxVersion = ProtocolConstants.PROTOCOL_VERSION_MIN;

    /**
     * Field to keep a value of the HeartBeat Interval
     */
    private int mHeartbeatInterval = HeartbeatMonitor.HEARTBEAT_INTERVAL;

    /**
     * Field to keep a value of the capability to response for the HeartBeat which is sent from
     * SDL side with the Ack
     */
    private boolean mDoHeartbeatAck = true;

    /**
     * Field to keep a value that indicates whether to process Ack for the Heartbeat message or not.
     * Default value is true.
     */
    private boolean mDoProcessHearBeatSDLAck = true;

    /**
     * Field indicated that whether it is necessary to process Policy Table Snapshot which
     * comes from SDL or not
     */
    private boolean mDoProcessPolicyTableSnapshot = true;

    /**
     * This is a holder field for the test data of the Policy Table Update response
     */
    private byte[] mPolicyTableUpdateData = new byte[0];

    /**
     * @return a value that indicates whether to process Ack for the Heartbeat message or not.
     * Default value is true.
     */
    public boolean isDoProcessHearBeatSDLAck() {
        return mDoProcessHearBeatSDLAck;
    }

    /**
     * Set a value that indicates whether to process Ack for the Heartbeat message or not.
     * Default value is true.
     *
     * @param value true or false
     */
    public void setDoProcessHearBeatSDLAck(boolean value) {
        mDoProcessHearBeatSDLAck = value;
    }

    /**
     * @return an interval in milliseconds for the HeartBeat messages
     */
    public int getHeartbeatInterval() {
        return mHeartbeatInterval;
    }

    /**
     * Set an interval in milliseconds for the HeartBeat messages
     *
     * @param value interval in milliseconds
     */
    public void setHeartbeatInterval(int value) {
        mHeartbeatInterval = value;
    }

    /**
     * @return true if it is necessary to send Ack for the SDL's heartBeat message, false otherwise
     */
    public boolean isDoHeartbeatAck() {
        return mDoHeartbeatAck;
    }

    /**
     * Set true if it is necessary to send Ack for the SDL's heartBeat message, false otherwise
     * @param value true or false
     */
    public void setDoHeartbeatAck(boolean value) {
        mDoHeartbeatAck = value;
    }

    public MutationManager getSecureServiceMutationManager() {
        return secureServiceMutationManager;
    }

    public MutationManager getHandshakeMutationManager() {
        return handshakeMutationManager;
    }

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
     * @return <b>true</b> if it is necessary to call
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} when RPC session has been established,
     * <b>false</b> - if not
     */
    public boolean isDoCallRegisterAppInterface() {
        return mDoCallRegisterAppInterface;
    }

    /**
     * Set <b>true</b> if it is necessary to call
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} when RPC session has been established,
     * <b>false</b> - if not.<br/
     * <b> When set this value to false it is important to revert it back to true when Test is
     * complete!</b>
     *
     * @param mDoCallRegisterAppInterface boolean  value
     */
    public void setDoCallRegisterAppInterface(boolean mDoCallRegisterAppInterface) {
        this.mDoCallRegisterAppInterface = mDoCallRegisterAppInterface;
    }

    /**
     * @return whether to process Root Device check procedure, boolean value
     */
    public boolean isDoRootDeviceCheck() {
        return mDoRootDeviceCheck;
    }

    /**
     * Set whether or not to process Root Device check procedure
     *
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

    /**
     * @return <b>true</b> if a new Session need to be secured, <b>false</b> otherwise
     */
    public boolean isDoStartSecureSession() {
        return mDoStartSecureSession;
    }

    /**
     * Set a flag that indicates that a new Session need to be start as secured
     *
     * @param mDoStartSecureSession <b>true</b> if a new Session need to be secured,
     *                              <b>false</b> otherwise
     */
    public void setDoStartSecureSession(boolean mDoStartSecureSession) {
        this.mDoStartSecureSession = mDoStartSecureSession;
    }

    /**
     * @return protocol minimum version
     */
    public byte getProtocolMinVersion() {
        return mProtocolMinVersion;
    }

    /**
     * Set a minimum version of the protocol
     *
     * @param mProtocolVersion protocol version int value
     */
    public void setProtocolMinVersion(byte mProtocolVersion) {
        this.mProtocolMinVersion = mProtocolVersion;
    }

    /**
     * @return protocol maximum version
     */
    public byte getProtocolMaxVersion() {
        return mProtocolMaxVersion;
    }

    /**
     * Set a maximum version of the protocol
     *
     * @param mProtocolVersion protocol version int value
     */
    public void setProtocolMaxVersion(byte mProtocolVersion) {
        this.mProtocolMaxVersion = mProtocolVersion;
    }

    public void setHandshakeErrorList(List<SecurityInternalError> errorList) {
        if (errorList == null) {
            return;
        }
        if (errorList.isEmpty()) {
            errorList.add(SecurityInternalError.UNKNOWN);
        }
        handshakeMutationManager = new MutationManager();
        for (SecurityInternalError securityInternalError : errorList) {
            handshakeMutationManager.addMutationForError(securityInternalError);
        }
    }

    public void setSecureServiceErrorList(List<SecurityInternalError> securityInternalErrorArrayList) {
        if (securityInternalErrorArrayList == null) {
            return;
        }
        if (securityInternalErrorArrayList.isEmpty()) {
            securityInternalErrorArrayList.add(SecurityInternalError.UNKNOWN);
        }
        secureServiceMutationManager = new MutationManager();
        for (SecurityInternalError securityInternalError : securityInternalErrorArrayList) {
            secureServiceMutationManager.addMutationForError(securityInternalError);
        }
    }

    /**
     * @return true if it is necessary to process Policy Table Snapshot which
     * comes from SDL or false - otherwise
     */
    public boolean isDoProcessPolicyTableSnapshot() {
        return mDoProcessPolicyTableSnapshot;
    }

    /**
     * Set true if it is necessary to process Policy Table Snapshot which
     * comes from SDL or false - otherwise
     *
     * @param value true or false
     */
    public void setDoProcessPolicyTableSnapshot(boolean value) {
        mDoProcessPolicyTableSnapshot = value;
    }

    /**
     * @return the data for the Policy Table Update response
     */
    public byte[] getPolicyTableUpdateData() {
        return mPolicyTableUpdateData;
    }

    /**
     * Set the data for the Policy Table Update response
     *
     * @param value array of the bytes
     */
    public void setPolicyTableUpdateData(byte[] value) {
        mPolicyTableUpdateData = value;
    }
}