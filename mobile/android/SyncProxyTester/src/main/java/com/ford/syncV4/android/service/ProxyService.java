package com.ford.syncV4.android.service;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.media.MediaPlayer;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Environment;
import android.os.IBinder;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.util.Pair;
import android.util.SparseArray;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.PlaceholderFragment;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.listener.ConnectionListenersManager;
import com.ford.syncV4.android.manager.AppIdManager;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.manager.ApplicationIconManager;
import com.ford.syncV4.android.manager.LastUsedHashIdsManager;
import com.ford.syncV4.android.manager.PutFileTransferManager;
import com.ford.syncV4.android.manager.RPCRequestsResumableManager;
import com.ford.syncV4.android.manager.RestoreConnectionManager;
import com.ford.syncV4.android.module.ModuleTest;
import com.ford.syncV4.android.policies.PoliciesTest;
import com.ford.syncV4.android.policies.PoliciesTesterActivity;
import com.ford.syncV4.android.policies.PolicyFilesManager;
import com.ford.syncV4.android.receivers.SyncReceiver;
import com.ford.syncV4.android.service.proxy.OnSystemRequestHandler;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.SyncProxyConfigurationResources;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.AddCommandResponse;
import com.ford.syncV4.proxy.rpc.AddSubMenu;
import com.ford.syncV4.proxy.rpc.AddSubMenuResponse;
import com.ford.syncV4.proxy.rpc.AlertManeuverResponse;
import com.ford.syncV4.proxy.rpc.AlertResponse;
import com.ford.syncV4.proxy.rpc.ChangeRegistrationResponse;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSetResponse;
import com.ford.syncV4.proxy.rpc.DeleteCommandResponse;
import com.ford.syncV4.proxy.rpc.DeleteFileResponse;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSetResponse;
import com.ford.syncV4.proxy.rpc.DeleteSubMenuResponse;
import com.ford.syncV4.proxy.rpc.DiagnosticMessageResponse;
import com.ford.syncV4.proxy.rpc.EncodedSyncPDataResponse;
import com.ford.syncV4.proxy.rpc.EndAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.GenericResponse;
import com.ford.syncV4.proxy.rpc.GetDTCsResponse;
import com.ford.syncV4.proxy.rpc.GetVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.ListFilesResponse;
import com.ford.syncV4.proxy.rpc.OnAudioPassThru;
import com.ford.syncV4.proxy.rpc.OnButtonEvent;
import com.ford.syncV4.proxy.rpc.OnButtonPress;
import com.ford.syncV4.proxy.rpc.OnCommand;
import com.ford.syncV4.proxy.rpc.OnDriverDistraction;
import com.ford.syncV4.proxy.rpc.OnEncodedSyncPData;
import com.ford.syncV4.proxy.rpc.OnHMIStatus;
import com.ford.syncV4.proxy.rpc.OnHashChange;
import com.ford.syncV4.proxy.rpc.OnKeyboardInput;
import com.ford.syncV4.proxy.rpc.OnLanguageChange;
import com.ford.syncV4.proxy.rpc.OnPermissionsChange;
import com.ford.syncV4.proxy.rpc.OnSyncPData;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.OnTBTClientState;
import com.ford.syncV4.proxy.rpc.OnTouchEvent;
import com.ford.syncV4.proxy.rpc.OnVehicleData;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.PerformInteractionResponse;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.ReadDIDResponse;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.RegisterAppInterfaceResponse;
import com.ford.syncV4.proxy.rpc.ResetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.ScrollableMessageResponse;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.SetAppIconResponse;
import com.ford.syncV4.proxy.rpc.SetDisplayLayoutResponse;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.SetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimerResponse;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.ShowConstantTBTResponse;
import com.ford.syncV4.proxy.rpc.ShowResponse;
import com.ford.syncV4.proxy.rpc.SliderResponse;
import com.ford.syncV4.proxy.rpc.SpeakResponse;
import com.ford.syncV4.proxy.rpc.SubscribeButton;
import com.ford.syncV4.proxy.rpc.SubscribeButtonResponse;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.SyncPDataResponse;
import com.ford.syncV4.proxy.rpc.SystemRequestResponse;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterfaceResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeButtonResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.UpdateTurnListResponse;
import com.ford.syncV4.proxy.rpc.enums.AppHMIType;
import com.ford.syncV4.proxy.rpc.enums.AppInterfaceUnregisteredReason;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.HMILevel;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.test.ITestConfigCallback;
import com.ford.syncV4.test.TestConfig;
import com.ford.syncV4.transport.BTTransportConfig;
import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.transport.usb.USBTransportConfig;
import com.ford.syncV4.util.BTHelper;
import com.ford.syncV4.util.Base64;
import com.ford.syncV4.util.StringUtils;
import com.ford.syncV4.util.logger.Logger;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Vector;
import java.util.concurrent.ConcurrentHashMap;

public class ProxyService extends Service implements IProxyListenerALMTesting, ITestConfigCallback {

    static final String TAG = ProxyService.class.getSimpleName();

    public static final int HEARTBEAT_INTERVAL = 5000;
    public static final int HEARTBEAT_INTERVAL_MAX = Integer.MAX_VALUE;
    private Integer autoIncCorrId = 1;

    private static final String ICON_SYNC_FILENAME = "icon.png";
    private static final String ICON_FILENAME_SUFFIX = ".png";

    private static final int XML_TEST_COMMAND = 100;
    private static final int POLICIES_TEST_COMMAND = 101;

    private SyncProxyALM mSyncProxy;
    private final Vector<LogAdapter> mLogAdapters = new Vector<LogAdapter>();
    private ModuleTest mModuleTest;
    private MediaPlayer mEmbeddedAudioPlayer;
    private Boolean playingAudio = false;
    protected SyncReceiver mediaButtonReceiver;

    private boolean firstHMIStatusChange = true;
    private HMILevel prevHMILevel = HMILevel.HMI_NONE;

    private boolean mWaitingForResponse = false;
    private IProxyServiceEvent mProxyServiceEvent;
    private ICloseSession mCloseSessionCallback;

    /**
     * Table of the {@link com.ford.syncV4.android.manager.PutFileTransferManager}'s
     * each {@link com.ford.syncV4.android.manager.PutFileTransferManager} is associated with
     * concrete AppId
     */
    private Map<String, PutFileTransferManager> mPutFileTransferManagers;
    private Map<String, ApplicationIconManager> mApplicationIconManagerHashtable =
            new ConcurrentHashMap<String, ApplicationIconManager>();
    private ConnectionListenersManager mConnectionListenersManager;
    private final IBinder mBinder = new ProxyServiceBinder(this);

    /**
     * Semaphore object to wait for the connection to be restored up to
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} notification receive
     */
    private final RestoreConnectionManager restoreConnectionToRAI = new RestoreConnectionManager();

    /**
     * Semaphore object to wait for the connection to be restored up to RPC Service
     */
    private final RestoreConnectionManager restoreConnectionToRPCService =
            new RestoreConnectionManager();

    /**
     * Table of the {@link com.ford.syncV4.android.manager.RPCRequestsResumableManager}'s
     * Each manager provide functionality to process RPC requests which are involved in app
     * resumption
     */
    private ConcurrentHashMap<String, RPCRequestsResumableManager> mRpcRequestsResumableManager =
            new ConcurrentHashMap<String, RPCRequestsResumableManager>();

    /**
     * Map of the existed syncProxyTester applications, mobile application Id is a Key
     */
    private final HashMap<String, RegisterAppInterface> registerAppInterfaceHashMap =
            new HashMap<String, RegisterAppInterface>();

    // This Config object stores all the necessary data for SDK testing
    private TestConfig mTestConfig = new TestConfig();

    private final HashSet<String> mSessionsCounter = new HashSet<String>();

    @Override
    public void onCreate() {
        super.onCreate();

        createInfoMessageForAdapter("ProxyService.onCreate()");
        Logger.i(TAG + " OnCreate, mSyncProxy:" + mSyncProxy);

        // Init Listener managers (ConnectionListenersManager, etc ...)
        mConnectionListenersManager = new ConnectionListenersManager();

        IntentFilter mediaIntentFilter = new IntentFilter();
        mediaIntentFilter.addAction(Intent.ACTION_MEDIA_BUTTON);

        mediaButtonReceiver = new SyncReceiver();
        registerReceiver(mediaButtonReceiver, mediaIntentFilter);

        //startProxyIfNetworkConnected();

        mPutFileTransferManagers = new ConcurrentHashMap<String, PutFileTransferManager>();

        MainApp.getInstance().getLastUsedHashIdsManager().init();
    }

