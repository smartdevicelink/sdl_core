package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnMultiChoiceClickListener;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.util.Pair;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CheckedTextView;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.Toast;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.mobilenav.AudioServicePreviewFragment;
import com.ford.syncV4.android.activity.mobilenav.MobileNavPreviewFragment;
import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.constants.SyncSubMenu;
import com.ford.syncV4.android.listener.ConnectionListener;
import com.ford.syncV4.android.listener.ConnectionListenersManager;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.manager.BluetoothDeviceManager;
import com.ford.syncV4.android.manager.IBluetoothDeviceManager;
import com.ford.syncV4.android.module.GenericRequest;
import com.ford.syncV4.android.module.ModuleTest;
import com.ford.syncV4.android.policies.PoliciesTesterActivity;
import com.ford.syncV4.android.receivers.ISyncReceiver;
import com.ford.syncV4.android.receivers.SyncReceiver;
import com.ford.syncV4.android.service.ICloseSession;
import com.ford.syncV4.android.service.IProxyServiceBinder;
import com.ford.syncV4.android.service.IProxyServiceEvent;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.TTSChunkFactory;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.AddSubMenu;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.AlertManeuver;
import com.ford.syncV4.proxy.rpc.ChangeRegistration;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.DeleteCommand;
import com.ford.syncV4.proxy.rpc.DeleteFile;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteSubMenu;
import com.ford.syncV4.proxy.rpc.DiagnosticMessage;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.proxy.rpc.EndAudioPassThru;
import com.ford.syncV4.proxy.rpc.GetDTCs;
import com.ford.syncV4.proxy.rpc.GetVehicleData;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.OnAudioPassThru;
import com.ford.syncV4.proxy.rpc.OnKeyboardInput;
import com.ford.syncV4.proxy.rpc.OnTouchEvent;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThru;
import com.ford.syncV4.proxy.rpc.PerformInteraction;
import com.ford.syncV4.proxy.rpc.ReadDID;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.ResetGlobalProperties;
import com.ford.syncV4.proxy.rpc.ScrollableMessage;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.SetDisplayLayout;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimer;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.ShowConstantTBT;
import com.ford.syncV4.proxy.rpc.Slider;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.Speak;
import com.ford.syncV4.proxy.rpc.StartTime;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.SyncPData;
import com.ford.syncV4.proxy.rpc.SystemRequest;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.Turn;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterface;
import com.ford.syncV4.proxy.rpc.UnsubscribeButton;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.UpdateTurnList;
import com.ford.syncV4.proxy.rpc.VrHelpItem;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.LayoutMode;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.util.Base64;
import com.ford.syncV4.util.logger.Logger;
import com.lamerman.FileDialog;
import com.lamerman.SelectionMode;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Vector;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class SyncProxyTester extends FragmentActivity implements OnClickListener,
        IBluetoothDeviceManager, ConnectionListener, PutFileDialog.PutFileDialogListener,
        IProxyServiceEvent {

    private static final String ButtonSubscriptions = "ButtonSubscriptions";
    private static final String VehicleDataSubscriptions = "VehicleDataSubscriptions";
    /**
     * The name of the file where all the data coming with
     * {@link OnAudioPassThru} notifications is saved. The root directory is the
     * external storage.
     */
    private static final String AUDIOPASSTHRU_OUTPUT_FILE = "audiopassthru.wav";
    /**
     * String to join/split help, timeout, VR prompts, etc.
     */
    public static final String JOIN_STRING = ",";
    private static final int ALERT_MAXSOFTBUTTONS = 4;
    private static final int SCROLLABLEMESSAGE_MAXSOFTBUTTONS = 8;
    private static final int SHOW_MAXSOFTBUTTONS = 8;
    private static final int ALERTMANEUVER_MAXSOFTBUTTONS = 3;
    private static final int SHOWCONSTANTTBT_MAXSOFTBUTTONS = 3;
    private static final int UPDATETURNLIST_MAXSOFTBUTTONS = 1;
    private static final int CREATECHOICESET_MAXCHOICES = 100;
    private final static int REQUEST_CHOOSE_XML_TEST = 51;
    private static final int CHOICESETID_UNSET = -1;
    private static final String MSC_PREFIX = "msc_";
    private static SyncProxyTester _activity;
    private static ArrayList<Object> _logMessages = new ArrayList<Object>();
    private LogAdapter mLogAdapter;
    private static byte[] _ESN;
    /**
     * Autoincrementing id for new softbuttons.
     */
    private static int autoIncSoftButtonId = 5500;
    /**
     * Autoincrementing id for new choices.
     */
    private static int autoIncChoiceId = 9000;
    /**
     * In onCreate() specifies if it is the first time the activity is created
     * during this app launch.
     */
    private static boolean isFirstActivityRun = true;
    private final int PROXY_START = 5;
    private final int XML_TEST = 7;
    private final int POLICIES_TEST = 8;
    private final int MNU_TOGGLE_CONSOLE = 9;
    private final int MNU_CLEAR = 10;
    private final int MNU_EXIT = 11;
    private final int MNU_TOGGLE_MEDIA = 12;
    private final int MNU_CLOSESESSION = 14;
    private final int MNU_APP_VERSION = 15;
    private final int MNU_CLEAR_FUNCTIONS_USAGE = 16;
    private final int MNU_WAKELOCK = 17;
    private final int MNU_SET_UP_POLICY_FILES = 18;
    private final int MNU_HASH_ID_SETUP = 19;
    private final int MNU_FEEDBACK = 20;
    private ModuleTest mTesterMain;
    private ScrollView _scroller = null;
    private ListView mListview = null;
    private ArrayAdapter<SyncSubMenu> mSubmenuAdapter = null;
    private ArrayAdapter<Integer> mCommandAdapter = null;
    private Map<Integer, Integer> mCommandIdToParentSubmenuMap = null;
    private ArrayAdapter<Integer> mChoiceSetAdapter = null;
    private ArrayAdapter<String> mPutFileAdapter = null;
    /**
     * Latest choiceSetId, required to add it to the adapter when a successful
     * CreateInteractionChoiceSetResponse comes.
     */
    private int mLatestCreateChoiceSetId = CHOICESETID_UNSET;
    /**
     * Latest choiceSetId, required to delete it from the adapter when a
     * successful DeleteInteractionChoiceSetResponse comes.
     */
    private int _latestDeleteChoiceSetId = CHOICESETID_UNSET;
    /**
     * Latest SyncSubMenu, required to delete the submenu from the adapter
     * when a successful DeleteSubMenuResponse comes.
     */
    private SyncSubMenu _latestDeleteSubmenu = null;
    /**
     * Latest SyncSubMenu, required to add the submenu from the adapter when a
     * successful AddSubMenuResponse comes.
     */
    private SyncSubMenu mLatestAddSubmenu = null;
    private Pair<Integer, Integer> mLatestAddCommand = null;
    private Integer _latestDeleteCommandCmdID = null;
    private int mAutoIncCorrId = 101;
    private int autoIncChoiceSetId = 1;
    private int autoIncChoiceSetIdCmdId = 1;
    private ArrayAdapter<ButtonName> mButtonAdapter = null;
    private boolean[] isButtonSubscribed = null;
    private ArrayAdapter<VehicleDataType> _vehicleDataType = null;
    private boolean[] isVehicleDataSubscribed = null;
    /**
     * List of soft buttons for current function. Passed between
     * {@link SoftButtonsListActivity} and this activity.
     */
    private Vector<SoftButton> currentSoftButtons;
    /**
     * The Include Soft Buttons checkbox in the current dialog. Kept here to
     * check it when the user has explicitly set the soft buttons.
     */
    private CheckBox chkIncludeSoftButtons;
    /**
     * Reference to PutFile dialog's local filename text field, so that the
     * filename is set after choosing.
     */
    private EditText txtLocalFileName;
    /**
     * Stores the number of selections of each message to sort them by
     * most-popular usage.
     */
    private Map<String, Integer> messageSelectCount;
    /**
     * The output stream to write audioPassThru data.
     */
    private OutputStream audioPassThruOutStream = null;
    /**
     * Media player for the stream of audio pass thru.
     */
    private MediaPlayer audioPassThruMediaPlayer = null;
    /**
     * The most recent sent PerformAudioPassThru message, saved in case we need
     * to retry the request.
     */
    private PerformAudioPassThru latestPerformAudioPassThruMsg = null;
    /**
     * Shared ArrayAdapter containing ImageType values.
     */
    private ArrayAdapter<ImageType> imageTypeAdapter;
    // Request id for SoftButtonsListActivity
    static final int REQUEST_LIST_SOFTBUTTONS = 43;
    // Request id for ChoiceListActivity
    static final int REQUEST_LIST_CHOICES = 45;
    /**
     * Time out in milliseconds for exit from application. If application is not correctly
     * destroyed within specified timeout - then we force destroy procedure
     */
    private static final int EXIT_TIMEOUT = 3000;
    /**
     * Handler object to monitor exit procedure. If exit procedure fails, then this object will
     * manage application to destroy
     */
    private Handler mStopProxyServiceTimeOutHandler;
    /**
     * Handler object to monitor stop non RPC services. If stop procedure will fail, then this object
     * will start destroy service functionality
     */
    private Handler mStopServicesTimeOutHandler;
    /**
     * Counter of the Services
     */
    private AtomicInteger mServicesCounter = new AtomicInteger();
    /**
     * Handler object to monitor stop proxy procedure for the Bluetooth connection.
     */
    private Handler mBluetoothStopProxyServiceTimeOutHandler;
    /**
     * progress dialog of the Exit Application
     */
    private ProgressDialog mExitProgressDialog;
    /**
     * UI Handler to perform actions in UI Thread
     */
    private final Handler mUIHandler = new Handler(Looper.getMainLooper());
    private final static String APP_SETUP_DIALOG_TAG = "AppSetupDialogTag";
    private final static String POLICY_FILES_SETUP_DIALOG_TAG = "PolicyFilesSetupDialogTag";
    private final static String PUT_FILE_DIALOG_TAG = "PutFileDialogTag";
    private final static String ADD_COMMAND_DIALOG_TAG = "AddCommandDialogTag";
    private final static String SEND_SINGLE_RPC_COMMAND_DIALOG_TAG = "SendSingleRPCCommandDialogTag";
    private final static String PERFORM_AUDIO_PASS_THRU_DIALOG_TAG = "PerformAudioPassThruDialogTag";
    private final static String SYSTEM_REQST_DIALOG_TAG = "SystemRequestDialogTag";
    private final static String ADD_SUB_MENU_DIALOG_TAG = "AddSubMenuDialogTag";
    private final static String SET_GLOBAL_PROPERTIES_DIALOG_TAG = "SetGlobalPropertiesDialogTag";
    private final static String SUBSCRIPTION_VEHICLE_DATA_DIALOG_TAG = "SubscriptionVehicleDataDialogTag";
    private final static String REGISTER_APP_INTERFACE_DIALOG_TAG = "RegisterAppInterfaceDialogTag";
    private final static String HASH_ID_SET_UP_DIALOG_TAG = "HashIdSetUpDialogTag";
    private final static String FEEDBACK_DIALOG_TAG = "FeedbackDialogTag";

    private SyncReceiver mSyncReceiver;
    private BluetoothDeviceManager mBluetoothDeviceManager;
    private Session rpcSession = new Session();

    // Get Bound Proxy Service from MainApp
    private ProxyService mBoundProxyService;
    private ExecutorService mStreamCommandsExecutorService;

    public static SyncProxyTester getInstance() {
        return _activity;
    }

    public static void setESN(byte[] ESN) {
        _ESN = ESN;
    }

    public static int getNewSoftButtonId() {
        return autoIncSoftButtonId++;
    }

    public static int getNewChoiceId() {
        return autoIncChoiceId++;
    }

    public void runInUIThread(Runnable runnable) {
        mUIHandler.post(runnable);
    }
    
    private ArrayAdapter<String> adapter;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Logger.d(SyncProxyTester.class.getSimpleName() + " On Create");

        _activity = this;

        setContentView(R.layout.main);

        getProxyService();

        addListeners();

        mStreamCommandsExecutorService = Executors.newFixedThreadPool(3);

        _scroller = (ScrollView) findViewById(R.id.scrollConsole);

        findViewById(R.id.btnSendMessage).setOnClickListener(this);
        findViewById(R.id.btnPlayPause).setOnClickListener(this);

        resetAdapters();

        _vehicleDataType = new ArrayAdapter<VehicleDataType>(this,
                android.R.layout.simple_spinner_item, VehicleDataType.values());
        _vehicleDataType.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        imageTypeAdapter = new ArrayAdapter<ImageType>(this, android.R.layout.simple_spinner_item, ImageType.values());
        imageTypeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        mLogAdapter = new LogAdapter("SyncProxyTester", false, this, R.layout.row, _logMessages);

        mListview = (ListView) findViewById(R.id.messageList);
        mListview.setClickable(true);
        mListview.setAdapter(mLogAdapter);
        mListview.setTranscriptMode(ListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);
        mListview.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Object listObj = parent.getItemAtPosition(position);
                if (listObj instanceof RPCMessage) {
                    AlertDialog.Builder builder = new AlertDialog.Builder(SyncProxyTester.this);
                    String rawJSON = "";

                    Integer corrId = -1;
                    if (listObj instanceof RPCRequest) {
                        corrId = ((RPCRequest) listObj).getCorrelationID();
                    } else if (listObj instanceof RPCResponse) {
                        corrId = ((RPCResponse) listObj).getCorrelationID();
                    }

                    try {
                        rawJSON = ((RPCMessage) listObj).serializeJSON(
                                mBoundProxyService.syncProxyGetWiProVersion()).toString(2);
                        builder.setTitle("Raw JSON" + (corrId != -1 ? " (Corr ID " + corrId + ")" : ""));
                    } catch (Exception e) {
                        try {
                            rawJSON = ((RPCMessage) listObj).getFunctionName() +
                                    " (" + ((RPCMessage) listObj).getMessageType() + ")";
                        } catch (Exception e1) {
                            rawJSON = "Undefined";
                        }
                    }
                    builder.setMessage(rawJSON);
                    builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            dialog.cancel();
                        }
                    });
                    AlertDialog ad = builder.create();
                    ad.show();
                } else if (listObj instanceof String) {
                    AlertDialog.Builder builder = new AlertDialog.Builder(SyncProxyTester.this);
                    builder.setMessage(listObj.toString());
                    builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            dialog.cancel();
                        }
                    });
                    AlertDialog ad = builder.create();
                    ad.show();
                }
            }
        });

        if (isFirstActivityRun) {
            DialogFragment appSetupDialogFragment = AppSetUpDialog.newInstance();
            appSetupDialogFragment.show(getFragmentManager(), APP_SETUP_DIALOG_TAG);
            appSetupDialogFragment.setCancelable(false);
        } else {
            onSetUpDialogResult();
        }

        isFirstActivityRun = false;
    }

    public void onSetUpDialogResult() {
        setUpReceiver();
        showProtocolPropertiesInTitle();
        if (mBoundProxyService != null) {
            initProxyService();
            try {
                mBoundProxyService.syncProxyOpenSession();
            } catch (SyncException e) {
                Logger.e(SyncProxyTester.class.getSimpleName() + " syncProxyOpenSession", e);
            }
        } else {
            MainApp.getInstance().bindProxyToMainApp(new IProxyServiceBinder() {
                @Override
                public void onServiceBindComplete() {
                    Logger.d("Service Bind Complete");
                    getProxyService();
                    initProxyService();

                    mBoundProxyService.startProxyIfNetworkConnected();
                }
            });
        }
    }

    private void setUpReceiver() {
        IntentFilter intentFilter = new IntentFilter();

        // provide access to the connection states with a remote device.
        //intentFilter.addAction(BluetoothDevice.ACTION_ACL_CONNECTED);
        //intentFilter.addAction(BluetoothDevice.ACTION_ACL_DISCONNECTED);

        intentFilter.addAction(Intent.ACTION_MEDIA_BUTTON);
        intentFilter.addAction(AudioManager.ACTION_AUDIO_BECOMING_NOISY);

        if (mSyncReceiver == null) {
            mSyncReceiver = new SyncReceiver();

            mSyncReceiver.setSyncReceiver(new ISyncReceiver() {
                @Override
                public void onReceive() {
                    if (mBoundProxyService != null) {
                        mBoundProxyService.pauseAnnoyingRepetitiveAudio();
                    }
                }
            });

            if (AppPreferencesManager.getTransportType() == TransportType.BLUETOOTH) {
                intentFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
                if (mBluetoothDeviceManager == null) {
                    mBluetoothDeviceManager = new BluetoothDeviceManager();
                }
                mBluetoothDeviceManager.setBluetoothDeviceManagerCallback(this);
                mBluetoothDeviceManager.initState();
                mSyncReceiver.setBluetoothReceiverCallback(mBluetoothDeviceManager);
            }

        }
        registerReceiver(mSyncReceiver, intentFilter);
    }

    @Override
    public void onBluetoothDeviceRestoreConnection() {
        Logger.i("Bluetooth connection restored");
        if (AppPreferencesManager.getTransportType() != TransportType.BLUETOOTH) {
            return;
        }
        // TODO : Consider this case.
        /*if (mBoundProxyService == null) {
            bindProxyService(this, mProxyServiceConnectionProxy);
        }*/
    }

    @Override
    public void onBluetoothDeviceTurningOff() {
        Logger.i("Bluetooth turning off");
        if (AppPreferencesManager.getTransportType() != TransportType.BLUETOOTH) {
            return;
        }

        if (mBluetoothStopProxyServiceTimeOutHandler == null) {
            mBluetoothStopProxyServiceTimeOutHandler = new Handler();
        } else {
            mBluetoothStopProxyServiceTimeOutHandler.removeCallbacks(
                    mBluetoothStopServicePostDelayedCallback);
        }
        mBluetoothStopProxyServiceTimeOutHandler.postDelayed(
                mBluetoothStopServicePostDelayedCallback, EXIT_TIMEOUT);

        if (mBoundProxyService.hasServiceInServicesPool(ServiceType.Audio_Service)) {
            stopAudioService();
        }
        if (mBoundProxyService.hasServiceInServicesPool(ServiceType.Mobile_Nav)) {
            stopMobileNavService();
        }

        if (mBoundProxyService != null) {
            mBoundProxyService.destroyService();
        }
    }

    @Override
    public void onProxyClosed() {
        resetAdapters();
    }

    @Override
    public void onPutFileSelected(String fileName) {
        if (mPutFileAdapter != null) {
            mPutFileAdapter.add(fileName);
        }
    }

    /**
     * Proxy Service Events section
     */

    @Override
    public void onDisposeComplete() {
        Logger.d("Dispose Service complete");

        if (mBluetoothStopProxyServiceTimeOutHandler != null) {
            mBluetoothStopProxyServiceTimeOutHandler.removeCallbacks(
                    mBluetoothStopServicePostDelayedCallback);
        } else {
            if (mStopServicesTimeOutHandler != null) {
                mStopServicesTimeOutHandler.removeCallbacks(mEndServicesPostDelayedCallback);
            }
            if (mStopProxyServiceTimeOutHandler != null) {
                mStopProxyServiceTimeOutHandler.removeCallbacks(mExitPostDelayedCallback);
            }

            if (mStopServicesTimeOutHandler == null && mStopProxyServiceTimeOutHandler == null) {
                dismissExitDialog();
                return;
            }

            closeApplication();
        }
    }

    private void dismissExitDialog() {
        runInUIThread(new Runnable() {
            public void run() {
                getExitDialog().dismiss();
            }
        });
    }

    private void closeApplication() {
        MainApp.getInstance().unbindProxyFromMainApp();
        runInUIThread(new Runnable() {
            @Override
            public void run() {
                getExitDialog().dismiss();
                exitApp();
            }
        });
    }

    @Override
    public void onDisposeError() {
        if (mBluetoothStopProxyServiceTimeOutHandler != null) {
            mBluetoothStopProxyServiceTimeOutHandler.removeCallbacks(
                    mBluetoothStopServicePostDelayedCallback);
        }
    }

    @Override
    public void onServiceEnd(ServiceType serviceType) {
        mLogAdapter.logMessage("Service '" + serviceType + "' ended");

        if (mServicesCounter == null) {
            Logger.w("Service End -> Services counter is NULL");
            executeDestroyService();
            return;
        }
        int remainServicesNumber = mServicesCounter.decrementAndGet();
        mLogAdapter.logMessage("Services number remained:" + remainServicesNumber);
        if (remainServicesNumber == 1) {
            executeDestroyService();
        }
    }

    @Override
    public void onServiceStart(ServiceType serviceType, byte sessionId) {
        mLogAdapter.logMessage("Service '" + serviceType + "' started, " +
                "protocol version: " + mBoundProxyService.syncProxyGetWiProVersion(), true);

        if (mBoundProxyService == null) {
            mLogAdapter.logMessage(SyncProxyTester.class.getSimpleName() + " '" + serviceType +
                    "' service can not " + "start with NULL Proxy Service", Log.WARN);
            return;
        }

        if (serviceType == ServiceType.Audio_Service) {
            MainApp.getInstance().runInUIThread(new Runnable() {
                @Override
                public void run() {
                    OutputStream outputStream = mBoundProxyService.syncProxyStartAudioDataTransfer();
                    if (outputStream != null) {
                        AudioServicePreviewFragment fragment = (AudioServicePreviewFragment)
                                getSupportFragmentManager().findFragmentById(R.id.audioFragment);
                        fragment.setAudioServiceStateOn(outputStream);
                    }
                }
            });
        } else if (serviceType == ServiceType.Mobile_Nav) {
            MainApp.getInstance().runInUIThread(new Runnable() {
                @Override
                public void run() {
                    OutputStream outputStream = mBoundProxyService.syncProxyStartH264();
                    if (outputStream != null) {
                        MobileNavPreviewFragment fragment = (MobileNavPreviewFragment)
                                getSupportFragmentManager().findFragmentById(R.id.videoFragment);
                        fragment.setMobileNaviStateOn(outputStream);
                    }
                }
            });
        } else if (serviceType == ServiceType.RPC) {
            mServicesCounter.set(0);
            rpcSession.setSessionId(sessionId);
        }

        mServicesCounter.incrementAndGet();
    }

    @Override
    public void onAckReceived(int frameReceived, ServiceType serviceType) {
        mLogAdapter.logMessage("Service '" + serviceType + "' Ack received, n:" + frameReceived);
    }

    @Override
    public void onStartServiceNackReceived(final ServiceType serviceType) {
        mLogAdapter.logMessage("Start Service '" + serviceType + "' Nack received", true);

        MainApp.getInstance().runInUIThread(new Runnable() {
            @Override
            public void run() {
                if (serviceType == ServiceType.Mobile_Nav) {
                    MobileNavPreviewFragment fragment = (MobileNavPreviewFragment)
                            getSupportFragmentManager().findFragmentById(R.id.videoFragment);
                    fragment.setStateOff();
                } else if (serviceType == ServiceType.Audio_Service) {
                    AudioServicePreviewFragment fragment = (AudioServicePreviewFragment)
                            getSupportFragmentManager().findFragmentById(R.id.audioFragment);
                    fragment.setStateOff();
                }
            }
        });
    }

    /**
     * Return the next correlation id
     *
     * @return int
     */
    public int getCorrelationid() {
        return mAutoIncCorrId++;
    }

    private void loadMessageSelectCount() {
        SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME, 0);
        messageSelectCount = new Hashtable<String, Integer>();
        for (Entry<String, ?> entry : prefs.getAll().entrySet()) {
            if (entry.getKey().startsWith(MSC_PREFIX)) {
                messageSelectCount.put(entry.getKey().substring(MSC_PREFIX.length()),
                        (Integer) entry.getValue());
            }
        }
    }

    private void saveMessageSelectCount() {
        if (messageSelectCount == null) {
            return;
        }
        SharedPreferences.Editor editor = getSharedPreferences(Const.PREFS_NAME, 0).edit();
        for (Entry<String, Integer> entry : messageSelectCount.entrySet()) {
            editor.putInt(MSC_PREFIX + entry.getKey(), entry.getValue());
        }
        editor.commit();
    }

    private void clearMessageSelectCount() {
        messageSelectCount = new Hashtable<String, Integer>();
        SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = prefs.edit();
        for (Entry<String, ?> entry : prefs.getAll().entrySet()) {
            if (entry.getKey().startsWith(MSC_PREFIX)) {
                editor.remove(entry.getKey());
            }
        }
        editor.commit();
    }

    /**
     * Initializes/resets the adapters keeping created submenus, interaction
     * choice set ids, etc.
     */
    private void resetAdapters() {
        // set up storage for subscription records
        isButtonSubscribed = new boolean[ButtonName.values().length];
        mButtonAdapter = new ArrayAdapter<ButtonName>(this,
                android.R.layout.select_dialog_multichoice, ButtonName.values()) {
            public View getView(int position, View convertView, ViewGroup parent) {
                CheckedTextView ret = (CheckedTextView) super.getView(position, convertView, parent);
                ret.setChecked(isButtonSubscribed[position]);
                return ret;
            }
        };

        isVehicleDataSubscribed = new boolean[VehicleDataType.values().length];

        mSubmenuAdapter = new ArrayAdapter<SyncSubMenu>(this, android.R.layout.select_dialog_item);
        mSubmenuAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        mCommandAdapter = new ArrayAdapter<Integer>(this, android.R.layout.select_dialog_item);
        mCommandAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        mCommandIdToParentSubmenuMap = new Hashtable<Integer, Integer>();

        mChoiceSetAdapter = new ArrayAdapter<Integer>(this, android.R.layout.simple_spinner_item);
        mChoiceSetAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        mPutFileAdapter = new ArrayAdapter<String>(this, android.R.layout.select_dialog_item);
        mPutFileAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
    }

    /**
     * Displays the current protocol properties in the activity's title.
     */
    private void showProtocolPropertiesInTitle() {
        final SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME, 0);
        int protocolVersion = getCurrentProtocolVersion();
        boolean isMedia = prefs.getBoolean(Const.PREFS_KEY_ISMEDIAAPP, Const.PREFS_DEFAULT_ISMEDIAAPP);
        String transportType = null;
        switch (prefs.getInt(Const.Transport.PREFS_KEY_TRANSPORT_TYPE,
                Const.Transport.PREFS_DEFAULT_TRANSPORT_TYPE)) {
            case Const.Transport.KEY_TCP:
                transportType = "WiFi";
                break;
            case Const.Transport.KEY_BLUETOOTH:
                transportType = "BT";
                break;
            case Const.Transport.KEY_USB:
                transportType = "USB";
                break;
        }
        setTitle(getResources().getString(R.string.tester_app_name) + " (v"
                + protocolVersion + ", " + (isMedia ? "" : "non-") + "media, "
                + transportType + ")");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Logger.d(SyncProxyTester.class.getSimpleName() + " On Destroy");

        if (mSyncReceiver != null) {
            unregisterReceiver(mSyncReceiver);
        }

        removeListeners();

        _activity = null;
        if (mBoundProxyService != null) {
            mBoundProxyService.setLogAdapter(null);
        }
        closeAudioPassThruStream();
        closeAudioPassThruMediaPlayer();
    }

    public Dialog onCreateDialog(int id) {
        Dialog dialog = null;
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        switch (id) {
            case 1:
                builder.setTitle("Raw JSON");
                builder.setMessage("This is the raw JSON message here");
                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });
                dialog = builder.create();
                break;
            case 2:
                break;
            default:
                dialog = null;
        }
        return dialog;
    }

    /* Creates the menu items */
    public boolean onCreateOptionsMenu(Menu menu) {
        boolean result = super.onCreateOptionsMenu(menu);
        if (result) {
            menu.add(0, PROXY_START, 0, "Proxy Start");
            menu.add(0, MNU_TOGGLE_CONSOLE, 0, "Toggle Console");
            menu.add(0, MNU_CLEAR, 0, "Clear Messages");
            menu.add(0, MNU_EXIT, 0, "Exit");
/*			menu.add(0, MNU_TOGGLE_MEDIA, 0, "Toggle Media");*/
            menu.add(0, MNU_APP_VERSION, 0, "App version");
            menu.add(0, MNU_CLOSESESSION, 0, "Close Session");
            menu.add(0, MNU_HASH_ID_SETUP, 0, "HashId setup");
            menu.add(0, MNU_CLEAR_FUNCTIONS_USAGE, 0, "Reset functions usage");
            menu.add(0, XML_TEST, 0, "XML Test");
            menu.add(0, POLICIES_TEST, 0, "Policies Test");
            menu.add(0, MNU_SET_UP_POLICY_FILES, 0, "Set Up Policy files");
            MenuItem menuitem = menu.add(0, MNU_WAKELOCK, 0, "Lock screen while testing");
            menuitem.setCheckable(true);
            menuitem.setChecked(!AppPreferencesManager.getDisableLockFlag());
            menu.add(0, MNU_FEEDBACK, 0, "Feedback");
            return true;
        } else {
            return false;
        }
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        MenuItem menuItem = menu.findItem(MNU_WAKELOCK);
        if (menuItem != null) {
            menuItem.setChecked(!AppPreferencesManager.getDisableLockFlag());
        }
        return super.onPrepareOptionsMenu(menu);
    }

    private int getCurrentProtocolVersion() {
        return Const.PROTOCOL_VERSION_2;
    }

    private boolean getIsMedia() {
        return getSharedPreferences(Const.PREFS_NAME, 0).getBoolean(
                Const.PREFS_KEY_ISMEDIAAPP, Const.PREFS_DEFAULT_ISMEDIAAPP);
    }

    /* Handles item selections */
    public boolean onOptionsItemSelected(MenuItem item) {

        switch (item.getItemId()) {
            case PROXY_START:
                BluetoothAdapter mBtAdapter = BluetoothAdapter.getDefaultAdapter();
                if (!mBtAdapter.isEnabled()) {
                    mBtAdapter.enable();
                }

                /*// TODO : To be reconsider
                if (mBoundProxyService == null) {
                    bindProxyService(this, mProxyServiceConnectionProxy);
                } else {
                    mBoundProxyService.setLogAdapter(mLogAdapter);
                }*/

                if (mBoundProxyService != null) {
                    mBoundProxyService.reset();
                }

                if (!mBtAdapter.isDiscovering()) {
                    Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                    discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
                    startActivity(discoverableIntent);
                }
                return true;

            case XML_TEST:
                xmlTest();
                break;
            case POLICIES_TEST:
                if (PoliciesTesterActivity.getInstance() == null) {
                    Intent i = new Intent(this, PoliciesTesterActivity.class);
                    i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    i.addFlags(Intent.FLAG_ACTIVITY_NO_USER_ACTION);
                    startActivity(i);
                    if (_ESN != null) PoliciesTesterActivity.setESN(_ESN);
                }
                //PoliciesTest.runPoliciesTest();
                break;
            case MNU_SET_UP_POLICY_FILES:
                DialogFragment mPolicyFilesSetUpDialog = PolicyFilesSetUpDialog.newInstance();
                mPolicyFilesSetUpDialog.show(getFragmentManager(), POLICY_FILES_SETUP_DIALOG_TAG);
                break;
            case MNU_EXIT:
                stopProxyServiceOnExit();
                break;
            case MNU_TOGGLE_CONSOLE:
                if (_scroller.getVisibility() == ScrollView.VISIBLE) {
                    _scroller.setVisibility(ScrollView.GONE);
                    mListview.setVisibility(ListView.VISIBLE);
                } else {
                    _scroller.setVisibility(ScrollView.VISIBLE);
                    mListview.setVisibility(ListView.GONE);
                }
                return true;
            case MNU_CLEAR:
                mLogAdapter.clear();
                return true;
            case MNU_TOGGLE_MEDIA:
                SharedPreferences settings = getSharedPreferences(Const.PREFS_NAME, 0);
                boolean isMediaApp = settings.getBoolean(Const.PREFS_KEY_ISMEDIAAPP, Const.PREFS_DEFAULT_ISMEDIAAPP);
                SharedPreferences.Editor editor = settings.edit();
                editor.putBoolean(Const.PREFS_KEY_ISMEDIAAPP, !isMediaApp);

                // Don't forget to commit your edits!!!
                editor.commit();
                //super.finish();
                return true;
            case MNU_CLOSESESSION:
                closeSession();
                return true;
            case MNU_APP_VERSION: {
                new AlertDialog.Builder(this)
                        .setTitle("App version")
                        .setMessage("Ver:" + AppUtils.getApplicationVersion() +
                                ", Code:" + String.valueOf(AppUtils.getCodeVersionNumber()) +
                                "\n" + AppUtils.getBuildInfo() + "\n\nCHANGELOG:\n" + AppUtils.getChangeLog())
                        .setNeutralButton(android.R.string.ok, null).create().show();
                break;
            }
            case MNU_CLEAR_FUNCTIONS_USAGE:
                clearMessageSelectCount();
                break;
            case MNU_WAKELOCK:
                AppPreferencesManager.toggleDisableLock();
                break;
            case MNU_HASH_ID_SETUP:
                DialogFragment hashIdSetUpDialog = HashIdSetUpDialog.newInstance();
                hashIdSetUpDialog.show(getFragmentManager(), HASH_ID_SET_UP_DIALOG_TAG);
                break;
            case MNU_FEEDBACK:
                DialogFragment feedbackDialog = FeedbackDialog.newInstance();
                feedbackDialog.show(getFragmentManager(), FEEDBACK_DIALOG_TAG);
                break;
        }

        return false;
    }

    private void closeSession() {
        if (mBoundProxyService != null) {
            mBoundProxyService.setCloseSessionCallback(new ICloseSession() {
                @Override
                public void onCloseSessionComplete() {
                    finishActivity();
                }
            });
            mBoundProxyService.syncProxyCloseSession(true);
        }
    }

    private void finishActivity() {
        super.finish();
    }

    private void xmlTest() {
        openXmlFilePathDialog();
    }

    private void xmlTestContinue(String filePath) {
        if (mTesterMain != null) {
            SafeToast.showToastAnyThread("start your engines");
        } else {
            mBoundProxyService.startModuleTest();
            SafeToast.showToastAnyThread("Start the app on SYNC first");
        }
        mTesterMain.restart(filePath);
    }

