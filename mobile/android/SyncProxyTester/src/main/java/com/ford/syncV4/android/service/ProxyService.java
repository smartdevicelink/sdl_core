package com.ford.syncV4.android.service;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.media.MediaPlayer;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Environment;
import android.os.IBinder;
import android.util.Log;
import android.util.Pair;
import android.util.SparseArray;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.constants.FlavorConst;
import com.ford.syncV4.android.listener.ConnectionListenersManager;
import com.ford.syncV4.android.manager.AppIdManager;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.manager.LastUsedHashIdsManager;
import com.ford.syncV4.android.manager.PutFileTransferManager;
import com.ford.syncV4.android.manager.RPCRequestsResumableManager;
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
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.constants.Names;
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
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.transport.BTTransportConfig;
import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.transport.usb.USBTransportConfig;
import com.ford.syncV4.util.Base64;
import com.ford.syncV4.util.TestConfig;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.Vector;

public class ProxyService extends Service implements IProxyListenerALMTesting {

    static final String TAG = "SyncProxyTester";

    public static final int HEARTBEAT_INTERVAL = 5000;
    public static final int HEARTBEAT_INTERVAL_MAX = Integer.MAX_VALUE;
    private Integer autoIncCorrId = 1;

    private static final String ICON_SYNC_FILENAME = "icon.png";
    private static final String ICON_FILENAME_SUFFIX = ".png";

    private static final int XML_TEST_COMMAND = 100;
    private static final int POLICIES_TEST_COMMAND = 101;

    private SyncProxyALM mSyncProxy;
    private LogAdapter mLogAdapter;
    private ModuleTest mTesterMain;
    private MediaPlayer mEmbeddedAudioPlayer;
    private Boolean playingAudio = false;
    protected SyncReceiver mediaButtonReceiver;

    private boolean firstHMIStatusChange = true;
    private HMILevel prevHMILevel = HMILevel.HMI_NONE;

    private boolean mWaitingForResponse = false;
    private IProxyServiceEvent mProxyServiceEvent;
    private ICloseSession mCloseSessionCallback;

    private int mAwaitingInitIconResponseCorrelationID;
    private PutFileTransferManager mPutFileTransferManager;
    private ConnectionListenersManager mConnectionListenersManager;
    private final IBinder mBinder = new ProxyServiceBinder(this);
    // This manager provide functionality to process RPC requests which are involved in app resumption
    private RPCRequestsResumableManager mRpcRequestsResumableManager =
            new RPCRequestsResumableManager();
    // This Config object stores all the necessary data for SDK testing
    private TestConfig mTestConfig = new TestConfig();

    @Override
    public void onCreate() {
        super.onCreate();

        createInfoMessageForAdapter("ProxyService.onCreate()");
        Log.i(TAG, ProxyService.class.getSimpleName() + " OnCreate, mSyncProxy:" + mSyncProxy);

        // Init Listener managers (ConnectionListenersManager, etc ...)
        mConnectionListenersManager = new ConnectionListenersManager();

        IntentFilter mediaIntentFilter = new IntentFilter();
        mediaIntentFilter.addAction(Intent.ACTION_MEDIA_BUTTON);

        mediaButtonReceiver = new SyncReceiver();
        registerReceiver(mediaButtonReceiver, mediaIntentFilter);

        //startProxyIfNetworkConnected();

        mPutFileTransferManager = new PutFileTransferManager();

        mRpcRequestsResumableManager.setCallback(new RPCRequestsResumableManager.RPCRequestsResumableManagerCallback() {
            @Override
            public void onSendRequest(RPCRequest request) {
                syncProxySendRPCRequest(request);
            }
        });

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
            Log.d(TAG, "created " + SyncProxyTester.getInstance());
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(TAG, ProxyService.class.getSimpleName() + " OnStartCommand");
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
                        Log.d(TAG, ni.getTypeName());
                        if (ni.isConnected()) {
                            Log.d(TAG,
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
        Log.i(TAG, "ProxyService. Start Proxy If Network Connected");
        boolean doStartProxy = false;
        if (transportType == Const.Transport.KEY_BLUETOOTH) {
            Log.i(TAG, "ProxyService. Transport = Bluetooth.");
            BluetoothAdapter mBtAdapter = BluetoothAdapter.getDefaultAdapter();
            if (mBtAdapter != null) {
                if (mBtAdapter.isEnabled()) {
                    doStartProxy = true;
                }
            }
        } else {
            Log.i(TAG, "ProxyService. Transport = Default.");
            //TODO: This code is commented out for simulator purposes
            /*
            Log.d(TAG, "ProxyService. onStartCommand(). Transport = WiFi.");
			if (hasWiFiConnection() == true) {
				Log.d(TAG, "ProxyService. onStartCommand(). WiFi enabled.");
				startProxy();
			} else {
				Log.w(TAG,
						"ProxyService. onStartCommand(). WiFi is not enabled.");
			}
			*/
            doStartProxy = true;
        }
        if (doStartProxy) {

            // Prepare all necessary data that need to be use in the Tests
            prepareTestConfig();

            boolean result = startProxy();
            Log.i(TAG, ProxyService.class.getSimpleName() + " Proxy complete result:" + result);
            /*if (result) {
                try {
                    mSyncProxy.openSession();
                } catch (SyncException e) {
                    Log.e(TAG, "ProxyService - OpenSession", e);
                }
            }*/
        }
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
        SyncProxyALM.enableDebugTool();

        createInfoMessageForAdapter("ProxyService.startProxy()");
        Log.i(TAG, ProxyService.class.getSimpleName() + " Start Proxy");

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
                String serviceTypeToCypher = settings.getString(
                        Const.PREFS_KEY_CYPHER_SERVICE, Const.PREFS_DEFAULT_CYPHER_SERVICE);
                ServiceType typeToCypher = ServiceType.RPC;
                if ( serviceTypeToCypher.equals(ServiceType.MOBILE_NAV_NAME)){
                    typeToCypher = ServiceType.Mobile_Nav;
                }else if( serviceTypeToCypher.equals(ServiceType.AUDIO_SERVICE_NAME)){
                    typeToCypher = ServiceType.Audio_Service;
                }else if( serviceTypeToCypher.equals(ServiceType.RPC_NAME)){
                    typeToCypher = ServiceType.RPC;
                }

                Language lang = Language.valueOf(settings.getString(
                        Const.PREFS_KEY_LANG, Const.PREFS_DEFAULT_LANG));
                Language hmiLang = Language.valueOf(settings.getString(
                        Const.PREFS_KEY_HMILANG, Const.PREFS_DEFAULT_HMILANG));
                Log.i(TAG, "Using protocol version " + versionNumber);
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
                BaseTransportConfig config = null;
                TransportType transportType = AppPreferencesManager.getTransportType();
                String appID = AppIdManager.getAppIdByTransport(transportType);
                switch (transportType) {
                    case BLUETOOTH:
                        config = new BTTransportConfig();
                        break;
                    case TCP:
                        config = new TCPTransportConfig(tcpPort, ipAddress);
                        ((TCPTransportConfig) config).setIsNSD(mIsNSD);
                        ((TCPTransportConfig) config).setApplicationContext(this);
                        break;
                    case USB:
                        config = new USBTransportConfig(getApplicationContext());
                        break;
                }

                // Apply custom AppId in case of such possibility selected
                if (AppPreferencesManager.getIsCustomAppId()) {
                    appID = AppPreferencesManager.getCustomAppId();
                }


                SyncProxyALM.setServiceToCypher(typeToCypher);
                mSyncProxy = new SyncProxyALM(this,
                        /*sync proxy configuration resources*/null,
                        /*enable advanced lifecycle management true,*/
                        appName,
                        /*ngn media app*/null,
                        /*vr synonyms*/null,
                        /*is media app*/isMediaApp, appHMITypes,
                        syncMsgVersion,
                        /*language desired*/lang,
                        /*HMI Display Language Desired*/hmiLang,
                        appID,
                        /*autoActivateID*/null,
                        /*callbackToUIThre1ad*/ false,
                        /*preRegister*/ false,
                        versionNumber,
                        config, mTestConfig);

            } catch (SyncException e) {
                Log.e(TAG, e.toString());
                //error creating proxy, returned proxy = null
                if (mSyncProxy == null) {
                    stopServiceBySelf();
                    return false;
                }
            }
        }

        OnSystemRequestHandler mOnSystemRequestHandler = new OnSystemRequestHandler(mLogAdapter);

        mSyncProxy.setOnSystemRequestHandler(mOnSystemRequestHandler);

        createInfoMessageForAdapter("ProxyService.startProxy() complete");
        Log.i(TAG, ProxyService.class.getSimpleName() + " Start Proxy complete:" + mSyncProxy);

        return mSyncProxy != null && mSyncProxy.getIsConnected();
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
        return Const.PROTOCOL_VERSION_2;
    }

