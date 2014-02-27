package com.ford.syncV4.proxy;

import android.os.Handler;
import android.os.Looper;
import android.telephony.TelephonyManager;
import android.util.Log;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.messageDispatcher.IDispatchingStrategy;
import com.ford.syncV4.messageDispatcher.IncomingProtocolMessageComparitor;
import com.ford.syncV4.messageDispatcher.InternalProxyMessageComparitor;
import com.ford.syncV4.messageDispatcher.OutgoingProtocolMessageComparitor;
import com.ford.syncV4.messageDispatcher.ProxyMessageDispatcher;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.protocol.heartbeat.HeartbeatMonitor;
import com.ford.syncV4.proxy.callbacks.InternalProxyMessage;
import com.ford.syncV4.proxy.callbacks.OnError;
import com.ford.syncV4.proxy.callbacks.OnProxyClosed;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.converter.IRPCRequestConverter;
import com.ford.syncV4.proxy.converter.IRPCRequestConverterFactory;
import com.ford.syncV4.proxy.converter.SyncRPCRequestConverterFactory;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.interfaces.IProxyListenerBase;
import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.AddSubMenu;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.ButtonCapabilities;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteCommand;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteSubMenu;
import com.ford.syncV4.proxy.rpc.DisplayCapabilities;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.proxy.rpc.ListFiles;
import com.ford.syncV4.proxy.rpc.OnAppInterfaceUnregistered;
import com.ford.syncV4.proxy.rpc.OnLanguageChange;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.PerformInteraction;
import com.ford.syncV4.proxy.rpc.PresetBankCapabilities;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.RegisterAppInterfaceResponse;
import com.ford.syncV4.proxy.rpc.ResetGlobalProperties;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimer;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.SoftButtonCapabilities;
import com.ford.syncV4.proxy.rpc.Speak;
import com.ford.syncV4.proxy.rpc.SubscribeButton;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.SyncPData;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterface;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterfaceResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeButton;
import com.ford.syncV4.proxy.rpc.VehicleType;
import com.ford.syncV4.proxy.rpc.enums.AppHMIType;
import com.ford.syncV4.proxy.rpc.enums.AppInterfaceUnregisteredReason;
import com.ford.syncV4.proxy.rpc.enums.AudioStreamingState;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;
import com.ford.syncV4.proxy.rpc.enums.HMILevel;
import com.ford.syncV4.proxy.rpc.enums.HmiZoneCapabilities;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.SyncConnectionState;
import com.ford.syncV4.proxy.rpc.enums.SyncDisconnectedReason;
import com.ford.syncV4.proxy.rpc.enums.SyncInterfaceAvailability;
import com.ford.syncV4.proxy.rpc.enums.SystemContext;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;
import com.ford.syncV4.proxy.rpc.enums.VrCapabilities;
import com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandler;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;
import com.ford.syncV4.service.Service;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.syncConnection.ISyncConnectionListener;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.trace.SyncTrace;
import com.ford.syncV4.trace.TraceDeviceInfo;
import com.ford.syncV4.trace.enums.InterfaceActivityDirection;
import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.transport.SiphonServer;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.util.Base64;
import com.ford.syncV4.util.CommonUtils;
import com.ford.syncV4.util.DebugTool;
import com.ford.syncV4.util.TestConfig;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.util.EntityUtils;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.List;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