/*	public void startSyncProxyService() {
        // Get the local Bluetooth adapter
        BluetoothAdapter mBtAdapter = BluetoothAdapter.getDefaultAdapter();

        //BT Adapter exists, is enabled, and there are paired devices with the name SYNC
		//Ideally start service and start proxy if already connected to sync
		//but, there is no way to tell if a device is currently connected (pre OS 4.0)
        
        if (mBtAdapter != null)
		{
			if ((mBtAdapter.isEnabled() && mBtAdapter.getBondedDevices().isEmpty() != true)) 
			{
				// Get a set of currently paired devices
				Set<BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();
		
				boolean isSYNCpaired = false;
				// Check if there is a paired device with the name "SYNC"
		        if (pairedDevices.size() > 0) {
		            for (BluetoothDevice device : pairedDevices) {
		               if (device.getName().toString().equals("SYNC")) {
		            	   isSYNCpaired  = true;
		            	   break;
		               }
		            }
		        } else {
		        	Logger.i("TAG", "A No Paired devices with the name sync");
		        }
		        
		        if (isSYNCpaired == true) { 		        	
		        	_applinkService = new ProxyService();
		    		if (ProxyService.getInstance() == null) {
		    			Intent startIntent = new Intent(this, ProxyService.class);
		    			startService(startIntent);
		    			//bindService(startIntent, this, Context.BIND_AUTO_CREATE);
		    		} else {
		    			// need to get the instance and add myself as a listener
		    			ProxyService.getInstance().setLogAdapter(this);
		    		}
		        }
			}
		}
	}*/

    /**
     * Adds the function name to the adapter.
     */
    private void fillFunctionsAdapter() {
        adapter = new ArrayAdapter<String>(this, android.R.layout.select_dialog_item);

        adapter.add(Names.Alert);
        adapter.add(Names.Speak);
        adapter.add(Names.Show);
        adapter.add(ButtonSubscriptions);
        adapter.add(Names.AddCommand);
        adapter.add(Names.DeleteCommand);
        adapter.add(Names.AddSubMenu);
        adapter.add(Names.DeleteSubMenu);
        adapter.add(Names.SetGlobalProperties);
        adapter.add(Names.ResetGlobalProperties);
        adapter.add(Names.SetMediaClockTimer);
        adapter.add(Names.CreateInteractionChoiceSet);
        adapter.add(Names.DeleteInteractionChoiceSet);
        adapter.add(Names.PerformInteraction);
        adapter.add(Names.EncodedSyncPData);
        adapter.add(Names.SyncPData);
        adapter.add(Names.Slider);
        adapter.add(Names.ScrollableMessage);
        adapter.add(Names.ChangeRegistration);
        adapter.add(Names.PutFile);
        adapter.add(Names.DeleteFile);
        adapter.add(Names.ListFiles);
        adapter.add(Names.SetAppIcon);
        adapter.add(Names.PerformAudioPassThru);
        adapter.add(Names.EndAudioPassThru);
        adapter.add(VehicleDataSubscriptions);
        adapter.add(Names.GetVehicleData);
        adapter.add(Names.ReadDID);
        adapter.add(Names.GetDTCs);
        adapter.add(Names.ShowConstantTBT);
        adapter.add(Names.AlertManeuver);
        adapter.add(Names.UpdateTurnList);
        adapter.add(Names.SetDisplayLayout);
        adapter.add(Names.DiagnosticMessage);
        adapter.add(Names.RegisterAppInterface);
        adapter.add(Names.UnregisterAppInterface);
        adapter.add(Names.SystemRequest);
        adapter.add(GenericRequest.NAME);

        loadMessageSelectCount();
    }

    /**
     * @return array adapter with a function names to perform message sending operations
     */
    public ArrayAdapter<String> getFunctionsAdapter() {
        return adapter;
    }

    public void onClick(View v) {
        if (v == findViewById(R.id.btnSendMessage)) {
            if (adapter == null) {
                fillFunctionsAdapter();
            }
            adapter.sort(new Comparator<String>() {
                @Override
                public int compare(String lhs, String rhs) {
                    // compare based on the number of selections so far
                    Integer lCount = messageSelectCount.get(lhs);
                    if (lCount == null) {
                        lCount = 0;
                    }
                    Integer rCount = messageSelectCount.get(rhs);
                    if (rCount == null) {
                        rCount = 0;
                    }
                    return rCount - lCount;
                }
            });

            new AlertDialog.Builder(this)
                    .setTitle("Pick a Function")
                    .setAdapter(adapter, new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            if (adapter.getItem(which).equals(Names.Alert)) {
                                sendAlert();
                            } else if (adapter.getItem(which).equals(Names.Speak)) {
                                //something
                                AlertDialog.Builder builder;
                                AlertDialog dlg;

                                Context mContext = adapter.getContext();
                                LayoutInflater inflater = (LayoutInflater) mContext
                                        .getSystemService(LAYOUT_INFLATER_SERVICE);
                                View layout = inflater.inflate(R.layout.speak, null);
                                final EditText txtSpeakText1 = (EditText) layout.findViewById(R.id.txtSpeakText1);
                                final EditText txtSpeakText2 = (EditText) layout.findViewById(R.id.txtSpeakText2);
                                final EditText txtSpeakText3 = (EditText) layout.findViewById(R.id.txtSpeakText3);
                                final EditText txtSpeakText4 = (EditText) layout.findViewById(R.id.txtSpeakText4);

                                final Spinner spnSpeakType1 = (Spinner) layout.findViewById(R.id.spnSpeakType1);
                                final Spinner spnSpeakType2 = (Spinner) layout.findViewById(R.id.spnSpeakType2);
                                final Spinner spnSpeakType3 = (Spinner) layout.findViewById(R.id.spnSpeakType3);
                                final Spinner spnSpeakType4 = (Spinner) layout.findViewById(R.id.spnSpeakType4);

                                ArrayAdapter<SpeechCapabilities> speechSpinnerAdapter = new ArrayAdapter<SpeechCapabilities>(
                                        adapter.getContext(), android.R.layout.simple_spinner_item, SpeechCapabilities.values());
                                int textCapabilityPos = speechSpinnerAdapter.getPosition(SpeechCapabilities.TEXT);
                                spnSpeakType1.setAdapter(speechSpinnerAdapter);
                                spnSpeakType1.setSelection(textCapabilityPos);
                                spnSpeakType2.setAdapter(speechSpinnerAdapter);
                                spnSpeakType2.setSelection(textCapabilityPos);
                                spnSpeakType3.setAdapter(speechSpinnerAdapter);
                                spnSpeakType3.setSelection(textCapabilityPos);
                                spnSpeakType4.setAdapter(speechSpinnerAdapter);
                                spnSpeakType4.setSelection(textCapabilityPos);

                                builder = new AlertDialog.Builder(mContext);
                                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        Speak msg = new Speak();
                                        msg.setCorrelationID(getCorrelationid());
                                        String speak1 = txtSpeakText1.getText().toString();
                                        String speak2 = txtSpeakText2.getText().toString();
                                        String speak3 = txtSpeakText3.getText().toString();
                                        String speak4 = txtSpeakText4.getText().toString();
                                        Vector<TTSChunk> chunks = new Vector<TTSChunk>();

                                        if (speak1.length() > 0) {
                                            chunks.add(TTSChunkFactory.createChunk((SpeechCapabilities) spnSpeakType1.getSelectedItem(), speak1));
                                        }
                                        if (speak2.length() > 0) {
                                            chunks.add(TTSChunkFactory.createChunk((SpeechCapabilities) spnSpeakType2.getSelectedItem(), speak2));
                                        }
                                        if (speak3.length() > 0) {
                                            chunks.add(TTSChunkFactory.createChunk((SpeechCapabilities) spnSpeakType3.getSelectedItem(), speak3));
                                        }
                                        if (speak4.length() > 0) {
                                            chunks.add(TTSChunkFactory.createChunk(SpeechCapabilities.SAPI_PHONEMES, speak4));
                                        }
                                        msg.setTtsChunks(chunks);
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    }
                                });
                                builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        dialog.cancel();
                                    }
                                });
                                builder.setView(layout);
                                dlg = builder.create();
                                dlg.show();
                            } else if (adapter.getItem(which).equals(Names.Show)) {
                                sendShow();
                            } else if (adapter.getItem(which).equals(ButtonSubscriptions)) {
                                //something
                                AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
                                builder.setAdapter(mButtonAdapter, new DialogInterface.OnClickListener() {

                                    public void onClick(DialogInterface dialog, int which) {
                                        boolean needToSubscribe = !isButtonSubscribed[which];
                                        ButtonName buttonName = ButtonName.values()[which];
                                        int corrId = getCorrelationid();
                                        if (needToSubscribe) {
                                            if (mBoundProxyService != null) {
                                                mBoundProxyService.commandSubscribeButtonResumable(
                                                        buttonName, corrId);
                                            }
                                        } else {
                                            UnsubscribeButton msg = new UnsubscribeButton();
                                            msg.setCorrelationID(corrId);
                                            msg.setButtonName(buttonName);
                                            mLogAdapter.logMessage(msg, true);
                                            if (mBoundProxyService != null) {
                                                mBoundProxyService.syncProxySendRPCRequest(msg);
                                            }
                                        }
                                        isButtonSubscribed[which] = !isButtonSubscribed[which];
                                    }
                                });
                                AlertDialog dlg = builder.create();
                                dlg.show();
                            } else if (adapter.getItem(which).equals(Names.AddCommand)) {
                                sendAddCommand();
                            } else if (adapter.getItem(which).equals(Names.SystemRequest)) {
                                sendSystemRequest();
                            } else if (adapter.getItem(which).equals(Names.DeleteCommand)) {
                                sendDeleteCommand();
                            } else if (adapter.getItem(which).equals(Names.AddSubMenu)) {
                                sendAddSubmenu();
                            } else if (adapter.getItem(which).equals(Names.DeleteSubMenu)) {
                                sendDeleteSubMenu();
                            } else if (adapter.getItem(which).equals(Names.SetGlobalProperties)) {
                                sendSetGlobalProperties();
                            } else if (adapter.getItem(which).equals(Names.ResetGlobalProperties)) {
                                sendResetGlobalProperties();
                            } else if (adapter.getItem(which).equals(Names.SetMediaClockTimer)) {
                                sendSetMediaClockTimer();
                            } else if (adapter.getItem(which).equals(Names.CreateInteractionChoiceSet)) {
                                sendCreateInteractionChoiceSet();
                            } else if (adapter.getItem(which).equals(Names.DeleteInteractionChoiceSet)) {
                                //something
                                AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
                                builder.setAdapter(mChoiceSetAdapter, new DialogInterface.OnClickListener() {

                                    public void onClick(DialogInterface dialog, int which) {
                                        DeleteInteractionChoiceSet msg = new DeleteInteractionChoiceSet();
                                        msg.setCorrelationID(getCorrelationid());
                                        int commandSetID = mChoiceSetAdapter.getItem(which);
                                        msg.setInteractionChoiceSetID(commandSetID);
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                        if (_latestDeleteChoiceSetId != CHOICESETID_UNSET) {
                                            Logger.w("Latest deleteChoiceSetId should be unset, but equals to " + _latestDeleteChoiceSetId);
                                        }
                                        _latestDeleteChoiceSetId = commandSetID;
                                    }
                                });
                                AlertDialog dlg = builder.create();
                                dlg.show();
                            } else if (adapter.getItem(which).equals(Names.PerformInteraction)) {
                                sendPerformInteraction();
                            } else if (adapter.getItem(which).equals(Names.EncodedSyncPData)) {
                                sendSyncPData(true);
                            } else if (adapter.getItem(which).equals(Names.SyncPData)) {
                                sendSyncPData(false);
                            } else if (adapter.getItem(which).equals(Names.Slider)) {
                                sendSlider();
                            } else if (adapter.getItem(which).equals(Names.ScrollableMessage)) {
                                //something
                                AlertDialog.Builder builder;
                                AlertDialog dlg;

                                final Context mContext = adapter.getContext();
                                LayoutInflater inflater = (LayoutInflater) mContext
                                        .getSystemService(LAYOUT_INFLATER_SERVICE);
                                View layout = inflater.inflate(R.layout.scrollablemessage, null);
                                final EditText txtScrollableMessageBody = (EditText) layout.findViewById(R.id.txtScrollableMessageBody);
                                chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.chkIncludeSBs);
                                final EditText txtTimeout = (EditText) layout.findViewById(R.id.scrollablemessage_editTimeout);

                                SoftButton sb1 = new SoftButton();
                                sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                                sb1.setText("Reply");
                                sb1.setType(SoftButtonType.SBT_TEXT);
                                sb1.setIsHighlighted(false);
                                sb1.setSystemAction(SystemAction.STEAL_FOCUS);
                                SoftButton sb2 = new SoftButton();
                                sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                                sb2.setText("Close");
                                sb2.setType(SoftButtonType.SBT_TEXT);
                                sb2.setIsHighlighted(false);
                                sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
                                currentSoftButtons = new Vector<SoftButton>();
                                currentSoftButtons.add(sb1);
                                currentSoftButtons.add(sb2);

                                Button btnSoftButtons = (Button) layout.findViewById(R.id.scrollablemessage_btnSoftButtons);
                                btnSoftButtons.setOnClickListener(new OnClickListener() {
                                    @Override
                                    public void onClick(View v) {
                                        IntentHelper.addObjectForKey(currentSoftButtons,
                                                Const.INTENTHELPER_KEY_OBJECTSLIST);
                                        Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                        intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                                                SCROLLABLEMESSAGE_MAXSOFTBUTTONS);
                                        startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                                    }
                                });

                                builder = new AlertDialog.Builder(mContext);
                                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        ScrollableMessage msg = new ScrollableMessage();
                                        msg.setCorrelationID(getCorrelationid());
                                        try {
                                            msg.setTimeout(Integer.parseInt(txtTimeout.getText().toString()));
                                        } catch (NumberFormatException e) {
                                            // do nothing, leave the default timeout
                                        }
                                        msg.setScrollableMessageBody(txtScrollableMessageBody.getEditableText().toString());
                                        if (chkIncludeSoftButtons.isChecked() &&
                                                (currentSoftButtons != null) &&
                                                (currentSoftButtons.size() > 0)) {
                                            msg.setSoftButtons(currentSoftButtons);
                                        }
                                        currentSoftButtons = null;
                                        chkIncludeSoftButtons = null;
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    }
                                });
                                builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        currentSoftButtons = null;
                                        chkIncludeSoftButtons = null;
                                        dialog.cancel();
                                    }
                                });
                                builder.setView(layout);
                                dlg = builder.create();
                                dlg.show();
                            } else if (adapter.getItem(which).equals(Names.ChangeRegistration)) {
                                //ChangeRegistration
                                AlertDialog.Builder builder;
                                AlertDialog dlg;

                                Context mContext = adapter.getContext();
                                LayoutInflater inflater = (LayoutInflater) mContext
                                        .getSystemService(LAYOUT_INFLATER_SERVICE);
                                View layout = inflater.inflate(R.layout.changeregistration, null);

                                final Spinner spnLanguage = (Spinner) layout.findViewById(R.id.spnLanguage);
                                ArrayAdapter<Language> spinnerAdapterLanguage = new ArrayAdapter<Language>(adapter.getContext(),
                                        android.R.layout.simple_spinner_item, Language.values());
                                spinnerAdapterLanguage.setDropDownViewResource(
                                        android.R.layout.simple_spinner_dropdown_item);
                                spnLanguage.setAdapter(spinnerAdapterLanguage);

                                final Spinner spnHmiDisplayLanguage = (Spinner) layout.findViewById(R.id.spnHmiDisplayLanguage);
                                ArrayAdapter<Language> spinnerAdapterHmiDisplayLanguage = new ArrayAdapter<Language>(adapter.getContext(),
                                        android.R.layout.simple_spinner_item, Language.values());
                                spinnerAdapterHmiDisplayLanguage.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                                spnHmiDisplayLanguage.setAdapter(spinnerAdapterHmiDisplayLanguage);

                                builder = new AlertDialog.Builder(mContext);
                                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        ChangeRegistration msg = new ChangeRegistration();
                                        msg.setLanguage((Language) spnLanguage.getSelectedItem());
                                        msg.setHmiDisplayLanguage((Language) spnHmiDisplayLanguage.getSelectedItem());
                                        msg.setCorrelationID(getCorrelationid());
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    }
                                });
                                builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        dialog.cancel();
                                    }
                                });
                                builder.setView(layout);
                                dlg = builder.create();
                                dlg.show();
                            } else if (adapter.getItem(which).equals(Names.PutFile)) {
                                DialogFragment putFileDialogFragment = PutFileDialog.newInstance(getCorrelationid());
                                putFileDialogFragment.show(getFragmentManager(), PUT_FILE_DIALOG_TAG);
                            } else if (adapter.getItem(which).equals(Names.DeleteFile)) {
                                //DeleteFile
                                AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
                                builder.setAdapter(mPutFileAdapter, new DialogInterface.OnClickListener() {

                                    public void onClick(DialogInterface dialog, int which) {
                                        String syncFileName = mPutFileAdapter.getItem(which);
                                        DeleteFile msg = new DeleteFile();
                                        msg.setSyncFileName(syncFileName);
                                        msg.setCorrelationID(getCorrelationid());
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                        mPutFileAdapter.remove(syncFileName);
                                    }
                                });
                                AlertDialog dlg = builder.create();
                                dlg.show();
                            } else if (adapter.getItem(which).equals(Names.ListFiles)) {
                                //ListFiles
                                if (mBoundProxyService != null) {
                                    mBoundProxyService.commandListFiles();
                                }
                            } else if (adapter.getItem(which).equals(Names.SetAppIcon)) {
                                //SetAppIcon
                                AlertDialog.Builder builder;
                                AlertDialog dlg;

                                Context mContext = adapter.getContext();
                                LayoutInflater inflater = (LayoutInflater) mContext
                                        .getSystemService(LAYOUT_INFLATER_SERVICE);
                                View layout = inflater.inflate(R.layout.setappicon, null);

                                final EditText txtSyncFileName = (EditText) layout.findViewById(R.id.syncFileNameIcon);

                                builder = new AlertDialog.Builder(mContext);
                                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        String syncFileName = txtSyncFileName.getText().toString();
                                        SetAppIcon msg = new SetAppIcon();
                                        msg.setSyncFileName(syncFileName);
                                        msg.setCorrelationID(getCorrelationid());
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    }
                                });
                                builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        dialog.cancel();
                                    }
                                });
                                builder.setView(layout);
                                dlg = builder.create();
                                dlg.show();
                            } else if (adapter.getItem(which).equals(Names.PerformAudioPassThru)) {
                                DialogFragment performAudioPassThruDialog = PerformAudioPassThruDialog.newInstance();
                                performAudioPassThruDialog.show(getFragmentManager(), PERFORM_AUDIO_PASS_THRU_DIALOG_TAG);
                            } else if (adapter.getItem(which).equals(Names.EndAudioPassThru)) {
                                //EndAudioPassThru
                                EndAudioPassThru msg = new EndAudioPassThru();
                                msg.setCorrelationID(getCorrelationid());
                                if (mBoundProxyService != null) {
                                    mBoundProxyService.syncProxySendRPCRequest(msg);
                                }
                            } else if (adapter.getItem(which).equals(VehicleDataSubscriptions)) {
                                sendVehicleDataSubscriptions();
                            } else if (adapter.getItem(which).equals(Names.GetVehicleData)) {
                                sendGetVehicleData();
                            } else if (adapter.getItem(which).equals(Names.ReadDID)) {
                                //ReadDID
                                AlertDialog.Builder builder;
                                AlertDialog dlg;

                                final Context mContext = adapter.getContext();
                                LayoutInflater inflater = (LayoutInflater) mContext
                                        .getSystemService(LAYOUT_INFLATER_SERVICE);
                                View layout = inflater.inflate(R.layout.readdid, null);

                                final EditText txtECUNameDID = (EditText) layout.findViewById(R.id.txtECUNameDID);
                                final EditText txtDIDLocation = (EditText) layout.findViewById(R.id.txtDIDLocation);

                                builder = new AlertDialog.Builder(mContext);
                                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        try {
                                            Vector<Integer> didlocations = new Vector<Integer>();
                                            didlocations.add(Integer.parseInt(txtDIDLocation.getText().toString()));
                                            ReadDID msg = new ReadDID();
                                            msg.setEcuName(Integer.parseInt(txtECUNameDID.getText().toString()));
                                            msg.setDidLocation(didlocations);
                                            msg.setCorrelationID(getCorrelationid());
                                            if (mBoundProxyService != null) {
                                                mBoundProxyService.syncProxySendRPCRequest(msg);
                                            }
                                        } catch (NumberFormatException e) {
                                            SafeToast.showToastAnyThread("Couldn't parse number");
                                        }
                                    }
                                });
                                builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        dialog.cancel();
                                    }
                                });
                                builder.setView(layout);
                                dlg = builder.create();
                                dlg.show();
                            } else if (adapter.getItem(which).equals(Names.GetDTCs)) {
                                sendGetDTCs();
                            } else if (adapter.getItem(which).equals(Names.ShowConstantTBT)) {
                                sendShowConstantTBT();
                            } else if (adapter.getItem(which).equals(Names.AlertManeuver)) {
                                sendAlertManeuver();
                            } else if (adapter.getItem(which).equals(Names.UpdateTurnList)) {
                                sendUpdateTurnList();
                            } else if (adapter.getItem(which).equals(Names.SetDisplayLayout)) {
                                sendSetDisplayLayout();
                            } else if (adapter.getItem(which).equals(Names.UnregisterAppInterface)) {
                                sendUnregisterAppInterface();
                            } else if (adapter.getItem(which).equals(Names.RegisterAppInterface)) {
                                sendRegisterAppInterface();
                            } else if (adapter.getItem(which).equals(Names.DiagnosticMessage)) {
                                sendDiagnosticMessage();
                            } else if (adapter.getItem(which).equals(GenericRequest.NAME)) {
                                sendGenericRequest();
                            }

                            String function = adapter.getItem(which);
                            Integer curCount = messageSelectCount.get(function);
                            if (curCount == null) {
                                curCount = 0;
                            }
                            messageSelectCount.put(function, curCount + 1);

                            saveMessageSelectCount();
                        }

                        private void sendAlertManeuver() {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.alertmaneuver, null);

                            final EditText txtTtsChunks = (EditText) layout.findViewById(R.id.txtTtsChunks);
                            final CheckBox useSoftButtons = (CheckBox) layout.findViewById(R.id.alertManeuver_chkIncludeSBs);

                            SoftButton sb1 = new SoftButton();
                            sb1.setSoftButtonID(
                                    SyncProxyTester.getNewSoftButtonId());
                            sb1.setText("Reply");
                            sb1.setType(SoftButtonType.SBT_TEXT);
                            sb1.setIsHighlighted(false);
                            sb1.setSystemAction(SystemAction.STEAL_FOCUS);
                            SoftButton sb2 = new SoftButton();
                            sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                            sb2.setText("Close");
                            sb2.setType(SoftButtonType.SBT_TEXT);
                            sb2.setIsHighlighted(false);
                            sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
                            currentSoftButtons = new Vector<SoftButton>();
                            currentSoftButtons.add(sb1);
                            currentSoftButtons.add(sb2);

                            Button btnSoftButtons = (Button) layout.findViewById(R.id.alertManeuver_btnSoftButtons);
                            btnSoftButtons.setOnClickListener(new OnClickListener() {
                                @Override
                                public void onClick(View v) {
                                    IntentHelper.addObjectForKey(
                                            currentSoftButtons,
                                            Const.INTENTHELPER_KEY_OBJECTSLIST);
                                    Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                    intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                                            ALERTMANEUVER_MAXSOFTBUTTONS);
                                    startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                                }
                            });

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    Vector<TTSChunk> ttsChunks = new Vector<TTSChunk>();
                                    String ttsChunksString = txtTtsChunks.getText().toString();
                                    for (String ttsChunk : ttsChunksString.split(JOIN_STRING)) {
                                        TTSChunk chunk = TTSChunkFactory.createChunk(
                                                SpeechCapabilities.TEXT,
                                                ttsChunk);
                                        ttsChunks.add(chunk);
                                    }

                                    if (!ttsChunks.isEmpty()) {
                                        AlertManeuver msg = new AlertManeuver();
                                        msg.setTtsChunks(ttsChunks);
                                        msg.setCorrelationID(getCorrelationid());
                                        if (useSoftButtons.isChecked()) {
                                            if (currentSoftButtons != null) {
                                                msg.setSoftButtons(currentSoftButtons);
                                            } else {
                                                msg.setSoftButtons(new Vector<SoftButton>());
                                            }
                                        }
                                        currentSoftButtons = null;
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    } else {
                                        SafeToast.showToastAnyThread("No TTS Chunks entered");
                                    }
                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    currentSoftButtons = null;
                                    dialog.cancel();
                                }
                            });
                            builder.setView(layout);
                            builder.show();
                        }

                        /**
                         * Opens the UI for DeleteCommand and sends it.
                         */
                        private void sendDeleteCommand() {
                            AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
                            builder.setAdapter(mCommandAdapter, new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    DeleteCommand msg = new DeleteCommand();
                                    msg.setCorrelationID(getCorrelationid());
                                    int cmdID = mCommandAdapter.getItem(which);
                                    msg.setCmdID(cmdID);
                                    if (mBoundProxyService != null) {
                                        mBoundProxyService.syncProxySendRPCRequest(msg);
                                    }

                                    if (_latestDeleteCommandCmdID != null) {
                                        Logger.w("Latest deleteCommand should be null, but it is " +
                                                        _latestDeleteCommandCmdID);
                                    }
                                    _latestDeleteCommandCmdID = cmdID;
                                }
                            });
                            builder.show();
                        }

                        /**
                         * Sends Alert message.
                         */
                        private void sendAlert() {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.alert, null);
                            final EditText txtSpeak = (EditText) layout.findViewById(R.id.txtSpeak);
                            final EditText txtAlertField1 = (EditText) layout.findViewById(R.id.txtAlertField1);
                            final EditText txtAlertField2 = (EditText) layout.findViewById(R.id.txtAlertField2);
                            final EditText txtAlertField3 = (EditText) layout.findViewById(R.id.txtAlertField3);
                            final EditText txtDuration = (EditText) layout.findViewById(R.id.txtDuration);
                            final CheckBox chkPlayTone = (CheckBox) layout.findViewById(R.id.chkPlayTone);
                            final CheckBox useProgressIndicator = (CheckBox) layout.findViewById(R.id.alert_useProgressIndicator);
                            final CheckBox useDuration = (CheckBox) layout.findViewById(R.id.alert_useDuration);

                            chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.chkIncludeSBs);

                            SoftButton sb1 = new SoftButton();
                            sb1.setSoftButtonID(
                                    SyncProxyTester.getNewSoftButtonId());
                            sb1.setText("ReRoute");
                            sb1.setType(SoftButtonType.SBT_TEXT);
                            sb1.setIsHighlighted(false);
                            sb1.setSystemAction(SystemAction.STEAL_FOCUS);
                            SoftButton sb2 = new SoftButton();
                            sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                            sb2.setText("Close");
                            sb2.setType(SoftButtonType.SBT_TEXT);
                            sb2.setIsHighlighted(false);
                            sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
                            currentSoftButtons = new Vector<SoftButton>();
                            currentSoftButtons.add(sb1);
                            currentSoftButtons.add(sb2);

                            Button btnSoftButtons = (Button) layout.findViewById(R.id.alert_btnSoftButtons);
                            btnSoftButtons.setOnClickListener(new OnClickListener() {
                                @Override
                                public void onClick(View v) {
                                    IntentHelper
                                            .addObjectForKey(currentSoftButtons,
                                                    Const.INTENTHELPER_KEY_OBJECTSLIST);
                                    Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                    intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER, ALERT_MAXSOFTBUTTONS);
                                    startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                                }
                            });

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    try {
                                        Alert msg = new Alert();
                                        msg.setCorrelationID(getCorrelationid());
                                        msg.setAlertText1(txtAlertField1.getText().toString());
                                        msg.setAlertText2(txtAlertField2.getText().toString());
                                        msg.setAlertText3(txtAlertField3.getText().toString());
                                        if (useDuration.isChecked()) {
                                            msg.setDuration(Integer.parseInt(txtDuration.getText().toString()));
                                        }
                                        msg.setPlayTone(chkPlayTone.isChecked());
                                        msg.setProgressIndicator(useProgressIndicator.isChecked());

                                        String toSpeak = txtSpeak.getText().toString();
                                        if (toSpeak.length() > 0) {
                                            Vector<TTSChunk> ttsChunks = TTSChunkFactory
                                                    .createSimpleTTSChunks(
                                                            toSpeak);
                                            msg.setTtsChunks(ttsChunks);
                                        }
                                        if (chkIncludeSoftButtons.isChecked() &&
                                                (currentSoftButtons != null) &&
                                                (currentSoftButtons.size() > 0)) {
                                            msg.setSoftButtons(currentSoftButtons);
                                        }
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    } catch (NumberFormatException e) {
                                        SafeToast.showToastAnyThread("Couldn't parse number");
                                    }
                                    currentSoftButtons = null;
                                    chkIncludeSoftButtons = null;
                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    currentSoftButtons = null;
                                    chkIncludeSoftButtons = null;
                                    dialog.cancel();
                                }
                            });
                            builder.setView(layout);
                            builder.show();
                        }

                        /**
                         * Sends UnregisterAppInterface message.
                         */
                        private void sendUnregisterAppInterface() {
                            UnregisterAppInterface unregisterAppInterface = new UnregisterAppInterface();
                            unregisterAppInterface.setCorrelationID(getCorrelationid());

                            SendSingleRPCRequestDialog sendSingleRPCRequestDialog = SendSingleRPCRequestDialog.newInstance();
                            sendSingleRPCRequestDialog.setCommand(unregisterAppInterface);
                            sendSingleRPCRequestDialog.show(getFragmentManager(), SEND_SINGLE_RPC_COMMAND_DIALOG_TAG);
                        }

                        /**
                         * Opens the dialog for DeleteSubMenu message and sends it.
                         */
                        private void sendDeleteSubMenu() {
                            AlertDialog.Builder builder =
                                    new AlertDialog.Builder(adapter.getContext());
                            builder.setAdapter(mSubmenuAdapter,
                                    new DialogInterface.OnClickListener() {
                                        @Override
                                        public void onClick(DialogInterface dialog,
                                                            int which) {
                                            SyncSubMenu menu =
                                                    mSubmenuAdapter.getItem(which);
                                            DeleteSubMenu msg = new DeleteSubMenu();
                                            msg.setCorrelationID(getCorrelationid());
                                            msg.setMenuID(menu.getSubMenuId());
                                            if (mBoundProxyService != null) {
                                                mBoundProxyService.syncProxySendRPCRequest(msg);
                                            }

                                            if (_latestDeleteSubmenu != null) {
                                                Logger.w("Latest deleteSubmenu should be null, but equals to " + _latestDeleteSubmenu);
                                            }
                                            _latestDeleteSubmenu = menu;
                                        }
                                    });
                            builder.show();
                        }

                        /**
                         * Opens the dialog for GetVehicleData message and sends it.
                         */
                        private void sendGetVehicleData() {
                            AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
                            builder.setAdapter(_vehicleDataType, new DialogInterface.OnClickListener() {

                                public void onClick(DialogInterface dialog, int which) {
                                    GetVehicleData msg = new GetVehicleData();

                                    final String[] methodNames =
                                            {"Gps", "Speed", "Rpm",
                                                    "FuelLevel",
                                                    "FuelLevel_State",
                                                    "InstantFuelConsumption",
                                                    "ExternalTemperature",
                                                    "Vin", "Prndl",
                                                    "TirePressure",
                                                    "Odometer",
                                                    "BeltStatus",
                                                    "BodyInformation",
                                                    "DeviceStatus",
                                                    "DriverBraking",
                                                    "WiperStatus",
                                                    "HeadLampStatus",
                                                    "BatteryVoltage",
                                                    "EngineTorque",
                                                    "AccPedalPosition",
                                                    "SteeringWheelAngle",
                                                    "ECallInfo",
                                                    "AirbagStatus",
                                                    "EmergencyEvent",
                                                    "ClusterModeStatus",
                                                    "MyKey"};
                                    final String setterName = "set" + methodNames[which];
                                    setVehicleDataParam(msg, GetVehicleData.class, setterName);

                                    msg.setCorrelationID(getCorrelationid());
                                    if (mBoundProxyService != null) {
                                        mBoundProxyService.syncProxySendRPCRequest(msg);
                                    }
                                }
                            });
                            builder.show();
                        }

                        /**
                         * Opens the dialog for GetDTCs message and sends it.
                         */
                        private void sendGetDTCs() {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.getdtcs, null);

                            final EditText txtECUNameDTC = (EditText) layout.findViewById(R.id.getdtcs_txtECUNameDTC);
                            final EditText txtdtcMask = (EditText) layout.findViewById(R.id.getdtcs_dtcMask);

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    try {
                                        GetDTCs msg = new GetDTCs();
                                        msg.setEcuName(Integer.parseInt(txtECUNameDTC.getText().toString()));
                                        msg.setDTCMask(Integer.parseInt(txtdtcMask.getText().toString()));
                                        msg.setCorrelationID(getCorrelationid());
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    } catch (NumberFormatException e) {
                                        SafeToast.showToastAnyThread("Couldn't parse number");
                                    }
                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    dialog.cancel();
                                }
                            });
                            builder.setView(layout);
                            builder.show();
                        }

                        /**
                         * Opens the dialog for AddSubMenu message and sends it.
                         */
                        private void sendAddSubmenu() {
                            DialogFragment addSubMenuDialogFragment = AddSubMenuDialog.newInstance();
                            addSubMenuDialogFragment.show(getFragmentManager(), ADD_SUB_MENU_DIALOG_TAG);
                        }

                        private void sendSystemRequest(){
                            DialogFragment fragment = SystemRequestDialog.newInstance();
                            fragment.show(getFragmentManager(), SYSTEM_REQST_DIALOG_TAG);
                        }

                        /**
                         * Opens the dialog for AddCommand message and sends it.
                         */
                        private void sendAddCommand() {
                            DialogFragment addCommandDialogFragment = AddCommandDialog.newInstance();
                            addCommandDialogFragment.show(getFragmentManager(), ADD_COMMAND_DIALOG_TAG);
                        }

                        /**
                         * Opens the dialog for SyncPData or EncodedSyncPData message and sends it.
                         * @param sendEncoded true to send EncodedSyncPData message; SyncPData otherwise
                         */
                        private void sendSyncPData(final Boolean sendEncoded) {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.encodedsyncpdata, null);

                            txtLocalFileName = (EditText) layout.findViewById(R.id.encodedsyncpdata_localFileName);
                            final Button btnSelectLocalFile = (Button) layout.findViewById(R.id.encodedsyncpdata_selectFileButton);
                            btnSelectLocalFile.setOnClickListener(new OnClickListener() {
                                @Override
                                public void onClick(View v) {
                                    // show Choose File dialog
                                    Intent intent = new Intent(mContext, FileDialog.class);
                                    intent.putExtra(FileDialog.START_PATH, "/sdcard");
                                    intent.putExtra(FileDialog.CAN_SELECT_DIR, false);
                                    intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
                                    startActivityForResult(intent, Const.REQUEST_FILE_OPEN);
                                }
                            });

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int id) {
                                    String filename = txtLocalFileName.getText().toString();
                                    byte[] data = AppUtils.contentsOfResource(filename);
                                    if (data != null) {
                                        RPCRequest request = null;
                                        if (sendEncoded) {
                                            String base64Data = Base64.encodeBytes(data);
                                            EncodedSyncPData msg = new EncodedSyncPData();
                                            Vector<String> syncPData = new Vector<String>();
                                            syncPData.add(base64Data);
                                            msg.setData(syncPData);
                                            msg.setCorrelationID(getCorrelationid());
                                            request = msg;
                                        } else {
                                            SyncPData msg = new SyncPData();
                                            msg.setCorrelationID(getCorrelationid());
                                            msg.setBulkData(data);
                                            request = msg;
                                        }
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(request);
                                        }
                                    } else {
                                        Toast.makeText(mContext, "Can't read data from file", Toast.LENGTH_LONG).show();
                                    }
                                    txtLocalFileName = null;
                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    txtLocalFileName = null;
                                    dialog.cancel();
                                }
                            });
                            builder.setView(layout);
                            builder.show();
                        }

                        /**
                         * Opens the dialog for ShowConstantTBT message and sends it.
                         */
                        private void sendShowConstantTBT() {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.showconstanttbt,
                                    (ViewGroup) findViewById(R.id.showconstanttbt_Root));

                            final CheckBox useNavigationText1 = (CheckBox) layout.findViewById(R.id.showconstanttbt_useNavigationText1);
                            final EditText txtNavigationText1 = (EditText) layout.findViewById(R.id.showconstanttbt_txtNavigationText1);
                            final CheckBox useNavigationText2 = (CheckBox) layout.findViewById(R.id.showconstanttbt_useNavigationText2);
                            final EditText txtNavigationText2 = (EditText) layout.findViewById(R.id.showconstanttbt_txtNavigationText2);
                            final CheckBox useETA = (CheckBox) layout.findViewById(R.id.showconstanttbt_useETA);
                            final EditText txtEta = (EditText) layout.findViewById(R.id.showconstanttbt_txtEta);
                            final CheckBox useTimeToDestination = (CheckBox) layout.findViewById(R.id.showconstanttbt_useTimeToDestination);
                            final EditText txtTimeToDestination = (EditText) layout.findViewById(R.id.showconstanttbt_txtTimeToDestination);
                            final CheckBox useTotalDistance = (CheckBox) layout.findViewById(R.id.showconstanttbt_useTotalDistance);
                            final EditText txtTotalDistance = (EditText) layout.findViewById(R.id.showconstanttbt_txtTotalDistance);
                            final CheckBox chkUseTurnIcon = (CheckBox) layout.findViewById(R.id.showconstanttbt_turnIconCheck);
                            final Spinner spnTurnIconType = (Spinner) layout.findViewById(R.id.showconstanttbt_turnIconType);
                            final EditText txtTurnIconValue = (EditText) layout.findViewById(R.id.showconstanttbt_turnIconValue);
                            final CheckBox chkUseNextTurnIcon = (CheckBox) layout.findViewById(R.id.showconstanttbt_nextTurnIconCheck);
                            final Spinner spnNextTurnIconType = (Spinner) layout.findViewById(R.id.showconstanttbt_nextTurnIconType);
                            final EditText txtNextTurnIconValue = (EditText) layout.findViewById(R.id.showconstanttbt_nextTurnIconValue);
                            final CheckBox useDistanceToManeuver = (CheckBox) layout.findViewById(R.id.showconstanttbt_useDistanceToManeuver);
                            final EditText txtDistanceToManeuver = (EditText) layout.findViewById(R.id.showconstanttbt_txtDistanceToManeuver);
                            final CheckBox useDistanceToManeuverScale = (CheckBox) layout.findViewById(R.id.showconstanttbt_useDistanceToManeuverScale);
                            final EditText txtDistanceToManeuverScale = (EditText) layout.findViewById(R.id.showconstanttbt_txtDistanceToManeuverScale);
                            final CheckBox useManeuverComplete = (CheckBox) layout.findViewById(R.id.showconstanttbt_useManeuverComplete);
                            final CheckBox chkManeuverComplete = (CheckBox) layout.findViewById(R.id.showconstanttbt_chkManeuverComplete);
                            final CheckBox useSoftButtons = (CheckBox) layout.findViewById(R.id.showconstanttbt_useSoftButtons);

                            spnTurnIconType.setAdapter(imageTypeAdapter);
                            spnTurnIconType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));

                            spnNextTurnIconType.setAdapter(imageTypeAdapter);
                            spnNextTurnIconType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));

                            SoftButton sb1 = new SoftButton();
                            sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                            sb1.setText("Reply");
                            sb1.setType(SoftButtonType.SBT_TEXT);
                            sb1.setIsHighlighted(false);
                            sb1.setSystemAction(SystemAction.STEAL_FOCUS);
                            SoftButton sb2 = new SoftButton();
                            sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                            sb2.setText("Close");
                            sb2.setType(SoftButtonType.SBT_TEXT);
                            sb2.setIsHighlighted(false);
                            sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
                            currentSoftButtons = new Vector<SoftButton>();
                            currentSoftButtons.add(sb1);
                            currentSoftButtons.add(sb2);

                            Button btnSoftButtons = (Button) layout.findViewById(R.id.showconstanttbt_btnSoftButtons);
                            btnSoftButtons.setOnClickListener(new OnClickListener() {
                                @Override
                                public void onClick(View v) {
                                    IntentHelper.addObjectForKey(currentSoftButtons,
                                            Const.INTENTHELPER_KEY_OBJECTSLIST);
                                    Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                    intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                                            SHOWCONSTANTTBT_MAXSOFTBUTTONS);
                                    startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                                }
                            });

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    try {
                                        ShowConstantTBT msg = new ShowConstantTBT();

                                        if (useNavigationText1.isChecked()) {
                                            msg.setNavigationText1(txtNavigationText1.getText().toString());
                                        }
                                        if (useNavigationText2.isChecked()) {
                                            msg.setNavigationText2(txtNavigationText2.getText().toString());
                                        }
                                        if (useETA.isChecked()) {
                                            msg.setEta(txtEta.getText().toString());
                                        }
                                        if (useTimeToDestination.isChecked()) {
                                            msg.setTimeToDestination(txtTimeToDestination.getText().toString());
                                        }
                                        if (useTotalDistance.isChecked()) {
                                            msg.setTotalDistance(txtTotalDistance.getText().toString());
                                        }

                                        if (chkUseTurnIcon.isChecked()) {
                                            Image image = new Image();
                                            image.setImageType(imageTypeAdapter.getItem(
                                                    spnTurnIconType.getSelectedItemPosition()));
                                            image.setValue(txtTurnIconValue.getText().toString());
                                            msg.setTurnIcon(image);
                                        }

                                        if (chkUseNextTurnIcon.isChecked()) {
                                            Image image = new Image();
                                            image.setImageType(imageTypeAdapter.getItem(
                                                    spnNextTurnIconType.getSelectedItemPosition()));
                                            image.setValue(txtNextTurnIconValue.getText().toString());
                                            msg.setNextTurnIcon(image);
                                        }

                                        if (useDistanceToManeuver.isChecked()) {
                                            msg.setDistanceToManeuver((float) Integer.parseInt(txtDistanceToManeuver.getText().toString()));
                                        }
                                        if (useDistanceToManeuverScale.isChecked()) {
                                            msg.setDistanceToManeuverScale((float) Integer.parseInt(txtDistanceToManeuverScale.getText().toString()));
                                        }
                                        if (useManeuverComplete.isChecked()) {
                                            msg.setManeuverComplete(chkManeuverComplete.isChecked());
                                        }
                                        msg.setCorrelationID(getCorrelationid());
                                        if (useSoftButtons.isChecked()) {
                                            if (currentSoftButtons != null) {
                                                msg.setSoftButtons(currentSoftButtons);
                                            } else {
                                                msg.setSoftButtons(new Vector<SoftButton>());
                                            }
                                        }
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    } catch (NumberFormatException e) {
                                        SafeToast.showToastAnyThread("Couldn't parse number");
                                    }
                                    currentSoftButtons = null;
                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    currentSoftButtons = null;
                                    dialog.cancel();
                                }
                            });
                            builder.setView(layout);
                            builder.show();
                        }

                        /**
                         * Opens the dialog for CreateInteractionChoiceSet message and sends it.
                         */
                        private void sendCreateInteractionChoiceSet() {
                            Choice choice1 = new Choice();
                            choice1.setChoiceID(getNewChoiceId());
                            choice1.setMenuName("super");
                            Vector<String> vrCommands = new Vector<String>();
                            vrCommands.add("super");
                            vrCommands.add("best");
                            choice1.setVrCommands(vrCommands);
                            Image image = new Image();
                            image.setImageType(ImageType.DYNAMIC);
                            image.setValue("turn_left.png");
                            choice1.setImage(image);
                            choice1.setSecondaryText("42");
                            choice1.setTertiaryText("The Cat");

                            Choice choice2 = new Choice();
                            choice2.setChoiceID(getNewChoiceId());
                            choice2.setMenuName("awesome");
                            vrCommands = new Vector<String>();
                            vrCommands.add("magnificent");
                            vrCommands.add("incredible");
                            choice2.setVrCommands(vrCommands);
                            image = new Image();
                            image.setImageType(ImageType.DYNAMIC);
                            image.setValue("action.png");
                            choice2.setImage(image);
                            choice2.setTertiaryText("Schrödinger's cat");

                            Vector<Choice> choices = new Vector<Choice>();
                            choices.add(choice1);
                            choices.add(choice2);

                            IntentHelper.addObjectForKey(choices,
                                    Const.INTENTHELPER_KEY_OBJECTSLIST);
                            Intent intent = new Intent(adapter.getContext(),
                                    ChoiceListActivity.class);
                            intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                                    CREATECHOICESET_MAXCHOICES);
                            startActivityForResult(intent, REQUEST_LIST_CHOICES);
                        }

                        /**
                         * Opens the dialog for Show message and sends it.
                         */
                        private void sendShow() {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.show, null);

                            final CheckBox mainField1Check = (CheckBox) layout.findViewById(R.id.show_mainField1Check);
                            final EditText mainField1 = (EditText) layout.findViewById(R.id.show_mainField1);
                            final CheckBox mainField2Check = (CheckBox) layout.findViewById(R.id.show_mainField2Check);
                            final EditText mainField2 = (EditText) layout.findViewById(R.id.show_mainField2);
                            final CheckBox mainField3Check = (CheckBox) layout.findViewById(R.id.show_mainField3Check);
                            final EditText mainField3 = (EditText) layout.findViewById(R.id.show_mainField3);
                            final CheckBox mainField4Check = (CheckBox) layout.findViewById(R.id.show_mainField4Check);
                            final EditText mainField4 = (EditText) layout.findViewById(R.id.show_mainField4);
                            final CheckBox textAlignmentCheck = (CheckBox) layout.findViewById(R.id.show_textAlignmentCheck);
                            final Spinner textAlignmentSpinner = (Spinner) layout.findViewById(R.id.show_textAlignmentSpinner);
                            final CheckBox statusBarCheck = (CheckBox) layout.findViewById(R.id.show_statusBarCheck);
                            final EditText statusBar = (EditText) layout.findViewById(R.id.show_statusBar);
                            final CheckBox mediaClockCheck = (CheckBox) layout.findViewById(R.id.show_mediaClockCheck);
                            final EditText mediaClock = (EditText) layout.findViewById(R.id.show_mediaClock);
                            final CheckBox mediaTrackCheck = (CheckBox) layout.findViewById(R.id.show_mediaTrackCheck);
                            final EditText mediaTrack = (EditText) layout.findViewById(R.id.show_mediaTrack);
                            final CheckBox graphicCheck = (CheckBox) layout.findViewById(R.id.show_graphicCheck);
                            final Spinner graphicType = (Spinner) layout.findViewById(R.id.show_graphicType);
                            final EditText graphic = (EditText) layout.findViewById(R.id.show_graphic);
                            final CheckBox secondaryGraphicCheck = (CheckBox) layout.findViewById(R.id.show_secondaryGraphicCheck);
                            final Spinner secondaryGraphicType = (Spinner) layout.findViewById(R.id.show_secondaryGraphicType);
                            final EditText secondaryGraphic = (EditText) layout.findViewById(R.id.show_secondaryGraphic);
                            chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.show_chkIncludeSBs);
                            final Button softButtons = (Button) layout.findViewById(R.id.show_btnSoftButtons);
                            final CheckBox customPresetsCheck = (CheckBox) layout.findViewById(R.id.show_customPresetsCheck);
                            final EditText customPresets = (EditText) layout.findViewById(R.id.show_customPresets);

                            final ArrayAdapter<TextAlignment> textAlignmentAdapter = new ArrayAdapter<TextAlignment>(
                                    mContext, android.R.layout.simple_spinner_item, TextAlignment.values());
                            textAlignmentAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                            textAlignmentSpinner.setAdapter(textAlignmentAdapter);
                            textAlignmentSpinner.setSelection(textAlignmentAdapter.getPosition(TextAlignment.CENTERED));

                            final boolean isMedia = getIsMedia();

                            if (!isMedia) {
                                int visibility = View.GONE;
                                mediaClock.setVisibility(visibility);
                                mediaTrack.setVisibility(visibility);
                                mediaTrackCheck.setVisibility(visibility);
                                mediaClockCheck.setVisibility(visibility);
                            }

                            graphicType.setAdapter(imageTypeAdapter);
                            graphicType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));
                            secondaryGraphicType.setAdapter(imageTypeAdapter);
                            secondaryGraphicType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));

                            SoftButton sb1 = new SoftButton();
                            sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                            sb1.setText("KeepContext");
                            sb1.setType(SoftButtonType.SBT_TEXT);
                            sb1.setIsHighlighted(false);
                            sb1.setSystemAction(SystemAction.KEEP_CONTEXT);
                            SoftButton sb2 = new SoftButton();
                            sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                            sb2.setText("StealFocus");
                            sb2.setType(SoftButtonType.SBT_TEXT);
                            sb2.setIsHighlighted(false);
                            sb2.setSystemAction(SystemAction.STEAL_FOCUS);
                            SoftButton sb3 = new SoftButton();
                            sb3.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                            sb3.setText("Default");
                            sb3.setType(SoftButtonType.SBT_TEXT);
                            sb3.setIsHighlighted(false);
                            sb3.setSystemAction(SystemAction.DEFAULT_ACTION);
                            currentSoftButtons = new Vector<SoftButton>();
                            currentSoftButtons.add(sb1);
                            currentSoftButtons.add(sb2);
                            currentSoftButtons.add(sb3);

                            Button btnSoftButtons = (Button) layout.findViewById(R.id.show_btnSoftButtons);
                            btnSoftButtons.setOnClickListener(new OnClickListener() {
                                @Override
                                public void onClick(View v) {
                                    IntentHelper
                                            .addObjectForKey(currentSoftButtons, Const.INTENTHELPER_KEY_OBJECTSLIST);
                                    Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                    intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER, SHOW_MAXSOFTBUTTONS);
                                    startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                                }
                            });

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    Show msg = new Show();
                                    msg.setCorrelationID(getCorrelationid());

                                    if (mainField1Check.isChecked()) {
                                        msg.setMainField1(mainField1.getText().toString());
                                    }
                                    if (mainField2Check.isChecked()) {
                                        msg.setMainField2(mainField2.getText().toString());
                                    }
                                    if (mainField3Check.isChecked()) {
                                        msg.setMainField3(mainField3.getText().toString());
                                    }
                                    if (mainField4Check.isChecked()) {
                                        msg.setMainField4(mainField4.getText().toString());
                                    }
                                    if (textAlignmentCheck.isChecked()) {
                                        msg.setAlignment(textAlignmentAdapter.getItem(textAlignmentSpinner.getSelectedItemPosition()));
                                    }
                                    if (statusBarCheck.isChecked()) {
                                        msg.setStatusBar(statusBar.getText().toString());
                                    }
                                    if (isMedia) {
                                        if (mediaClockCheck.isChecked()) {
                                            msg.setMediaClock(mediaClock.getText().toString());
                                        }
                                        if (mediaTrackCheck.isChecked()) {
                                            msg.setMediaTrack(mediaTrack.getText().toString());
                                        }
                                    }
                                    if (graphicCheck.isChecked()) {
                                        Image image = new Image();
                                        image.setImageType((ImageType) graphicType.getSelectedItem());
                                        image.setValue(graphic.getText().toString());
                                        msg.setGraphic(image);
                                    }
                                    if (secondaryGraphicCheck.isChecked()) {
                                        Image image = new Image();
                                        image.setImageType((ImageType) secondaryGraphicType.getSelectedItem());
                                        image.setValue(secondaryGraphic.getText().toString());
                                        msg.setSecondaryGraphic(image);
                                    }
                                    if (chkIncludeSoftButtons.isChecked() &&
                                            (currentSoftButtons != null) &&
                                            (currentSoftButtons.size() > 0)) {
                                        msg.setSoftButtons(currentSoftButtons);
                                    }
                                    currentSoftButtons = null;
                                    chkIncludeSoftButtons = null;
                                    if (customPresetsCheck.isChecked()) {
                                        String[] customPresetsList = customPresets.getText().
                                                toString().split(JOIN_STRING);
                                        msg.setCustomPresets(new Vector<String>(Arrays.
                                                asList(customPresetsList)));
                                    }
                                    if (mBoundProxyService != null) {
                                        mBoundProxyService.syncProxySendRPCRequest(msg);
                                    }
                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    currentSoftButtons = null;
                                    chkIncludeSoftButtons = null;
                                    dialog.cancel();
                                }
                            });
                            builder.setView(layout);
                            builder.show();
                        }

                        /**
                         * Opens the dialog for PerformInteraction message and sends it.
                         */
                        private void sendPerformInteraction() {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);

                            View layout = inflater.inflate(R.layout.performinteraction,
                                    (ViewGroup) findViewById(R.id.performinteraction_Root));

                            final EditText initialText = (EditText) layout.findViewById(R.id.performinteraction_initialText);
                            final EditText initialPrompt = (EditText) layout.findViewById(R.id.performinteraction_initialPrompt);
                            final Spinner interactionModeSpinner = (Spinner) layout.findViewById(R.id.performinteraction_interactionModeSpinner);
                            final Button choiceSetIDs = (Button) layout.findViewById(R.id.performinteraction_choiceSetIDs);
                            final CheckBox helpPromptCheck = (CheckBox) layout.findViewById(R.id.performinteraction_helpPromptCheck);
                            final EditText helpPrompt = (EditText) layout.findViewById(R.id.performinteraction_helpPrompt);
                            final CheckBox timeoutPromptCheck = (CheckBox) layout.findViewById(R.id.performinteraction_timeoutPromptCheck);
                            final EditText timeoutPrompt = (EditText) layout.findViewById(R.id.performinteraction_timeoutPrompt);
                            final CheckBox timeoutCheck = (CheckBox) layout.findViewById(R.id.performinteraction_timeoutCheck);
                            final EditText timeout = (EditText) layout.findViewById(R.id.performinteraction_timeout);
                            final CheckBox vrHelpItemCheck = (CheckBox) layout.findViewById(R.id.performinteraction_vrHelpItemCheck);
                            final EditText vrHelpItemPos = (EditText) layout.findViewById(R.id.performinteraction_vrHelpItemPos);
                            final EditText vrHelpItemText = (EditText) layout.findViewById(R.id.performinteraction_vrHelpItemText);
                            final EditText vrHelpItemImage = (EditText) layout.findViewById(R.id.performinteraction_vrHelpItemImage);
                            final CheckBox interactionLayoutCheck =
                                    (CheckBox) layout.findViewById(
                                            R.id.performinteraction_interactionLayoutCheck);
                            final Spinner interactionLayoutSpinner =
                                    (Spinner) layout.findViewById(
                                            R.id.performinteraction_interactionLayoutSpinner);

                            final ArrayAdapter<InteractionMode> interactionModeAdapter = new ArrayAdapter<InteractionMode>(
                                    mContext, android.R.layout.simple_spinner_item, InteractionMode.values());
                            interactionModeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                            interactionModeSpinner.setAdapter(interactionModeAdapter);
                            interactionModeSpinner.setSelection(interactionModeAdapter.getPosition(InteractionMode.BOTH));

                            final ArrayAdapter<LayoutMode>
                                    interactionLayoutAdapter =
                                    new ArrayAdapter<LayoutMode>(mContext,
                                            android.R.layout.simple_spinner_item,
                                            LayoutMode.values());
                            interactionLayoutAdapter.setDropDownViewResource(
                                    android.R.layout.simple_spinner_dropdown_item);
                            interactionLayoutSpinner
                                    .setAdapter(interactionLayoutAdapter);

                            final String[] choiceSetIDStrings = new String[mChoiceSetAdapter.getCount()];
                            final boolean[] choiceSetIDSelections = new boolean[choiceSetIDStrings.length];

                            for (int i = 0; i < mChoiceSetAdapter.getCount(); ++i) {
                                choiceSetIDStrings[i] = mChoiceSetAdapter.getItem(i).toString();
                            }

                            choiceSetIDs.setOnClickListener(new OnClickListener() {
                                @Override
                                public void onClick(View v) {
                                    new AlertDialog.Builder(mContext).
                                            setMultiChoiceItems(choiceSetIDStrings, choiceSetIDSelections, new OnMultiChoiceClickListener() {
                                                @Override
                                                public void onClick(DialogInterface dialog, int which, boolean isChecked) {
                                                }
                                            }).
                                            setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                                @Override
                                                public void onClick(DialogInterface dialog, int which) {
                                                    dialog.dismiss();
                                                }
                                            }).
                                            show();
                                }
                            });

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    // fail if no interaction choice set selected
                                    Vector<Integer> choiceSetIDs = new Vector<Integer>();
                                    for (int i = 0; i < choiceSetIDSelections.length; ++i) {
                                        if (choiceSetIDSelections[i]) {
                                            choiceSetIDs.add(mChoiceSetAdapter.getItem(i));
                                        }
                                    }
                                    sendPerformInteractionRequest(choiceSetIDs);
                                }

                                private void sendPerformInteractionRequest(Vector<Integer> choiceSetIDs) {

                                    PerformInteraction msg = new PerformInteraction();
                                    msg.setCorrelationID(getCorrelationid());
                                    msg.setInitialText(initialText.getText().toString());
                                    msg.setInitialPrompt(ttsChunksFromString(initialPrompt.getText().toString()));
                                    msg.setInteractionMode(
                                            interactionModeAdapter.getItem(
                                                    interactionModeSpinner
                                                            .getSelectedItemPosition()));
                                    msg.setInteractionChoiceSetIDList(choiceSetIDs);

                                    if (helpPromptCheck.isChecked()) {
                                        msg.setHelpPrompt(ttsChunksFromString(helpPrompt.getText().toString()));
                                    }

                                    if (timeoutPromptCheck.isChecked()) {
                                        msg.setTimeoutPrompt(ttsChunksFromString(timeoutPrompt.getText().toString()));
                                    }

                                    if (timeoutCheck.isChecked()) {
                                        try {
                                            msg.setTimeout(Integer.parseInt(timeout.getText().toString()));
                                        } catch (NumberFormatException e) {
                                            // set default timeout
                                            msg.setTimeout(10000);
                                        }
                                    }

                                    if (vrHelpItemCheck.isChecked()) {
                                        Vector<VrHelpItem> vrHelpItems = new Vector<VrHelpItem>();

                                        String[] itemTextArray = vrHelpItemText.getText().toString().split(JOIN_STRING);
                                        String[] itemPosArray = vrHelpItemPos.getText().toString().split(JOIN_STRING);
                                        String[] itemImageArray = vrHelpItemImage.getText().toString()
                                                .split(JOIN_STRING);
                                        int itemsCount = Math.min(itemTextArray.length,
                                                Math.min(itemPosArray.length, itemImageArray.length));

                                        for (int i = 0; i < itemsCount; ++i) {
                                            VrHelpItem item = new VrHelpItem();
                                            item.setText(itemTextArray[i]);

                                            try {
                                                item.setPosition(Integer.parseInt(itemPosArray[i]));
                                            } catch (NumberFormatException e) {
                                                // set default position
                                                item.setPosition(1);
                                            }

                                            Image image = new Image();
                                            image.setValue(itemImageArray[i]);
                                            image.setImageType(ImageType.DYNAMIC);
                                            item.setImage(image);

                                            vrHelpItems.add(item);
                                        }

                                        msg.setVrHelp(vrHelpItems);
                                    }

                                    if (interactionLayoutCheck.isChecked()) {
                                        msg.setInteractionLayout(interactionLayoutAdapter
                                                .getItem(interactionLayoutSpinner
                                                        .getSelectedItemPosition()));
                                    }
                                    if (mBoundProxyService != null) {
                                        mBoundProxyService.syncProxySendRPCRequest(msg);
                                    }

                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    dialog.cancel();
                                }
                            });

                            builder.setView(layout);
                            builder.show();
                        }

                        private void sendUpdateTurnList() {
                            AlertDialog.Builder builder;

                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.updateturnlist, null);
                            final EditText txtTurnList = (EditText) layout.findViewById(R.id.updateturnlist_txtTurnList);
                            final EditText txtIconList = (EditText) layout.findViewById(R.id.updateturnlist_txtIconList);
                            final CheckBox useTurnList = (CheckBox) layout.findViewById(R.id.updateturnlist_useTurnList);
                            final CheckBox useIconList = (CheckBox) layout.findViewById(R.id.updateturnlist_useIconList);
                            final CheckBox useSoftButtons = (CheckBox) layout.findViewById(R.id.updateturnlist_chkIncludeSBs);

                            SoftButton sb1 = new SoftButton();
                            sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                            sb1.setText("Close");
                            sb1.setType(SoftButtonType.SBT_TEXT);
                            sb1.setIsHighlighted(false);
                            sb1.setSystemAction(SystemAction.DEFAULT_ACTION);
                            currentSoftButtons = new Vector<SoftButton>();
                            currentSoftButtons.add(sb1);

                            Button btnSoftButtons = (Button) layout.findViewById(R.id.updateturnlist_btnSoftButtons);
                            btnSoftButtons.setOnClickListener(new OnClickListener() {
                                @Override
                                public void onClick(View v) {
                                    IntentHelper.addObjectForKey(currentSoftButtons,
                                            Const.INTENTHELPER_KEY_OBJECTSLIST);
                                    Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                    intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                                            UPDATETURNLIST_MAXSOFTBUTTONS);
                                    startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                                }
                            });

                            builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                /*
                                 * the number of items to send is determined as max of turn items
								 * and icon items. only when the both fields are empty, we
								 * don't send anything.
								 */
                                    boolean turnListEnabled = useTurnList.isChecked();
                                    boolean iconListEnabled = useIconList.isChecked();
                                    String turnListString = txtTurnList.getText().toString();
                                    String iconListString = txtIconList.getText().toString();
                                    if ((turnListString.length() > 0) || (iconListString.length() > 0)) {
                                        Vector<Turn> tarray = new Vector<Turn>();

                                        String[] iconNames = iconListString.split(JOIN_STRING);
                                        String[] turnNames = turnListString.split(JOIN_STRING);
                                        int turnCount = Math.max(iconNames.length, turnNames.length);

                                        for (int i = 0; i < turnCount; ++i) {
                                            Turn t = new Turn();
                                            if (turnListEnabled) {
                                                t.setNavigationText((i < turnNames.length) ? turnNames[i] : "");
                                            }

                                            if (iconListEnabled) {
                                                Image ti = new Image();
                                                ti.setValue((i < iconNames.length) ? iconNames[i] : "");
                                                ti.setImageType(ImageType.DYNAMIC);
                                                t.setTurnIcon(ti);
                                            }
                                            tarray.add(t);
                                        }
                                        UpdateTurnList msg = new UpdateTurnList();
                                        msg.setCorrelationID(getCorrelationid());
                                        msg.setTurnList(tarray);
                                        if (useSoftButtons.isChecked()) {
                                            if (currentSoftButtons != null) {
                                                msg.setSoftButtons(
                                                        currentSoftButtons);
                                            } else {
                                                msg.setSoftButtons(
                                                        new Vector<SoftButton>());
                                            }
                                        }
                                        currentSoftButtons = null;

                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    } else {
                                        Toast.makeText(mContext, "Both fields are empty, nothing to send",
                                                Toast.LENGTH_LONG).show();
                                    }
                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    currentSoftButtons = null;
                                    dialog.cancel();
                                }
                            });
                            builder.setView(layout);
                            builder.show();
                        }

                        private void updateDynamicFooter(EditText txtNumTicks,
                                                         EditText txtSliderFooter, String joinString) {
                            // set numTicks comma-separated strings
                            int numTicks = 0;
                            try {
                                numTicks = Integer.parseInt(txtNumTicks.getText().toString());
                            } catch (NumberFormatException e) {
                                // do nothing, leave 0
                            }
                            if (numTicks > 0) {
                                StringBuilder b = new StringBuilder();
                                for (int i = 0; i < numTicks; ++i) {
                                    b.append(joinString).append(i + 1);
                                }
                                txtSliderFooter.setText(b.toString().substring(joinString.length()));
                            } else {
                                txtSliderFooter.setText("");
                            }
                        }

                        private void sendSlider() {
                            AlertDialog.Builder builder;

                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.slider, null);
                            final EditText txtNumTicks = (EditText) layout.findViewById(R.id.txtNumTicks);
                            final EditText txtPosititon = (EditText) layout.findViewById(R.id.txtPosition);
                            final EditText txtSliderHeader = (EditText) layout.findViewById(R.id.txtSliderHeader);
                            final EditText txtSliderFooter = (EditText) layout.findViewById(R.id.txtSliderFooter);
                            final CheckBox useTimeout = (CheckBox) layout.findViewById(R.id.slider_useTimeout);
                            final EditText txtTimeout = (EditText) layout.findViewById(R.id.txtTimeout);

                            final CheckBox chkDynamicFooter = (CheckBox) layout.findViewById(R.id.slider_chkDynamicFooter);
                            chkDynamicFooter.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                                @Override
                                public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                                    if (!isChecked) {
                                        // set default static text
                                        txtSliderFooter.setText(R.string.slider_footer);
                                    } else {
                                        updateDynamicFooter(txtNumTicks, txtSliderFooter, JOIN_STRING);
                                    }
                                }
                            });

                            /**
                             * Process a possibility to exclude Footer from Slider request
                             */
                            final boolean[] isUseFooterInSlider = {true};
                            final CheckBox useFooterInSliderCheckBox = (CheckBox) layout.findViewById(R.id.use_footer_in_slider_checkbox);
                            isUseFooterInSlider[0] = useFooterInSliderCheckBox.isChecked();
                            useFooterInSliderCheckBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                                @Override
                                public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                                    isUseFooterInSlider[0] = b;
                                    chkDynamicFooter.setEnabled(b);
                                    txtSliderFooter.setEnabled(b);
                                }
                            });

                            builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    try {
                                        Slider msg = new Slider();
                                        if (useTimeout.isChecked()) {
                                            msg.setTimeout(Integer.parseInt(txtTimeout.getText().toString()));
                                        }
                                        msg.setNumTicks(Integer.parseInt(txtNumTicks.getText().toString()));
                                        msg.setSliderHeader(txtSliderHeader.getText().toString());

                                        /**
                                         * Do not set Footer
                                         */
                                        if (isUseFooterInSlider[0]) {
                                            Vector<String> footerElements;
                                            String footer = txtSliderFooter.getText().toString();
                                            if (chkDynamicFooter.isChecked()) {
                                                footerElements = new Vector<String>(Arrays.asList(footer.split(JOIN_STRING)));
                                            } else {
                                                footerElements = new Vector<String>();
                                                footerElements.add(footer);
                                            }
                                            msg.setSliderFooter(footerElements);
                                        }

                                        msg.setPosition(Integer.parseInt(txtPosititon.getText().toString()));
                                        msg.setCorrelationID(getCorrelationid());
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    } catch (NumberFormatException e) {
                                        SafeToast.showToastAnyThread("Couldn't parse number");
                                    }
                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    dialog.cancel();
                                }
                            });
                            builder.setView(layout);
                            builder.show();
                        }

                        private void sendVehicleDataSubscriptions() {
                            DialogFragment subscriptionsVehicleDataDialog =
                                    SubscriptionsVehicleDataDialog.newInstance();
                            subscriptionsVehicleDataDialog.show(getFragmentManager(),
                                    SUBSCRIPTION_VEHICLE_DATA_DIALOG_TAG);
                        }

                        private void sendSetGlobalProperties() {
                            DialogFragment setGlobalPropertiesDialog =
                                    SetGlobalPropertiesDialog.newInstance();
                            setGlobalPropertiesDialog.show(getFragmentManager(),
                                    SET_GLOBAL_PROPERTIES_DIALOG_TAG);
                        }

                        private void sendResetGlobalProperties() {
                            AlertDialog.Builder builder;

                            Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.resetglobalproperties,
                                    (ViewGroup) findViewById(R.id.resetglobalproperties_Root));

                            final CheckBox choiceHelpPrompt = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceHelpPrompt);
                            final CheckBox choiceTimeoutPrompt = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceTimeoutPrompt);
                            final CheckBox choiceVRHelpTitle = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceVRHelpTitle);
                            final CheckBox choiceVRHelpItem = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceVRHelpItems);
                            final CheckBox choiceKeyboardProperties = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceKeyboardProperties);
                            final CheckBox choiceMenuIcon = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceMenuIcon);
                            final CheckBox choiceMenuName = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceMenuName);

                            builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    ResetGlobalProperties msg = new ResetGlobalProperties();
                                    Vector<GlobalProperty> properties = new Vector<GlobalProperty>();

                                    if (choiceHelpPrompt.isChecked()) {
                                        properties.add(GlobalProperty.HELPPROMPT);
                                    }

                                    if (choiceTimeoutPrompt.isChecked()) {
                                        properties.add(GlobalProperty.TIMEOUTPROMPT);
                                    }

                                    if (choiceVRHelpTitle.isChecked()) {
                                        properties.add(GlobalProperty.VRHELPTITLE);
                                    }

                                    if (choiceVRHelpItem.isChecked()) {
                                        properties.add(GlobalProperty.VRHELPITEMS);
                                    }

                                    if (choiceMenuIcon.isChecked()) {
                                        properties.add(GlobalProperty.MENUICON);
                                    }

                                    if (choiceMenuName.isChecked()) {
                                        properties.add(GlobalProperty.MENUNAME);
                                    }

                                    if (choiceKeyboardProperties.isChecked()) {
                                        properties.add(GlobalProperty.KEYBOARDPROPERTIES);
                                    }

                                    if (!properties.isEmpty()) {
                                        msg.setProperties(properties);
                                        msg.setCorrelationID(getCorrelationid());
                                        if (mBoundProxyService != null) {
                                            mBoundProxyService.syncProxySendRPCRequest(msg);
                                        }
                                    } else {
                                        Toast.makeText(getApplicationContext(), "No items selected", Toast.LENGTH_LONG).show();
                                    }
                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    dialog.cancel();
                                }
                            });
                            builder.setView(layout);
                            builder.create().show();
                        }


                        /**
                         * Opens the dialog for SetDisplayLayout message and sends it.
                         */
                        private void sendSetDisplayLayout() {
                            Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.setdisplaylayout,
                                    (ViewGroup) findViewById(R.id.setdisplaylayout_itemRoot));

                            final EditText editDisplayLayout = (EditText) layout.findViewById(R.id.setdisplaylayout_displayLayout);

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    SetDisplayLayout msg = new SetDisplayLayout();
                                    msg.setCorrelationID(getCorrelationid());
                                    msg.setDisplayLayout(editDisplayLayout.getText().toString());
                                    if (mBoundProxyService != null) {
                                        mBoundProxyService.syncProxySendRPCRequest(msg);
                                    }
                                }
                            });
                            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    dialog.cancel();
                                }
                            });
                            builder.setView(layout);
                            builder.show();
                        }

                        /**
                         * Sends a GenericRequest message.
                         */
                        private void sendGenericRequest() {
                            GenericRequest msg = new GenericRequest();
                            msg.setCorrelationID(getCorrelationid());
                            if (mBoundProxyService != null) {
                                mBoundProxyService.syncProxySendRPCRequest(msg);
                            }
                        }
                    })
                    .setNegativeButton("Close", null)
                    .show();
        } else if (v == findViewById(R.id.btnPlayPause)) {
            mBoundProxyService.playPauseAnnoyingRepetitiveAudio();
        }
    }

    private void sendSetMediaClockTimer() {
        AlertDialog.Builder builder;

        Context mContext = this;
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.setmediaclock, null);
        final EditText txtStartHours = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_startTimeHours);
        final EditText txtStartMinutes = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_startTimeMinutes);
        final EditText txtStartSeconds = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_startTimeSeconds);
        final EditText txtEndHours = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_endTimeHours);
        final EditText txtEndMinutes = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_endTimeMinutes);
        final EditText txtEndSeconds = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_endTimeSeconds);
        final Spinner spnUpdateMode = (Spinner) layout.findViewById(
                R.id.setmediaclocktimer_spnUpdateMode);

        ArrayAdapter<UpdateMode> spinnerAdapter =
                new ArrayAdapter<UpdateMode>(mContext,
                        android.R.layout.simple_spinner_item,
                        UpdateMode.values());
        spinnerAdapter.setDropDownViewResource(
                android.R.layout.simple_spinner_dropdown_item);
        spnUpdateMode.setAdapter(spinnerAdapter);

        builder = new AlertDialog.Builder(mContext);
        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                SetMediaClockTimer msg = new SetMediaClockTimer();
                msg.setCorrelationID(getCorrelationid());
                UpdateMode updateMode =
                        (UpdateMode) spnUpdateMode.getSelectedItem();
                msg.setUpdateMode(updateMode);

                try {
                    Integer hours = Integer.parseInt(
                            txtStartHours.getText().toString());
                    Integer minutes = Integer.parseInt(
                            txtStartMinutes.getText().toString());
                    Integer seconds = Integer.parseInt(
                            txtStartSeconds.getText().toString());
                    StartTime startTime = new StartTime();
                    startTime.setHours(hours);
                    startTime.setMinutes(minutes);
                    startTime.setSeconds(seconds);
                    msg.setStartTime(startTime);
                } catch (NumberFormatException e) {
                    // skip setting start time if parsing failed
                }

                try {
                    Integer hours =
                            Integer.parseInt(txtEndHours.getText().toString());
                    Integer minutes = Integer.parseInt(
                            txtEndMinutes.getText().toString());
                    Integer seconds = Integer.parseInt(
                            txtEndSeconds.getText().toString());
                    StartTime endTime = new StartTime();
                    endTime.setHours(hours);
                    endTime.setMinutes(minutes);
                    endTime.setSeconds(seconds);
                    msg.setEndTime(endTime);
                } catch (NumberFormatException e) {
                    // skip setting start time if parsing failed
                }
                if (mBoundProxyService != null) {
                    mBoundProxyService.syncProxySendRPCRequest(msg);
                }
            }
        });
        builder.setNegativeButton("Cancel",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });
        builder.setView(layout);
        builder.show();
    }

    private void initProxyService() {
        mBoundProxyService.setLogAdapter(mLogAdapter);
        mBoundProxyService.setProxyServiceEvent(this);
    }

    private void getProxyService() {
        mBoundProxyService = null;
        mBoundProxyService = MainApp.getInstance().getBoundProxyService();
    }

    private void sendDiagnosticMessage() {
        final Context mContext = this;
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.diagnosticmessage, null);
        final EditText txtTargetID = (EditText) layout.findViewById(
                R.id.diagnosticmessage_txtTargetID);
        final EditText txtMessageLength = (EditText) layout.findViewById(
                R.id.diagnosticmessage_txtMessageLength);
        final EditText txtMessageData = (EditText) layout.findViewById(
                R.id.diagnosticmessage_txtMessageData);
        final CheckBox useTargetID = (CheckBox) layout.findViewById(
                R.id.diagnosticmessage_useTargetID);
        final CheckBox useMessageLength = (CheckBox) layout.findViewById(
                R.id.diagnosticmessage_useMessageLength);
        final CheckBox useMessageData = (CheckBox) layout.findViewById(
                R.id.diagnosticmessage_useMessageData);

        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                try {
                    DiagnosticMessage msg = new DiagnosticMessage();
                    msg.setCorrelationID(getCorrelationid());

                    if (useTargetID.isChecked()) {
                        msg.setTargetID(Integer.valueOf(
                                txtTargetID.getText().toString()));
                    }

                    if (useMessageLength.isChecked()) {
                        msg.setMessageLength(Integer.valueOf(
                                txtMessageLength.getText().toString()));
                    }

                    if (useMessageData.isChecked()) {
                        final String[] msgData = txtMessageData.getText()
                                .toString()
                                .split(JOIN_STRING);
                        final Vector<Integer> data = new Vector<Integer>();
                        for (String s : msgData) {
                            data.add(Integer.valueOf(s));
                        }
                        msg.setMessageData(data);
                    }
                    if (mBoundProxyService != null) {
                        mBoundProxyService.syncProxySendRPCRequest(msg);
                    }
                } catch (NumberFormatException e) {
                    SafeToast.showToastAnyThread("Couldn't parse number");
                }
            }
        });
        builder.setNegativeButton("Cancel",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });
        builder.setView(layout);
        builder.show();
    }

    /**
     * Sends RegisterAppInterface message.
     */
    private void sendRegisterAppInterface() {
        DialogFragment registerAppInterfaceDialog = RegisterAppInterfaceDialog.newInstance();
        registerAppInterfaceDialog.show(getFragmentManager(), REGISTER_APP_INTERFACE_DIALOG_TAG);
    }

    /**
     * Splits the string with a comma and returns a vector of TTSChunks.
     */
    public Vector<TTSChunk> ttsChunksFromString(String string) {
        Vector<TTSChunk> chunks = new Vector<TTSChunk>();
        for (String stringChunk : string.split(JOIN_STRING)) {
            TTSChunk chunk = TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, stringChunk);
            chunks.add(chunk);
        }
        return chunks;
    }

    /**
     * Calls the setter with setterName on the msg.
     */
    public void setVehicleDataParam(RPCRequest msg, Class msgClass, String setterName) {
        try {
            Method setter = msgClass.getMethod(setterName, Boolean.class);
            setter.invoke(msg, true);
        } catch (NoSuchMethodException e) {
            Logger.e("Can't set vehicle data", e);
        } catch (IllegalAccessException e) {
            Logger.e("Can't set vehicle data", e);
        } catch (InvocationTargetException e) {
            Logger.e("Can't set vehicle data", e);
        }
    }

    public void addSubMenuToList(final SyncSubMenu sm) {
        runOnUiThread(new Runnable() {
            public void run() {
                mSubmenuAdapter.add(sm);
            }
        });
    }

    /**
     * This method provides {@link android.widget.ArrayAdapter} to the UI components of the
     * DialogFragments
     *
     * @return {@link android.widget.ArrayAdapter}
     */
    public ArrayAdapter<SyncSubMenu> getSubMenuAdapter() {
        return mSubmenuAdapter;
    }

    /**
     * This method provides {@link android.widget.ArrayAdapter} to the UI components of the
     * DialogFragments
     *
     * @return {@link android.widget.ArrayAdapter}
     */
    public ArrayAdapter<ImageType> getImageTypeAdapter() {
        return imageTypeAdapter;
    }

    /**
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.android.activity.SubscriptionsVehicleDataDialog}
     *
     * @param unsubscribeVehicleData {@link com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData}
     */
    public void onUnsubscribeVehicleDialogResult(UnsubscribeVehicleData unsubscribeVehicleData) {
        if (mBoundProxyService != null) {
            mBoundProxyService.commandUnsubscribeVehicleInterface(unsubscribeVehicleData);
        }
    }

    /**
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.android.activity.SubscriptionsVehicleDataDialog}
     *
     * @param subscribeVehicleData {@link com.ford.syncV4.proxy.rpc.SubscribeVehicleData}
     */
    public void onSubscribeVehicleDialogResult(SubscribeVehicleData subscribeVehicleData) {
        if (mBoundProxyService != null) {
            mBoundProxyService.commandSubscribeVehicleInterfaceResumable(subscribeVehicleData);
        }
    }

    /**
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.android.activity.SetGlobalPropertiesDialog}
     *
     * @param setGlobalProperties {@link com.ford.syncV4.proxy.rpc.SetGlobalProperties} request
     */
    public void onSetGlobalPropertiesDialogResult(SetGlobalProperties setGlobalProperties) {
        if (mBoundProxyService != null) {
            mBoundProxyService.commandSetGlobalPropertiesResumable(setGlobalProperties);
        }
    }

    /**
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.android.activity.AddSubMenuDialog}
     *
     * @param addSubMenu  {@link com.ford.syncV4.android.activity.AddSubMenuDialog} request
     * @param syncSubMenu SubMenu structure
     */
    public void onAddSubMenuDialogResult(AddSubMenu addSubMenu, SyncSubMenu syncSubMenu) {
        if (mBoundProxyService != null) {
            mBoundProxyService.commandAddSubMenuResumable(addSubMenu);
        }
        if (mLatestAddSubmenu != null) {
            Logger.w("Latest AddSubMenu should be null, but equals to " + mLatestAddSubmenu);
        }
        mLatestAddSubmenu = syncSubMenu;
    }

    /**
     *
     */
    public void onPolicyFilesSetUpDialogResult_SendUpdate() {
        if (mBoundProxyService != null) {
            mBoundProxyService.sendPolicyTableUpdate();
        }
    }

    /**
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.android.activity.AddCommandDialog}
     *
     * @param addCommand {@link com.ford.syncV4.proxy.rpc.AddCommand}
     */
    public void onAddCommandDialogResult(AddCommand addCommand) {
        if (mBoundProxyService != null) {
            mBoundProxyService.commandAddCommandResumable(addCommand);
        }

        if (mLatestAddCommand != null) {
            Logger.w("Latest addCommand should be null, but it is " + mLatestAddCommand.first +
                            " / " + mLatestAddCommand.second);
        }
        Integer parentID = null;
        if (addCommand.getMenuParams() != null) {
            parentID = addCommand.getMenuParams().getParentID();
        }
        mLatestAddCommand = new Pair<Integer, Integer>(addCommand.getCmdID(), parentID);
    }

    /**
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.android.activity.PerformAudioPassThruDialog}
     *
     * @param performAudioPassThru {@link com.ford.syncV4.proxy.rpc.PerformAudioPassThru}
     */
    public void onPerformAudioPassThruDialogResult(PerformAudioPassThru performAudioPassThru) {
        latestPerformAudioPassThruMsg = performAudioPassThru;
        if (mBoundProxyService != null) {
            mBoundProxyService.syncProxySendRPCRequest(performAudioPassThru);
        }
    }

    /**
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface}
     *
     * @param registerAppInterface {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface}
     */
    public void onRegisterAppInterfaceDialogResult(RegisterAppInterface registerAppInterface) {
        if (mBoundProxyService != null) {
            if (mBoundProxyService.isSyncProxyConnected()) {
                mBoundProxyService.syncProxySendRPCRequest(registerAppInterface);
            } else {
                // This may happen if "UnregisterAppInterface" command has been sent manually
                // from the SPT

                Logger.w("OnRegisterAppInterfaceDialogResult -> SyncProxy not connected");

                onSetUpDialogResult();
            }
        } else {
            Logger.w("OnRegisterAppInterfaceDialogResult -> mBoundProxyService is NULL");
        }
    }

    /**
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.android.activity.SendSingleRPCRequestDialog}. This Dialog set some
     * properties to at the command, such as "send as encoded" or "UnregisterAppInterface do not
     * close USB connection"
     *
     * @param rpcRequest modified {@link com.ford.syncV4.proxy.RPCMessage}
     */
    public void onSingleRPCCommandDialogResult(RPCRequest rpcRequest, boolean doEncode,
                                               boolean doCloseUSBReader) {
        if (mBoundProxyService != null) {

            if (doCloseUSBReader) {
                // When set this value to true it is important to revert it back to false when Test is
                // complete!
                mBoundProxyService.getTestConfig().setDoKeepUSBTransportConnected(true);
            }

            mBoundProxyService.syncProxySendRPCRequest(rpcRequest);
        }
    }

    /**
     * Adds command ID to the adapter, and maps it to its parent submenu.
     *
     * @param cmdID     ID of the new command
     * @param submenuID ID of the command's parent submenu
     */
    private void addCommandToList(Integer cmdID, Integer submenuID) {
        mCommandAdapter.add(cmdID);
        if (null != submenuID) {
            mCommandIdToParentSubmenuMap.put(cmdID, submenuID);
        }
    }

    /**
     * Removes command ID from the adapter.
     *
     * @param cmdID ID of the command
     */
    private void removeCommandFromList(Integer cmdID) {
        mCommandAdapter.remove(cmdID);
        mCommandIdToParentSubmenuMap.remove(cmdID);
    }

    //upon onDestroy(), dispose current proxy and create a new one to enable auto-start
    //call resetProxy() to do so
    /*public void endSyncProxyInstance() {
        if (mBoundProxyService != null) {
            SyncProxyALM proxyInstance = mBoundProxyService.getProxyInstance();
            //if proxy exists, reset it
            if (proxyInstance != null) {
                if (proxyInstance.getCurrentTransportType() == TransportType.BLUETOOTH) {
                    mBoundProxyService.reset();
                } else {
                    Logger.e("endSyncProxyInstance. No reset required if transport is TCP");
                }
                //if proxy == null create proxy
            } else {
                mBoundProxyService.startProxy();
            }
        }
    }*/

    public void setTesterMain(ModuleTest _instance) {
        this.mTesterMain = _instance;
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        saveMessageSelectCount();
    }

    @Override
    public void onPause() {
        super.onPause();
        saveMessageSelectCount();
    }

    /**
     * Return a clone of the {@code isVehicleDataSubscribed}
     *
     * @return a clone of the {@code isVehicleDataSubscribed}
     */
    public boolean[] cloneIsVehicleDataSubscribed() {
        return isVehicleDataSubscribed.clone();
    }

    /**
     * @param position position in the array
     * @return
     */
    public boolean getIsVehicleDataSubscribedAt(int position) {
        return isVehicleDataSubscribed[position];
    }

    /**
     * Set a velue of the {@code isVehicleDataSubscribed} array
     *
     * @param value
     */
    public void setIsVehicleDataSubscribed(boolean[] value) {
        isVehicleDataSubscribed = value;
    }

    /**
     * Called when a CreateChoiceSetResponse comes. If successful, add it to the
     * adapter. In any case, remove the key from the map.
     */
    public void onCreateChoiceSetResponse(boolean success) {
        if (mLatestCreateChoiceSetId != CHOICESETID_UNSET) {
            if (success) {
                mChoiceSetAdapter.add(mLatestCreateChoiceSetId);
            }
            mLatestCreateChoiceSetId = CHOICESETID_UNSET;
        } else {
            Logger.w("Latest createChoiceSetId is unset");
        }
    }

    /**
     * Called when a DeleteChoiceSetResponse comes. If successful, remove it
     * from the adapter.
     */
    public void onDeleteChoiceSetResponse(boolean success) {
        if (_latestDeleteChoiceSetId != CHOICESETID_UNSET) {
            if (success) {
                mChoiceSetAdapter.remove(_latestDeleteChoiceSetId);
            }
            _latestDeleteChoiceSetId = CHOICESETID_UNSET;
        } else {
            Logger.w("Latest deleteChoiceSetId is unset");
        }
    }

    /**
     * Called when a DeleteSubMenuResponse comes. If successful, remove it from
     * the adapter. We also need to delete all the commands that were added to
     * this submenu.
     */
    public void onDeleteSubMenuResponse(boolean success) {
        if (_latestDeleteSubmenu != null) {
            if (success) {
                mSubmenuAdapter.remove(_latestDeleteSubmenu);

                for (Iterator<Entry<Integer, Integer>> it = mCommandIdToParentSubmenuMap
                        .entrySet().iterator(); it.hasNext(); ) {
                    Entry<Integer, Integer> entry = it.next();
                    if (entry.getValue() == _latestDeleteSubmenu.getSubMenuId()) {
                        mCommandAdapter.remove(entry.getKey());
                        it.remove();
                    }
                }
            }
            _latestDeleteSubmenu = null;
        } else {
            Logger.w("Latest deleteSubMenu is unset");
        }
    }

    /**
     * Called when a AddSubMenuResponse comes. If successful, add it to the
     * adapter.
     */
    public void onAddSubMenuResponse(boolean success) {
        if (mLatestAddSubmenu != null) {
            if (success) {
                addSubMenuToList(mLatestAddSubmenu);
            }
            mLatestAddSubmenu = null;
        } else {
            Logger.w("Latest addSubMenu is unset");
        }
    }

    /**
     * Called when a AddCommandResponse comes. If successful, add it to the
     * adapter.
     */
    public void onAddCommandResponse(boolean success) {
        if (mLatestAddCommand != null) {
            if (success) {
                addCommandToList(mLatestAddCommand.first, mLatestAddCommand.second);
            }
            mLatestAddCommand = null;
        } else {
            Logger.w("Latest addCommand is null");
        }
    }

    /**
     * Called when a DeleteCommandResponse comes. If successful, remove it from
     * the adapter.
     */
    public void onDeleteCommandResponse(boolean success) {
        if (_latestDeleteCommandCmdID != null) {
            if (success) {
                removeCommandFromList(_latestDeleteCommandCmdID);
            }
            _latestDeleteCommandCmdID = null;
        } else {
            Logger.w("Latest deleteCommand is unset");
        }
    }

    /**
     * Called whenever an OnAudioPassThru notification comes. The aptData is the
     * audio data sent in it.
     */
    public void onAudioPassThru(byte[] aptData) {
        if (aptData == null) {
            Logger.w("onAudioPassThru aptData is null");
            return;
        }
        Logger.i("data len " + aptData.length);

        File outFile = audioPassThruOutputFile();
        try {
            if (audioPassThruOutStream == null) {
                audioPassThruOutStream = new BufferedOutputStream(
                        new FileOutputStream(outFile, false));
            }
            audioPassThruOutStream.write(aptData);
            audioPassThruOutStream.flush();
        } catch (FileNotFoundException e) {
            logToConsoleAndUI(
                    "Output file "
                            + (outFile != null ? outFile.toString()
                            : "'unknown'")
                            + " can't be opened for writing", e);
        } catch (IOException e) {
            logToConsoleAndUI("Can't write to output file", e);
        }

		/*
         * if there is current player, save the current position, stop and
		 * release it, so that we recreate it with the appended file and jump to
		 * that position, emulating seamless stream playing
		 */
        int audioPosition = -1;
        if (audioPassThruMediaPlayer != null) {
            audioPosition = audioPassThruMediaPlayer.getCurrentPosition();
            audioPassThruMediaPlayer.stop();
            audioPassThruMediaPlayer.reset();
            audioPassThruMediaPlayer.release();
            audioPassThruMediaPlayer = null;
        }

        audioPassThruMediaPlayer = new MediaPlayer();
        try {
            if (isExtStorageWritable()) {
                audioPassThruMediaPlayer.setDataSource(outFile.toString());
            } else {
                /*
                 * setDataSource with a filename on the internal storage throws
				 * "java.io.IOException: Prepare failed.: status=0x1", so we
				 * open the file with a special method
				 */
                audioPassThruMediaPlayer.setDataSource(openFileInput(
                        AUDIOPASSTHRU_OUTPUT_FILE).getFD());
            }
            audioPassThruMediaPlayer.prepare();
            if (audioPosition != -1) {
                audioPassThruMediaPlayer.seekTo(audioPosition);
            }
            audioPassThruMediaPlayer.start();
        } catch (IOException e) {
            Logger.e(e.toString());
        }
    }

    /**
     * Called when a PerformAudioPassThru response comes. Save the file only if
     * the result is success. If the result is retry, send the latest request
     * again.
     */
    public void onPerformAudioPassThruResponse(Result result) {
        closeAudioPassThruStream();
        closeAudioPassThruMediaPlayer();
        if (Result.SUCCESS != result) {
            File outFile = audioPassThruOutputFile();
            if ((outFile != null) && outFile.exists()) {
                if (!outFile.delete()) {
                    logToConsoleAndUI("Failed to delete output file", null);
                }
            }

            if ((Result.RETRY == result) && (latestPerformAudioPassThruMsg != null)) {
                latestPerformAudioPassThruMsg.setCorrelationID(getCorrelationid());
                if (mBoundProxyService != null) {
                    mBoundProxyService.syncProxySendRPCRequest(latestPerformAudioPassThruMsg);
                }
            }
        }
    }

    /**
     * Called when an EndAudioPassThru response comes. The logic is the same as
     * when a PerformAudioPassThru response comes.
     */
    public void onEndAudioPassThruResponse(Result result) {
        onPerformAudioPassThruResponse(result);
    }

    private void closeAudioPassThruStream() {
        if (audioPassThruOutStream != null) {
            Logger.d("closing audioPassThruOutStream");
            try {
                audioPassThruOutStream.flush();
                audioPassThruOutStream.close();
            } catch (IOException e) {
                Logger.w("Can't close output file", e);
            }
            audioPassThruOutStream = null;
        }
    }

    private void closeAudioPassThruMediaPlayer() {
        if (audioPassThruMediaPlayer == null) {
            return;
        }

        if (audioPassThruMediaPlayer.isPlaying()) {
            audioPassThruMediaPlayer.setOnCompletionListener(new OnCompletionListener() {
                @Override
                public void onCompletion(MediaPlayer mp) {
                    Logger.d("mediaPlayer completed");
                    audioPassThruMediaPlayer.reset();
                    audioPassThruMediaPlayer.release();
                    audioPassThruMediaPlayer = null;
                }
            });
        } else {
            // the player has stopped
            Logger.d("mediaPlayer is stopped");
            audioPassThruMediaPlayer.release();
            audioPassThruMediaPlayer = null;
        }
    }

    private File audioPassThruOutputFile() {
        File baseDir = isExtStorageWritable() ? Environment
                .getExternalStorageDirectory() : getFilesDir();
        File outFile = new File(baseDir, AUDIOPASSTHRU_OUTPUT_FILE);
        return outFile;
    }

    private void logToConsoleAndUI(String msg, Throwable thr) {
        Logger.d(msg, thr);
        Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_SHORT).show();
    }

    /**
     * Returns whether the external storage is available for writing.
     */
    private boolean isExtStorageWritable() {
        String state = Environment.getExternalStorageState();
        return Environment.MEDIA_MOUNTED.equals(state);
    }

    private void sendCreateInteractionChoiceSet(Vector<Choice> choices) {
        int choiceSetID = autoIncChoiceSetId++;
        if (mBoundProxyService != null) {
            mBoundProxyService.commandCreateInteractionChoiceSetResumable(choices, choiceSetID,
                    getCorrelationid());

            if (mLatestCreateChoiceSetId != CHOICESETID_UNSET) {
                Logger.w("Latest createChoiceSetId should be unset, but equals to " +
                        mLatestCreateChoiceSetId);
            }
            mLatestCreateChoiceSetId = choiceSetID;
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_LIST_SOFTBUTTONS:
                if (resultCode == RESULT_OK) {
                    currentSoftButtons = (Vector<SoftButton>) IntentHelper.
                            getObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                    if (chkIncludeSoftButtons != null) {
                        chkIncludeSoftButtons.setChecked(true);
                    }
                }
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                break;

            case REQUEST_LIST_CHOICES:
                if (resultCode == RESULT_OK) {
                    Vector<Choice> choices = (Vector<Choice>) IntentHelper.
                            getObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                    sendCreateInteractionChoiceSet(choices);
                }
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                break;

            case Const.REQUEST_FILE_OPEN:
                if (resultCode == RESULT_OK) {
                    String fileName = data.getStringExtra(FileDialog.RESULT_PATH);
                    if (txtLocalFileName != null) {
                        txtLocalFileName.setText(fileName);
                    }
                }
                break;

            case REQUEST_CHOOSE_XML_TEST:
                if (resultCode == RESULT_OK) {
                    String filePath = data.getStringExtra(FileDialog.RESULT_PATH);
                    if (filePath != null) {
                        xmlTestContinue(filePath);
                    }
                }
                break;

            default:
                Logger.i("Unknown request code: " + requestCode);
                break;
        }
    }

    /**
     * Called when the app is acivated from HMI for the first time. ProxyService
     * automatically subscribes to buttons, so we reflect that in the
     * subscription list.
     */
    public void buttonsSubscribed(Vector<ButtonName> buttons) {
        List<ButtonName> buttonNames = Arrays.asList(ButtonName.values());
        for (ButtonName buttonName : buttons) {
            isButtonSubscribed[buttonNames.indexOf(buttonName)] = true;
        }
    }

    /**
     * Opens a dialog so that the user can select an XML test or a directory
     * with XML tests. The result will come to onActivityResult method.
     */
    public void openXmlFilePathDialog() {
        Intent intent = new Intent(this, FileDialog.class);
        String sdcardPath = Environment.getExternalStorageDirectory().getPath();
        intent.putExtra(FileDialog.START_PATH, sdcardPath);
        intent.putExtra(FileDialog.CAN_SELECT_DIR, true);
        intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
        intent.putExtra(FileDialog.FORMAT_FILTER, new String[]{"xml"});
        startActivityForResult(intent, REQUEST_CHOOSE_XML_TEST);
    }

    public void startMobileNaviService() {
        if (isProxyReadyForWork()) {
            if (mBoundProxyService != null) {
                mLogAdapter.logMessage("Should start Mobile Navi Service", true);
                mBoundProxyService.syncProxyStartMobileNavService(rpcSession);
            } else {
                mLogAdapter.logMessage("Could not start mobile nav Service", true);
            }
        }
    }

    public void onMobileNaviError(String errorMsg) {
        onMobileNaviError(errorMsg, true);
    }

    public void onMobileNaviError(String errorMsg, boolean addToUI) {
        mLogAdapter.logMessage(errorMsg, addToUI);
        MobileNavPreviewFragment fr = (MobileNavPreviewFragment) getSupportFragmentManager().findFragmentById(R.id.videoFragment);
        fr.setStateOff();
        closeMobileNaviOutputStream();
        AudioServicePreviewFragment audioFragement = (AudioServicePreviewFragment) getSupportFragmentManager().findFragmentById(R.id.audioFragment);
        audioFragement.setStateOff();
        closeAudioOutputStream();
    }

    public void logError(final Exception e) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mLogAdapter.logMessage(e.getMessage(), true);
            }
        });
    }

    private void closeMobileNaviOutputStream() {
        if (mBoundProxyService != null) {
            mBoundProxyService.syncProxyStopH264();
        }
    }

    private void closeAudioOutputStream() {
        if (mBoundProxyService != null) {
            mBoundProxyService.syncProxyStopAudioDataTransfer();
        }
    }

    public void stopMobileNavService() {
        mStreamCommandsExecutorService.submit(new Runnable() {
            @Override
            public void run() {
                if (isProxyReadyForWork()) {
                    mLogAdapter.logMessage("Should stop Mobile Navi service", true);
                    if (mBoundProxyService != null) {
                        mBoundProxyService.syncProxyStopMobileNaviService();
                    }
                    closeMobileNaviOutputStream();
                }
            }
        });
    }

    public boolean isProxyReadyForWork() {
        if (mBoundProxyService == null) {
            return false;
        }
        if (!mBoundProxyService.isSyncProxyNotNull()) {
            onMobileNaviError("Error. Proxy is null");
            return false;
        }
        if (!mBoundProxyService.isSyncProxyConnected()) {
            onMobileNaviError("Error. Proxy is not connected");
            return false;
        }
        if (!mBoundProxyService.isSyncProxyConnectionNotNull()) {
            onMobileNaviError("Error. sync connection is null");
            return false;
        }
        return true;
    }

    public void startAudioService() {
        if (isProxyReadyForWork()) {
            mLogAdapter.logMessage("Should start Mobile Audio service", true);

            mStreamCommandsExecutorService.submit(new Runnable() {
                @Override
                public void run() {
                    mBoundProxyService.syncProxyStartAudioService(rpcSession);
                }
            });
        }
    }

    public void stopAudioService() {
        mStreamCommandsExecutorService.submit(new Runnable() {
            @Override
            public void run() {
                if (isProxyReadyForWork()) {
                    mLogAdapter.logMessage("Should stop Mobile Audio service", true);
                    mBoundProxyService.syncProxyStopAudioService();
                    closeAudioOutputStream();
                }
            }
        });
    }

    public void onTouchEventReceived(OnTouchEvent notification) {

    }

    public void onKeyboardInputReceived(OnKeyboardInput event) {

    }

    /**
     * Add all necessary listeners
     */
    private void addListeners() {
        ConnectionListenersManager.addConnectionListener(this);
    }

    /**
     * Remove all subscribed listeners
     */
    private void removeListeners() {
        ConnectionListenersManager.removeConnectionListener(this);
    }

    /**
     * Callback of the exit timer. If the correct destroy procedure fails we use Process.killProcess
     */
    private Runnable mBluetoothStopServicePostDelayedCallback = new Runnable() {
        @Override
        public void run() {
            Logger.w("Bluetooth Stop Service timer callback");
            mBluetoothStopProxyServiceTimeOutHandler.removeCallbacks(mBluetoothStopServicePostDelayedCallback);
            stopService(new Intent(SyncProxyTester.this, ProxyService.class));
        }
    };

    /**
     * Exit Application section
     */

    /**
     * Exit from Activity
     */
    private void exitApp() {
        Logger.i("Exit " + SyncProxyTester.class.getSimpleName() + "\n\n");
        isFirstActivityRun = true;
        //stopService(new Intent(this, ProxyService.class));
        super.finish();

        MainApp.getInstance().exitApp();
    }

    // TODO : Move this block to MainApp

    /**
     * Stops the proxy service.
     */
    private void stopProxyServiceOnExit() {
        getExitDialog().show();

        if (mStopProxyServiceTimeOutHandler == null) {
            mStopProxyServiceTimeOutHandler = new Handler();
        } else {
            mStopProxyServiceTimeOutHandler.removeCallbacks(mExitPostDelayedCallback);
        }

        if (mStopServicesTimeOutHandler == null) {
            mStopServicesTimeOutHandler = new Handler();
        } else {
            mStopServicesTimeOutHandler.removeCallbacks(mEndServicesPostDelayedCallback);
        }

        if (mBoundProxyService == null) {
            return;
        }

        mStopServicesTimeOutHandler.postDelayed(mEndServicesPostDelayedCallback, EXIT_TIMEOUT);

        if (mBoundProxyService.hasServiceInServicesPool(ServiceType.Audio_Service)) {
            stopAudioService();
        }

        if (mBoundProxyService.hasServiceInServicesPool(ServiceType.Mobile_Nav)) {
            stopMobileNavService();
        }

        if (mServicesCounter.get() == 1) {
            executeDestroyService();
        }
    }

    private void executeDestroyService() {

        if (mStopServicesTimeOutHandler != null) {
            mStopServicesTimeOutHandler.removeCallbacks(mEndServicesPostDelayedCallback);
        }

        ExecutorService executorService = Executors.newSingleThreadExecutor();
        executorService.submit(new Runnable() {
            @Override
            public void run() {

                Logger.d("Start Destroy Service");
                mStopProxyServiceTimeOutHandler.postDelayed(mExitPostDelayedCallback, EXIT_TIMEOUT);

                mBoundProxyService.destroyService();
            }
        });
    }

    private ProgressDialog getExitDialog() {
        if (mExitProgressDialog == null) {
            mExitProgressDialog = new ProgressDialog(this);
            mExitProgressDialog.setTitle(R.string.exit_dialog_title);
            mExitProgressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            mExitProgressDialog.setIndeterminate(true);
        }
        return mExitProgressDialog;
    }

    /**
     * Callback of the exit timer. If the correct destroy procedure fails we use Process.killProcess
     */
    private Runnable mExitPostDelayedCallback = new Runnable() {
        @Override
        public void run() {
            Logger.w("Exit App timer callback");
            mStopProxyServiceTimeOutHandler.removeCallbacks(mExitPostDelayedCallback);
            getExitDialog().dismiss();
            exitApp();
            android.os.Process.killProcess(android.os.Process.myPid());
        }
    };

    /**
     * Callback of the End Services timer.
     */
    private Runnable mEndServicesPostDelayedCallback = new Runnable() {
        @Override
        public void run() {
            Logger.w("End Services callback");
            mStopServicesTimeOutHandler.removeCallbacks(mEndServicesPostDelayedCallback);

            executeDestroyService();
        }
    };

    public void onUSBNoSuchDeviceException() {
        MainApp.getInstance().runInUIThread(new Runnable() {
            @Override
            public void run() {
                AlertDialog.Builder builder = new AlertDialog.Builder(SyncProxyTester.this);
                builder.setTitle("USB problem");
                builder.setMessage("Last session over USB was interrupted incorrectly.\nTry UNPLUG and PLUG USB cable again")
                        .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {

                            }
                        });
                builder.create().show();
            }
        });
    }

    public void onSystemRequestDialogResult(SystemRequest systemRequest) {
        systemRequest.setCorrelationID(getCorrelationid());
        if (mBoundProxyService != null) {
            mBoundProxyService.syncProxySendRPCRequest(systemRequest);
        }
    }
}