    private boolean getAutoSetAppIconFlag() {
        return getSharedPreferences(Const.PREFS_NAME, 0).getBoolean(
                Const.PREFS_KEY_AUTOSETAPPICON,
                Const.PREFS_DEFAULT_AUTOSETAPPICON);
    }

    @Override
    public void onDestroy() {
        createInfoMessageForAdapter("ProxyService.onDestroy()");

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

    public void sendPolicyTableUpdate() {
        PolicyFilesManager.sendPolicyTableUpdate(mSyncProxy, mLogAdapter);
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
        createInfoMessageForAdapter("ProxyService.disposeSyncProxy()");

        MainApp.getInstance().getLastUsedHashIdsManager().save();

        if (mSyncProxy != null) {
            try {
                mSyncProxy.dispose();
            } catch (SyncException e) {
                Log.e(TAG, e.toString());
                if (mProxyServiceEvent != null) {
                    mProxyServiceEvent.onDisposeError();
                }
            }
            mSyncProxy = null;
        }
    }

    public int getServicesNumber() {
        return mSyncProxy.getServicesNumber();
    }

    public boolean hasServiceInServicesPool(ServiceType serviceType) {
        return mSyncProxy != null && mSyncProxy.hasServiceInServicesPool(serviceType);
    }

    private void initialize() {
        Log.d(TAG, "Initialize predefined view");
        playingAudio = true;
        playAnnoyingRepetitiveAudio();

        try {
            show("Sync Proxy", "Tester");
        } catch (SyncException e) {
            createErrorMessageForAdapter("Error sending show", e);
        }

        commandSubscribeButtonPredefined(ButtonName.OK, getNextCorrelationID());
        commandSubscribeButtonPredefined(ButtonName.SEEKLEFT, getNextCorrelationID());
        commandSubscribeButtonPredefined(ButtonName.SEEKRIGHT, getNextCorrelationID());
        commandSubscribeButtonPredefined(ButtonName.TUNEUP, getNextCorrelationID());
        commandSubscribeButtonPredefined(ButtonName.TUNEDOWN, getNextCorrelationID());

        Vector<ButtonName> buttons = new Vector<ButtonName>(Arrays.asList(new ButtonName[]{
                ButtonName.OK, ButtonName.SEEKLEFT, ButtonName.SEEKRIGHT, ButtonName.TUNEUP,
                ButtonName.TUNEDOWN}));
        SyncProxyTester.getInstance().buttonsSubscribed(buttons);

        commandAddCommandPredefined(XML_TEST_COMMAND, new Vector<String>(Arrays.asList(new String[]{"XML Test", "XML"})), "XML Test");
        commandAddCommandPredefined(POLICIES_TEST_COMMAND, new Vector<String>(Arrays.asList(new String[]{"Policies Test", "Policies"})), "Policies Test");
    }

    private void sendPutFileForAppIcon() {
        Log.d(TAG, "PutFileForAppIcon");
        mAwaitingInitIconResponseCorrelationID = getNextCorrelationID();
        commandPutFile(FileType.GRAPHIC_PNG, ICON_SYNC_FILENAME, AppUtils.contentsOfResource(R.raw.fiesta),
                mAwaitingInitIconResponseCorrelationID, true);
    }

    private void show(String mainField1, String mainField2) throws SyncException {
        Show msg = new Show();
        msg.setCorrelationID(getNextCorrelationID());
        msg.setMainField1(mainField1);
        msg.setMainField2(mainField2);
        if (mLogAdapter != null) {
            mLogAdapter.logMessage(msg, true);
        }
        mSyncProxy.sendRPCRequest(msg);
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

    public void startModuleTest() {
        mTesterMain = new ModuleTest(this, mLogAdapter);
    }

    public void waiting(boolean waiting) {
        mWaitingForResponse = waiting;
    }

    public void setLogAdapter(LogAdapter logAdapter) {
        // TODO : Reconsider. Implement log message dispatching instead
        mLogAdapter = logAdapter;
    }

    protected int getNextCorrelationID() {
        return autoIncCorrId++;
    }

    @Override
    public void onOnHMIStatus(OnHMIStatus notification) {
        createDebugMessageForAdapter(notification);
        //createDebugMessageForAdapter("HMI level:" + notification.getHmiLevel());

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
                if (playingAudio) playAnnoyingRepetitiveAudio();
                break;
            case NOT_AUDIBLE:
                pauseAnnoyingRepetitiveAudio();
                break;
            default:
                return;
        }

        final HMILevel curHMILevel = notification.getHmiLevel();
        final Boolean appInterfaceRegistered = mSyncProxy.getAppInterfaceRegistered();

        if ((HMILevel.HMI_NONE == curHMILevel) && appInterfaceRegistered && firstHMIStatusChange) {
            if (!isModuleTesting()) {
                // Process an init state of the predefined requests here, assume that if
                // hashId is not null means this is resumption
                if (mSyncProxy.getHashId() == null) {
                    sendPutFileForAppIcon();
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
                        mTesterMain = new ModuleTest(this, mLogAdapter);
                        //mTesterMain = ModuleTest.getModuleTestInstance();

                        // Process an init state of the predefined requests here, assume that if
                        // hashId is not null means this is resumption
                        if (mSyncProxy.getHashId() == null) {
                            initialize();
                        } else {
                            setAppIcon();
                        }
                    } else {
                        try {
                            if (mTesterMain != null && !mWaitingForResponse && mTesterMain.getThreadContext() != null) {
                                show("Sync Proxy", "Tester Ready");
                            }
                        } catch (SyncException e) {
                            if (mLogAdapter == null)
                                Log.w(TAG, "LogAdapter is null");
                            if (mLogAdapter != null)
                                mLogAdapter.logMessage("Error sending show", Log.ERROR, e, true);
                            else Log.e(TAG, "Error sending show", e);
                        }
                    }
                }

                if (hmiChange && firstHMIStatusChange) {
                    firstHMIStatusChange = false;

                    // Process an init state of the predefined requests here, assume that if
                    // hashId is not null means this is resumption
                    if (mSyncProxy.getHashId() == null) {
                        try {
                            // upload turn icons
                            sendIconFromResource(R.drawable.turn_left);
                            sendIconFromResource(R.drawable.turn_right);
                            sendIconFromResource(R.drawable.turn_forward);
                            sendIconFromResource(R.drawable.action);
                        } catch (SyncException e) {
                            Log.w(TAG, "Failed to put images", e);
                        }
                    }
                }
            }
        }
    }