    public void showLockMain() {
        if (SyncProxyTester.getInstance() == null) {
            Intent i = new Intent(this, SyncProxyTester.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            startActivity(i);

            // quite a few things downstream depend on the main activity and its
            // fields being alive, so wait for a while here
            int numTries = 9;
            while ((SyncProxyTester.getInstance() == null) && (numTries-- >= 0)) {
                try {
                    Thread.sleep(50);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Logger.d(TAG, "created " + SyncProxyTester.getInstance());
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Logger.i(TAG + " OnStartCommand");
        createInfoMessageForAdapter("ProxyService.onStartCommand()");
        return START_STICKY;
    }

    /**
     * Function checks if WiFi enabled.
     * Manifest permission is required:
     * <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE"/>
     *
     * @return true if enabled
     */
    private boolean hasWiFiConnection() {
        boolean result = false;

        ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        if (cm != null) {
            NetworkInfo[] netInfo = cm.getAllNetworkInfo();
            if (netInfo != null) {
                for (NetworkInfo ni : netInfo) {
                    if (ni.getTypeName().equalsIgnoreCase("WIFI")) {
                        Logger.d(TAG, ni.getTypeName());
                        if (ni.isConnected()) {
                            Logger.d(TAG,
                                    "ProxyService().hasWiFiConnection(): wifi conncetion found");
                            result = true;
                        }
                    }
                }
            }
        }
        return result;
    }

    public void startProxyIfNetworkConnected() {
        SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME, MODE_PRIVATE);
        int transportType = prefs.getInt(
                Const.Transport.PREFS_KEY_TRANSPORT_TYPE,
                Const.Transport.PREFS_DEFAULT_TRANSPORT_TYPE);
        Logger.i(TAG, " Start Proxy If Network Connected");
        boolean doStartProxy = false;
        if (transportType == Const.Transport.KEY_BLUETOOTH) {
            Logger.i(TAG, " Transport = Bluetooth.");
            if (BTHelper.isBTAdapterAvailable()) {
                doStartProxy = true;
            }
        } else {
            Logger.i(TAG, " Transport = Default.");
            //TODO: This code is commented out for simulator purposes
            /*
            Logger.d(CLASS_NAME, "ProxyService. onStartCommand(). Transport = WiFi.");
			if (hasWiFiConnection() == true) {
				Logger.d(CLASS_NAME, "ProxyService. onStartCommand(). WiFi enabled.");
				startProxy();
			} else {
				Logger.w(CLASS_NAME,
						"ProxyService. onStartCommand(). WiFi is not enabled.");
			}
			*/
            doStartProxy = true;
        }
        if (doStartProxy) {

            // Prepare all necessary data that need to be use in the Tests
            prepareTestConfig();

            boolean result = startProxy();
            Logger.i(TAG + " Proxy complete result:" + result);
        }
    }

    /**
     * @return an object which contains Testing configuration data
     */
    public TestConfig getTestConfig() {
        return mTestConfig;
    }

    /**
     * @return reference to the {@link com.ford.syncV4.android.manager.RestoreConnectionManager}
     * when restore connection up to {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} is needed
     */
    public RestoreConnectionManager getRestoreConnectionToRAI() {
        return restoreConnectionToRAI;
    }

    /**
     * @return reference to the {@link com.ford.syncV4.android.manager.RestoreConnectionManager}
     * when restore connection up to RPC service is needed
     */
    public RestoreConnectionManager getRestoreConnectionToRPCService() {
        return restoreConnectionToRPCService;
    }

    public boolean hasRPCRunning(String appId) {
        for (String aMSessionsCounter : mSessionsCounter) {
            Logger.d(TAG + " Available RPC: " + aMSessionsCounter);
        }
        return mSessionsCounter.contains(appId);
    }

    /**
     * Prepare all necessary parameters to be passed to Sync proxy
     */
    private void prepareTestConfig() {
        mTestConfig.setUseHashId(AppPreferencesManager.getUseHashId());
        mTestConfig.setCustomHashId(AppPreferencesManager.getCustomHashId());
        mTestConfig.setUseCustomHashId(AppPreferencesManager.getUseCustomHashId());
    }

    private boolean startProxy() {
        createInfoMessageForAdapter(" Start SYNC Proxy");

        String appId = "";
        if (mSyncProxy == null) {
            try {
                SharedPreferences settings = getSharedPreferences(Const.PREFS_NAME, 0);
                boolean isMediaApp = settings.getBoolean(
                        Const.PREFS_KEY_ISMEDIAAPP,
                        Const.PREFS_DEFAULT_ISMEDIAAPP);
                boolean isNaviApp = settings.getBoolean(
                        Const.PREFS_KEY_ISNAVIAPP,
                        Const.PREFS_DEFAULT_ISNAVIAPP);
                int versionNumber = getCurrentProtocolVersion();
                String appName = settings.getString(Const.PREFS_KEY_APPNAME,
                        Const.PREFS_DEFAULT_APPNAME);
                String vrSynonymsString = settings.getString(Const.PREFS_KEY_VR_SYNONYMS, "");
                Vector<Object> vrSynonyms = null;
                if (!vrSynonymsString.equals("")) {
                    vrSynonyms = new Vector<Object>(Arrays.asList(
                            vrSynonymsString.split(SyncProxyTester.JOIN_STRING)));
                }
                Language lang = Language.valueOf(settings.getString(
                        Const.PREFS_KEY_LANG, Const.PREFS_DEFAULT_LANG));
                Language hmiLang = Language.valueOf(settings.getString(
                        Const.PREFS_KEY_HMILANG, Const.PREFS_DEFAULT_HMILANG));
                Logger.i(TAG, " Using protocol version " + versionNumber);
                String ipAddress = settings.getString(
                        Const.Transport.PREFS_KEY_TRANSPORT_IP,
                        Const.Transport.PREFS_DEFAULT_TRANSPORT_IP);
                int tcpPort = settings.getInt(
                        Const.Transport.PREFS_KEY_TRANSPORT_PORT,
                        Const.Transport.PREFS_DEFAULT_TRANSPORT_PORT);
                boolean mIsNSD = settings.getBoolean(Const.Transport.PREFS_KEY_IS_NSD, false);

                SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
                syncMsgVersion.setMajorVersion(2);
                syncMsgVersion.setMinorVersion(2);
                Vector<AppHMIType> appHMITypes = createAppTypeVector(isNaviApp);
                BaseTransportConfig transportConfig = null;
                TransportType transportType = AppPreferencesManager.getTransportType();
                appId = AppIdManager.getAppIdByTransport(transportType);
                switch (transportType) {
                    case BLUETOOTH:
                        transportConfig = new BTTransportConfig();
                        break;
                    case TCP:
                        transportConfig = new TCPTransportConfig(tcpPort, ipAddress);
                        ((TCPTransportConfig) transportConfig).setIsNSD(mIsNSD);
                        ((TCPTransportConfig) transportConfig).setApplicationContext(this);
                        break;
                    case USB:
                        transportConfig = new USBTransportConfig(getApplicationContext());
                        break;
                }

                // Apply custom AppId in case of such possibility selected
                if (AppPreferencesManager.getIsCustomAppId()) {
                    appId = AppPreferencesManager.getCustomAppId();
                }

                SyncProxyConfigurationResources syncProxyConfigurationResources =
                        new SyncProxyConfigurationResources();
                syncProxyConfigurationResources.setTelephonyManager(
                        (TelephonyManager) MainApp.getInstance().getSystemService(Context.TELEPHONY_SERVICE));

                mTestConfig.setProtocolMinVersion((byte) AppPreferencesManager.getProtocolMinVersion());
                mTestConfig.setProtocolMaxVersion((byte) AppPreferencesManager.getProtocolMaxVersion());

                Logger.i("Start SYNC Proxy's instance, vrSynonyms:" + vrSynonyms);
                mSyncProxy = new SyncProxyALM(this,
                        syncProxyConfigurationResources /*sync proxy configuration resources*/,
                        appName,
                        null,                           /*ngn media app*/
                        vrSynonyms,
                        isMediaApp,                     /*is media app*/
                        appHMITypes,
                        syncMsgVersion,
                        lang,                           /*language desired*/
                        hmiLang,                        /*HMI Display Language Desired*/
                        appId,
                        null,                           /*autoActivateID*/
                        false,                          /*callbackToUIThread*/
                        false,                          /*preRegister*/
                        versionNumber,
                        transportConfig,
                        mTestConfig);
            } catch (SyncException e) {
                Logger.e("SYNC Proxy start error:" + e.toString());
                //error creating proxy, returned proxy = null
                if (mSyncProxy == null) {

                    if (mProxyServiceEvent != null) {
                        mProxyServiceEvent.onProxyInitError(e.getMessage());
                    }

                    stopServiceBySelf();
                    return false;
                }
            }
        }

        LogAdapter logAdapter = getLogAdapterByAppId(appId);
        OnSystemRequestHandler mOnSystemRequestHandler = new OnSystemRequestHandler(logAdapter);

        mSyncProxy.setOnSystemRequestHandler(mOnSystemRequestHandler);
        mSyncProxy.setTestConfigCallback(this);

        createInfoMessageForAdapter(" SYNC Proxy started");

        return mSyncProxy != null && mSyncProxy.getIsConnected();
    }

    /**
     * Updates {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} object, use for the Tests only
     *
     * @param registerAppInterface {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} request
     */
    public void updateRegisterAppInterface(RegisterAppInterface registerAppInterface) {
        mSyncProxy.updateRegisterAppInterfaceParameters(registerAppInterface);
    }

    private Vector<AppHMIType> createAppTypeVector(boolean naviApp) {
        if (naviApp) {
            Vector<AppHMIType> vector = new Vector<AppHMIType>();
            vector.add(AppHMIType.NAVIGATION);
            return vector;
        }
        return null;
    }

    private int getCurrentProtocolVersion() {
        return ProtocolConstants.PROTOCOL_VERSION_MIN;
    }

    @Override
    public void onDestroy() {
        createInfoMessageForAdapter("OnDestroy");

        // In case service is destroying by System
        if (mProxyServiceEvent == null) {
            // TODO : Reconsider this case, for instance if we just close Session
            //disposeSyncProxy();
        }
        setProxyServiceEvent(null);
        if (mEmbeddedAudioPlayer != null) {
            mEmbeddedAudioPlayer.release();
        }
        unregisterReceiver(mediaButtonReceiver);
        super.onDestroy();
    }

    public void sendPolicyTableUpdate(String appId, FileType fileType, RequestType requestType) {
        LogAdapter logAdapter = getLogAdapterByAppId(appId);
        PolicyFilesManager.sendPolicyTableUpdate(appId, mSyncProxy, fileType, requestType, logAdapter);
    }

    public void setCloseSessionCallback(ICloseSession closeSessionCallback) {
        mCloseSessionCallback = closeSessionCallback;
    }

    public void setProxyServiceEvent(IProxyServiceEvent proxyServiceEvent) {
        mProxyServiceEvent = proxyServiceEvent;
    }

    public void destroyService() {
        disposeSyncProxy();
    }

    private void disposeSyncProxy() {
        createInfoMessageForAdapter(" Dispose SYNC Proxy");

        MainApp.getInstance().getLastUsedHashIdsManager().save();

        if (mSyncProxy != null) {
            try {
                mSyncProxy.dispose();
                mSyncProxy.closeSyncConnection();
            } catch (SyncException e) {
                Logger.e(TAG, e.toString());
                if (mProxyServiceEvent != null) {
                    mProxyServiceEvent.onDisposeError();
                }
            }
            mSyncProxy = null;
        }
    }

    /**
     * @return the number of the current session's services
     */
    public int getServicesNumber() {
        return mSyncProxy.getServicesNumber();
    }

    public boolean hasServiceInServicesPool(String appId, ServiceType serviceType) {
        return mSyncProxy != null && mSyncProxy.hasServiceInServicesPool(appId, serviceType);
    }

    private void initializePredefinedView(String appId) {
        Logger.d(TAG, "Initialize predefined view, appId:" + appId);
        playingAudio = true;
        playAnnoyingRepetitiveAudio();

        try {
            show(appId, "Sync Proxy", "Tester");
        } catch (SyncException e) {
            createErrorMessageForAdapter(appId, "Error sending show:" + e.getMessage());
        }

        commandSubscribeButtonPredefined(appId, ButtonName.OK, getNextCorrelationID());
        commandSubscribeButtonPredefined(appId, ButtonName.SEEKLEFT, getNextCorrelationID());
        commandSubscribeButtonPredefined(appId, ButtonName.SEEKRIGHT, getNextCorrelationID());
        commandSubscribeButtonPredefined(appId, ButtonName.TUNEUP, getNextCorrelationID());
        commandSubscribeButtonPredefined(appId, ButtonName.TUNEDOWN, getNextCorrelationID());

        Vector<ButtonName> buttons = new Vector<ButtonName>(Arrays.asList(new ButtonName[]{
                ButtonName.OK, ButtonName.SEEKLEFT, ButtonName.SEEKRIGHT, ButtonName.TUNEUP,
                ButtonName.TUNEDOWN}));
        SyncProxyTester.getInstance().buttonsSubscribed(buttons);

        commandAddCommandPredefined(appId, XML_TEST_COMMAND,
                new Vector<String>(Arrays.asList(new String[]{"XML Test", "XML"})), "XML Test");
        commandAddCommandPredefined(appId, POLICIES_TEST_COMMAND,
                new Vector<String>(Arrays.asList(new String[]{"Policies Test", "Policies"})),
                "Policies Test");
    }

    private void show(String appId, String mainField1, String mainField2) throws SyncException {
        Show msg = new Show();
        msg.setCorrelationID(getNextCorrelationID());
        msg.setMainField1(mainField1);
        msg.setMainField2(mainField2);
        mSyncProxy.sendRPCRequest(appId, msg);
    }

    public void playPauseAnnoyingRepetitiveAudio() {
        if (mEmbeddedAudioPlayer != null && mEmbeddedAudioPlayer.isPlaying()) {
            playingAudio = false;
            pauseAnnoyingRepetitiveAudio();
        } else {
            playingAudio = true;
            playAnnoyingRepetitiveAudio();
        }
    }

    private void playAnnoyingRepetitiveAudio() {
        if (mEmbeddedAudioPlayer == null) {
            mEmbeddedAudioPlayer = MediaPlayer.create(this, R.raw.arco);
            mEmbeddedAudioPlayer.setLooping(true);
        }
        mEmbeddedAudioPlayer.start();

        createDebugMessageForAdapter("Playing audio");
    }

    public void pauseAnnoyingRepetitiveAudio() {
        if (mEmbeddedAudioPlayer != null && mEmbeddedAudioPlayer.isPlaying()) {
            mEmbeddedAudioPlayer.pause();

            createDebugMessageForAdapter("Paused Audio");
        }
    }

    public boolean isSyncProxyNotNull() {
        return mSyncProxy != null;
    }

    public boolean isSyncProxyConnected() {
        return mSyncProxy != null && mSyncProxy.getIsConnected();
    }

    public boolean isSyncProxyConnectionNotNull() {
        return mSyncProxy != null && mSyncProxy.getSyncConnection() != null;
    }

    public void restartModuleTest(String appId, String filePath) {
        mModuleTest.restart(appId, filePath);
    }

    public void startModuleTest(String appId) {
        LogAdapter logAdapter = getLogAdapterByAppId(appId);
        if (logAdapter != null) {
            mModuleTest = new ModuleTest(appId, this, logAdapter);
        } else {
            Logger.w(TAG + " new ModuleTest Log adapter is null");
        }
    }

    /**
     * @return an instance of the Test Module
     */
    public ModuleTest getModuleTest() {
        return mModuleTest;
    }

    public void waiting(boolean waiting) {
        mWaitingForResponse = waiting;
    }

    /**
     * Add {@link com.ford.syncV4.android.adapters.LogAdapter} instance
     * @param logAdapter {@link com.ford.syncV4.android.adapters.LogAdapter}
     */
    public void addLogAdapter(LogAdapter logAdapter) {
        if (logAdapter == null) {
            return;
        }
        if (mLogAdapters.contains(logAdapter)) {
            return;
        }
        mLogAdapters.add(logAdapter);
    }

    public int getNextCorrelationID() {
        return autoIncCorrId++;
    }

    @Override
    public void onOnHMIStatus(String appId, OnHMIStatus notification) {
        //Logger.d(TAG + " OnHMIStatusChange AppId:" + appId);

        createDebugMessageForAdapter(appId, notification);

        switch (notification.getSystemContext()) {
            case SYSCTXT_MAIN:
                break;
            case SYSCTXT_VRSESSION:
                break;
            case SYSCTXT_MENU:
                break;
            default:
                return;
        }

        switch (notification.getAudioStreamingState()) {
            case AUDIBLE:
                if (playingAudio) {
                    playAnnoyingRepetitiveAudio();
                }
                break;
            case NOT_AUDIBLE:
                pauseAnnoyingRepetitiveAudio();
                break;
            default:
                return;
        }

        final HMILevel curHMILevel = notification.getHmiLevel();
        final boolean appInterfaceRegistered = mSyncProxy.getAppInterfaceRegistered(appId);

        if ((HMILevel.HMI_NONE == curHMILevel) && appInterfaceRegistered) {
            if (!isModuleTesting()) {
                if (AppPreferencesManager.getAutoSetAppIconFlag()) {
                    ApplicationIconManager applicationIconManager =
                            mApplicationIconManagerHashtable.get(appId);
                    if (applicationIconManager == null) {
                        applicationIconManager = new ApplicationIconManager(appId);
                        mApplicationIconManagerHashtable.put(appId, applicationIconManager);
                    }
                    applicationIconManager.setApplicationIcon(this);
                }
            }
        }

        if (prevHMILevel != curHMILevel) {
            boolean hmiChange = false;
            boolean hmiFull = false;
            switch (curHMILevel) {
                case HMI_FULL:
                    hmiFull = true;
                    hmiChange = true;
                    break;
                case HMI_LIMITED:
                    hmiChange = true;
                    break;
                case HMI_BACKGROUND:
                    hmiChange = true;
                    break;
                case HMI_NONE:
                    break;
                default:
                    return;
            }
            prevHMILevel = curHMILevel;

            if (appInterfaceRegistered) {
                if (hmiFull) {
                    if (firstHMIStatusChange) {
                        showLockMain();

                        LogAdapter logAdapter = getLogAdapterByAppId(appId);
                        if (logAdapter != null) {
                            mModuleTest = new ModuleTest(appId, this, logAdapter);
                        } else {
                            Logger.w(TAG + " new ModuleTest Log adapter is null");
                        }

                        // Process an init state of the predefined requests here, assume that if
                        // hashId is not null means this is resumption
                        if (mSyncProxy.getHashId(appId) == null) {
                            initializePredefinedView(appId);
                        }
                    } else {
                        try {
                            if (mModuleTest != null && !mWaitingForResponse &&
                                    mModuleTest.getXMLTestThreadContext() != null) {
                                show(appId, "Sync Proxy", "Tester Ready");
                            }
                        } catch (SyncException e) {
                            createErrorMessageForAdapter(appId, "Error sending show");
                        }
                    }
                }

                if (hmiChange && firstHMIStatusChange) {
                    firstHMIStatusChange = false;

                    // Process an init state of the predefined requests here, assume that if
                    // hashId is not null means this is resumption
                    if (mSyncProxy.getHashId(appId) == null) {
                        // upload turn icons
                        //Logger.d("Upload Icons");
                        sendIconFromResource(appId, R.drawable.turn_left);
                        sendIconFromResource(appId, R.drawable.turn_right);
                        sendIconFromResource(appId, R.drawable.turn_forward);
                        sendIconFromResource(appId, R.drawable.action);
                    }
                }
            }
        }
    }

    @Override
    public void onHashChange(String appId, OnHashChange onHashChange) {
        createDebugMessageForAdapter(appId, onHashChange);

        LastUsedHashIdsManager lastUsedHashIdsManager =
                MainApp.getInstance().getLastUsedHashIdsManager();
        lastUsedHashIdsManager.addNewId(onHashChange.getHashID());
    }

    /**
     * Checks and returns if the module testing is in progress.
     *
     * @return true if the module testing is in progress
     */
    private boolean isModuleTesting() {
        return mWaitingForResponse && mModuleTest.getXMLTestThreadContext() != null;
    }

    public void sendIconFromResource(String appId, int resource) {
        commandPutFile(appId, FileType.GRAPHIC_PNG,
                getResources().getResourceEntryName(resource) + ICON_FILENAME_SUFFIX,
                AppUtils.contentsOfResource(resource), getNextCorrelationID(), true);
    }

    @Override
    public void onOnCommand(String appId, OnCommand notification) {
        createDebugMessageForAdapter(appId, notification);
        switch (notification.getCmdID()) {
            case XML_TEST_COMMAND:
                mModuleTest.restart(appId, null);
                break;
            case POLICIES_TEST_COMMAND:
                LogAdapter logAdapter = getLogAdapterByAppId(appId);
                if (logAdapter != null) {
                    PoliciesTest.runPoliciesTest(appId, this, logAdapter);
                } else {
                    Logger.w(TAG + " PoliciesTest.runPoliciesTest Log adapter is null");
                }
                break;
            default:
                break;
        }
    }

    @Override
    public void onProxyClosed(final String info, Exception e) {
        String message = info;
        if (StringUtils.isEmpty(message)) {
            if (e != null) {
                message = e.getMessage();
            } else {
                message = "<no message>";
            }
        }
        createErrorMessageForAdapter("SYNC Proxy closed:" + message);
        boolean wasConnected = !firstHMIStatusChange;
        firstHMIStatusChange = true;
        prevHMILevel = HMILevel.HMI_NONE;
        for (String appId: mApplicationIconManagerHashtable.keySet()) {
            mApplicationIconManagerHashtable.get(appId).reset();
        }

        if (wasConnected) {
            mConnectionListenersManager.dispatch();
        }

        Logger.d("Is Module testing:" + isModuleTesting());

        if (!isModuleTesting()) {
            if (e == null) {
                return;
            }
            if (e instanceof SyncException) {
                final SyncExceptionCause cause = ((SyncException) e).getSyncExceptionCause();
                if ((cause != SyncExceptionCause.SYNC_PROXY_CYCLED) &&
                        (cause != SyncExceptionCause.BLUETOOTH_DISABLED) &&
                        (cause != SyncExceptionCause.SYNC_REGISTRATION_ERROR)) {
                    //reset();
                }
            }
        } else {
            mSyncProxy = null;
        }
    }

    public void reset() {
        if (mSyncProxy == null) {
            return;
        }
        // In case we run exit() - this is a quick marker of exiting.
        if (mProxyServiceEvent != null) {
            return;
        }
        try {
            mSyncProxy.resetProxy();
        } catch (SyncException e1) {
            Logger.e("Reset proxy error:" + e1);
            //something goes wrong, the proxy returns as null, stop the service.
            //do not want a running service with a null proxy
            if (mSyncProxy == null) {
                stopServiceBySelf();
            }
        }
    }

    /**
     * Restarting SyncProxyALM. For example after changing transport type
     */
    public void restart() {
        Logger.i(TAG, " Restart SYNC Proxy");
        disposeSyncProxy();
        startProxyIfNetworkConnected();
    }

    public void restart_withAppId_for_test(String appId) {
        Logger.i(TAG, " Restart SYNC Proxy with AppId");
        AppPreferencesManager.setCustomAppId(appId);


        restart();
    }

    @Override
    public void onError(String info, Throwable e) {
        createErrorMessageForAdapter("Proxy error: " + info);
    }

    /**
     * ******************************
     *  SYNC AppLink Base Callbacks *
     * ******************************
     */
    @Override
    public void onAddSubMenuResponse(final String appId, AddSubMenuResponse response) {
        createDebugMessageForAdapter(appId, response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onAddSubMenuResponse(appId, success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onCreateInteractionChoiceSetResponse(final String appId,
                                                     CreateInteractionChoiceSetResponse response) {
        createDebugMessageForAdapter(appId, response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onCreateChoiceSetResponse(appId, success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onDeleteCommandResponse(final String appId, DeleteCommandResponse response) {
        createDebugMessageForAdapter(appId, response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onDeleteCommandResponse(appId, success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onDeleteInteractionChoiceSetResponse(final String appId,
                                                     DeleteInteractionChoiceSetResponse response) {
        createDebugMessageForAdapter(appId, response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onDeleteChoiceSetResponse(appId, success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onDeleteSubMenuResponse(final String appId, DeleteSubMenuResponse response) {
        createDebugMessageForAdapter(appId, response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onDeleteSubMenuResponse(appId, success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onEncodedSyncPDataResponse(String appId, EncodedSyncPDataResponse response) {
        Logger.i("syncp", "onEncodedSyncPDataResponse: " + response.getInfo() +
                response.getResultCode() + response.getSuccess());
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onResetGlobalPropertiesResponse(String appId,
                                                ResetGlobalPropertiesResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onSetMediaClockTimerResponse(String appId, SetMediaClockTimerResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onSpeakResponse(String appId, SpeakResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onSubscribeButtonResponse(String appId, SubscribeButtonResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onUnsubscribeButtonResponse(String appId, UnsubscribeButtonResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onOnDriverDistraction(OnDriverDistraction notification) {
        createDebugMessageForAdapter(notification);
    }

    @Override
    public void onGenericResponse(GenericResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    /**
     * *******************************************
     *  SYNC AppLink Soft Button Image Callbacks *
     * *******************************************
     */
    @Override
    public void onPutFileResponse(String appId, PutFileResponse response) {
        createDebugMessageForAdapter(appId, response);
        final int receivedCorrelationId = response.getCorrelationID();

        if (AppPreferencesManager.getAutoSetAppIconFlag()) {
            ApplicationIconManager applicationIconManager =
                    mApplicationIconManagerHashtable.get(appId);
            if (applicationIconManager != null) {
                applicationIconManager.setAppIcon(this, receivedCorrelationId);
            }
        }

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(receivedCorrelationId,
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
        PutFileTransferManager putFileTransferManager = mPutFileTransferManagers.get(appId);
        if (putFileTransferManager == null) {
            return;
        }
        putFileTransferManager.removePutFileFromAwaitArray(receivedCorrelationId);
    }

    @Override
    public void onDeleteFileResponse(String appId, DeleteFileResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onListFilesResponse(String appId, ListFilesResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onSetAppIconResponse(String appId, SetAppIconResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onOnButtonEvent(String appId, OnButtonEvent notification) {
        createDebugMessageForAdapter(appId, notification);
    }

    @Override
    public void onOnButtonPress(String appId, OnButtonPress notification) {
        createDebugMessageForAdapter(appId, notification);
        switch (notification.getButtonName()) {
            case OK:
                playPauseAnnoyingRepetitiveAudio();
                break;
            case SEEKLEFT:
                break;
            case SEEKRIGHT:
                break;
            case TUNEUP:
                break;
            case TUNEDOWN:
                break;
            default:
                break;
        }
    }

    /**
     * *********************************
     *  SYNC AppLink Updated Callbacks *
     * *********************************
     */
    @Override
    public void onAddCommandResponse(final String appId, AddCommandResponse response) {
        createDebugMessageForAdapter(appId, response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onAddCommandResponse(appId, success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onAlertResponse(String appId, AlertResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onPerformInteractionResponse(String appId, PerformInteractionResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onSetGlobalPropertiesResponse(String appId, SetGlobalPropertiesResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onShowResponse(String appId, ShowResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    /**
     * *****************************
     *  SYNC AppLink New Callbacks *
     * *****************************
     */
    @Override
    public void onSliderResponse(String appId, SliderResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onScrollableMessageResponse(String appId, ScrollableMessageResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onChangeRegistrationResponse(String appId, ChangeRegistrationResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onSetDisplayLayoutResponse(String appId, SetDisplayLayoutResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onOnLanguageChange(String appId, OnLanguageChange notification) {
        createDebugMessageForAdapter(appId, notification);
    }

    /**
     * *****************************************
     *  SYNC AppLink Audio Pass Thru Callbacks *
     * *****************************************
     */
    @Override
    public void onPerformAudioPassThruResponse(String appId, PerformAudioPassThruResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }

        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final Result result = response.getResultCode();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onPerformAudioPassThruResponse(result);
            }
        });
    }

    @Override
    public void onEndAudioPassThruResponse(String appId, EndAudioPassThruResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }

        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final Result result = response.getResultCode();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onEndAudioPassThruResponse(result);
            }
        });
    }

    @Override
    public void onOnAudioPassThru(String appId, OnAudioPassThru notification) {
        createDebugMessageForAdapter(appId, notification);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final byte[] aptData = notification.getAPTData();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onAudioPassThru(aptData);
            }
        });
    }

    /**
     * **************************************
     *  SYNC AppLink Vehicle Data Callbacks *
     * **************************************
     */
    @Override
    public void onSubscribeVehicleDataResponse(String appId,
                                               SubscribeVehicleDataResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onUnsubscribeVehicleDataResponse(String appId,
                                                 UnsubscribeVehicleDataResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onGetVehicleDataResponse(String appId, GetVehicleDataResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onReadDIDResponse(String appId, ReadDIDResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onGetDTCsResponse(String appId, GetDTCsResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onOnVehicleData(String appId, OnVehicleData notification) {
        createDebugMessageForAdapter(appId, notification);
    }

    /**
     * *******************************
     *  SYNC AppLink TBT Callbacks   *
     * *******************************
     */
    @Override
    public void onShowConstantTBTResponse(String appId, ShowConstantTBTResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onAlertManeuverResponse(String appId, AlertManeuverResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onUpdateTurnListResponse(String appId, UpdateTurnListResponse response) {
        createDebugMessageForAdapter(appId, response);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onSystemRequestResponse(String appId, SystemRequestResponse response) {
        createDebugMessageForAdapter(appId, response);

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(
                    new Pair<Integer, Result>(response.getCorrelationID(),
                            response.getResultCode())
            );
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onMobileNaviStart(String appId) {
        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onServiceStart(ServiceType.Mobile_Nav, appId);
        }
    }

    @Override
    public void onAudioServiceStart(String appId) {
        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onServiceStart(ServiceType.Audio_Service, appId);
        }
    }

    @Override
    public void onMobileNavAckReceived(String appId, int frameReceivedNumber) {
        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onAckReceived(appId, frameReceivedNumber, ServiceType.Mobile_Nav);
        }
    }

    @Override
    public void onStartServiceNackReceived(String appId, ServiceType serviceType) {
        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onStartServiceNackReceived(appId, serviceType);
        }
    }

    @Override
    public void onOnTouchEvent(final String appId, OnTouchEvent notification) {
        final OnTouchEvent event = notification;
        createDebugMessageForAdapter(notification);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onTouchEventReceived(appId, event);
            }
        });

    }

    @Override
    public void onKeyboardInput(final String appId, OnKeyboardInput msg) {
        final OnKeyboardInput event = msg;
        createDebugMessageForAdapter(appId, msg);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onKeyboardInputReceived(appId, event);
            }
        });
    }

    @Override
    public void onAppUnregisteredAfterLanguageChange(String appId, OnLanguageChange msg) {
        String message =
                String.format("OnAppInterfaceUnregistered (LANGUAGE_CHANGE) from %s to %s",
                        msg.getLanguage(), msg.getHmiDisplayLanguage());
        createDebugMessageForAdapter(message);
        mSyncProxy.resetLanguagesDesired(appId, msg.getLanguage(), msg.getHmiDisplayLanguage());
    }

    @Override
    public void onAppUnregisteredReason(String appId, AppInterfaceUnregisteredReason reason) {
        createDebugMessageForAdapter("onAppUnregisteredReason:" + reason + ", appId:" + appId);
    }

    @Override
    public void onProtocolServiceEnded(ServiceType serviceType, String appId) {
        String response = " EndService received serType:" + serviceType.getName() +
                ", appId:" + appId;
        createDebugMessageForAdapter(appId, response);

        endProtocolService(appId, serviceType);
    }

    @Override
    public void onProtocolServiceEndedAck(ServiceType serviceType, String appId) {
        String response = " EndServiceAck received serType:" + serviceType.getName() +
                ", appId:" + appId;
        createDebugMessageForAdapter(appId, response);

        endProtocolService(appId, serviceType);
    }

    @Override
    public void onSessionStarted(String appId) {
        Logger.d(TAG, " Session started, appId:" + appId);

        mSessionsCounter.add(appId);

        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onServiceStart(ServiceType.RPC, appId);
        }

        restoreConnectionToRPCService.releaseLock();
    }

    @Override
    public void onOnTBTClientState(OnTBTClientState notification) {
        createDebugMessageForAdapter(notification);
    }

    @Override
    public void onStartSession(String appId) {
        createDebugMessageForAdapter(appId, " Session going to start, " +
                "protocol version: " + syncProxyGetWiProVersion());
    }

    @Override
    public void onRPCRequest(String appId, RPCRequest rpcRequest) {
        createDebugMessageForAdapter(rpcRequest);
    }

    @Override
    public void onOnSystemRequest(String appId, OnSystemRequest notification) {
        createDebugMessageForAdapter(appId, notification);
    }

    /**
     * ******************************
     * * SYNC AppLink Policies Callback's **
     * *******************************
     */
    @Override
    public void onOnPermissionsChange(String appId, OnPermissionsChange notification) {
        createDebugMessageForAdapter(appId, notification);
    }

    EncodedSyncPDataHeader encodedSyncPDataHeaderfromGPS;

    @Override
    public void onOnEncodedSyncPData(OnEncodedSyncPData notification) {
        Logger.i("syncp", "MessageType: " + notification.getMessageType());

        createDebugMessageForAdapter(notification);

        EncodedSyncPDataHeader encodedSyncPDataHeader;
        try {
            encodedSyncPDataHeader = EncodedSyncPDataHeader.parseEncodedSyncPDataHeader(
                    Base64.decode(notification.getData().get(0)));
        } catch (IOException e) {
            Logger.e(TAG + " Can't decode base64 string", e);
            return;
        }

        if (encodedSyncPDataHeader.getServiceType() == 3 && encodedSyncPDataHeader.getCommandType() == 1) {
            saveEncodedSyncPData(encodedSyncPDataHeader.getPayload());

            Logger.i("EncodedSyncPDataHeader", "Protocol Version: " + encodedSyncPDataHeader.getProtocolVersion());
            Logger.i("EncodedSyncPDataHeader", "Response Required: " + encodedSyncPDataHeader.getResponseRequired());
            Logger.i("EncodedSyncPDataHeader", "High Bandwidth: " + encodedSyncPDataHeader.getHighBandwidth());
            Logger.i("EncodedSyncPDataHeader", "Signed: " + encodedSyncPDataHeader.getSigned());
            Logger.i("EncodedSyncPDataHeader", "Encrypted: " + encodedSyncPDataHeader.getEncrypted());
            Logger.i("EncodedSyncPDataHeader", "Payload Size: " + encodedSyncPDataHeader.getPayloadSize());
            Logger.i("EncodedSyncPDataHeader", "Has ESN: " + encodedSyncPDataHeader.getHasESN());
            Logger.i("EncodedSyncPDataHeader", "Service Type: " + encodedSyncPDataHeader.getServiceType());
            Logger.i("EncodedSyncPDataHeader", "Command Type: " + encodedSyncPDataHeader.getCommandType());
            Logger.i("EncodedSyncPDataHeader", "CPU Destination: " + encodedSyncPDataHeader.getCPUDestination());
            Logger.i("EncodedSyncPDataHeader", "Encryption Key Index: " + encodedSyncPDataHeader.getEncryptionKeyIndex());

            byte[] tempESN = encodedSyncPDataHeader.getESN();
            String stringESN = "";
            for (int i = 0; i < 8; i++) stringESN += tempESN[i];
            Logger.i("EncodedSyncPDataHeader", "ESN: " + stringESN);

            try {
                Logger.i("EncodedSyncPDataHeader", "Module Message ID: " + encodedSyncPDataHeader.getModuleMessageID());
            } catch (Exception e) {

            }
            try {
                Logger.i("EncodedSyncPDataHeader", "Server Message ID: " + encodedSyncPDataHeader.getServerMessageID());
            } catch (Exception e) {

            }
            try {
                Logger.i("EncodedSyncPDataHeader", "Message Status: " + encodedSyncPDataHeader.getMessageStatus());
            } catch (Exception e) {

            }

            //create header for syncp packet
            if (encodedSyncPDataHeader.getHighBandwidth()) {
                byte[] tempIV = encodedSyncPDataHeader.getIV();
                String stringIV = "";
                for (int i = 0; i < 16; i++) stringIV += tempIV[i];
                Logger.i("EncodedSyncPDataHeader", "IV: " + stringIV);

                byte[] tempPayload = encodedSyncPDataHeader.getPayload();
                String stringPayload = "";
                for (int i = 0; i < encodedSyncPDataHeader.getPayloadSize(); i++)
                    stringPayload += tempPayload[i];
                Logger.i("EncodedSyncPDataHeader", "Payload: " + stringPayload);

                byte[] tempSignatureTag = encodedSyncPDataHeader.getSignatureTag();
                String stringSignatureTag = "";
                for (int i = 0; i < 16; i++) stringSignatureTag += tempSignatureTag[i];
                Logger.i("EncodedSyncPDataHeader", "Signature Tag: " + stringSignatureTag);
            } else {
                byte[] tempIV = encodedSyncPDataHeader.getIV();
                String stringIV = "";
                for (int i = 0; i < 8; i++) stringIV += tempIV[i];
                Logger.i("EncodedSyncPDataHeader", "IV: " + stringIV);

                byte[] tempPayload = encodedSyncPDataHeader.getPayload();
                String stringPayload = "";
                for (int i = 0; i < encodedSyncPDataHeader.getPayloadSize(); i++)
                    stringPayload += tempPayload[i];
                Logger.i("EncodedSyncPDataHeader", "Payload: " + stringPayload);

                byte[] tempSignatureTag = encodedSyncPDataHeader.getSignatureTag();
                String stringSignatureTag = "";
                for (int i = 0; i < 8; i++) stringSignatureTag += tempSignatureTag[i];
                Logger.i("EncodedSyncPDataHeader", "Signature Tag: " + stringSignatureTag);
            }

            encodedSyncPDataHeaderfromGPS = encodedSyncPDataHeader;
            SyncProxyTester.setESN(tempESN);
            if (PoliciesTesterActivity.getInstance() != null) {
                PoliciesTesterActivity.setESN(tempESN);
                PoliciesTesterActivity.setHeader(encodedSyncPDataHeader);
            }
        }

        if (encodedSyncPDataHeader.getServiceType() == 7) {
            saveEncodedSyncPData(encodedSyncPDataHeader.getPayload());
        }
    }

    private void saveEncodedSyncPData(byte[] data) {
        String filePath = Environment.getExternalStorageDirectory().getPath() + "/policiesResults.txt";
        AppUtils.saveDataToFile(data, filePath);
    }

    @Override
    public IBinder onBind(Intent intent) {
        createInfoMessageForAdapter("Service on bind");
        return mBinder;
    }

    @Override
    public void onRegisterAppInterfaceResponse(String appId, RegisterAppInterfaceResponse response) {
        createDebugMessageForAdapter(appId, response);

        restoreConnectionToRAI.releaseLock();

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }

        try {
            processRegisterAppInterfaceResponse(appId, response);
        } catch (SyncException e) {
            createErrorMessageForAdapter("Can not process RAIResponse:" + e.getMessage());
        }
    }

    @Override
    public void onUnregisterAppInterfaceResponse(String appId,
                                                 UnregisterAppInterfaceResponse response) {
        createDebugMessageForAdapter(appId, response);

        ApplicationIconManager applicationIconManager =
                mApplicationIconManagerHashtable.get(appId);
        if (applicationIconManager != null) {
            applicationIconManager.reset();
        }

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onSyncPDataResponse(String appId, SyncPDataResponse response) {
        createDebugMessageForAdapter(appId, response);

        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(response.getCorrelationID(),
                    response.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }

    @Override
    public void onOnSyncPData(OnSyncPData notification) {
        createDebugMessageForAdapter(notification);
    }

    private void resendUnsentPutFiles(String appId) {
        PutFileTransferManager putFileTransferManager = mPutFileTransferManagers.get(appId);
        if (putFileTransferManager == null) {
            return;
        }
        SparseArray<PutFile> unsentPutFiles = putFileTransferManager.getCopy();
        putFileTransferManager.clear();
        for (int i = 0; i < unsentPutFiles.size(); i++) {
            commandPutFile(appId, unsentPutFiles.valueAt(i));
        }
        unsentPutFiles.clear();
    }

    private void stopServiceBySelf() {
        Logger.i(TAG + " Stop Service By Self");
        stopSelf();
    }

    // TODO : Set command factory in separate place
    /**
     * Commands Section
     */

    /**
     * Create and send ListFiles command
     */

    /**
     * Create and send ListFiles command
     */
    public void commandListFiles(String appId) {
        try {
            mSyncProxy.listFiles(appId, getNextCorrelationID());
        } catch (SyncException e) {
            createErrorMessageForAdapter(appId, "ListFiles send error: " + e);
        }
    }

    /**
     * Create and send PutFile command
     *
     * @param putFile PurFile to be send
     */
    public void commandPutFile(String appId, PutFile putFile) {
        commandPutFile(appId, null, null, null, getNextCorrelationID(), null, putFile);
    }

    /**
     * Create and send PutFile command
     *
     * @param fileType     Type of the File
     * @param syncFileName Name of the File
     * @param bulkData     Data of the File
     */
    public void commandPutFile(String appId, FileType fileType, String syncFileName, byte[] bulkData) {
        commandPutFile(appId, fileType, syncFileName, bulkData, -1, null, null);
    }

    /**
     * Create and send PutFile command
     *
     * @param fileType      Type of the File
     * @param syncFileName  Name of the File
     * @param bulkData      Data of the File
     * @param correlationId Unique identifier of the command
     */
    public void commandPutFile(String appId, FileType fileType, String syncFileName, byte[] bulkData,
                               int correlationId) {
        commandPutFile(appId, fileType, syncFileName, bulkData, correlationId, null, null);
    }

    /**
     * Create and send PutFile command
     *
     * @param fileType        Type of the File
     * @param syncFileName    Name of the File
     * @param bulkData        Data of the File
     * @param correlationId   Unique identifier of the command
     * @param doSetPersistent
     */
    public void commandPutFile(String appId, FileType fileType, String syncFileName, byte[] bulkData,
                               int correlationId, Boolean doSetPersistent) {
        commandPutFile(appId, fileType, syncFileName, bulkData, correlationId, doSetPersistent, null);
    }

    /**
     * Create and send PutFile command
     *
     * @param fileType        Type of the File
     * @param syncFileName    Name of the File
     * @param bulkData        Data of the File
     * @param correlationId   Unique identifier of the command
     * @param doSetPersistent
     * @param putFile         PurFile to be send
     */
    public void commandPutFile(String appId, FileType fileType, String syncFileName,
                               byte[] bulkData, int correlationId,
                               Boolean doSetPersistent, PutFile putFile) {
        commandPutFile(appId, fileType, syncFileName, bulkData, correlationId,
                doSetPersistent, null, null, null, putFile);
    }

    /**
     * Create and send PutFile command
     *
     * @param fileType        Type of the File
     * @param syncFileName    Name of the File
     * @param bulkData        Data of the File
     * @param correlationId   Unique identifier of the command
     * @param doSetPersistent
     * @param isSystemFile
     * @param length
     * @param offset
     * @param putFile         PurFile to be send
     */
    public void commandPutFile(String appId, FileType fileType, String syncFileName,
                               byte[] bulkData, int correlationId,
                               Boolean doSetPersistent, Boolean isSystemFile,
                               Integer length, Integer offset, PutFile putFile) {
        int mCorrelationId = correlationId;
        if (correlationId == -1) {
            mCorrelationId = getNextCorrelationID();
        }

        PutFile newPutFile = RPCRequestFactory.buildPutFile();

        if (putFile == null) {
            newPutFile.setFileType(fileType);
            newPutFile.setSyncFileName(syncFileName);
            if (doSetPersistent != null) {
                newPutFile.setPersistentFile(doSetPersistent);
            }

            if (isSystemFile != null) {
                newPutFile.setSystemFile(isSystemFile);
            }

            if (length != null) {
                newPutFile.setLength(length);
            }

            if (offset != null) {
                newPutFile.setOffset(offset);
            }

            newPutFile.setBulkData(bulkData);
        } else {
            newPutFile = putFile;
        }

        newPutFile.setCorrelationID(mCorrelationId);

        PutFileTransferManager putFileTransferManager = mPutFileTransferManagers.get(appId);
        if (putFileTransferManager == null) {
            putFileTransferManager = new PutFileTransferManager();
            mPutFileTransferManagers.put(appId, putFileTransferManager);
        }
        putFileTransferManager.addPutFileToAwaitArray(mCorrelationId, newPutFile);

        syncProxySendPutFilesResumable(appId, newPutFile);
    }

    /**
     * Call a method from SDK to send <b>SubscribeButton</b> request
     *
     * @param buttonName {@link com.ford.syncV4.proxy.rpc.enums.ButtonName}
     */
    public void commandSubscribeButtonPredefined(String appId, ButtonName buttonName,
                                                 int correlationId) {
        SubscribeButton subscribeButton = RPCRequestFactory.buildSubscribeButton();
        subscribeButton.setCorrelationID(correlationId);
        subscribeButton.setButtonName(buttonName);

        sendRPCRequestWithPreprocess(appId, subscribeButton);
    }

    /**
     * Call a method from SDK to send <b>SubscribeButton</b> request which will be used in application
     * resumption.
     *
     * @param correlationId Unique identifier of the command
     * @param buttonName    {@link com.ford.syncV4.proxy.rpc.enums.ButtonName}
     */
    public void commandSubscribeButtonResumable(String appId, ButtonName buttonName,
                                                int correlationId) {
        SubscribeButton subscribeButton = RPCRequestFactory.buildSubscribeButton();
        subscribeButton.setCorrelationID(correlationId);
        subscribeButton.setButtonName(buttonName);

        syncProxySendRPCRequestResumable(appId, subscribeButton);
    }

    /**
     * Call a method from SDK to send <b>UnsubscribeVehicleData</b> request.
     *
     * @param unsubscribeVehicleData {@link com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData}
     */
    public void commandUnsubscribeVehicleInterface(String appId,
                                                   UnsubscribeVehicleData unsubscribeVehicleData) {
        sendRPCRequestWithPreprocess(appId, unsubscribeVehicleData);
    }

    /**
     * Call a method from SDK to send <b>SubscribeVehicleData</b> request which will be used in
     * application resumption.
     *
     * @param subscribeVehicleData {@link com.ford.syncV4.proxy.rpc.SubscribeVehicleData}
     */
    public void commandSubscribeVehicleInterfaceResumable(String appId,
                                                          SubscribeVehicleData subscribeVehicleData) {
        syncProxySendRPCRequestResumable(appId, subscribeVehicleData);
    }

    /**
     * Call a method from SDK to send <b>AddCommand</b> request which will be used in application
     * resumption.
     *
     * @param commandId  Id of the command
     * @param vrCommands Vector of the VR Commands
     * @param menuName   Name of the Menu
     */
    public void commandAddCommandResumable(String appId, Integer commandId, Vector<String> vrCommands,
                                           String menuName) {
        AddCommand addCommand = RPCRequestFactory.buildAddCommand(commandId, menuName, vrCommands,
                getNextCorrelationID());
        syncProxySendRPCRequestResumable(appId, addCommand);
    }

    /**
     * Call a method from SDK to send <b>AddCommand</b> request
     *
     * @param commandId  Id of the command
     * @param vrCommands Vector of the VR Commands
     * @param menuName   Name of the Menu
     */
    public void commandAddCommandPredefined(String appId, Integer commandId,
                                            Vector<String> vrCommands, String menuName) {
        AddCommand addCommand = RPCRequestFactory.buildAddCommand(commandId, menuName, vrCommands,
                getNextCorrelationID());
        sendRPCRequestWithPreprocess(appId, addCommand);
    }

    /**
     * Call a method from SDK to send <b>AddCommand</b> request which will be used in application
     * resumption.
     *
     * @param addCommand {@link com.ford.syncV4.proxy.rpc.AddCommand} object
     */
    public void commandAddCommandResumable(String appId, AddCommand addCommand) {
        syncProxySendRPCRequestResumable(appId, addCommand);
    }

    /**
     * Call a method from SDK to send <b>AddSubMenu</b> request which will be used in application
     * resumption.
     *
     * @param setGlobalProperties {@link com.ford.syncV4.proxy.rpc.SetGlobalProperties}
     */
    public void commandSetGlobalPropertiesResumable(String appId,
                                                    SetGlobalProperties setGlobalProperties) {
        syncProxySendRPCRequestResumable(appId, setGlobalProperties);
    }

    /**
     * Call a method from SDK to send <b>AddSubMenu</b> request which will be used in application
     * resumption.
     *
     * @param addSubMenu {@link com.ford.syncV4.proxy.rpc.AddSubMenu} object
     */
    public void commandAddSubMenuResumable(String appId, AddSubMenu addSubMenu) {
        syncProxySendRPCRequestResumable(appId, addSubMenu);
    }

    /**
     * Call a method from SDK to create and send <b>CreateInteractionChoiceSet</b> request which
     * will be used in application resumption.
     *
     * @param choiceSet              Set of the {@link com.ford.syncV4.proxy.rpc.Choice} objects
     * @param interactionChoiceSetID Id of the interaction Choice set
     * @param correlationID          correlation Id
     */
    public void commandCreateInteractionChoiceSetResumable(String appId, Vector<Choice> choiceSet,
                                                           Integer interactionChoiceSetID,
                                                           Integer correlationID) {

        CreateInteractionChoiceSet createInteractionChoiceSet =
                RPCRequestFactory.buildCreateInteractionChoiceSet(choiceSet,
                        interactionChoiceSetID, correlationID);
        syncProxySendRPCRequestResumable(appId, createInteractionChoiceSet);
    }

    /**
     * SyncProxy section, transfer call methods from Application to SyncProxy
     */

    public void syncProxyStopAudioDataTransfer() {
        if (mSyncProxy != null) {
            mSyncProxy.stopAudioDataTransfer();
        }
    }

    public void syncProxyStopH264() {
        if (mSyncProxy != null) {
            mSyncProxy.stopH264();
        }
    }

    /**
     * // TODO : For Tests Only
     * @param appId
     * @throws SyncException
     */
    public void syncProxyCloseSession(String appId) throws SyncException {
        if (mSyncProxy != null) {
            mSyncProxy.doUnregisterAppInterface(appId);
        }
    }

    /**
     * // TODO : For Tests Only
     * @param syncAppId
     * @throws SyncException
     */
    public void syncProxyOpenSession(String syncAppId) throws SyncException {
        if (mSyncProxy != null) {

            SharedPreferences settings = getSharedPreferences(Const.PREFS_NAME, 0);
            String appName = settings.getString(Const.PREFS_KEY_APPNAME,
                    Const.PREFS_DEFAULT_APPNAME);
            boolean isMediaApp = settings.getBoolean(
                    Const.PREFS_KEY_ISMEDIAAPP, Const.PREFS_DEFAULT_ISMEDIAAPP);
            boolean isNaviApp = settings.getBoolean(
                    Const.PREFS_KEY_ISNAVIAPP, Const.PREFS_DEFAULT_ISNAVIAPP);
            Language lang = Language.valueOf(settings.getString(
                    Const.PREFS_KEY_LANG, Const.PREFS_DEFAULT_LANG));
            Language hmiLang = Language.valueOf(settings.getString(
                    Const.PREFS_KEY_HMILANG, Const.PREFS_DEFAULT_HMILANG));
            // Apply custom AppId in case of such possibility selected
            TransportType transportType = AppPreferencesManager.getTransportType();
            String appId = AppIdManager.getAppIdByTransport(transportType);
            if (AppPreferencesManager.getIsCustomAppId()) {
                appId = AppPreferencesManager.getCustomAppId();
            }
            Vector<AppHMIType> appHMITypes = createAppTypeVector(isNaviApp);

            RegisterAppInterface registerAppInterface = registerAppInterfaceHashMap.get(syncAppId);
            Logger.d(TAG + " Open Session, appId:" + syncAppId + " RAI:" + registerAppInterface);
            if (registerAppInterface == null) {
                registerAppInterface = RPCRequestFactory.buildRegisterAppInterface();
                registerAppInterfaceHashMap.put(syncAppId, registerAppInterface);
            }
            registerAppInterface.setAppName(appName);
            registerAppInterface.setLanguageDesired(lang);
            registerAppInterface.setHmiDisplayLanguageDesired(hmiLang);
            registerAppInterface.setAppId(appId);
            registerAppInterface.setIsMediaApplication(isMediaApp);
            registerAppInterface.setAppType(appHMITypes);

            SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
            syncMsgVersion.setMajorVersion(2);
            syncMsgVersion.setMinorVersion(2);
            registerAppInterface.setSyncMsgVersion(syncMsgVersion);

            mSyncProxy.updateRegisterAppInterfaceParameters(registerAppInterface);

            if (mSyncProxy.getIsConnected()) {
                mSyncProxy.initializeSession(syncAppId);
            } else {
                mSyncProxy.initializeProxy();
            }
        } else {
            Logger.w(TAG + " OpenSession, proxy NULL");
        }
    }

    public void syncProxyStartAudioService(String appId) {
        if (mSyncProxy != null) {
            mSyncProxy.startAudioService(appId);
        }
    }

    public void syncProxyStopAudioService(String appId) {
        if (mSyncProxy != null) {
            mSyncProxy.stopAudioService(appId);
        }
    }

    public OutputStream syncProxyStartAudioDataTransfer(String appId) {
        if (mSyncProxy != null) {
            return mSyncProxy.startAudioDataTransfer(appId);
        }
        return null;
    }

    /**
     * Invalidates provided Application Id, clear all Services associated and remove it from the list
     *
     * @param appId Application id
     */
    public void invalidateAppId(String appId) {
        mSyncProxy.invalidateAppId(appId);
    }

    /**
     * Invalidates all previous {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} requests
     */
    public void invalidateAllRAIs() {
        mSyncProxy.invalidateAllRAIs();
    }

    /**
     * This method is send RPC Request to the Sync Proxy
     *
     * @param appId   Application id
     * @param request object of {@link com.ford.syncV4.proxy.RPCRequest} type
     */
    public void sendRPCRequestWithPreprocess(String appId, RPCRequest request) {
        sendRPCRequestWithPreprocess(appId, request, mSyncProxy.getJsonRPCMarshaller(), false);
    }

    /**
     * This method is send RPC Request to the Sync Proxy
     *
     * @param appId      Application id
     * @param request    Object of {@link com.ford.syncV4.proxy.RPCRequest} type
     * @param sendAsItIs This parameter is <b>for the
     *                   {@link com.ford.syncV4.android.module.ModuleTest}</b> only and indicates
     *                   that {@link com.ford.syncV4.proxy.RPCRequest} should be send as it is,
     *                   without further modification (as it were formed in XML test)
     */
    public void sendRPCRequestWithPreprocess(String appId, RPCRequest request,
                                             IJsonRPCMarshaller jsonRPCMarshaller,
                                             boolean sendAsItIs) {
        if (request == null) {
            createErrorMessageForAdapter(appId, " RPC request is NULL");
            return;
        }
        try {
            if (request.getFunctionName().equals(Names.RegisterAppInterface)) {
                sendRegisterRequest((RegisterAppInterface) request, jsonRPCMarshaller, sendAsItIs);
            } else {
                mSyncProxy.sendRPCRequest(appId, request, jsonRPCMarshaller);
            }
        } catch (SyncException e) {
            e.printStackTrace();
            createErrorMessageForAdapter(appId, " RPC request '" + request.getFunctionName() + "'" +
                    " send error");
        }
    }

    /**
     * This method is send RPC Request to the Sync Proxy
     *
     * @param appId      Application id
     * @param request    Object of {@link com.ford.syncV4.proxy.RPCRequest} type
     */
    public void syncProxySendRPCRequest(String appId, RPCRequest request) {
        if (request == null) {
            createErrorMessageForAdapter("RPC request is NULL");
            return;
        }
        try {
            mSyncProxy.sendRPCRequest(appId, request);
        } catch (SyncException e) {
            createErrorMessageForAdapter("RPC request '" + request.getFunctionName() + "'" +
                    " send error");
        }
    }

    /**
     * This method is for the requests on which resumption is depends on. All the requests will be
     * stored in the collection in order to re-use them when resumption will have place.
     *
     * @param request {@link com.ford.syncV4.proxy.RPCRequest} object
     */
    public void syncProxySendRPCRequestResumable(String appId, RPCRequest request) {
        if (request == null) {
            createErrorMessageForAdapter("Resumable RPC request is NULL");
            return;
        }

        RPCRequestsResumableManager rpcRequestsResumableManager =
                mRpcRequestsResumableManager.get(appId);
        if (rpcRequestsResumableManager == null) {
            rpcRequestsResumableManager = new RPCRequestsResumableManager(appId);
            rpcRequestsResumableManager.setCallback(
                    new RPCRequestsResumableManager.RPCRequestsResumableManagerCallback() {
                @Override
                public void onSendRequest(String appId, RPCRequest request) {
                    sendRPCRequestWithPreprocess(appId, request);
                }
            });
            mRpcRequestsResumableManager.put(appId, rpcRequestsResumableManager);
        }

        if (mSyncProxy.getIsConnected()) {
            rpcRequestsResumableManager.addRequestConnected(request);
        } else {
            rpcRequestsResumableManager.addRequestDisconnected(request);
        }

        sendRPCRequestWithPreprocess(appId, request);
    }

    /**
     * @param putFile
     */
    public void syncProxySendPutFilesResumable(String appId, PutFile putFile) {
        if (putFile == null) {
            createErrorMessageForAdapter("Resumable PuFile is NULL");
            return;
        }

        //mRpcRequestsResumableManager.addPutFile(putFile);

        sendRPCRequestWithPreprocess(appId, putFile);
    }

    /**
     * Sends {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} request to the SYNC proxy
     *
     * @param registerAppInterface {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} request
     *
     * @throws SyncException
     */
    private void sendRegisterRequest(RegisterAppInterface registerAppInterface) throws SyncException {
        sendRegisterRequest(registerAppInterface, new JsonRPCMarshaller(), false);
    }

    /**
     * Sends {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} request to the SYNC proxy
     *
     * @param registerAppInterface {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} request
     * @param sendAsItIs           This parameter is <b>for the
     *                             {@link com.ford.syncV4.android.module.ModuleTest}</b> only and
     *                             indicates that {@link com.ford.syncV4.proxy.RPCRequest} should be
     *                             send as it is, without further modification
     *                             (as it were formed in XML test)
     *
     * @throws SyncException
     */
    private void sendRegisterRequest(RegisterAppInterface registerAppInterface,
                                     IJsonRPCMarshaller jsonRPCMarshaller, boolean sendAsItIs)
            throws SyncException {

        String appId = String.valueOf(registerAppInterface.getAppId());
        if (mModuleTest != null) {
            mModuleTest.setAppId(appId);
        }
        AppPreferencesManager.setCustomAppId(appId);

        boolean isSyncProxyNull = mSyncProxy == null;
        if (isSyncProxyNull) {
            // Assume that Sync Poxy has been destroyed, initialize new one

            startProxyIfNetworkConnected();

            Logger.d("Re-start proxy on RAI, marshaller:" + jsonRPCMarshaller);
            mSyncProxy.setJsonRPCMarshaller(jsonRPCMarshaller);

            // Assume that at this point a new instance of the SyncProxy is created

            // TODO it's seems stupid in order to register send onTransportConnected
            mSyncProxy.updateRegisterAppInterfaceParameters(registerAppInterface, sendAsItIs);
        } else {
            if (mSyncProxy.getSyncConnection() == null) {
                return;
            }

            mSyncProxy.setJsonRPCMarshaller(jsonRPCMarshaller);

            // TODO it's seems stupid in order to register send onTransportConnected
            mSyncProxy.updateRegisterAppInterfaceParameters(registerAppInterface, sendAsItIs);

            boolean hasRPCRunning = hasRPCRunning(appId);
            Logger.d("Send RegisterRequest, appId:" + appId + ", hasRPC:" + hasRPCRunning);
            if (hasRPCRunning) {
                // In case of XML Test is running and the next tag is in use:
                // <action actionName="startRPCService" pause="2000"/>
                // what is needed is only to run RPC Request
                mSyncProxy.sendRPCRequest(appId, registerAppInterface);
            } else {
                //mSyncProxy.sendRPCRequest(appId, registerAppInterface);
                mSyncProxy.getSyncConnection().onTransportConnected();
            }
        }
    }

    public byte syncProxyGetWiProVersion() {
        if (mSyncProxy != null && mSyncProxy.getSyncConnection() != null) {
            return mSyncProxy.getSyncConnection().getProtocolVersion();
        }
        return ProtocolConstants.PROTOCOL_VERSION_UNDEFINED;
    }

    public void syncProxyStopMobileNaviService(String appId) {
        if (mSyncProxy != null) {
            mSyncProxy.stopMobileNaviService(appId);
        }
    }

    public void syncProxyStartMobileNavService(String appId) {
        if (mSyncProxy != null && mSyncProxy.getSyncConnection() != null) {
            mSyncProxy.startMobileNavService(appId);
        }
    }

    public OutputStream syncProxyStartH264(String appId) {
        if (mSyncProxy != null) {
            return mSyncProxy.startH264(appId);
        }
        return null;
    }

    public void syncProxySetJsonRPCMarshaller(IJsonRPCMarshaller jsonRPCMarshaller) {
        if (mSyncProxy != null) {
            mSyncProxy.setJsonRPCMarshaller(jsonRPCMarshaller);
        } else {
            Logger.w(TAG + " set JSON Marshaller, proxy is null");
        }
    }

    private void setAppIcon(String appId) {
        SetAppIcon setAppIcon = RPCRequestFactory.buildSetAppIcon();
        setAppIcon.setSyncFileName(ICON_SYNC_FILENAME);
        setAppIcon.setCorrelationID(getNextCorrelationID());

        sendRPCRequestWithPreprocess(appId, setAppIcon);
    }

    private void endProtocolService(String appId, ServiceType serviceType) {
        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onServiceEnd(serviceType);
        }
        if (serviceType == ServiceType.RPC) {
            createDebugMessageForAdapter(appId, " RPC service stopped");

            if (registerAppInterfaceHashMap.containsKey(appId)) {
                RegisterAppInterface result = registerAppInterfaceHashMap.remove(appId);
                Logger.d("RAI object has been removed:" + result);
            }

            if (mProxyServiceEvent != null) {
                mSessionsCounter.remove(appId);
                Logger.d("End Protocol Service:" + mSessionsCounter.size());
                if (mSessionsCounter.size() == 0) {
                    mProxyServiceEvent.onDisposeComplete();
                }
            }

            if (mCloseSessionCallback != null) {
                mCloseSessionCallback.onCloseSessionComplete();
            }
        }
    }

    /**
     * Process a response of the {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} request
     *
     * @param response {@link com.ford.syncV4.proxy.rpc.RegisterAppInterfaceResponse} object
     */
    private void processRegisterAppInterfaceResponse(String appId,
                                                     RegisterAppInterfaceResponse response)
            throws SyncException {

        if (!response.getSuccess()) {
            return;
        }

        RPCRequestsResumableManager rpcRequestsResumableManager =
                mRpcRequestsResumableManager.get(appId);
        if (rpcRequestsResumableManager != null) {
            if (response.getResultCode() == Result.SUCCESS) {
                //rpcRequestsResumableManager.sendAllPutFiles();
                rpcRequestsResumableManager.sendAllRequestsDisconnected();
            } else if (response.getResultCode() == Result.RESUME_FAILED) {
                //rpcRequestsResumableManager.sendAllPutFiles();
                rpcRequestsResumableManager.sendAllRequestsConnected();
                rpcRequestsResumableManager.sendAllRequestsDisconnected();
            }

            //rpcRequestsResumableManager.cleanAllPutFiles();
            rpcRequestsResumableManager.cleanAllRequestsConnected();
            rpcRequestsResumableManager.cleanAllRequestsDisconnected();
        }

        // Restore a PutFile which has not been sent
        resendUnsentPutFiles(appId);
        // Restore Services
        mSyncProxy.restoreServices(appId);
    }

    // TODO: Reconsider this section, this is a first step to optimize log procedure

    /**
     * Logger section. Send log message to adapter and log it to the ADB
     */

    private void createErrorMessageForAdapter(String appId, Object messageObject) {
        createErrorMessageForAdapter(appId, messageObject, null);
    }

    private void createErrorMessageForAdapter(Object messageObject) {
        createErrorMessageForAdapter(messageObject, null);
    }

    private void createErrorMessageForAdapter(Object messageObject, Throwable throwable) {
        createErrorMessageForAdapter(PlaceholderFragment.EMPTY_APP_ID, messageObject, throwable);
    }

    private void createErrorMessageForAdapter(String appId, Object messageObject, Throwable throwable) {
        if (mLogAdapters.isEmpty()) {
            Logger.w(TAG, "LogAdapters are empty, appId:" + appId);
            if (throwable != null) {
                Logger.e(TAG + " " + messageObject.toString(), throwable);
            } else {
                Logger.e(TAG, messageObject.toString());
            }
            return;
        }
        for (LogAdapter logAdapter : mLogAdapters) {
            if (appId.equals(PlaceholderFragment.EMPTY_APP_ID)) {
                if (throwable != null) {
                    logAdapter.logMessage(messageObject, Log.ERROR, throwable, true);
                } else {
                    logAdapter.logMessage(messageObject, Log.ERROR, true);
                }
            } else {
                if (logAdapter.getAppId().equals(appId)) {
                    if (throwable != null) {
                        logAdapter.logMessage(messageObject, Log.ERROR, throwable, true);
                    } else {
                        logAdapter.logMessage(messageObject, Log.ERROR, true);
                    }
                }
            }
        }
    }

    private void createInfoMessageForAdapter(Object messageObject) {
        createMessageForAdapter(PlaceholderFragment.EMPTY_APP_ID, messageObject, Log.INFO);
    }

    private void createDebugMessageForAdapter(String appId, Object messageObject) {
        createMessageForAdapter(appId, messageObject, Log.DEBUG);
    }

    private void createDebugMessageForAdapter(Object messageObject) {
        createMessageForAdapter(PlaceholderFragment.EMPTY_APP_ID, messageObject, Log.DEBUG);
    }

    private void createMessageForAdapter(Object messageObject, Integer type) {
        createMessageForAdapter(PlaceholderFragment.EMPTY_APP_ID, messageObject, type);
    }

    private void createMessageForAdapter(String appId, Object messageObject, Integer type) {
        if (mLogAdapters.isEmpty()) {
            Logger.w(TAG, "LogAdapters are empty, appId:" + appId);
            if (type == Log.DEBUG) {
                Logger.d(TAG, messageObject.toString());
            } else if (type == Log.INFO) {
                Logger.i(TAG, messageObject.toString());
            }
            return;
        }
        for (LogAdapter logAdapter : mLogAdapters) {
            if (appId.equals(PlaceholderFragment.EMPTY_APP_ID)) {
                logAdapter.logMessage(messageObject, type, true);
            } else {
                if (logAdapter.getAppId().equals(appId)) {
                    logAdapter.logMessage(messageObject, type, true);
                }
            }
        }
    }

    private LogAdapter getLogAdapterByAppId(String appId) {
        for (LogAdapter logAdapter : mLogAdapters) {
            if (logAdapter.getAppId() == null) {
                continue;
            }
            if (logAdapter.getAppId().equals(appId)) {
                return logAdapter;
            }
        }
        return null;
    }

    @Override
    public void onUSBNoSuchDeviceException() {
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        if (mainActivity != null) {
            mainActivity.onUSBNoSuchDeviceException();
        }
    }

    @Override
    public void onDiagnosticMessageResponse(String appId,
                                            DiagnosticMessageResponse diagnosticMessageResponse) {
        createDebugMessageForAdapter(appId, diagnosticMessageResponse);
        if (isModuleTesting()) {
            ModuleTest.sResponses.add(new Pair<Integer, Result>(
                    diagnosticMessageResponse.getCorrelationID(),
                    diagnosticMessageResponse.getResultCode()));
            synchronized (mModuleTest.getXMLTestThreadContext()) {
                mModuleTest.getXMLTestThreadContext().notify();
            }
        }
    }
}