public abstract class SyncProxyBase<proxyListenerType extends IProxyListenerBase> implements
        ISystemRequestProxy {
    // Used for calls to Android Log class.
    public static final String TAG = "SyncProxy";
    // Synchronization Objects
    static final Object CONNECTION_REFERENCE_LOCK = new Object(),
            INCOMING_MESSAGE_QUEUE_THREAD_LOCK = new Object(),
            OUTGOING_MESSAGE_QUEUE_THREAD_LOCK = new Object(),
            INTERNAL_MESSAGE_QUEUE_THREAD_LOCK = new Object(),
            APP_INTERFACE_REGISTERED_LOCK = new Object();
    /**
     * Interval between heartbeat messages, in milliseconds.
     * NOTE: this value is not specified in the protocol, and thus must be
     * negotiated with the Sync.
     */
    static final int HEARTBEAT_INTERVAL = 5000;
    protected static final String SYNC_LIB_TRACE_KEY = "42baba60-eb57-11df-98cf-0800200c9a66";
    /**
     * Delay between proxy disconnect (e.g., transport error) and another proxy
     * reconnect attempt.
     */
    private static final int PROXY_RECONNECT_DELAY = 5000;
    /**
     * Lock to access the _currentReconnectTimerTask member.
     */
    private static final Object RECONNECT_TIMER_TASK_LOCK = new Object();
    final int HEARTBEAT_CORRELATION_ID = 65531; // TODO: remove
    // Protected Correlation IDs
    public final int REGISTER_APP_INTERFACE_CORRELATION_ID = 65529,
            UNREGISTER_APP_INTERFACE_CORRELATION_ID = 65530,
            POLICIES_CORRELATION_ID = 65535;
    private IRPCMessageHandler rpcMessageHandler;

    public Boolean getAdvancedLifecycleManagementEnabled() {
        return _advancedLifecycleManagementEnabled;
    }

    // SyncProxy Advanced Lifecycle Management
    protected Boolean _advancedLifecycleManagementEnabled = false;


    // Proxy State Variables
    protected Boolean _appInterfaceRegisterd = false;
    protected Boolean _haveReceivedFirstFocusLevel = false;
    protected Boolean _haveReceivedFirstFocusLevelFull = false;
    protected Boolean _proxyDisposed = false;

    public SyncConnectionState getSyncConnectionState() {
        return _syncConnectionState;
    }

    public void setSyncConnectionState(SyncConnectionState syncConnectionState) {
        this._syncConnectionState = syncConnectionState;
    }

    protected SyncConnectionState _syncConnectionState = null;
    protected SyncInterfaceAvailability _syncIntefaceAvailablity = null;
    protected HMILevel _hmiLevel = null;
    protected AudioStreamingState _audioStreamingState = null;
    protected SystemContext _systemContext = null;

    public SyncMsgVersion getSyncMsgVersion() throws SyncException {
        return _syncMsgVersion;
    }

    public void setSyncMsgVersion(SyncMsgVersion syncMsgVersion) {
        this._syncMsgVersion = syncMsgVersion;
    }

    // Variables set by RegisterAppInterfaceResponse
    protected SyncMsgVersion _syncMsgVersion = null;
    protected String _autoActivateIdReturned = null;

    public Language getSyncLanguage() throws SyncException {
        return _syncLanguage;
    }

    public void setSyncLanguage(Language syncLanguage) {
        this._syncLanguage = syncLanguage;
    }

    protected Language _syncLanguage = null;

    public Language getHmiDisplayLanguage() throws SyncException {
        return _hmiDisplayLanguage;
    }

    public void setHmiDisplayLanguage(Language hmiDisplayLanguage) {
        this._hmiDisplayLanguage = hmiDisplayLanguage;
    }

    protected Language _hmiDisplayLanguage = null;

    public DisplayCapabilities getDisplayCapabilities() throws SyncException {
        return _displayCapabilities;
    }

    public void setDisplayCapabilities(DisplayCapabilities displayCapabilities) {
        this._displayCapabilities = displayCapabilities;
    }

    protected DisplayCapabilities _displayCapabilities = null;

    public Vector<ButtonCapabilities> getButtonCapabilities() throws SyncException {
        return _buttonCapabilities;
    }

    public void setButtonCapabilities(Vector<ButtonCapabilities> buttonCapabilities) {
        this._buttonCapabilities = buttonCapabilities;
    }

    protected Vector<ButtonCapabilities> _buttonCapabilities = null;

    public Vector<SoftButtonCapabilities> getSoftButtonCapabilities() throws SyncException {
        return _softButtonCapabilities;
    }

    public void setSoftButtonCapabilities(Vector<SoftButtonCapabilities> softButtonCapabilities) {
        this._softButtonCapabilities = softButtonCapabilities;
    }

    protected Vector<SoftButtonCapabilities> _softButtonCapabilities = null;

    public PresetBankCapabilities getPresetBankCapabilities() throws SyncException {
        return _presetBankCapabilities;
    }

    public void setPresetBankCapabilities(PresetBankCapabilities presetBankCapabilities) {
        this._presetBankCapabilities = presetBankCapabilities;
    }

    protected PresetBankCapabilities _presetBankCapabilities = null;

    public Vector<HmiZoneCapabilities> getHmiZoneCapabilities() throws SyncException {
        return _hmiZoneCapabilities;
    }

    public void setHmiZoneCapabilities(Vector<HmiZoneCapabilities> hmiZoneCapabilities) {
        this._hmiZoneCapabilities = hmiZoneCapabilities;
    }

    protected Vector<HmiZoneCapabilities> _hmiZoneCapabilities = null;

    public Vector<SpeechCapabilities> getSpeechCapabilities() throws SyncException {
        return _speechCapabilities;
    }

    public void setSpeechCapabilities(Vector<SpeechCapabilities> speechCapabilities) {
        this._speechCapabilities = speechCapabilities;
    }

    protected Vector<SpeechCapabilities> _speechCapabilities = null;

    public Vector<VrCapabilities> getVrCapabilities() throws SyncException {
        return _vrCapabilities;
    }

    public void setVrCapabilities(Vector<VrCapabilities> vrCapabilities) {
        this._vrCapabilities = vrCapabilities;
    }

    protected Vector<VrCapabilities> _vrCapabilities = null;

    public VehicleType getVehicleType() throws SyncException {
        return _vehicleType;
    }

    public void setVehicleType(VehicleType vehicleType) {
        this._vehicleType = vehicleType;
    }

    protected VehicleType _vehicleType = null;

    public Boolean getFirstTimeFull() {
        return firstTimeFull;
    }

    public void setFirstTimeFull(Boolean firstTimeFull) {
        this.firstTimeFull = firstTimeFull;
    }

    protected Boolean firstTimeFull = true;
    protected byte _wiproVersion = 1;

    SyncConnection mSyncConnection;

    // RPC Session ID
    protected Session currentSession = Session.createSession(ServiceType.RPC, Session.DEFAULT_SESSION_ID);
    Boolean _haveReceivedFirstNonNoneHMILevel = false;

    public proxyListenerType getProxyListener() {
        return _proxyListener;
    }

    private proxyListenerType _proxyListener = null;
    // Device Info for logging
    private TraceDeviceInfo _traceDeviceInterrogator = null;
    // Declare Queuing Threads
    private ProxyMessageDispatcher<ProtocolMessage> _incomingProxyMessageDispatcher;
    private ProxyMessageDispatcher<ProtocolMessage> _outgoingProxyMessageDispatcher;
    private ProxyMessageDispatcher<InternalProxyMessage> _internalProxyMessageDispatcher;

    public Boolean getCallbackToUIThread() {
        return _callbackToUIThread;
    }

    public void setCallbackToUIThread(Boolean callbackToUIThread) {
        this._callbackToUIThread = callbackToUIThread;
    }

    // Flag indicating if callbacks should be called from UIThread
    private Boolean _callbackToUIThread = false;

    public Handler getMainUIHandler() {
        return _mainUIHandler;
    }

    public void setMainUIHandler(Handler mainUIHandler) {
        this._mainUIHandler = mainUIHandler;
    }

    // UI Handler
    private Handler _mainUIHandler = null;
    // Parameters passed to the constructor from the app to register an app interface
    private String _applicationName = null;
    private Vector<TTSChunk> _ttsName = null;
    private String _ngnMediaScreenAppName = null;
    private Boolean _isMediaApp = null;
    private Language _syncLanguageDesired = null;
    private Language _hmiDisplayLanguageDesired = null;
    private Vector<AppHMIType> _appHMIType = null;
    private String _appID = null;
    private String _autoActivateIdDesired = null;
    private SyncMsgVersion _syncMsgVersionRequest = null;
    private Vector<String> _vrSynonyms = null;
    // Updated hashID which can be used over connection cycles
    // (i.e. loss of connection, ignition cycles, etc.)
    private String mHashId = null;
    // This Config object stores all the necessary data for SDK testing
    private TestConfig mTestConfig;

    /**
     * Set hashID which can be used over connection cycles
     *
     * @return value of the hashId
     */
    public String getHashId() {
        // For the Test Cases
        if (mTestConfig != null) {
            if (!mTestConfig.isUseHashId()) {
                return null;
            }
            if (mTestConfig.isUseCustomHashId()) {
                return mTestConfig.getCustomHashId();
            }
        }

        return mHashId;
    }

    /**
     * Get hashID which can be used over connection cycles
     *
     * @param mHashId value of the hashId
     */
    public void setHashId(String mHashId) {
        this.mHashId = mHashId;
    }

    public OnLanguageChange getLastLanguageChange() {
        return _lastLanguageChange;
    }

    public void setLastLanguageChange(OnLanguageChange lastLanguageChange) {
        this._lastLanguageChange = lastLanguageChange;
    }

    private OnLanguageChange _lastLanguageChange = null;
    // JSON RPC Marshaller
    private IJsonRPCMarshaller _jsonRPCMarshaller = new JsonRPCMarshaller();
    /**
     * Contains current configuration for the transport that was selected during
     * construction of this object
     */
    private BaseTransportConfig _transportConfig = null;

    public HMILevel getPriorHmiLevel() {
        return _priorHmiLevel;
    }

    public void setPriorHmiLevel(HMILevel priorHmiLevel) {
        this._priorHmiLevel = priorHmiLevel;
    }

    private HMILevel _priorHmiLevel = null;

    public AudioStreamingState getPriorAudioStreamingState() {
        return _priorAudioStreamingState;
    }

    public void setPriorAudioStreamingState(AudioStreamingState priorAudioStreamingState) {
        this._priorAudioStreamingState = priorAudioStreamingState;
    }

    private AudioStreamingState _priorAudioStreamingState = null;
    // Interface broker
    private SyncInterfaceBroker _interfaceBroker = null;
    /**
     * Timer that is used to schedule proxy reconnect tasks.
     */
    private Timer _reconnectTimer = null;
    /**
     * Currently scheduled proxy reconnect task, if any.
     */
    private TimerTask _currentReconnectTimerTask = null;
    private static int heartBeatInterval = HEARTBEAT_INTERVAL;
    private IRPCRequestConverterFactory rpcRequestConverterFactory =
            new SyncRPCRequestConverterFactory();
    private IProtocolMessageHolder protocolMessageHolder =
            new ProtocolMessageHolder();

    /**
     * Handler for OnSystemRequest notifications.
     */
    private IOnSystemRequestHandler onSystemRequestHandler;

    /**
     * A set of internal requests' correlation IDs that are currently in
     * progress.
     */
    private Set<Integer> internalRequestCorrelationIDs;

    /**
     * Correlation ID that was last used for messages created internally.
     */
    private int lastCorrelationId = 40000;

    public void setSyncConnection(SyncConnection syncConnection) {
        this.mSyncConnection = syncConnection;
    }

    /**
     * Constructor.
     *
     * @param listener                          Type of listener for this proxy base.
     * @param syncProxyConfigurationResources   Configuration resources for this proxy.
     * @param enableAdvancedLifecycleManagement Flag that ALM should be enabled or not.
     * @param appName                           Client application name.
     * @param ttsName                           TTS name.
     * @param ngnMediaScreenAppName             Media Screen Application name.
     * @param vrSynonyms                        List of synonyms.
     * @param isMediaApp                        Flag that indicates that client application if media application or not.
     * @param syncMsgVersion                    Version of Sync Message.
     * @param languageDesired                   Desired language.
     * @param hmiDisplayLanguageDesired         Desired language for HMI.
     * @param appHMIType                        Type of application.
     * @param appID                             Application identifier.
     * @param autoActivateID                    Auto activation identifier.
     * @param callbackToUIThread                Flag that indicates that this proxy should send callback to UI thread or not.
     * @param transportConfig                   Configuration of transport to be used by underlying connection.
     * @throws SyncException
     */
    protected SyncProxyBase(proxyListenerType listener, SyncProxyConfigurationResources syncProxyConfigurationResources,
                            boolean enableAdvancedLifecycleManagement, String appName, Vector<TTSChunk> ttsName,
                            String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion,
                            Language languageDesired, Language hmiDisplayLanguageDesired, Vector<AppHMIType> appHMIType, String appID,
                            String autoActivateID, boolean callbackToUIThread, BaseTransportConfig transportConfig, TestConfig testConfig)
            throws SyncException {

        mTestConfig = testConfig;

        setupSyncProxyBaseComponents(callbackToUIThread);
        // Set variables for Advanced Lifecycle Management
        setAdvancedLifecycleManagementEnabled(enableAdvancedLifecycleManagement);
        updateRegisterAppInterfaceParameters(appName, ttsName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, hmiDisplayLanguageDesired, appHMIType, appID, autoActivateID);
        setTransportConfig(transportConfig);
        checkConditionsInvalidateProxy(listener);
        setProxyListener(listener);
        // Get information from syncProxyConfigurationResources
        setupTelephoneManager(syncProxyConfigurationResources);
        setupMessageDispatchers();
        tryInitialiseProxy();
        // Trace that ctor has fired
        SyncTrace.logProxyEvent("SyncProxy Created, instanceID=" + this.toString(), SYNC_LIB_TRACE_KEY);
    }

    /**
     * Constructor.
     *
     * @param listener                          Type of listener for this proxy base.
     * @param syncProxyConfigurationResources   Configuration resources for this proxy.
     * @param enableAdvancedLifecycleManagement Flag that ALM should be enabled or not.
     * @param appName                           Client application name.
     * @param ttsName                           TTS name.
     * @param ngnMediaScreenAppName             Media Screen Application name.
     * @param vrSynonyms                        List of synonyms.
     * @param isMediaApp                        Flag that indicates that client application if media application or not.
     * @param syncMsgVersion                    Version of Sync Message.
     * @param languageDesired                   Desired language.
     * @param hmiDisplayLanguageDesired         Desired language for HMI.
     * @param appHMIType                        Type of application.
     * @param appID                             Application identifier.
     * @param autoActivateID                    Auto activation identifier.
     * @param callbackToUIThread                Flag that indicates that this proxy should send callback to UI thread or not.
     * @param preRegister                       Flag that indicates that this proxy should be pre-registerd or not.
     * @param version                           Version of Sync protocol to be used by the underlying connection.
     * @param transportConfig                   Configuration of transport to be used by underlying connection.
     * @throws SyncException
     */
    protected SyncProxyBase(proxyListenerType listener, SyncProxyConfigurationResources syncProxyConfigurationResources,
                            boolean enableAdvancedLifecycleManagement, String appName, Vector<TTSChunk> ttsName,
                            String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion,
                            Language languageDesired, Language hmiDisplayLanguageDesired, Vector<AppHMIType> appHMIType, String appID,
                            String autoActivateID, boolean callbackToUIThread, boolean preRegister, int version,
                            BaseTransportConfig transportConfig, SyncConnection connection, TestConfig testConfig)
            throws SyncException {

        mTestConfig = testConfig;

        setWiProVersion((byte) version);
        setAppInterfacePreRegisterd(preRegister);

        setupSyncProxyBaseComponents(callbackToUIThread);

        // Set variables for Advanced Lifecycle Management
        setAdvancedLifecycleManagementEnabled(enableAdvancedLifecycleManagement);
        updateRegisterAppInterfaceParameters(appName, ttsName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, hmiDisplayLanguageDesired, appHMIType, appID, autoActivateID);
        setTransportConfig(transportConfig);

        // Test conditions to invalidate the proxy
        checkConditionsInvalidateProxy(listener);

        setProxyListener(listener);
        setSyncConnection(connection);

        setupTelephoneManager(syncProxyConfigurationResources);


        setupMessageDispatchers();
        tryInitialiseProxy();


        // Trace that ctor has fired
        SyncTrace.logProxyEvent("SyncProxy Created, instanceID=" + this.toString(), SYNC_LIB_TRACE_KEY);
    }

    private void updateRegisterAppInterfaceParameters(String appName, Vector<TTSChunk> ttsName, String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion, Language languageDesired, Language hmiDisplayLanguageDesired, Vector<AppHMIType> appHMIType, String appID, String autoActivateID) {
        _applicationName = appName;
        _ttsName = ttsName;
        _ngnMediaScreenAppName = ngnMediaScreenAppName;
        _isMediaApp = isMediaApp;
        _syncMsgVersionRequest = syncMsgVersion;
        _vrSynonyms = vrSynonyms;
        _syncLanguageDesired = languageDesired;
        _hmiDisplayLanguageDesired = hmiDisplayLanguageDesired;
        _appHMIType = appHMIType;
        _appID = appID;
        _autoActivateIdDesired = autoActivateID;
    }

    private void setupMessageDispatchers() {
        setupInternalProxyMessageDispatcher();
        setupIncomingProxyMessageDispatcher();
        setupOutgoingMessageDispatcher();
    }

    private void setTransportConfig(BaseTransportConfig transportConfig) {
        _transportConfig = transportConfig;
    }

    private void setAdvancedLifecycleManagementEnabled(boolean enableAdvancedLifecycleManagement) {
        _advancedLifecycleManagementEnabled = enableAdvancedLifecycleManagement;
    }

    private void setProxyListener(proxyListenerType listener) {
        _proxyListener = listener;
    }

    protected void setAppInterfacePreRegisterd(boolean preRegister) {
        if (preRegister) _appInterfaceRegisterd = preRegister;
    }

    protected void setAppInterfaceRegisterd(boolean registerd) {
        _appInterfaceRegisterd = registerd;
    }

    private void setupSyncProxyBaseComponents(boolean callbackToUIThread) {
        _interfaceBroker = new SyncInterfaceBroker();

        _callbackToUIThread = callbackToUIThread;

        if (_callbackToUIThread) {
            _mainUIHandler = new Handler(Looper.getMainLooper());
        }

        rpcMessageHandler = new RPCMessageHandler(this);

    }

    private void checkConditionsInvalidateProxy(proxyListenerType listener) {
        // Test conditions to invalidate the proxy
        if (listener == null) {
            throw new IllegalArgumentException("IProxyListener listener must be provided to instantiate SyncProxy object.");
        }
        if (_advancedLifecycleManagementEnabled) {
            if (_applicationName == null) {
                throw new IllegalArgumentException("To use SyncProxyALM, an application name, appName, must be provided");
            }
            if (_applicationName.length() < 1 || _applicationName.length() > 100) {
                throw new IllegalArgumentException("A provided application name, appName, must be between 1 and 100 characters in length.");
            }
            if (_isMediaApp == null) {
                throw new IllegalArgumentException("isMediaApp must not be null when using SyncProxyALM.");
            }
        }
    }

    private void setupOutgoingMessageDispatcher() {
        // Setup Outgoing ProxyMessage Dispatcher
        synchronized (OUTGOING_MESSAGE_QUEUE_THREAD_LOCK) {
            // Ensure outgoingProxyMessageDispatcher is null
            if (_outgoingProxyMessageDispatcher != null) {
                _outgoingProxyMessageDispatcher.dispose();
                _outgoingProxyMessageDispatcher = null;
            }

            _outgoingProxyMessageDispatcher = new ProxyMessageDispatcher<ProtocolMessage>("OUTGOING_MESSAGE_DISPATCHER",
                    new OutgoingProtocolMessageComparitor(),
                    new IDispatchingStrategy<ProtocolMessage>() {
                        @Override
                        public void dispatch(ProtocolMessage message) {
                            dispatchOutgoingMessage(message);
                        }

                        @Override
                        public void handleDispatchingError(String info, Exception ex) {
                            handleErrorsFromOutgoingMessageDispatcher(info, ex);
                        }

                        @Override
                        public void handleQueueingError(String info, Exception ex) {
                            handleErrorsFromOutgoingMessageDispatcher(info, ex);
                        }
                    });
        }
    }

    private void setupInternalProxyMessageDispatcher() {
        // Setup Internal ProxyMessage Dispatcher
        synchronized (INTERNAL_MESSAGE_QUEUE_THREAD_LOCK) {
            // Ensure internalProxyMessageDispatcher is null
            if (_internalProxyMessageDispatcher != null) {
                _internalProxyMessageDispatcher.dispose();
                _internalProxyMessageDispatcher = null;
            }

            _internalProxyMessageDispatcher = new ProxyMessageDispatcher<InternalProxyMessage>("INTERNAL_MESSAGE_DISPATCHER",
                    new InternalProxyMessageComparitor(),
                    new IDispatchingStrategy<InternalProxyMessage>() {

                        @Override
                        public void dispatch(InternalProxyMessage message) {
                            dispatchInternalMessage(message);
                        }

                        @Override
                        public void handleDispatchingError(String info, Exception ex) {
                            handleErrorsFromInternalMessageDispatcher(info, ex);
                        }

                        @Override
                        public void handleQueueingError(String info, Exception ex) {
                            handleErrorsFromInternalMessageDispatcher(info, ex);
                        }
                    });
        }
    }

    private void setupTelephoneManager(SyncProxyConfigurationResources syncProxyConfigurationResources) {
        // Get information from syncProxyConfigurationResources
        TelephonyManager telephonyManager = null;
        if (syncProxyConfigurationResources != null) {
            telephonyManager = syncProxyConfigurationResources.getTelephonyManager();
        }

        // Use the telephonyManager to get and log phone info
        if (telephonyManager != null) {
            // Following is not quite thread-safe (because m_traceLogger could notifyOnAppInterfaceUnregistered null twice),
            // so we need to fix this, but vulnerability (i.e. two instances of listener) is
            // likely harmless.
            if (_traceDeviceInterrogator == null) {
                _traceDeviceInterrogator = new TraceDeviceInfo(syncProxyConfigurationResources.getTelephonyManager());
            } // end-if
        } // end-if
    }

    private void tryInitialiseProxy() throws SyncException {
        // Initialize the proxy
        try {
            initializeProxy();
        } catch (SyncException e) {
            // Couldn't initialize the proxy
            // Dispose threads and then rethrow exception

            if (_internalProxyMessageDispatcher != null) {
                _internalProxyMessageDispatcher.dispose();
                _internalProxyMessageDispatcher = null;
            }
            if (_incomingProxyMessageDispatcher != null) {
                _incomingProxyMessageDispatcher.dispose();
                _incomingProxyMessageDispatcher = null;
            }
            if (_outgoingProxyMessageDispatcher != null) {
                _outgoingProxyMessageDispatcher.dispose();
                _outgoingProxyMessageDispatcher = null;
            }
            throw e;
        }
    }

    private void setupIncomingProxyMessageDispatcher() {
        // Setup Incoming ProxyMessage Dispatcher
        synchronized (INCOMING_MESSAGE_QUEUE_THREAD_LOCK) {
            // Ensure incomingProxyMessageDispatcher is null
            if (_incomingProxyMessageDispatcher != null) {
                _incomingProxyMessageDispatcher.dispose();
                _incomingProxyMessageDispatcher = null;
            }

            _incomingProxyMessageDispatcher = new ProxyMessageDispatcher<ProtocolMessage>("INCOMING_MESSAGE_DISPATCHER",
                    new IncomingProtocolMessageComparitor(),
                    new IDispatchingStrategy<ProtocolMessage>() {
                        @Override
                        public void dispatch(ProtocolMessage message) {
                            dispatchIncomingMessage((ProtocolMessage) message);
                        }

                        @Override
                        public void handleDispatchingError(String info, Exception ex) {
                            handleErrorsFromIncomingMessageDispatcher(info, ex);
                        }

                        @Override
                        public void handleQueueingError(String info, Exception ex) {
                            handleErrorsFromIncomingMessageDispatcher(info, ex);
                        }
                    });
        }
    }

    // Public method to enable the siphon transport
    public static void enableSiphonDebug() {
        SiphonServer.enableSiphonServer();
    }

    // Public method to disable the Siphon Trace Server
    public static void disableSiphonDebug() {
        SiphonServer.disableSiphonServer();
    }

    // Public method to enable the Debug Tool
    public static void enableDebugTool() {
        DebugTool.enableDebugTool();
    }

    // Public method to disable the Debug Tool
    public static void disableDebugTool() {
        DebugTool.disableDebugTool();
    }

    public static void setHeartBeatInterval(int heartBeatInterval) {
        SyncProxyBase.heartBeatInterval = heartBeatInterval;
    }

    public static int getHeartBeatInterval() {
        return heartBeatInterval;
    }

    public SyncConnection getSyncConnection() {
        return mSyncConnection;
    }

    public ProxyMessageDispatcher<ProtocolMessage> getIncomingProxyMessageDispatcher() {
        return _incomingProxyMessageDispatcher;
    }

    public SyncInterfaceBroker getInterfaceBroker() {
        return _interfaceBroker;
    }

    public byte getMobileNavSessionID() {
        return currentSession.getSessionId();
    }

    public void sendEncodedSyncPDataToUrl(String urlString, Vector<String> encodedSyncPData, Integer timeout) {
        try {
            final int CONNECTION_TIMEOUT = timeout * 1000; // in ms
            Log.i("sendEncodedSyncPDataToUrl", "sendEncodedSyncPDataToUrl() go! ");
            //Log.i("sendEncodedSyncPDataToUrl", "CONNECTION_TIMEOUT: " + CONNECTION_TIMEOUT);
            //Log.i("sendEncodedSyncPDataToUrl", "urlString: " + urlString);
            //Log.i("sendEncodedSyncPDataToUrl", "timeout: " + timeout);
            //Log.i("sendEncodedSyncPDataToUrl", "encodedSyncPData.firstElement(): " + encodedSyncPData.firstElement());

            // Form the JSON message to send to the cloud
            JSONArray jsonArrayOfSyncPPackets = new JSONArray(encodedSyncPData);
            JSONObject jsonObjectToSendToServer = new JSONObject();
            jsonObjectToSendToServer.put("data", jsonArrayOfSyncPPackets);
            String valid_json = jsonObjectToSendToServer.toString().replace("\\", "");
            byte[] bytesToSend = valid_json.getBytes("UTF-8");

            // Send the Bytes to the Cloud and get the Response
            HttpParams httpParams = new BasicHttpParams();

            // Set the timeout in milliseconds until a connection is established.
            // The default value is zero, that means the timeout is not used.
            HttpConnectionParams.setConnectionTimeout(httpParams, CONNECTION_TIMEOUT);

            // Set the default socket timeout (SO_TIMEOUT)
            // in milliseconds which is the timeout for waiting for data.
            HttpConnectionParams.setSoTimeout(httpParams, CONNECTION_TIMEOUT);

            HttpClient client = new DefaultHttpClient(httpParams);
            HttpPost request = new HttpPost(urlString);
            request.setHeader("Content-type", "application/json");
            request.setEntity(new ByteArrayEntity(bytesToSend));
            HttpResponse response = client.execute(request);
            Log.i("sendEncodedSyncPDataToUrl", "sent and received");

            // If response is null, then return
            if (response == null) {
                DebugTool.logError("Response from server returned null: ");
                Log.i("sendEncodedSyncPDataToUrl", "Response from server returned null: ");
                return;
            }

            Vector<String> encodedSyncPDataReceived = new Vector<String>();
            if (response.getStatusLine().getStatusCode() == 200) {

                // Convert the response to JSON
                JSONObject jsonResponse = new JSONObject(EntityUtils.toString(response.getEntity(), "UTF-8"));

                if (jsonResponse.get("data") instanceof JSONArray) {
                    JSONArray jsonArray = jsonResponse.getJSONArray("data");
                    for (int i = 0; i < jsonArray.length(); i++) {
                        if (jsonArray.get(i) instanceof String) {
                            encodedSyncPDataReceived.add(jsonArray.getString(i));
                        }
                    }
                } else if (jsonResponse.get("data") instanceof String) {
                    encodedSyncPDataReceived.add(jsonResponse.getString("data"));
                } else {
                    DebugTool.logError("sendEncodedSyncPDataToUrl: Data in JSON Object neither an array nor a string.");
                    //Log.i("sendEncodedSyncPDataToUrl", "sendEncodedSyncPDataToUrl: Data in JSON Object neither an array nor a string.");
                    return;
                }

                // Send new encodedSyncPDataRequest to SYNC
                EncodedSyncPData encodedSyncPDataRequest = RPCRequestFactory.buildEncodedSyncPData(encodedSyncPDataReceived, getPoliciesReservedCorrelationID());

                if (getIsConnected()) {
                    sendRPCRequestPrivate(encodedSyncPDataRequest);
                    Log.i("sendEncodedSyncPDataToUrl", "sent to sync");
                }
            } else if (response.getStatusLine().getStatusCode() == 500) {
                Log.i("sendEncodedSyncPDataToUrl", "Status 500");
                //returnVal = "Status 500";
            }

        } catch (SyncException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: Could not get data from JSONObject received.", e);
        } catch (JSONException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: JSONException: ", e);
        } catch (UnsupportedEncodingException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: Could not encode string.", e);
        } catch (ProtocolException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: Could not set request method to post.", e);
        } catch (MalformedURLException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: URL Exception when sending EncodedSyncPData to an external server.", e);
        } catch (IOException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: IOException: ", e);
        } catch (Exception e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: Unexpected Exception: ", e);
        }
    }

    public void sendSyncPDataToUrl(String urlString, byte[] bs, Integer timeout) {
        try {
            final int CONNECTION_TIMEOUT = timeout * 1000; // in ms
            Log.i("sendEncodedSyncPDataToUrl", "sendEncodedSyncPDataToUrl() go! ");
            //Log.i("sendEncodedSyncPDataToUrl", "CONNECTION_TIMEOUT: " + CONNECTION_TIMEOUT);
            //Log.i("sendEncodedSyncPDataToUrl", "urlString: " + urlString);
            //Log.i("sendEncodedSyncPDataToUrl", "timeout: " + timeout);
            //Log.i("sendEncodedSyncPDataToUrl", "encodedSyncPData.firstElement(): " + encodedSyncPData.firstElement());

            //base64 encode the binary syncp packet before sending to cloud
            String base64SyncP = Base64.encodeBytes(bs);
            //Log.i("text", "base64 encoded syncP: " + base64SyncP);

            // Form the JSON message to send to the cloud
            JSONArray jsonArrayOfSyncPPackets = new JSONArray(base64SyncP);
            JSONObject jsonObjectToSendToServer = new JSONObject();
            jsonObjectToSendToServer.put("data", jsonArrayOfSyncPPackets);
            String valid_json = jsonObjectToSendToServer.toString().replace("\\", "");
            byte[] bytesToSend = valid_json.getBytes("UTF-8");

            // Send the Bytes to the Cloud and get the Response
            HttpParams httpParams = new BasicHttpParams();

            // Set the timeout in milliseconds until a connection is established.
            // The default value is zero, that means the timeout is not used.
            HttpConnectionParams.setConnectionTimeout(httpParams, CONNECTION_TIMEOUT);

            // Set the default socket timeout (SO_TIMEOUT)
            // in milliseconds which is the timeout for waiting for data.
            HttpConnectionParams.setSoTimeout(httpParams, CONNECTION_TIMEOUT);
            HttpClient client = new DefaultHttpClient(httpParams);
            HttpPost request = new HttpPost(urlString);
            request.setHeader("Content-type", "application/json");
            request.setEntity(new ByteArrayEntity(bytesToSend));
            HttpResponse response = client.execute(request);

            Log.i("sendEncodedSyncPDataToUrl", "sent and received");

            // If response is null, then return
            if (response == null) {
                DebugTool.logError("Response from server returned null: ");
                Log.i("sendEncodedSyncPDataToUrl", "Response from server returned null: ");
                return;
            }

            Vector<String> encodedSyncPDataReceived = new Vector<String>();
            if (response.getStatusLine().getStatusCode() == 200) {
                // Convert the response to JSON
                JSONObject jsonResponse = new JSONObject(EntityUtils.toString(response.getEntity(), "UTF-8"));

                if (jsonResponse.get("data") instanceof JSONArray) {
                    JSONArray jsonArray = jsonResponse.getJSONArray("data");
                    for (int i = 0; i < jsonArray.length(); i++) {
                        if (jsonArray.get(i) instanceof String) {
                            encodedSyncPDataReceived.add(jsonArray.getString(i));
                            //Log.i("sendEncodedSyncPDataToUrl", "jsonArray.getString(i): " + jsonArray.getString(i));
                        }
                    }
                } else if (jsonResponse.get("data") instanceof String) {
                    encodedSyncPDataReceived.add(jsonResponse.getString("data"));
                    //Log.i("sendEncodedSyncPDataToUrl", "jsonResponse.getString(data): " + jsonResponse.getString("data"));
                } else {
                    DebugTool.logError("sendEncodedSyncPDataToUrl: Data in JSON Object neither an array nor a string.");
                    //Log.i("sendEncodedSyncPDataToUrl", "sendEncodedSyncPDataToUrl: Data in JSON Object neither an array nor a string.");
                    return;
                }

                //convert encodedsyncp packet to binary
                byte[] syncppacket = encodedSyncPDataReceived.firstElement().getBytes();

                // Send new binary syncp data to SYNC
                SyncPData syncPDataRequest = RPCRequestFactory.buildSyncPData(syncppacket, getPoliciesReservedCorrelationID());

                if (getIsConnected()) {
                    sendRPCRequestPrivate(syncPDataRequest);
                    Log.i("sendEncodedSyncPDataToUrl", "sent to sync");
                }
            } else if (response.getStatusLine().getStatusCode() == 500) {
                Log.i("sendEncodedSyncPDataToUrl", "Status 500");
                //returnVal = "Status 500";
            }

        } catch (SyncException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: Could not get data from JSONObject received.", e);
        } catch (JSONException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: JSONException: ", e);
        } catch (UnsupportedEncodingException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: Could not encode string.", e);
        } catch (ProtocolException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: Could not set request method to post.", e);
        } catch (MalformedURLException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: URL Exception when sending EncodedSyncPData to an external server.", e);
        } catch (IOException e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: IOException: ", e);
        } catch (Exception e) {
            DebugTool.logError("sendEncodedSyncPDataToUrl: Unexpected Exception: ", e);
        }
    }

    private int getPoliciesReservedCorrelationID() {
        return POLICIES_CORRELATION_ID;
    }

    // Test correlationID
    protected boolean isCorrelationIDProtected(Integer correlationID) {
        if (correlationID != null &&
                (HEARTBEAT_CORRELATION_ID == correlationID
                        || REGISTER_APP_INTERFACE_CORRELATION_ID == correlationID
                        || UNREGISTER_APP_INTERFACE_CORRELATION_ID == correlationID
                        || POLICIES_CORRELATION_ID == correlationID)) {
            return true;
        }

        return false;
    }

    // Protected isConnected method to allow legacy proxy to poll isConnected state
    public Boolean getIsConnected() {
        if (mSyncConnection == null) {
            return false;
        }
        return mSyncConnection.getIsConnected();
    }

    /**
     * Returns whether the application is registered in SYNC. Note: for testing
     * purposes, it's possible that the connection is established, but the
     * application is not registered.
     *
     * @return true if the application is registered in SYNC
     */
    public Boolean getAppInterfaceRegistered() {
        return _appInterfaceRegisterd;
    }

    private void initState() throws SyncException {
        // Reset all of the flags and state variables
        _haveReceivedFirstNonNoneHMILevel = false;
        _haveReceivedFirstFocusLevel = false;
        _haveReceivedFirstFocusLevelFull = false;
        _syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;
    }

    // Function to initialize new proxy connection
    public void initializeProxy() throws SyncException {
        initState();

        internalRequestCorrelationIDs = new HashSet<Integer>();

        // Setup SyncConnection
        synchronized (CONNECTION_REFERENCE_LOCK) {
            if (mSyncConnection == null) {
                mSyncConnection = new SyncConnection(_interfaceBroker);
                final HeartbeatMonitor heartbeatMonitor =
                        new HeartbeatMonitor();
                heartbeatMonitor.setInterval(heartBeatInterval);
                mSyncConnection.setHeartbeatMonitor(heartbeatMonitor);

                currentSession.setSessionId((byte) 0);
                mSyncConnection.setSessionId(currentSession.getSessionId());

                //mSyncConnection.setSessionId(currentSession.getSessionId());

                mSyncConnection.init(_transportConfig);
            }

            WiProProtocol protocol = (WiProProtocol) mSyncConnection.getWiProProtocol();
            protocol.setVersion(_wiproVersion);

            mSyncConnection.startTransport();
        }
    }

    private synchronized void closeSyncConnection(boolean keepConnection) {
        if (mSyncConnection != null) {
            mSyncConnection.closeConnection(currentSession.getSessionId(), keepConnection);
            mSyncConnection.setSessionId((byte) 0);
            if (!keepConnection) {
                setSyncConnection(null);
            }
        }
    }

    private void stopSession() {
        currentSession.stopSession();
    }

    @Deprecated
    public void close() throws SyncException {
        dispose();
    }

    private void cleanProxy(SyncDisconnectedReason disconnectedReason,
                            boolean keepConnection, boolean keepSession) throws SyncException {
        try {

            // ALM Specific Cleanup
            if (_advancedLifecycleManagementEnabled) {
                _syncConnectionState = SyncConnectionState.SYNC_DISCONNECTED;

                firstTimeFull = true;
                exitSession(keepConnection, keepSession);
            }
        } catch (SyncException e) {
            throw e;
        } finally {
            SyncTrace.logProxyEvent("SyncProxy cleaned.", SYNC_LIB_TRACE_KEY);
        }
    }

    private void exitSession(boolean keepConnection, boolean keepSession) throws SyncException {
        // Should we wait for the interface to be unregistered?
        Boolean waitForInterfaceUnregistered = false;
        synchronized (CONNECTION_REFERENCE_LOCK) {
            if (_appInterfaceRegisterd == true && mSyncConnection != null && mSyncConnection.getIsConnected()) {
                waitForInterfaceUnregistered = true;
                unregisterAppInterfacePrivate(UNREGISTER_APP_INTERFACE_CORRELATION_ID);
            }
            // Wait for the app interface to be unregistered
            if (waitForInterfaceUnregistered) {
                synchronized (APP_INTERFACE_REGISTERED_LOCK) {
                    try {
                        APP_INTERFACE_REGISTERED_LOCK.wait(1000);
                    } catch (InterruptedException e) {
                        // Do nothing
                    }
                }
            }

            // Clean up SYNC Connection

            if (!keepSession) {
                stopAllServices();
            }
            closeSyncConnection(keepConnection);
            if (!keepSession) {
                stopSession();
            }
        }
    }

    private void stopAllServices() {
        if (getServicePool().size() > 0) {
            stopMobileNaviService();
            stopAudioService();
        }
    }

    /**
     * Terminates the App's Interface Registration, closes the transport connection, ends the protocol currentSession, and frees any resources used by the proxy.
     */
    public void dispose() throws SyncException {
        if (_proxyDisposed) {
            throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
        }

        _proxyDisposed = true;

        SyncTrace.logProxyEvent("Application called dispose() method.", SYNC_LIB_TRACE_KEY);

        try {
            // Clean the proxy
            cleanProxy(SyncDisconnectedReason.APPLICATION_REQUESTED_DISCONNECT, false, false);

            clearReconnectTimer();

            // Close IncomingProxyMessageDispatcher thread
            synchronized (INCOMING_MESSAGE_QUEUE_THREAD_LOCK) {
                if (_incomingProxyMessageDispatcher != null) {
                    _incomingProxyMessageDispatcher.dispose();
                    _incomingProxyMessageDispatcher = null;
                }
            }

            // Close OutgoingProxyMessageDispatcher thread
            synchronized (OUTGOING_MESSAGE_QUEUE_THREAD_LOCK) {
                if (_outgoingProxyMessageDispatcher != null) {
                    _outgoingProxyMessageDispatcher.dispose();
                    _outgoingProxyMessageDispatcher = null;
                }
            }

            // Close InternalProxyMessageDispatcher thread
            synchronized (INTERNAL_MESSAGE_QUEUE_THREAD_LOCK) {
                if (_internalProxyMessageDispatcher != null) {
                    _internalProxyMessageDispatcher.dispose();
                    _internalProxyMessageDispatcher = null;
                }
            }

            _traceDeviceInterrogator = null;
        } catch (SyncException e) {
            throw e;
        } finally {
            SyncTrace.logProxyEvent("SyncProxy disposed.", SYNC_LIB_TRACE_KEY);
        }
    } // end-method

    // Method to cycle the proxy, only called in ALM
    protected void cycleProxy(SyncDisconnectedReason disconnectedReason) {
        Log.d(TAG, "CycleProxy, disconnectedReason:" + disconnectedReason);
        try {
            cleanProxy(disconnectedReason, false, true);
            scheduleInitializeProxy();
            notifyProxyClosed("Sync Proxy Cycled", new SyncException("Sync Proxy Cycled", SyncExceptionCause.SYNC_PROXY_CYCLED));
        } catch (SyncException e) {
            handleCyclingSyncException(e);
        } catch (Exception e) {
            notifyProxyClosed("Cycling the proxy failed.", e);
        }
    }

    /**
     * Unregisters the application from SYNC, and closes the currentSession.
     * Optionally, closes the transport connection.
     */
    public void closeSession(boolean keepConnection) {
        Log.d(TAG, "Close Session, keepConnection:" + keepConnection);
        try {
            cleanProxy(
                    SyncDisconnectedReason.APPLICATION_REQUESTED_DISCONNECT, keepConnection, false);
            notifyProxyClosed("Sync Proxy Cycled",
                    new SyncException("Sync Proxy Cycled", SyncExceptionCause.SYNC_PROXY_CYCLED));
        } catch (SyncException e) {
            handleCyclingSyncException(e);
        } catch (Exception e) {
            notifyProxyClosed("Cycling the proxy failed.", e);
        }
    }

    private void handleCyclingSyncException(SyncException e) {
        switch (e.getSyncExceptionCause()) {
            case BLUETOOTH_DISABLED:
                notifyProxyClosed("Bluetooth is disabled. Bluetooth must be enabled to connect to SYNC. Reattempt a connection once Bluetooth is enabled.",
                        new SyncException("Bluetooth is disabled. Bluetooth must be enabled to connect to SYNC. Reattempt a connection once Bluetooth is enabled.", SyncExceptionCause.BLUETOOTH_DISABLED));
                break;
            case BLUETOOTH_ADAPTER_NULL:
                notifyProxyClosed("Cannot locate a Bluetooth adapater. A SYNC connection is impossible on this device until a Bluetooth adapter is added.",
                        new SyncException("Cannot locate a Bluetooth adapater. A SYNC connection is impossible on this device until a Bluetooth adapter is added.", SyncExceptionCause.HEARTBEAT_PAST_DUE));
                break;
            default:
                notifyProxyClosed("Cycling the proxy failed.", e);
                break;
        }
    }

    protected void scheduleInitializeProxy() {
        Log.d(TAG, "Scheduling proxy initialization");
        if (currentSession.isServicesEmpty()) {
            Log.d(TAG, "Service list is empty. Scheduling proxy initialization canceled");
            return;
        }

        if (getCurrentReconnectTimerTask() != null) {
            Log.d(TAG, "Current reconnect task is already scheduled, canceling it first");
            clearCurrentReconnectTimerTask();
        }

        TimerTask reconnectTask = new TimerTask() {
            @Override
            public void run() {
                try {
                    Log.d(TAG, "Reconnect task is running, clearing reference");
                    setCurrentReconnectTimerTask(null);
                    initializeProxy();
                } catch (SyncException e) {
                    Log.e(TAG, "Cycling the proxy failed with SyncException.", e);
                    handleCyclingSyncException(e);
                } catch (Exception e) {
                    notifyProxyClosed("Cycling the proxy failed with Exception.", e);
                }
            }
        };
        setCurrentReconnectTimerTask(reconnectTask);

        Timer timer = getReconnectTimer();
        timer.schedule(reconnectTask, PROXY_RECONNECT_DELAY);

    }

    /**
     * Opens a currentSession, and registers the application. The connection must be
     * already opened.
     *
     * @throws SyncException if a transport connection is not opened
     */
    public void openSession() throws SyncException {
        if (!getIsConnected()) {
            throw new SyncException("Transport connection must be opened",
                    SyncExceptionCause.SYNC_CONNECTION_FAILED);
        }

        initState();
        mSyncConnection.onTransportConnected();
    }

    /**
     * ********** Functions used by the Message Dispatching Queues ***************
     */
    protected void dispatchIncomingMessage(ProtocolMessage message) {
        try {

            if (message.getSessionID() != currentSession.getSessionId()) {
                DebugTool.logWarning("Message is not from current session");
                Log.w(TAG, "Message is not from current session");
                return;
            }

            // Dispatching logic
            if (message.getServiceType().equals(ServiceType.RPC)) {
                try {
                    if (_wiproVersion == 1) {
                        if (message.getVersion() == 2) setWiProVersion(message.getVersion());
                    }

                    Hashtable hash = new Hashtable();
                    if (_wiproVersion == 2) {
                        Hashtable hashTemp = new Hashtable();
                        hashTemp.put(Names.correlationID, message.getCorrID());

                        if (message.getJsonSize() > 0) {
                            final Hashtable<String, Object> mhash = _jsonRPCMarshaller.unmarshall(message.getData());
                            if (mhash != null) {
                                hashTemp.put(Names.parameters, mhash);
                            } else {
                                String err = "Can't parse JSON: " + new String(message.getData());
                                DebugTool.logError(err);
                                Log.e(TAG, err);
                            }
                        }
                        FunctionID functionID = new FunctionID();
                        hashTemp.put(Names.function_name, functionID.getFunctionName(message.getFunctionID()));
                        if (message.getRPCType() == ProtocolMessage.RPCTYPE_REQUEST) {
                            hash.put(Names.request, hashTemp);
                        } else if (message.getRPCType() == ProtocolMessage.RPCTYPE_RESPONSE) {
                            hash.put(Names.response, hashTemp);
                        } else if (message.getRPCType() == ProtocolMessage.RPCTYPE_NOTIFICATION) {
                            hash.put(Names.notification, hashTemp);
                        }
                        if (message.getBulkData() != null)
                            hash.put(Names.bulkData, message.getBulkData());
                    } else {
                        final Hashtable<String, Object> mhash = _jsonRPCMarshaller.unmarshall(message.getData());
                        hash = mhash;
                    }
                    handleRPCMessage(hash);
                } catch (final Exception excp) {
                    DebugTool.logError("Failure handling protocol message: " + excp.toString(), excp);
                    passErrorToProxyListener("Error handing incoming protocol message.", excp);
                } // end-catch
            } else {
                // Handle other protocol message types here
            }
        } catch (final Exception e) {
            // Pass error to application through listener
            DebugTool.logError("Error handing proxy event.", e);
            passErrorToProxyListener("Error handing incoming protocol message.", e);
        }
    }

    protected void handleMobileNavMessage(ProtocolMessage message) {
        Log.i(TAG, "Mobile Nav Session message received" + message.toString());
        // TODO handle incoming mobile nav sessions
    }

    public byte getWiProVersion() {
        return this._wiproVersion;
    }

    private void setWiProVersion(byte version) {
        Log.i(TAG, "Setting WiPro version from " + (int) this._wiproVersion + " to " + (int) version);
        //Log.i(TAG, "setter called from: " + Log.getStackTraceString(new Exception()));
        this._wiproVersion = version;
    }

    private void handleErrorsFromIncomingMessageDispatcher(String info, Exception e) {
        passErrorToProxyListener(info, e);
    }

    private void dispatchOutgoingMessage(ProtocolMessage message) {
        if (mSyncConnection.getIsConnected()) {
            mSyncConnection.sendMessage(message);
        }
        /*synchronized (CONNECTION_REFERENCE_LOCK) {
            if (mSyncConnection != null) {

            }
        }*/
        SyncTrace.logProxyEvent("SyncProxy sending Protocol Message: " + message.toString(), SYNC_LIB_TRACE_KEY);
    }

    private void handleErrorsFromOutgoingMessageDispatcher(String info, Exception e) {
        passErrorToProxyListener(info, e);
    }

    void dispatchInternalMessage(final InternalProxyMessage message) {
        try {
            if (message.getFunctionName().equals(Names.OnProxyError)) {
                final OnError msg = (OnError) message;
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onError(msg.getInfo(), msg.getException());
                        }
                    });
                } else {
                    _proxyListener.onError(msg.getInfo(), msg.getException());
                }
                /**************Start Legacy Specific Call-backs************/
            } else if (message.getFunctionName().equals(Names.OnProxyOpened)) {
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            ((IProxyListener) _proxyListener).onProxyOpened();
                        }
                    });
                } else {
                    ((IProxyListener) _proxyListener).onProxyOpened();
                }
            } else if (message.getFunctionName().equals(Names.OnProxyClosed)) {
                final OnProxyClosed msg = (OnProxyClosed) message;
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onProxyClosed(msg.getInfo(), msg.getException());
                        }
                    });
                } else {
                    _proxyListener.onProxyClosed(msg.getInfo(), msg.getException());
                }
                /****************End Legacy Specific Call-backs************/
            } else {
                // Diagnostics
                SyncTrace.logProxyEvent("Unknown RPC Message encountered. Check for an updated version of the SYNC Proxy.", SYNC_LIB_TRACE_KEY);
                DebugTool.logError("Unknown RPC Message encountered. Check for an updated version of the SYNC Proxy.");
            }

            SyncTrace.logProxyEvent("Proxy fired callback: " + message.getFunctionName(), SYNC_LIB_TRACE_KEY);
        } catch (final Exception e) {
            // Pass error to application through listener
            DebugTool.logError("Error handing proxy event.", e);
            if (_callbackToUIThread) {
                // Run in UI thread
                _mainUIHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        _proxyListener.onError("Error handing proxy event.", e);
                    }
                });
            } else {
                _proxyListener.onError("Error handing proxy event.", e);
            }
        }
    }

    private void handleErrorsFromInternalMessageDispatcher(String info, Exception e) {
        DebugTool.logError(info, e);
        // This error cannot be passed to the user, as it indicates an error
        // in the communication between the proxy and the application.

        DebugTool.logError("InternalMessageDispatcher failed.", e);

        // Note, this is the only place where the _proxyListener should be referenced asynchronously,
        // with an error on the internalMessageDispatcher, we have no other reliable way of
        // communicating with the application.
        notifyProxyClosed("Proxy callback dispatcher is down. Proxy instance is invalid.", e);
        _proxyListener.onError(
                "Proxy callback dispatcher is down. Proxy instance is invalid.",
                e);
    }

    /**
     * ********** END Functions used by the Message Dispatching Queues ***************
     */

    // Private sendPRCRequest method. All RPCRequests are funneled through this method after
    // error checking.
    // FIXME: return to private?
    void sendRPCRequestPrivate(RPCRequest request) throws SyncException {
        try {
            SyncTrace.logRPCEvent(InterfaceActivityDirection.Transmit, request, SYNC_LIB_TRACE_KEY);

            final IRPCRequestConverter converter =
                    rpcRequestConverterFactory.getConverterForRequest(request);
            List<ProtocolMessage> protocolMessages =
                    converter.getProtocolMessages(request,
                            currentSession.getSessionId(), _jsonRPCMarshaller,
                            _wiproVersion);

            if (protocolMessages.size() > 0) {
                queueOutgoingMessage(protocolMessages.get(0));
                protocolMessages.remove(0);

                if (protocolMessages.size() > 0) {
                    protocolMessageHolder.saveMessages(protocolMessages);
                }
            }
        } catch (OutOfMemoryError e) {
            SyncTrace.logProxyEvent("OutOfMemory exception while sending request " + request.getFunctionName(), SYNC_LIB_TRACE_KEY);
            throw new SyncException("OutOfMemory exception while sending request " + request.getFunctionName(), e, SyncExceptionCause.INVALID_ARGUMENT);
        }
    }

    private void queueOutgoingMessage(ProtocolMessage message) {
        synchronized (OUTGOING_MESSAGE_QUEUE_THREAD_LOCK) {
            if (_outgoingProxyMessageDispatcher != null) {
                _outgoingProxyMessageDispatcher.queueMessage(message);
            }
        }
    }

    /**
     * Handles a response that is a part of partial request (i.e., split into
     * multiple protocol messages) if it is.
     *
     * @param response response from the SDL
     * @return true if the response has been handled; false when the
     * corresponding request is not partial or in case of an error
     */
    protected boolean handlePartialRPCResponse(RPCResponse response) {
        boolean success = false;
        final Integer responseCorrelationID = response.getCorrelationID();
        if (protocolMessageHolder.hasMessages(responseCorrelationID)) {
            if (Result.SUCCESS == response.getResultCode()) {
                final ProtocolMessage pm =
                        protocolMessageHolder.peekNextMessage(
                                responseCorrelationID);
                if (pm.getFunctionID() ==
                        FunctionID.getFunctionID(response.getFunctionName())) {
                    protocolMessageHolder.popNextMessage(responseCorrelationID);
                    queueOutgoingMessage(pm);

                    success = true;
                }
            } else {
                protocolMessageHolder.clearMessages(responseCorrelationID);
            }
        }

        return success;
    }

    /**
     * Handles an internal response, if it is, that is last in sequence. Such a
     * response shouldn't be exposed to the user. For example, a PutFile
     * responded for OnSystemRequest.
     *
     * @param response response from the SDL
     * @return true if the response has been handled; false when the
     * corresponding request is not internal or in case of an error
     */
    protected boolean handleLastInternalResponse(RPCResponse response) {
        final Integer correlationID = response.getCorrelationID();
        final boolean contains = internalRequestCorrelationIDs.contains(
                correlationID);
        if (contains) {
            internalRequestCorrelationIDs.remove(correlationID);
        }

        return contains;
    }

    private void handleRPCMessage(Hashtable hash){
        getRPCMessageHandler().handleRPCMessage(hash);
    }

    protected void handleOnSystemRequest(Hashtable hash) {
        final OnSystemRequest msg = new OnSystemRequest(hash);
        if (RequestType.HTTP == msg.getRequestType()) {
            if (msg.getFileType() == FileType.JSON) {
                Runnable request = new Runnable() {
                    @Override
                    public void run() {
                        onSystemRequestHandler.onPolicyTableSnapshotRequest(SyncProxyBase.this,
                                msg.getBulkData());
                    }
                };
                if (_callbackToUIThread) {
                    _mainUIHandler.post(request);
                } else {
                    request.run();
                }
            } else {
                final Vector<String> urls = msg.getUrl();
                if (urls != null) {
                    Runnable request = new Runnable() {
                        @Override
                        public void run() {
                            onSystemRequestHandler.onFilesDownloadRequest(
                                    SyncProxyBase.this, urls, msg.getFileType());
                        }
                    };
                    if (_callbackToUIThread) {
                        _mainUIHandler.post(request);
                    } else {
                        request.run();
                    }
                } else {
                    Log.w(TAG, "OnSystemRequest HTTP: no urls set");
                }
            }
        } else if (RequestType.FILE_RESUME == msg.getRequestType()) {
            final Vector<String> urls = msg.getUrl();
            final Integer offset = msg.getOffset();
            final Integer length = msg.getLength();
            final boolean allRequiredParamsSet =
                    (urls != null) && (offset != null) && (length != null);
            if (allRequiredParamsSet) {
                Runnable request = new Runnable() {
                    @Override
                    public void run() {
                        onSystemRequestHandler.onFileResumeRequest(
                                SyncProxyBase.this, urls.get(0), offset, length,
                                msg.getFileType());
                    }
                };

                if (_callbackToUIThread) {
                    _mainUIHandler.post(request);
                } else {
                    request.run();
                }
            } else {
                Log.w(TAG,
                        "OnSystemRequest FILE_RESUME: a required parameter is missing");
            }
        } else {
            if (_callbackToUIThread) {
                // Run in UI thread
                _mainUIHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        _proxyListener.onOnSystemRequest(msg);
                    }
                });
            } else {
                _proxyListener.onOnSystemRequest(msg);
            }
        }
    }

    protected void notifyOnAppInterfaceUnregistered(final OnAppInterfaceUnregistered msg) {
        notifyProxyClosed("OnAppInterfaceUnregistered", null);

        if (_callbackToUIThread) {
            // Run in UI thread
            _mainUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    ((IProxyListener) _proxyListener).onOnAppInterfaceUnregistered(msg);
                }
            });
        } else {
            ((IProxyListener) _proxyListener).onOnAppInterfaceUnregistered(msg);
        }
    }

    protected void onAppUnregisteredReason(final AppInterfaceUnregisteredReason reason) {
        if (reason == AppInterfaceUnregisteredReason.IGNITION_OFF ||
                reason == AppInterfaceUnregisteredReason.MASTER_RESET) {
            cycleProxy(SyncDisconnectedReason.convertAppInterfaceUnregisteredReason(reason));
        }

        if (getCallbackToUIThread()) {
            // Run in UI thread
            getMainUIHandler().post(new Runnable() {
                @Override
                public void run() {
                    getProxyListener().onAppUnregisteredReason(reason);
                }
            });
        } else {
            getProxyListener().onAppUnregisteredReason(reason);
        }
    }

    protected void onUnregisterAppInterfaceResponse(Hashtable hash) {
        stopAllServices();
        closeSyncConnection(true);
        stopSession();

        // UnregisterAppInterface
        _appInterfaceRegisterd = false;
        synchronized (APP_INTERFACE_REGISTERED_LOCK) {
            APP_INTERFACE_REGISTERED_LOCK.notify();
        }
        final UnregisterAppInterfaceResponse msg = new UnregisterAppInterfaceResponse(hash);
        if (_callbackToUIThread) {
            // Run in UI thread
            _mainUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (_proxyListener instanceof IProxyListener) {
                        ((IProxyListener) _proxyListener).onUnregisterAppInterfaceResponse(msg);
                    } else if (_proxyListener instanceof IProxyListenerALMTesting) {
                        ((IProxyListenerALMTesting) _proxyListener).onUnregisterAppInterfaceResponse(msg);
                    }
                }
            });
        } else {
            if (_proxyListener instanceof IProxyListener) {
                ((IProxyListener) _proxyListener).onUnregisterAppInterfaceResponse(msg);
            } else if (_proxyListener instanceof IProxyListenerALMTesting) {
                ((IProxyListenerALMTesting) _proxyListener).onUnregisterAppInterfaceResponse(msg);
            }
        }

        notifyProxyClosed("UnregisterAppInterfaceResponse", null);
    }

    /**
     * Takes an RPCRequest and sends it to SYNC.  Responses are captured through callback on IProxyListener.
     *
     * @throws SyncException
     */
    public void sendRPCRequest(RPCRequest request) throws SyncException {
        if (_proxyDisposed) {
            throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
        }

        // Test if request is null
        if (request == null) {
            SyncTrace.logProxyEvent("Application called sendRPCRequest method with a null RPCRequest.", SYNC_LIB_TRACE_KEY);
            throw new IllegalArgumentException("sendRPCRequest cannot be called with a null request.");
        }

        SyncTrace.logProxyEvent("Application called sendRPCRequest method for RPCRequest: ." + request.getFunctionName(), SYNC_LIB_TRACE_KEY);

        checkSyncConnection();

        // Test for illegal correlation ID
        if (isCorrelationIDProtected(request.getCorrelationID())) {

            SyncTrace.logProxyEvent("Application attempted to use the reserved correlation ID, " + request.getCorrelationID(), SYNC_LIB_TRACE_KEY);
            throw new SyncException("Invalid correlation ID. The correlation ID, " + request.getCorrelationID()
                    + " , is a reserved correlation ID.", SyncExceptionCause.RESERVED_CORRELATION_ID);
        }

        // Throw exception if RPCRequest is sent when SYNC is unavailable
        if (!_appInterfaceRegisterd && !request.getFunctionName().equals(Names.RegisterAppInterface)) {
            if (!allowExtraTesting()) {
                SyncTrace.logProxyEvent("Application attempted to send an RPCRequest (non-registerAppInterface), before the interface was registerd.", SYNC_LIB_TRACE_KEY);
                throw new SyncException("SYNC is currently unavailable. RPC Requests cannot be sent.", SyncExceptionCause.SYNC_UNAVAILALBE);
            }
        }

        if (_advancedLifecycleManagementEnabled) {
            if (request.getFunctionName().equals(Names.RegisterAppInterface)
                    || request.getFunctionName().equals(Names.UnregisterAppInterface)) {
                if (!allowExtraTesting()) {
                    SyncTrace.logProxyEvent("Application attempted to send a RegisterAppInterface or UnregisterAppInterface while using ALM.", SYNC_LIB_TRACE_KEY);
                    throw new SyncException("The RPCRequest, " + request.getFunctionName() +
                            ", is unnallowed using the Advanced Lifecycle Management Model.", SyncExceptionCause.INCORRECT_LIFECYCLE_MODEL);
                }
            }
        }
        sendRPCRequestPrivate(request);
    } // end-method

    private void checkSyncConnection() throws SyncException {
        // Test if SyncConnection is null
        synchronized (CONNECTION_REFERENCE_LOCK) {
            if (mSyncConnection == null || !mSyncConnection.getIsConnected()) {
                SyncTrace.logProxyEvent("Application attempted to send and RPCRequest without a connected transport.", SYNC_LIB_TRACE_KEY);
                throw new SyncException("There is no valid connection to SYNC. sendRPCRequest cannot be called until SYNC has been connected.", SyncExceptionCause.SYNC_UNAVAILALBE);
            }
        }
    }

    /**
     * Returns whether the class allows some extra testing features. For this,
     * the _proxyListener must be an instance of the IProxyListenerALMTesting
     * class.
     *
     * @return true if the extra testing features are enabled
     */
    private boolean allowExtraTesting() {
        return _proxyListener instanceof IProxyListenerALMTesting;
    }

    public void sendRPCRequest(RPCMessage request) throws SyncException {
        sendRPCRequest(request);
    }

    protected void notifyProxyClosed(final String info, final Exception e) {
        SyncTrace.logProxyEvent("NotifyProxyClose", SYNC_LIB_TRACE_KEY);

        OnProxyClosed message = new OnProxyClosed(info, e);
        queueInternalMessage(message);
    }

    private void passErrorToProxyListener(final String info, final Exception e) {
        OnError message = new OnError(info, e);
        queueInternalMessage(message);
    }

    private void startRPCProtocolService(final byte sessionID, final String correlationID) {
        currentSession.setSessionId(sessionID);
        addIfNotExsistRpcServiceToSession();
        mSyncConnection.setSessionId(sessionID);
        Log.i(TAG, "RPC Session started, sessionId:" + sessionID + ", correlationID:" + correlationID);
        if (_callbackToUIThread) {
            // Run in UI thread
            _mainUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    _proxyListener.onSessionStarted(sessionID, correlationID);
                }
            });
        } else {
            _proxyListener.onSessionStarted(sessionID, correlationID);
        }

        restartRPCProtocolSession();
    }

    private void addIfNotExsistRpcServiceToSession() {
        if (!currentSession.hasService(ServiceType.RPC)) {
            Service service = new Service();
            service.setServiceType(ServiceType.RPC);
            service.setSession(currentSession);
            currentSession.addService(service);
        }
    }

    private void restartRPCProtocolSession() {
        // Set Proxy Lifecycle Available
        if (_advancedLifecycleManagementEnabled) {

            try {
                registerAppInterfacePrivate(
                        _syncMsgVersionRequest,
                        _applicationName,
                        _ttsName,
                        _ngnMediaScreenAppName,
                        _vrSynonyms,
                        _isMediaApp,
                        _syncLanguageDesired,
                        _hmiDisplayLanguageDesired, _appHMIType,
                        _appID,
                        _autoActivateIdDesired,
                        REGISTER_APP_INTERFACE_CORRELATION_ID,
                        getHashId());

            } catch (Exception e) {
                notifyProxyClosed("Failed to register application interface with SYNC. Check parameter values given to SyncProxy constructor.", e);
            }
        } else {
            InternalProxyMessage message = new InternalProxyMessage(Names.OnProxyOpened);
            queueInternalMessage(message);
        }
    }

    protected void handleEndServiceAck(final ServiceType serviceType, final byte sessionId, final String correlationID) {
        Log.i(TAG, "EndService Ack received; Session Type " + serviceType.getName() + "; Session ID " + sessionId + "; Correlation ID " + correlationID);
        if (_callbackToUIThread) {
            // Run in UI thread
            _mainUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    _proxyListener.onProtocolServiceEnded(serviceType, sessionId, correlationID);
                }
            });
        } else {
            _proxyListener.onProtocolServiceEnded(serviceType, sessionId, correlationID);
        }
    }

    protected void handleMobileNavAck(int frameNumberReceived) {
        Log.i(TAG, "Mobile Nav Ack received = " + frameNumberReceived);
        final int fNumber = frameNumberReceived;
        if (_callbackToUIThread) {
            // Run in UI thread
            _mainUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    _proxyListener.onMobileNavAckReceived(fNumber);
                }
            });
        } else {
            _proxyListener.onMobileNavAckReceived(fNumber);
        }
    }

    protected void handleStartServiceNack(final ServiceType serviceType) {
        Log.i(TAG, "Service Nack received for " + serviceType);
        if (_callbackToUIThread) {
            // Run in UI thread
            _mainUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    _proxyListener.onStartServiceNackReceived(serviceType);
                }
            });
        } else {
            _proxyListener.onStartServiceNackReceived(serviceType);
        }
    }

    protected void startMobileNaviService(byte sessionID, String correlationID) {
        Log.i(TAG, "Mobile Navi service started " + correlationID);
        createService(sessionID, ServiceType.Mobile_Nav);
        if (_callbackToUIThread) {
            // Run in UI thread
            _mainUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    _proxyListener.onMobileNaviStart();
                }
            });
        } else {
            _proxyListener.onMobileNaviStart();
        }
    }

    protected void startAudioService(byte sessionID, String correlationID) {
        Log.i(TAG, "Mobile Audio service started  " + sessionID);
        createService(sessionID, ServiceType.Audio_Service);
        if (_callbackToUIThread) {
            // Run in UI thread
            _mainUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    _proxyListener.onAudioServiceStart();
                }
            });
        } else {
            _proxyListener.onAudioServiceStart();
        }
    }

    private void createService(byte sessionID, ServiceType serviceType) {
        if (sessionID != currentSession.getSessionId()) {
            throw new IllegalArgumentException("can't create service with sessionID " + sessionID);
        }
        Service service = currentSession.createService(serviceType);
        currentSession.addService(service);
    }

    public void stopMobileNaviService() {
        if (removeServiceFromSession(currentSession.getSessionId(), ServiceType.Mobile_Nav)) {
            Log.i(TAG, "Mobile Navi Service is going to stop, " +
                    "sesId:" + currentSession.getSessionId());
            try {
                getSyncConnection().closeMobileNaviService(currentSession.getSessionId());
            } catch (NullPointerException e) {
                Log.e(TAG, e.toString());
            }
        }
    }

    public void stopAudioService() {
        if (removeServiceFromSession(currentSession.getSessionId(), ServiceType.Audio_Service)) {
            Log.i(TAG, "Mobile Audio service is going to stop, " +
                    "sesId:" + currentSession.getSessionId());
            try {
                getSyncConnection().closeAudioService(currentSession.getSessionId());
            } catch (NullPointerException e) {
                Log.e(TAG, e.toString());
            }
        }
    }

    private boolean removeServiceFromSession(byte sessionID, ServiceType serviceType) {
        List<Service> servicePool = getServicePool();
        for (Service service : servicePool) {
            if ((service.getSession().getSessionId() == sessionID) && (serviceType.equals(service.getServiceType()))) {
                currentSession.removeService(service);
                return true;
            }
        }
        return false;
    }

    // Queue internal callback message
    private void queueInternalMessage(InternalProxyMessage message) {
        synchronized (INTERNAL_MESSAGE_QUEUE_THREAD_LOCK) {
            if (_internalProxyMessageDispatcher != null) {
                _internalProxyMessageDispatcher.queueMessage(message);
            }
        }
    }

    // Queue incoming ProtocolMessage
    private void queueIncomingMessage(ProtocolMessage message) {
        synchronized (INCOMING_MESSAGE_QUEUE_THREAD_LOCK) {
            if (_incomingProxyMessageDispatcher != null) {
                _incomingProxyMessageDispatcher.queueMessage(message);
            }
        }
    }

    public OutputStream startH264() {
        OutputStream stream = null;
        if (mSyncConnection != null) {
            stream = mSyncConnection.startH264(currentSession.getSessionId());
        }
        return stream;
    }

    public void stopH264() {
        if (mSyncConnection != null) {
            mSyncConnection.stopH264();
        }
    }

    public OutputStream startAudioDataTransfer() {
        OutputStream stream = null;
        if (mSyncConnection != null) {
            stream = mSyncConnection.startAudioDataTransfer(currentSession.getSessionId());
        }
        return stream;
    }

    public void stopAudioDataTransfer() {
        if (mSyncConnection != null) {
            mSyncConnection.stopAudioDataTransfer();
        }
    }

    /**
     * Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param commandID command Id
     * @param menuText menu text
     * @param parentID parent Id
     * @param position position
     * @param vrCommands VR Commands vector
     * @param correlationID correlation Id
     * @throws SyncException
     */
    public void addCommand(Integer commandID, String menuText, Integer parentID, Integer position,
                           Vector<String> vrCommands, Integer correlationID)
            throws SyncException {

        AddCommand addCommand = RPCRequestFactory.buildAddCommand(commandID, menuText, parentID, position,
                vrCommands, correlationID);

        sendRPCRequest(addCommand);
    }

    /**
     * Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param commandID command Id
     * @param menuText
     * @param position
     * @param vrCommands
     * @param correlationID
     * @throws SyncException
     */
    public void addCommand(Integer commandID, String menuText, Integer position,
                           Vector<String> vrCommands, Integer correlationID)
            throws SyncException {

        addCommand(commandID, menuText, null, position, vrCommands, correlationID);
    }

    /**
     * Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param commandID command Id
     * @param menuText
     * @param position
     * @param correlationID
     * @throws SyncException
     */
    public void addCommand(Integer commandID, String menuText, Integer position,
                           Integer correlationID)
            throws SyncException {

        addCommand(commandID, menuText, null, position, null, correlationID);
    }

    /**
     * Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param commandID command Id
     * @param menuText
     * @param correlationID
     * @throws SyncException
     */
    public void addCommand(Integer commandID, String menuText, Integer correlationID)
            throws SyncException {

        addCommand(commandID, menuText, null, null, null, correlationID);
    }

    /**
     * Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param commandID command Id
     * @param menuText menu text
     * @param vrCommands VR Commands vector
     * @param correlationID correlation Id
     * @throws SyncException
     */
    public void addCommand(Integer commandID, String menuText, Vector<String> vrCommands,
                           Integer correlationID)
            throws SyncException {
        addCommand(commandID, menuText, null, null, vrCommands, correlationID);
    }

    /**
     * Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param commandID command Id
     * @param vrCommands
     * @param correlationID
     * @throws SyncException
     */
    public void addCommand(Integer commandID, Vector<String> vrCommands, Integer correlationID)
            throws SyncException {

        addCommand(commandID, null, null, null, vrCommands, correlationID);
    }

    /**
     * Sends an AddSubMenu RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param menuID
     * @param menuName
     * @param position
     * @param correlationID
     * @throws SyncException
     */
    public void addSubMenu(Integer menuID, String menuName, Integer position, Integer correlationID)
            throws SyncException {

        AddSubMenu msg = RPCRequestFactory.buildAddSubMenu(menuID, menuName,
                position, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends an AddSubMenu RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param menuID
     * @param menuName
     * @param correlationID
     * @throws SyncException
     */
    public void addSubMenu(Integer menuID, String menuName, Integer correlationID)
            throws SyncException {

        addSubMenu(menuID, menuName, null, correlationID);
    }

    /**
     * Send a ListFiles RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param correlationID correlation Id
     * @throws SyncException
     */
    public void listFiles(Integer correlationID) throws SyncException {
        ListFiles listFiles = new ListFiles();
        listFiles.setCorrelationID(correlationID);
        sendRPCRequest(listFiles);
    }

    /**
     * Send a SetAppIcon RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param fileName a name of the file
     * @param correlationID correlation Id
     * @throws SyncException
     */
    public void setAppIcon(String fileName, Integer correlationID) throws SyncException {
        SetAppIcon setAppIcon = new SetAppIcon();
        setAppIcon.setSyncFileName(fileName);
        setAppIcon.setCorrelationID(correlationID);
        sendRPCRequest(setAppIcon);
    }

    /**
     * Send PutFile RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param putFile PutFile object to be send
     */
    public void putFile(PutFile putFile) throws SyncException {
        if (putFile != null) {
            sendRPCRequest(putFile);
        } else {
            // TODO : Process null object here
        }
    }

    /**
     * Sends an EncodedData RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param data
     * @param correlationID
     * @throws SyncException
     */
    public void encodedSyncPData(Vector<String> data, Integer correlationID)
            throws SyncException {

        Log.i("pt", "encodedSyncPData() giving to sync");
        EncodedSyncPData msg = RPCRequestFactory.buildEncodedSyncPData(data, correlationID);
        sendRPCRequest(msg);
    }

    /**
     * Sends a Data RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param data
     * @param correlationID
     * @throws SyncException
     */
    public void syncPData(byte[] data, Integer correlationID)
            throws SyncException {

        Log.i("pt", "syncPData() giving to sync");
        SyncPData msg = RPCRequestFactory.buildSyncPData(data, correlationID);
        sendRPCRequest(msg);
    }

    /**
     * Sends an Alert RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param ttsText
     * @param alertText1
     * @param alertText2
     * @param playTone
     * @param duration
     * @param correlationID
     * @throws SyncException
     */
    public void alert(String ttsText, String alertText1,
                      String alertText2, Boolean playTone, Integer duration,
                      Integer correlationID) throws SyncException {

        Alert msg = RPCRequestFactory.buildAlert(ttsText, alertText1, alertText2,
                playTone, duration, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends an Alert RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param ttsChunks
     * @param alertText1
     * @param alertText2
     * @param playTone
     * @param duration
     * @param correlationID
     * @throws SyncException
     */
    public void alert(Vector<TTSChunk> ttsChunks,
                      String alertText1, String alertText2, Boolean playTone,
                      Integer duration, Integer correlationID) throws SyncException {

        Alert msg = RPCRequestFactory.buildAlert(ttsChunks, alertText1, alertText2, playTone,
                duration, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends an Alert RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param ttsText
     * @param playTone
     * @param correlationID
     * @throws SyncException
     */
    public void alert(String ttsText, Boolean playTone,
                      Integer correlationID) throws SyncException {

        alert(ttsText, null, null, playTone, null, correlationID);
    }

    /**
     * Sends an Alert RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param chunks
     * @param playTone
     * @param correlationID
     * @throws SyncException
     */
    public void alert(Vector<TTSChunk> chunks, Boolean playTone,
                      Integer correlationID) throws SyncException {

        alert(chunks, null, null, playTone, null, correlationID);
    }

    /**
     * Sends an Alert RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param alertText1
     * @param alertText2
     * @param playTone
     * @param duration
     * @param correlationID
     * @throws SyncException
     */
    public void alert(String alertText1, String alertText2,
                      Boolean playTone, Integer duration, Integer correlationID)
            throws SyncException {

        alert((Vector<TTSChunk>) null, alertText1, alertText2, playTone, duration, correlationID);
    }

    /**
     * Sends a CreateInteractionChoiceSet RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param choiceSet Set of {@link com.ford.syncV4.proxy.rpc.Choice} objects
     * @param interactionChoiceSetID Id of the interaction Choice set
     * @param correlationID correlation Id
     * @throws SyncException
     */
    public void createInteractionChoiceSet(Vector<Choice> choiceSet, Integer interactionChoiceSetID,
                                           Integer correlationID) throws SyncException {

        CreateInteractionChoiceSet createInteractionChoiceSet =
                RPCRequestFactory.buildCreateInteractionChoiceSet(choiceSet,
                        interactionChoiceSetID, correlationID);

        sendRPCRequest(createInteractionChoiceSet);
    }

    /**
     * Sends a DeleteCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param commandID
     * @param correlationID
     * @throws SyncException
     */
    public void deleteCommand(Integer commandID,
                              Integer correlationID) throws SyncException {

        DeleteCommand msg = RPCRequestFactory.buildDeleteCommand(commandID, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends a DeleteInteractionChoiceSet RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param interactionChoiceSetID
     * @param correlationID
     * @throws SyncException
     */
    public void deleteInteractionChoiceSet(
            Integer interactionChoiceSetID, Integer correlationID)
            throws SyncException {

        DeleteInteractionChoiceSet msg = RPCRequestFactory.buildDeleteInteractionChoiceSet(
                interactionChoiceSetID, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends a DeleteSubMenu RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param menuID
     * @param correlationID
     * @throws SyncException
     */
    public void deleteSubMenu(Integer menuID,
                              Integer correlationID) throws SyncException {

        DeleteSubMenu msg = RPCRequestFactory.buildDeleteSubMenu(menuID, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends a PerformInteraction RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param initPrompt
     * @param displayText
     * @param interactionChoiceSetID
     * @param correlationID
     * @throws SyncException
     */
    public void performInteraction(String initPrompt,
                                   String displayText, Integer interactionChoiceSetID,
                                   Integer correlationID) throws SyncException {

        PerformInteraction msg = RPCRequestFactory.buildPerformInteraction(initPrompt,
                displayText, interactionChoiceSetID, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends a PerformInteraction RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param initPrompt
     * @param displayText
     * @param interactionChoiceSetID
     * @param correlationID
     * @throws SyncException
     */
    public void performInteraction(String initPrompt,
                                   String displayText, Integer interactionChoiceSetID,
                                   String helpPrompt, String timeoutPrompt,
                                   InteractionMode interactionMode, Integer timeout,
                                   Integer correlationID) throws SyncException {

        PerformInteraction msg = RPCRequestFactory.buildPerformInteraction(
                initPrompt, displayText, interactionChoiceSetID,
                helpPrompt, timeoutPrompt, interactionMode,
                timeout, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends a PerformInteraction RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param initPrompt
     * @param displayText
     * @param interactionChoiceSetIDList
     * @param helpPrompt
     * @param timeoutPrompt
     * @param interactionMode
     * @param timeout
     * @param correlationID
     * @throws SyncException
     */
    public void performInteraction(String initPrompt,
                                   String displayText, Vector<Integer> interactionChoiceSetIDList,
                                   String helpPrompt, String timeoutPrompt,
                                   InteractionMode interactionMode, Integer timeout,
                                   Integer correlationID) throws SyncException {

        PerformInteraction msg = RPCRequestFactory.buildPerformInteraction(initPrompt,
                displayText, interactionChoiceSetIDList,
                helpPrompt, timeoutPrompt, interactionMode, timeout,
                correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends a PerformInteraction RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param initChunks
     * @param displayText
     * @param interactionChoiceSetIDList
     * @param helpChunks
     * @param timeoutChunks
     * @param interactionMode
     * @param timeout
     * @param correlationID
     * @throws SyncException
     */
    public void performInteraction(
            Vector<TTSChunk> initChunks, String displayText,
            Vector<Integer> interactionChoiceSetIDList,
            Vector<TTSChunk> helpChunks, Vector<TTSChunk> timeoutChunks,
            InteractionMode interactionMode, Integer timeout,
            Integer correlationID) throws SyncException {

        PerformInteraction msg = RPCRequestFactory.buildPerformInteraction(
                initChunks, displayText, interactionChoiceSetIDList,
                helpChunks, timeoutChunks, interactionMode, timeout,
                correlationID);

        sendRPCRequest(msg);
    }

    // Protected registerAppInterface used to ensure only non-ALM applications call
    // ReqisterAppInterface
    protected void registerAppInterfacePrivate(
            SyncMsgVersion syncMsgVersion, String appName, Vector<TTSChunk> ttsName,
            String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp,
            Language languageDesired, Language hmiDisplayLanguageDesired, Vector<AppHMIType> appHMIType,
            String appID, String autoActivateID, Integer correlationID, String hashId)
            throws SyncException {

        final RegisterAppInterface msg = RPCRequestFactory.buildRegisterAppInterface(
                syncMsgVersion, appName, ttsName, ngnMediaScreenAppName, vrSynonyms, isMediaApp,
                languageDesired, hmiDisplayLanguageDesired, appHMIType, appID, correlationID, hashId);

        sendRPCRequestPrivate(msg);

        logOnRegisterAppRequest(msg);
    }

    private void logOnRegisterAppRequest(final RegisterAppInterface msg) {
        if (_callbackToUIThread) {
            // Run in UI thread
            _mainUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    _proxyListener.onRegisterAppRequest(msg);
                }
            });
        } else {
            _proxyListener.onRegisterAppRequest(msg);
        }
    }

    /**
     * Sends a SetGlobalProperties RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param helpPrompt
     * @param timeoutPrompt
     * @param correlationID
     * @throws SyncException
     */
    public void setGlobalProperties(
            String helpPrompt, String timeoutPrompt, Integer correlationID)
            throws SyncException {

        SetGlobalProperties req = RPCRequestFactory.buildSetGlobalProperties(helpPrompt,
                timeoutPrompt, correlationID);

        sendRPCRequest(req);
    }

    /**
     * Sends a SetGlobalProperties RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param helpChunks
     * @param timeoutChunks
     * @param correlationID
     * @throws SyncException
     */
    public void setGlobalProperties(
            Vector<TTSChunk> helpChunks, Vector<TTSChunk> timeoutChunks,
            Integer correlationID) throws SyncException {

        SetGlobalProperties req = RPCRequestFactory.buildSetGlobalProperties(
                helpChunks, timeoutChunks, correlationID);

        sendRPCRequest(req);
    }

    public void resetGlobalProperties(Vector<GlobalProperty> properties,
                                      Integer correlationID) throws SyncException {

        ResetGlobalProperties req = new ResetGlobalProperties();

        req.setCorrelationID(correlationID);
        req.setProperties(properties);

        sendRPCRequest(req);
    }

    /**
     * Sends a SetMediaClockTimer RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param hours
     * @param minutes
     * @param seconds
     * @param updateMode
     * @param correlationID
     * @throws SyncException
     */
    public void setMediaClockTimer(Integer hours,
                                   Integer minutes, Integer seconds, UpdateMode updateMode,
                                   Integer correlationID) throws SyncException {

        SetMediaClockTimer msg = RPCRequestFactory.buildSetMediaClockTimer(hours,
                minutes, seconds, updateMode, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Pauses the media clock. Responses are captured through callback on IProxyListener.
     *
     * @param correlationID
     * @throws SyncException
     */
    public void pauseMediaClockTimer(Integer correlationID)
            throws SyncException {

        SetMediaClockTimer msg = RPCRequestFactory.buildSetMediaClockTimer(0,
                0, 0, UpdateMode.PAUSE, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Resumes the media clock. Responses are captured through callback on IProxyListener.
     *
     * @param correlationID
     * @throws SyncException
     */
    public void resumeMediaClockTimer(Integer correlationID)
            throws SyncException {

        SetMediaClockTimer msg = RPCRequestFactory.buildSetMediaClockTimer(0,
                0, 0, UpdateMode.RESUME, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Clears the media clock. Responses are captured through callback on IProxyListener.
     *
     * @param correlationID
     * @throws SyncException
     */
    public void clearMediaClockTimer(Integer correlationID)
            throws SyncException {

        Show msg = RPCRequestFactory.buildShow(null, null, null, "     ", null, null, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends a Show RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param mainText1
     * @param mainText2
     * @param statusBar
     * @param mediaClock
     * @param mediaTrack
     * @param alignment
     * @param correlationID
     * @throws SyncException
     */
    public void show(String mainText1, String mainText2,
                     String statusBar, String mediaClock, String mediaTrack,
                     TextAlignment alignment, Integer correlationID)
            throws SyncException {

        Show msg = RPCRequestFactory.buildShow(mainText1, mainText2,
                statusBar, mediaClock, mediaTrack,
                alignment, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends a Show RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param mainText1
     * @param mainText2
     * @param alignment
     * @param correlationID
     * @throws SyncException
     */
    public void show(String mainText1, String mainText2,
                     TextAlignment alignment, Integer correlationID)
            throws SyncException {

        show(mainText1, mainText2, null, null, null, alignment, correlationID);
    }

    /**
     * Sends a Speak RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param ttsText
     * @param correlationID
     * @throws SyncException
     */
    public void speak(String ttsText, Integer correlationID)
            throws SyncException {

        Speak msg = RPCRequestFactory.buildSpeak(TTSChunkFactory.createSimpleTTSChunks(ttsText),
                correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends a Speak RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param ttsChunks
     * @param correlationID
     * @throws SyncException
     */
    public void speak(Vector<TTSChunk> ttsChunks,
                      Integer correlationID) throws SyncException {

        Speak msg = RPCRequestFactory.buildSpeak(ttsChunks, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Sends a SubscribeButton RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param buttonName
     * @param correlationID
     * @throws SyncException
     */
    public void subscribeButton(ButtonName buttonName,
                                Integer correlationID) throws SyncException {

        SubscribeButton msg = RPCRequestFactory.buildSubscribeButton(buttonName,
                correlationID);

        sendRPCRequest(msg);
    }

    // Protected unregisterAppInterface used to ensure no non-ALM app calls
    // unregisterAppInterface.
    protected void unregisterAppInterfacePrivate(Integer correlationID)
            throws SyncException {

        UnregisterAppInterface msg =
                RPCRequestFactory.buildUnregisterAppInterface(correlationID);

        sendRPCRequestPrivate(msg);
    }

    /**
     * Sends an UnsubscribeButton RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
     *
     * @param buttonName
     * @param correlationID
     * @throws SyncException
     */
    public void unsubscribeButton(ButtonName buttonName,
                                  Integer correlationID) throws SyncException {

        UnsubscribeButton msg = RPCRequestFactory.buildUnsubscribeButton(
                buttonName, correlationID);

        sendRPCRequest(msg);
    }

    /**
     * Creates a choice to be added to a choiceset. Choice has both a voice and a visual menu component.
     *
     * @param choiceID         -Unique ID used to identify this choice (returned in callback).
     * @param choiceMenuName   -Text name displayed for this choice.
     * @param choiceVrCommands -Vector of vrCommands used to select this choice by voice. Must contain
     *                         at least one non-empty element.
     * @return Choice created.
     * @throws SyncException
     */
    public Choice createChoiceSetChoice(Integer choiceID, String choiceMenuName,
                                        Vector<String> choiceVrCommands) {
        Choice returnChoice = new Choice();

        returnChoice.setChoiceID(choiceID);
        returnChoice.setMenuName(choiceMenuName);
        returnChoice.setVrCommands(choiceVrCommands);

        return returnChoice;
    }

    /**
     * Gets type of transport currently used by this SyncProxy.
     *
     * @return One of TransportType enumeration values.
     * @see TransportType
     */
    public TransportType getCurrentTransportType() throws IllegalStateException {
        if (mSyncConnection == null) {
            throw new IllegalStateException("Incorrect state of SyncProxyBase: Calling for getCurrentTransportType() while connection is not initialized");
        }

        return mSyncConnection.getCurrentTransportType();
    }

    /**
     * ***************** END Public Helper Methods ************************
     */

    public IJsonRPCMarshaller getJsonRPCMarshaller() {
        return this._jsonRPCMarshaller;
    }

    /**
     * @param jsonRPCMarshaller the jsonRPCMarshaller to set
     */
    public void setJsonRPCMarshaller(IJsonRPCMarshaller jsonRPCMarshaller) throws IllegalArgumentException {
        if (jsonRPCMarshaller == null) {
            throw new IllegalArgumentException("jsonRPCMarshaller must not be null");
        }

        this._jsonRPCMarshaller = jsonRPCMarshaller;
    }

    protected TimerTask getCurrentReconnectTimerTask() {
        TimerTask task;
        synchronized (RECONNECT_TIMER_TASK_LOCK) {
            task = _currentReconnectTimerTask;
        }
        return task;
    }

    private void setCurrentReconnectTimerTask(TimerTask currentReconnectTimerTask) {
        synchronized (RECONNECT_TIMER_TASK_LOCK) {
            if (currentReconnectTimerTask == null) {
                _currentReconnectTimerTask.cancel();
            }
            _currentReconnectTimerTask = currentReconnectTimerTask;
        }
    }

    private boolean clearCurrentReconnectTimerTask() {
        TimerTask task = getCurrentReconnectTimerTask();
        if (task != null) {
            Log.d(TAG, "Clearing reconnect timer task");
            boolean success = task.cancel();
            setCurrentReconnectTimerTask(null);
            if (!success) {
                Log.i(TAG, "Can't cancel scheduled reconnect task");
            }
            return success;
        }

        return true;
    }

    private Timer getReconnectTimer() {
        if (_reconnectTimer == null) {
            Log.d(TAG, "Reconnect timer is null, creating a new one");
            _reconnectTimer = new Timer("ReconnectTimer", true);
        }

        return _reconnectTimer;
    }

    private void clearReconnectTimer() {
        if (_reconnectTimer != null) {
            Log.d(TAG, "Clearing reconnect timer");
            _reconnectTimer.cancel();
            _reconnectTimer = null;
        } else {
            Log.d(TAG, "Reconnect timer is already null");
        }
    }

    /**
     * Sets the desired SYNC and HMI display languages, and re-registers the
     * application.
     */
    public void resetLanguagesDesired(Language syncLanguageDesired,
                                      Language hmiDisplayLanguageDesired) {
        this._syncLanguageDesired = syncLanguageDesired;
        this._hmiDisplayLanguageDesired = hmiDisplayLanguageDesired;

        restartRPCProtocolSession();
    }

    // TODO: Need to refactor it
    public List<Service> getServicePool() {
        return currentSession.getServiceList();
    }

    public boolean hasServiceInServicesPool(ServiceType serviceType) {
        return !currentSession.isServicesEmpty() && currentSession.hasService(serviceType);
    }

    /**
     * Return number of Services in current Session
     *
     * @return number of Services in current Session
     */
    public int getServicesNumber() {
        return currentSession.getServicesNumber();
    }

    public void setSyncMsgVersionRequest(SyncMsgVersion syncMsgVersionRequest) {
        _syncMsgVersionRequest = syncMsgVersionRequest;
    }

    public SyncMsgVersion getSyncMsgVersionRequest() {
        return _syncMsgVersionRequest;
    }

    public void setApplicationName(String applicationName) {
        _applicationName = applicationName;
    }

    public String getApplicationName() {
        return _applicationName;
    }

    public void setTTSName(Vector<TTSChunk> TTSName) {
        this._ttsName = TTSName;
    }

    public Vector<TTSChunk> getTTSName() {
        return _ttsName;
    }

    public void setNgnMediaScreenAppName(String ngnMediaScreenAppName) {
        this._ngnMediaScreenAppName = ngnMediaScreenAppName;
    }

    public String getNgnMediaScreenAppName() {
        return _ngnMediaScreenAppName;
    }

    public void setVrSynonyms(Vector<String> vrSynonyms) {
        this._vrSynonyms = vrSynonyms;
    }

    public Vector<String> getVrSynonyms() {
        return _vrSynonyms;
    }

    public void setIsMediApp(Boolean isMediApp) {
        this._isMediaApp = isMediApp;
    }

    public Boolean getIsMediaApp() {
        return _isMediaApp;
    }

    public void setSyncLanguageDesired(Language syncLanguageDesired) {
        this._syncLanguageDesired = syncLanguageDesired;
    }

    public Language getSyncLanguageDesired() {
        return _syncLanguageDesired;
    }

    public void setHmiDisplayLanguageDesired(Language hmiDisplayLanguageDesired) {
        this._hmiDisplayLanguageDesired = hmiDisplayLanguageDesired;
    }

    public Language getHmiDisplayLanguageDesired() {
        return _hmiDisplayLanguageDesired;
    }

    public void setAppHMIType(Vector<AppHMIType> appHMIType) {
        this._appHMIType = appHMIType;
    }

    public Vector<AppHMIType> getAppHMIType() {
        return _appHMIType;
    }

    public void setAppID(String appID) {
        this._appID = appID;
    }

    public String getAppID() {
        return _appID;
    }

    public void setAutoActivateIdReturned(String autoActivateIdDesired) {
        this._autoActivateIdReturned = autoActivateIdDesired;
    }

    public String getAutoActivateIdReturned() {
        return _autoActivateIdReturned;
    }

    public void updateRegisterAppInterfaceParameters(RegisterAppInterface msg) {
        _syncMsgVersionRequest = msg.getSyncMsgVersion();
        _applicationName = msg.getAppName();
        _ttsName = msg.getTtsName();
        _ngnMediaScreenAppName = msg.getNgnMediaScreenAppName();
        _vrSynonyms = msg.getVrSynonyms();
        _isMediaApp = msg.getIsMediaApplication();
        _syncLanguageDesired = msg.getLanguageDesired();
        _hmiDisplayLanguageDesired = msg.getHmiDisplayLanguageDesired();
        _appHMIType = msg.getAppType();
        _appID = msg.getAppID();
    }

    public IRPCMessageHandler getRPCMessageHandler() {
        return rpcMessageHandler;
    }

    public void setRPCMessageHandler(IRPCMessageHandler RPCMessageHandler) {
        this.rpcMessageHandler = RPCMessageHandler;
    }

    // Private Class to Interface with SyncConnection
    public class SyncInterfaceBroker implements ISyncConnectionListener {

        @Override
        public void onTransportDisconnected(String info) {
            // proxyOnTransportDisconnect is called to alert the proxy that a requested
            // disconnect has completed

//			if (_advancedLifecycleManagementEnabled) {
//				// If ALM, nothing is required to be done here
//			} else {
            // If original model, notify app the proxy is closed so it will delete and reinstanciate
            notifyProxyClosed(info, new SyncException("Transport disconnected.", SyncExceptionCause.SYNC_UNAVAILALBE));
//			}
        }

        @Override
        public void onTransportError(String info, Exception e) {
            DebugTool.logError("Transport failure: " + info, e);

            if (_transportConfig != null &&
                    _transportConfig.getTransportType() ==  TransportType.USB) {
                if (CommonUtils.isUSBNoSuchDeviceError(e.toString())) {

                    if (_callbackToUIThread) {
                        // Run in UI thread
                        _mainUIHandler.post(new Runnable() {
                            @Override
                            public void run() {
                                _proxyListener.onUSBNoSuchDeviceException();
                            }
                        });
                    } else {
                        _proxyListener.onUSBNoSuchDeviceException();
                    }

                    try {
                        dispose();
                    } catch (SyncException e1) {
                        e1.printStackTrace();
                    }

                    return;
                }
            }

            if (_advancedLifecycleManagementEnabled) {
                // Cycle the proxy
                cycleProxy(SyncDisconnectedReason.TRANSPORT_ERROR);
            } else {
                notifyProxyClosed(info, e);
            }
        }

        @Override
        public void onHeartbeatTimedOut() {
            final String msg = "Heartbeat timeout";
            DebugTool.logInfo(msg);
            notifyProxyClosed(msg, new SyncException(msg, SyncExceptionCause.HEARTBEAT_PAST_DUE));
        }

        @Override
        public void onProtocolMessageReceived(ProtocolMessage msg) {
            // AudioPathThrough is coming WITH BulkData but WITHOUT JSON Data
            // Policy Snapshot is coming WITH BulkData and WITH JSON Data
            if (msg.getData().length > 0 || msg.getBulkData().length > 0) {
                queueIncomingMessage(msg);
            }
        }

        @Override
        public void onProtocolSessionStarted(Session session, byte version, String correlationID) {
            if (_wiproVersion == 1) {
                if (version == 2) setWiProVersion(version);
            }
            if (session.hasService(ServiceType.RPC)) {
                startRPCProtocolService(session.getSessionId(), correlationID);
            }
        }

        @Override
        public void onProtocolServiceEnded(ServiceType serviceType,
                                           byte sessionID, String correlationID) {
            handleEndServiceAck(serviceType, sessionID, correlationID);
        }

        @Override
        public void onProtocolError(String info, Exception e) {
            passErrorToProxyListener(info, e);
        }

        @Override
        public void onMobileNavAckReceived(int frameReceivedNumber) {
            handleMobileNavAck(frameReceivedNumber);
        }

        @Override
        public void onStartServiceNackReceived(ServiceType serviceType) {
            handleStartServiceNack(serviceType);
        }

        @Override
        public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version,
                                             String correlationID) {
            if (_wiproVersion == 2) {
                if (serviceType.equals(ServiceType.Mobile_Nav)) {
                    startMobileNaviService(sessionID, correlationID);
                } else if (serviceType.equals(ServiceType.Audio_Service)) {
                    startAudioService(sessionID, correlationID);
                }
            }
        }
    }

    public IRPCRequestConverterFactory getRpcRequestConverterFactory() {
        return rpcRequestConverterFactory;
    }

    public void setRpcRequestConverterFactory(
            IRPCRequestConverterFactory rpcRequestConverterFactory) {
        this.rpcRequestConverterFactory = rpcRequestConverterFactory;
    }

    protected void processRegisterAppInterfaceResponse(final RegisterAppInterfaceResponse response) {
        // Create callback
        if (_callbackToUIThread) {
            // Run in UI thread
            _mainUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (_proxyListener instanceof IProxyListener) {
                        ((IProxyListener) _proxyListener).onRegisterAppInterfaceResponse(response);
                    } else if (_proxyListener instanceof IProxyListenerALMTesting) {
                        ((IProxyListenerALMTesting) _proxyListener)
                                .onRegisterAppInterfaceResponse(response);
                    }
                }
            });
        } else {
            if (_proxyListener instanceof IProxyListener) {
                ((IProxyListener) _proxyListener).onRegisterAppInterfaceResponse(response);
            } else if (_proxyListener instanceof IProxyListenerALMTesting) {
                ((IProxyListenerALMTesting) _proxyListener).onRegisterAppInterfaceResponse(response);
            }
        }
    }

    /**
     * Restore interrupted Services
     */
    public void restoreServices() {
        if (!currentSession.isServicesEmpty() && mSyncConnection.getIsConnected()) {
            if (currentSession.hasService(ServiceType.Mobile_Nav)) {
                mSyncConnection.startMobileNavService(currentSession);
            }
            if (currentSession.hasService(ServiceType.Audio_Service)) {
                mSyncConnection.startAudioService(currentSession);
            }
        }
    }

    public IOnSystemRequestHandler getOnSystemRequestHandler() {
        return onSystemRequestHandler;
    }

    public void setOnSystemRequestHandler(
            IOnSystemRequestHandler onSystemRequestHandler) {
        this.onSystemRequestHandler = onSystemRequestHandler;
    }

    /**
     * Returns the next correlation ID used for internal messages.
     *
     * @return next correlation ID
     */
    private int nextCorrelationId() {
        return ++lastCorrelationId;
    }

    @Override
    public void putSystemFile(String filename, byte[] data, FileType fileType)
            throws SyncException {
        putSystemFile(filename, data, null, fileType);
    }

    @Override
    public void putSystemFile(String filename, byte[] data, Integer offset,
                              FileType fileType) throws SyncException {
        final int correlationID = nextCorrelationId();

        PutFile putFile = RPCRequestFactory.buildPutFile(filename, fileType, null, data,
                correlationID);
        putFile.setSystemFile(true);
        if (offset != null) {
            putFile.setOffset(offset);
            putFile.setLength(data.length);
        }

        sendRPCRequest(putFile);
        internalRequestCorrelationIDs.add(correlationID);
    }

    @Override
    public void putPolicyTableUpdateFile(String filename, byte[] data) throws SyncException {
        final int correlationID = nextCorrelationId();

        PutFile putFile = RPCRequestFactory.buildPutFile(filename, FileType.JSON, null, data,
                correlationID);

        sendRPCRequest(putFile);
        internalRequestCorrelationIDs.add(correlationID);
    }
}