    @Override
    public void onHashChange(OnHashChange onHashChange) {
        createDebugMessageForAdapter(onHashChange);

        LastUsedHashIdsManager lastUsedHashIdsManager = MainApp.getInstance().getLastUsedHashIdsManager();
        lastUsedHashIdsManager.addNewId(onHashChange.getHashID());
    }

    /**
     * Checks and returns if the module testing is in progress.
     *
     * @return true if the module testing is in progress
     */
    private boolean isModuleTesting() {
        return mWaitingForResponse && mTesterMain.getThreadContext() != null;
    }

    private void sendIconFromResource(int resource) throws SyncException {
        commandPutFile(FileType.GRAPHIC_PNG,
                getResources().getResourceEntryName(resource) + ICON_FILENAME_SUFFIX,
                AppUtils.contentsOfResource(resource), getNextCorrelationID(), true);
    }

    @Override
    public void onOnCommand(OnCommand notification) {
        createDebugMessageForAdapter(notification);
        switch (notification.getCmdID()) {
            case XML_TEST_COMMAND:
                mTesterMain.restart(null);
                break;
            case POLICIES_TEST_COMMAND:
                PoliciesTest.runPoliciesTest(this, mLogAdapter);
                break;
            default:
                break;
        }
    }

    @Override
    public void onProxyClosed(final String info, Exception e) {
        if (e != null) {
            createErrorMessageForAdapter("OnProxyClosed:" + info + ", msg:" + e.getMessage());
        } else {
            createErrorMessageForAdapter("OnProxyClosed:" + info);
        }
        boolean wasConnected = !firstHMIStatusChange;
        firstHMIStatusChange = true;
        prevHMILevel = HMILevel.HMI_NONE;

        if (wasConnected) {
            mConnectionListenersManager.dispatch();
        }

        if (!isModuleTesting()) {
            if (e == null) {
                return;
            }
            if (e instanceof SyncException) {
                final SyncExceptionCause cause = ((SyncException) e).getSyncExceptionCause();
                if ((cause != SyncExceptionCause.SYNC_PROXY_CYCLED) &&
                        (cause != SyncExceptionCause.BLUETOOTH_DISABLED) &&
                        (cause != SyncExceptionCause.SYNC_REGISTRATION_ERROR)) {
                    reset();
                }
            }
            /*if ((SyncExceptionCause.SYNC_PROXY_CYCLED != cause) && mLogAdapter != null) {
                mLogAdapter.logMessage("onProxyClosed: " + info, Log.ERROR, e, true);
            }*/
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
            e1.printStackTrace();
            //something goes wrong, & the proxy returns as null, stop the service.
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
        Log.i(TAG, "ProxyService.Restart SyncProxyALM.");
        disposeSyncProxy();
        startProxyIfNetworkConnected();
    }

    @Override
    public void onError(String info, Throwable e) {
        createErrorMessageForAdapter("******onProxyError******", e);
        createErrorMessageForAdapter("Proxy error info: " + info);
    }

    /**
     * ******************************
     * * SYNC AppLink Base Callback's **
     * *******************************
     */
    @Override
    public void onAddSubMenuResponse(AddSubMenuResponse response) {
        createDebugMessageForAdapter(response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onAddSubMenuResponse(success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onCreateInteractionChoiceSetResponse(CreateInteractionChoiceSetResponse response) {
        createDebugMessageForAdapter(response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onCreateChoiceSetResponse(success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onDeleteCommandResponse(DeleteCommandResponse response) {
        createDebugMessageForAdapter(response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onDeleteCommandResponse(success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onDeleteInteractionChoiceSetResponse(DeleteInteractionChoiceSetResponse response) {
        createDebugMessageForAdapter(response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onDeleteChoiceSetResponse(success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onDeleteSubMenuResponse(DeleteSubMenuResponse response) {
        createDebugMessageForAdapter(response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onDeleteSubMenuResponse(success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onEncodedSyncPDataResponse(EncodedSyncPDataResponse response) {
        Log.i("syncp", "onEncodedSyncPDataResponse: " + response.getInfo() + response.getResultCode() + response.getSuccess());
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onResetGlobalPropertiesResponse(ResetGlobalPropertiesResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onSetMediaClockTimerResponse(SetMediaClockTimerResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onSpeakResponse(SpeakResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onSubscribeButtonResponse(SubscribeButtonResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onUnsubscribeButtonResponse(UnsubscribeButtonResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
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
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    /**
     * ******************************
     * * SYNC AppLink Soft Button Image Callback's **
     * *******************************
     */
    @Override
    public void onPutFileResponse(PutFileResponse response) {
        createDebugMessageForAdapter(response);
        int mCorrelationId = response.getCorrelationID();
        if (mCorrelationId == mAwaitingInitIconResponseCorrelationID && getAutoSetAppIconFlag()) {
            setAppIcon();
        }
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(mCorrelationId, response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
        mPutFileTransferManager.removePutFileFromAwaitArray(mCorrelationId);
    }

    @Override
    public void onDeleteFileResponse(DeleteFileResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onListFilesResponse(ListFilesResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
        //Log.d(TAG, "ListFiles:" + response.getFilenames().toString());
    }

    @Override
    public void onSetAppIconResponse(SetAppIconResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onOnButtonEvent(OnButtonEvent notification) {
        createDebugMessageForAdapter(notification);
    }

    @Override
    public void onOnButtonPress(OnButtonPress notification) {
        createDebugMessageForAdapter(notification);
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
     * ******************************
     * * SYNC AppLink Updated Callback's **
     * *******************************
     */
    @Override
    public void onAddCommandResponse(AddCommandResponse response) {
        createDebugMessageForAdapter(response);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        final boolean success = response.getSuccess();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onAddCommandResponse(success);
            }
        });

        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onAlertResponse(AlertResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onPerformInteractionResponse(PerformInteractionResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onSetGlobalPropertiesResponse(SetGlobalPropertiesResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onShowResponse(ShowResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    /**
     * ******************************
     * * SYNC AppLink New Callback's **
     * *******************************
     */
    @Override
    public void onSliderResponse(SliderResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onScrollableMessageResponse(ScrollableMessageResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onChangeRegistrationResponse(ChangeRegistrationResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onSetDisplayLayoutResponse(SetDisplayLayoutResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onOnLanguageChange(OnLanguageChange notification) {
        createDebugMessageForAdapter(notification);
    }

    /**
     * ******************************
     * * SYNC AppLink Audio Pass Thru Callback's **
     * *******************************
     */
    @Override
    public void onPerformAudioPassThruResponse(PerformAudioPassThruResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
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
    public void onEndAudioPassThruResponse(EndAudioPassThruResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
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
    public void onOnAudioPassThru(OnAudioPassThru notification) {
        createDebugMessageForAdapter(notification);
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
     * ******************************
     * * SYNC AppLink Vehicle Data Callback's **
     * *******************************
     */
    @Override
    public void onSubscribeVehicleDataResponse(SubscribeVehicleDataResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onUnsubscribeVehicleDataResponse(UnsubscribeVehicleDataResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onGetVehicleDataResponse(GetVehicleDataResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onReadDIDResponse(ReadDIDResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onGetDTCsResponse(GetDTCsResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onOnVehicleData(OnVehicleData notification) {
        createDebugMessageForAdapter(notification);
    }

    /**
     * ******************************
     * * SYNC AppLink TBT Callback's **
     * *******************************
     */
    @Override
    public void onShowConstantTBTResponse(ShowConstantTBTResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onAlertManeuverResponse(AlertManeuverResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onUpdateTurnListResponse(UpdateTurnListResponse response) {
        createDebugMessageForAdapter(response);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onSystemRequestResponse(SystemRequestResponse response) {
        createDebugMessageForAdapter(response);

        if (isModuleTesting()) {
            ModuleTest.responses.add(
                    new Pair<Integer, Result>(response.getCorrelationID(),
                            response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onMobileNaviStart() {
        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onServiceStart(ServiceType.Mobile_Nav, (byte) -1);
        }
    }

    @Override
    public void onAudioServiceStart() {
        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onServiceStart(ServiceType.Audio_Service, (byte) -1);
        }
    }

    @Override
    public void onMobileNavAckReceived(int frameReceivedNumber) {
        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onAckReceived(frameReceivedNumber, ServiceType.Mobile_Nav);
        }
    }

    @Override
    public void onStartServiceNackReceived(ServiceType serviceType) {
        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onStartServiceNackReceived(serviceType);
        }
    }

    @Override
    public void onOnTouchEvent(OnTouchEvent notification) {
        final OnTouchEvent event = notification;
        createDebugMessageForAdapter(notification);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onTouchEventReceived(event);
            }
        });

    }

    @Override
    public void onKeyboardInput(OnKeyboardInput msg) {
        final OnKeyboardInput event = msg;
        createDebugMessageForAdapter(msg);
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainActivity.onKeyboardInputReceived(event);
            }
        });
    }

    @Override
    public void onOnSystemRequest(OnSystemRequest notification) {
        createDebugMessageForAdapter(notification);
    }

    @Override
    public void onRegisterAppRequest(RegisterAppInterface msg) {
        Log.i(TAG, "OnRegisterAppRequest: " + msg.toString());
        createDebugMessageForAdapter(msg);
    }

    @Override
    public void onAppUnregisteredAfterLanguageChange(OnLanguageChange msg) {
        String message =
                String.format("OnAppInterfaceUnregistered (LANGUAGE_CHANGE) from %s to %s",
                        msg.getLanguage(), msg.getHmiDisplayLanguage());
        createDebugMessageForAdapter(message);
        mSyncProxy.resetLanguagesDesired(msg.getLanguage(), msg.getHmiDisplayLanguage());
    }

    @Override
    public void onAppUnregisteredReason(AppInterfaceUnregisteredReason reason) {
        createDebugMessageForAdapter("onAppUnregisteredReason:" + reason);
    }

    @Override
    public void onProtocolServiceEnded(final ServiceType serviceType, final Byte version,
                                       final String correlationID) {
        String response = "EndService Ack received; Session Type " + serviceType.getName() + "; " +
                "Session ID " + version + "; Correlation ID " + correlationID;
        createDebugMessageForAdapter(response);

        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onServiceEnd(serviceType);
        }
        if (serviceType == ServiceType.RPC) {
            mLogAdapter.logMessage("RPC service stopped", true);

            if (mProxyServiceEvent != null) {
                mProxyServiceEvent.onDisposeComplete();
            }

            if (mCloseSessionCallback != null) {
                mCloseSessionCallback.onCloseSessionComplete();
            }
        }
    }

    @Override
    public void onSessionStarted(final byte sessionID, final String correlationID) {
        Log.d(TAG, "SessionStart:" + sessionID + ", mProxyServiceEvent:" + mProxyServiceEvent);
        if (mProxyServiceEvent != null) {
            mProxyServiceEvent.onServiceStart(ServiceType.RPC, sessionID);
        }
    }

    @Override
    public void onOnTBTClientState(OnTBTClientState notification) {
        createDebugMessageForAdapter(notification);
    }

    /**
     * ******************************
     * * SYNC AppLink Policies Callback's **
     * *******************************
     */
    @Override
    public void onOnPermissionsChange(OnPermissionsChange notification) {
        createDebugMessageForAdapter(notification);
    }

    EncodedSyncPDataHeader encodedSyncPDataHeaderfromGPS;

    @Override
    public void onOnEncodedSyncPData(OnEncodedSyncPData notification) {
        Log.i("syncp", "MessageType: " + notification.getMessageType());

        createDebugMessageForAdapter(notification);

        EncodedSyncPDataHeader encodedSyncPDataHeader;
        try {
            encodedSyncPDataHeader = EncodedSyncPDataHeader.parseEncodedSyncPDataHeader(
                    Base64.decode(notification.getData().get(0)));
        } catch (IOException e) {
            Log.e(TAG, "Can't decode base64 string", e);
            return;
        }

        if (encodedSyncPDataHeader.getServiceType() == 3 && encodedSyncPDataHeader.getCommandType() == 1) {
            saveEncodedSyncPData(encodedSyncPDataHeader.getPayload());

            Log.i("EncodedSyncPDataHeader", "Protocol Version: " + encodedSyncPDataHeader.getProtocolVersion());
            Log.i("EncodedSyncPDataHeader", "Response Required: " + encodedSyncPDataHeader.getResponseRequired());
            Log.i("EncodedSyncPDataHeader", "High Bandwidth: " + encodedSyncPDataHeader.getHighBandwidth());
            Log.i("EncodedSyncPDataHeader", "Signed: " + encodedSyncPDataHeader.getSigned());
            Log.i("EncodedSyncPDataHeader", "Encrypted: " + encodedSyncPDataHeader.getEncrypted());
            Log.i("EncodedSyncPDataHeader", "Payload Size: " + encodedSyncPDataHeader.getPayloadSize());
            Log.i("EncodedSyncPDataHeader", "Has ESN: " + encodedSyncPDataHeader.getHasESN());
            Log.i("EncodedSyncPDataHeader", "Service Type: " + encodedSyncPDataHeader.getServiceType());
            Log.i("EncodedSyncPDataHeader", "Command Type: " + encodedSyncPDataHeader.getCommandType());
            Log.i("EncodedSyncPDataHeader", "CPU Destination: " + encodedSyncPDataHeader.getCPUDestination());
            Log.i("EncodedSyncPDataHeader", "Encryption Key Index: " + encodedSyncPDataHeader.getEncryptionKeyIndex());

            byte[] tempESN = encodedSyncPDataHeader.getESN();
            String stringESN = "";
            for (int i = 0; i < 8; i++) stringESN += tempESN[i];
            Log.i("EncodedSyncPDataHeader", "ESN: " + stringESN);

            try {
                Log.i("EncodedSyncPDataHeader", "Module Message ID: " + encodedSyncPDataHeader.getModuleMessageID());
            } catch (Exception e) {

            }
            try {
                Log.i("EncodedSyncPDataHeader", "Server Message ID: " + encodedSyncPDataHeader.getServerMessageID());
            } catch (Exception e) {

            }
            try {
                Log.i("EncodedSyncPDataHeader", "Message Status: " + encodedSyncPDataHeader.getMessageStatus());
            } catch (Exception e) {

            }

            //create header for syncp packet
            if (encodedSyncPDataHeader.getHighBandwidth()) {
                byte[] tempIV = encodedSyncPDataHeader.getIV();
                String stringIV = "";
                for (int i = 0; i < 16; i++) stringIV += tempIV[i];
                Log.i("EncodedSyncPDataHeader", "IV: " + stringIV);

                byte[] tempPayload = encodedSyncPDataHeader.getPayload();
                String stringPayload = "";
                for (int i = 0; i < encodedSyncPDataHeader.getPayloadSize(); i++)
                    stringPayload += tempPayload[i];
                Log.i("EncodedSyncPDataHeader", "Payload: " + stringPayload);

                byte[] tempSignatureTag = encodedSyncPDataHeader.getSignatureTag();
                String stringSignatureTag = "";
                for (int i = 0; i < 16; i++) stringSignatureTag += tempSignatureTag[i];
                Log.i("EncodedSyncPDataHeader", "Signature Tag: " + stringSignatureTag);
            } else {
                byte[] tempIV = encodedSyncPDataHeader.getIV();
                String stringIV = "";
                for (int i = 0; i < 8; i++) stringIV += tempIV[i];
                Log.i("EncodedSyncPDataHeader", "IV: " + stringIV);

                byte[] tempPayload = encodedSyncPDataHeader.getPayload();
                String stringPayload = "";
                for (int i = 0; i < encodedSyncPDataHeader.getPayloadSize(); i++)
                    stringPayload += tempPayload[i];
                Log.i("EncodedSyncPDataHeader", "Payload: " + stringPayload);

                byte[] tempSignatureTag = encodedSyncPDataHeader.getSignatureTag();
                String stringSignatureTag = "";
                for (int i = 0; i < 8; i++) stringSignatureTag += tempSignatureTag[i];
                Log.i("EncodedSyncPDataHeader", "Signature Tag: " + stringSignatureTag);
            }

            encodedSyncPDataHeaderfromGPS = encodedSyncPDataHeader;
            if (mLogAdapter != null) {
                SyncProxyTester.setESN(tempESN);
            }
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
    public void onRegisterAppInterfaceResponse(RegisterAppInterfaceResponse response) {
        createDebugMessageForAdapter(response);

        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }

        try {
            processRegisterAppInterfaceResponse(response);
        } catch (SyncException e) {
            createErrorMessageForAdapter("Can not process RAIResponse:" + e.getMessage());
        }
    }

    @Override
    public void onUnregisterAppInterfaceResponse(UnregisterAppInterfaceResponse response) {
        createDebugMessageForAdapter(response);

        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onSyncPDataResponse(SyncPDataResponse response) {
        createDebugMessageForAdapter(response);

        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }

    @Override
    public void onOnSyncPData(OnSyncPData notification) {
        createDebugMessageForAdapter(notification);
    }

    @Override
    public void onSecureServiceStart() {
        createDebugMessageForAdapter("Secure Service started");
    }

    private void resendUnsentPutFiles() {
        SparseArray<PutFile> unsentPutFiles = mPutFileTransferManager.getCopy();
        mPutFileTransferManager.clear();
        for (int i = 0; i < unsentPutFiles.size(); i++) {
            commandPutFile(unsentPutFiles.valueAt(i));
        }
        unsentPutFiles.clear();
    }

    private void stopServiceBySelf() {
        Log.i(TAG, ProxyService.class.getSimpleName() + " Stop Service By Self");
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
    public void commandListFiles() {
        try {
            mSyncProxy.listFiles(getNextCorrelationID());
            if (mLogAdapter != null) {
                mLogAdapter.logMessage("ListFiles sent", true);
            }
        } catch (SyncException e) {
            mLogAdapter.logMessage("ListFiles send error: " + e, Log.ERROR, e);
        }
    }

    /**
     * Create and send PutFile command
     *
     * @param putFile PurFile to be send
     */
    public void commandPutFile(PutFile putFile) {
        commandPutFile(null, null, null, getNextCorrelationID(), null, putFile);
    }

    /**
     * Create and send PutFile command
     *
     * @param fileType     Type of the File
     * @param syncFileName Name of the File
     * @param bulkData     Data of the File
     */
    public void commandPutFile(FileType fileType, String syncFileName, byte[] bulkData) {
        commandPutFile(fileType, syncFileName, bulkData, -1, null, null);
    }

    /**
     * Create and send PutFile command
     *
     * @param fileType      Type of the File
     * @param syncFileName  Name of the File
     * @param bulkData      Data of the File
     * @param correlationId Unique identifier of the command
     */
    public void commandPutFile(FileType fileType, String syncFileName, byte[] bulkData,
                               int correlationId) {
        commandPutFile(fileType, syncFileName, bulkData, correlationId, null, null);
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
    public void commandPutFile(FileType fileType, String syncFileName, byte[] bulkData,
                               int correlationId, Boolean doSetPersistent) {
        commandPutFile(fileType, syncFileName, bulkData, correlationId, doSetPersistent, null);
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
    public void commandPutFile(FileType fileType, String syncFileName,
                               byte[] bulkData, int correlationId,
                               Boolean doSetPersistent, PutFile putFile) {
        commandPutFile(fileType, syncFileName, bulkData, correlationId,
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
    public void commandPutFile(FileType fileType, String syncFileName,
                               byte[] bulkData, int correlationId,
                               Boolean doSetPersistent, Boolean isSystemFile,
                               Integer length, Integer offset,
                               PutFile putFile) {
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

        mPutFileTransferManager.addPutFileToAwaitArray(mCorrelationId, newPutFile);

        syncProxySendPutFilesResumable(newPutFile);

        //mAwaitingInitIconResponseCorrelationID = 0;
    }

    /**
     * Call a method from SDK to send <b>SubscribeButton</b> request
     *
     * @param buttonName {@link com.ford.syncV4.proxy.rpc.enums.ButtonName}
     */
    public void commandSubscribeButtonPredefined(ButtonName buttonName, int correlationId) {
        SubscribeButton subscribeButton = RPCRequestFactory.buildSubscribeButton();
        subscribeButton.setCorrelationID(correlationId);
        subscribeButton.setButtonName(buttonName);

        syncProxySendRPCRequest(subscribeButton);
    }

    /**
     * Call a method from SDK to send <b>SubscribeButton</b> request which will be used in application
     * resumption.
     *
     * @param correlationId Unique identifier of the command
     * @param buttonName    {@link com.ford.syncV4.proxy.rpc.enums.ButtonName}
     */
    public void commandSubscribeButtonResumable(ButtonName buttonName, int correlationId) {
        SubscribeButton subscribeButton = RPCRequestFactory.buildSubscribeButton();
        subscribeButton.setCorrelationID(correlationId);
        subscribeButton.setButtonName(buttonName);

        syncProxySendRPCRequestResumable(subscribeButton);
    }

    /**
     * Call a method from SDK to send <b>UnsubscribeVehicleData</b> request.
     *
     * @param unsubscribeVehicleData {@link com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData}
     */
    public void commandUnsubscribeVehicleInterface(UnsubscribeVehicleData unsubscribeVehicleData) {
        syncProxySendRPCRequest(unsubscribeVehicleData);
    }

    /**
     * Call a method from SDK to send <b>SubscribeVehicleData</b> request which will be used in
     * application resumption.
     *
     * @param subscribeVehicleData {@link com.ford.syncV4.proxy.rpc.SubscribeVehicleData}
     */
    public void commandSubscribeVehicleInterfaceResumable(SubscribeVehicleData subscribeVehicleData) {
        syncProxySendRPCRequestResumable(subscribeVehicleData);
    }

    /**
     * Call a method from SDK to send <b>AddCommand</b> request which will be used in application
     * resumption.
     *
     * @param commandId  Id of the command
     * @param vrCommands Vector of the VR Commands
     * @param menuName   Name of the Menu
     */
    public void commandAddCommandResumable(Integer commandId, Vector<String> vrCommands,
                                           String menuName) {
        AddCommand addCommand = RPCRequestFactory.buildAddCommand(commandId, menuName, vrCommands,
                getNextCorrelationID());
        syncProxySendRPCRequestResumable(addCommand);
    }

    /**
     * Call a method from SDK to send <b>AddCommand</b> request
     *
     * @param commandId  Id of the command
     * @param vrCommands Vector of the VR Commands
     * @param menuName   Name of the Menu
     */
    public void commandAddCommandPredefined(Integer commandId, Vector<String> vrCommands,
                                            String menuName) {
        AddCommand addCommand = RPCRequestFactory.buildAddCommand(commandId, menuName, vrCommands,
                getNextCorrelationID());
        syncProxySendRPCRequest(addCommand);
    }

    /**
     * Call a method from SDK to send <b>AddCommand</b> request which will be used in application
     * resumption.
     *
     * @param addCommand {@link com.ford.syncV4.proxy.rpc.AddCommand} object
     */
    public void commandAddCommandResumable(AddCommand addCommand) {
        syncProxySendRPCRequestResumable(addCommand);
    }

    /**
     * Call a method from SDK to send <b>AddSubMenu</b> request which will be used in application
     * resumption.
     *
     * @param setGlobalProperties {@link com.ford.syncV4.proxy.rpc.SetGlobalProperties}
     */
    public void commandSetGlobalPropertiesResumable(SetGlobalProperties setGlobalProperties) {
        syncProxySendRPCRequestResumable(setGlobalProperties);
    }

    /**
     * Call a method from SDK to send <b>AddSubMenu</b> request which will be used in application
     * resumption.
     *
     * @param addSubMenu {@link com.ford.syncV4.proxy.rpc.AddSubMenu} object
     */
    public void commandAddSubMenuResumable(AddSubMenu addSubMenu) {
        syncProxySendRPCRequestResumable(addSubMenu);
    }

    /**
     * Call a method from SDK to create and send <b>CreateInteractionChoiceSet</b> request which
     * will be used in application resumption.
     *
     * @param choiceSet              Set of the {@link com.ford.syncV4.proxy.rpc.Choice} objects
     * @param interactionChoiceSetID Id of the interaction Choice set
     * @param correlationID          correlation Id
     */
    public void commandCreateInteractionChoiceSetResumable(Vector<Choice> choiceSet,
                                                           Integer interactionChoiceSetID,
                                                           Integer correlationID) {

        CreateInteractionChoiceSet createInteractionChoiceSet =
                RPCRequestFactory.buildCreateInteractionChoiceSet(choiceSet,
                        interactionChoiceSetID, correlationID);
        syncProxySendRPCRequestResumable(createInteractionChoiceSet);
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

    public void syncProxyCloseSession(boolean keepConnection) {
        if (mSyncProxy != null) {
            mSyncProxy.closeSession(keepConnection);
        }
    }

    public void syncProxyOpenSession() throws SyncException {
        if (mSyncProxy != null) {
            if (mSyncProxy.getIsConnected()) {
                mSyncProxy.openSession();
            } else {
                mSyncProxy.initializeProxy();
            }
        }
    }

    public void syncProxyStartAudioService(Session session) {
        if (mSyncProxy != null && mSyncProxy.getSyncConnection() != null) {
            mSyncProxy.getSyncConnection().startAudioService(session);
        }
    }

    public void syncProxyStopAudioService() {
        if (mSyncProxy != null) {
            mSyncProxy.stopAudioService();
        }
    }

    public OutputStream syncProxyStartAudioDataTransfer() {
        if (mSyncProxy != null) {
            return mSyncProxy.startAudioDataTransfer();
        }
        return null;
    }

    /**
     * This method is send RPC Request to the Sync Proxy
     *
     * @param request object of {@link com.ford.syncV4.proxy.RPCRequest} type
     */
    public void syncProxySendRPCRequest(RPCRequest request) {
        if (request == null) {
            createErrorMessageForAdapter("RPC request is NULL");
            return;
        }
        try {
            if (request.getFunctionName().equals(Names.RegisterAppInterface)) {
                syncProxySendRegisterRequest((RegisterAppInterface) request);
            } else {
                createDebugMessageForAdapter(request);
                mSyncProxy.sendRPCRequest(request);
            }
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
    public void syncProxySendRPCRequestResumable(RPCRequest request) {
        if (request == null) {
            createErrorMessageForAdapter("Resumable RPC request is NULL");
            return;
        }

        if (mSyncProxy.getIsConnected()) {
            mRpcRequestsResumableManager.addRequestConnected(request);
        } else {
            mRpcRequestsResumableManager.addRequestDisconnected(request);
        }

        syncProxySendRPCRequest(request);
    }

    /**
     * @param putFile
     */
    public void syncProxySendPutFilesResumable(PutFile putFile) {
        if (putFile == null) {
            createErrorMessageForAdapter("Resumable PuFile is NULL");
            return;
        }

        //mRpcRequestsResumableManager.addPutFile(putFile);

        syncProxySendRPCRequest(putFile);
    }

    private void syncProxySendRegisterRequest(RegisterAppInterface msg) throws SyncException {
        if (mSyncProxy != null) {
            // TODO it's seems stupid in order to register send onTransportConnected
            mSyncProxy.updateRegisterAppInterfaceParameters(msg);
            if (mSyncProxy.getSyncConnection() != null) {
                mSyncProxy.getSyncConnection().onTransportConnected();
            }
        }
    }

    public byte syncProxyGetWiProVersion() {
        if (mSyncProxy != null) {
            return mSyncProxy.getWiProVersion();
        }
        return 0;
    }

    public void syncProxyStartMobileNavService(Session session) {
        if (mSyncProxy != null && mSyncProxy.getSyncConnection() != null) {
            mSyncProxy.getSyncConnection().startMobileNavService(session);
        }
    }

    public OutputStream syncProxyStartH264() {
        if (mSyncProxy != null) {
            return mSyncProxy.startH264();
        }
        return null;
    }

    public void syncProxyStopMobileNaviService() {
        if (mSyncProxy != null) {
            mSyncProxy.stopMobileNaviService();
        }
    }

    public IJsonRPCMarshaller syncProxyGetJsonRPCMarshaller() {
        if (mSyncProxy != null) {
            return mSyncProxy.getJsonRPCMarshaller();
        }
        return null;
    }

    public void syncProxySetJsonRPCMarshaller(IJsonRPCMarshaller jsonRPCMarshaller) {
        if (mSyncProxy != null) {
            mSyncProxy.setJsonRPCMarshaller(jsonRPCMarshaller);
        }
    }

    private void setAppIcon() {
        SetAppIcon setAppIcon = RPCRequestFactory.buildSetAppIcon();
        setAppIcon.setSyncFileName(ICON_SYNC_FILENAME);
        setAppIcon.setCorrelationID(getNextCorrelationID());

        syncProxySendRPCRequest(setAppIcon);

        mAwaitingInitIconResponseCorrelationID = 0;
    }

    /**
     * Process a response of the {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} request
     *
     * @param response {@link com.ford.syncV4.proxy.rpc.RegisterAppInterfaceResponse} object
     */
    private void processRegisterAppInterfaceResponse(RegisterAppInterfaceResponse response)
            throws SyncException {

        if (!response.getSuccess()) {
            return;
        }

        if (response.getResultCode() == Result.SUCCESS) {
            //mRpcRequestsResumableManager.sendAllPutFiles();
            mRpcRequestsResumableManager.sendAllRequestsDisconnected();
        } else if (response.getResultCode() == Result.RESUME_FAILED) {
            //mRpcRequestsResumableManager.sendAllPutFiles();
            mRpcRequestsResumableManager.sendAllRequestsConnected();
            mRpcRequestsResumableManager.sendAllRequestsDisconnected();
        }

        //mRpcRequestsResumableManager.cleanAllPutFiles();
        mRpcRequestsResumableManager.cleanAllRequestsConnected();
        mRpcRequestsResumableManager.cleanAllRequestsDisconnected();

        // Restore a PutFile which has not been sent
        resendUnsentPutFiles();
        // Restore Services
        mSyncProxy.restoreServices();
    }

    // TODO: Reconsider this section, this is a first step to optimize log procedure

    /**
     * Logger section. Send log message to adapter and log it to the ADB
     */

    private void createErrorMessageForAdapter(Object messageObject) {
        createErrorMessageForAdapter(messageObject, null);
    }

    private void createErrorMessageForAdapter(Object messageObject, Throwable throwable) {
        if (mLogAdapter == null) {
            Log.w(TAG, "LogAdapter is null");
        }
        if (mLogAdapter != null) {
            if (throwable != null) {
                mLogAdapter.logMessage(messageObject, Log.ERROR, throwable, true);
            } else {
                mLogAdapter.logMessage(messageObject, Log.ERROR, true);
            }
        } else {
            if (throwable != null) {
                Log.e(TAG, messageObject.toString(), throwable);
            } else {
                Log.e(TAG, messageObject.toString());
            }
        }
    }

    private void createInfoMessageForAdapter(Object messageObject) {
        createMessageForAdapter(messageObject, Log.INFO);
    }

    private void createDebugMessageForAdapter(Object messageObject) {
        createMessageForAdapter(messageObject, Log.DEBUG);
    }

    private void createMessageForAdapter(Object messageObject, Integer type) {
        if (mLogAdapter == null) {
            Log.w(TAG, "LogAdapter is null");
        }
        if (mLogAdapter != null) {
            mLogAdapter.logMessage(messageObject, type, true);
        } else {
            if (type == Log.DEBUG) {
                Log.d(TAG, messageObject.toString());
            } else if (type == Log.INFO) {
                Log.i(TAG, messageObject.toString());
            }
        }
    }

    @Override
    public void onUSBNoSuchDeviceException() {
        final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
        if (mainActivity != null) {
            mainActivity.onUSBNoSuchDeviceException();
        }
    }

    @Override
    public void onDiagnosticMessageResponse(DiagnosticMessageResponse diagnosticMessageResponse) {
        createDebugMessageForAdapter(diagnosticMessageResponse);
        if (isModuleTesting()) {
            ModuleTest.responses.add(new Pair<Integer, Result>(diagnosticMessageResponse.getCorrelationID(), diagnosticMessageResponse.getResultCode()));
            synchronized (mTesterMain.getThreadContext()) {
                mTesterMain.getThreadContext().notify();
            }
        }
    }
}