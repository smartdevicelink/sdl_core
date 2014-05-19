package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.DialogInterface;
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
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.view.ViewPager;
import android.support.v7.app.ActionBar;
import android.support.v7.app.ActionBarActivity;
import android.util.Pair;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.Toast;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.mobilenav.AudioServicePreviewFragment;
import com.ford.syncV4.android.activity.mobilenav.MobileNavPreviewFragment;
import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.adapters.SectionsPagerAdapter;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.constants.MenuConstants;
import com.ford.syncV4.android.constants.SyncSubMenu;
import com.ford.syncV4.android.listener.ConnectionListener;
import com.ford.syncV4.android.listener.ConnectionListenersManager;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.manager.BluetoothDeviceManager;
import com.ford.syncV4.android.manager.IBluetoothDeviceManager;
import com.ford.syncV4.android.module.GenericRequest;
import com.ford.syncV4.android.module.ModuleTest;
import com.ford.syncV4.android.receivers.ISyncReceiver;
import com.ford.syncV4.android.receivers.SyncReceiver;
import com.ford.syncV4.android.service.ICloseSession;
import com.ford.syncV4.android.service.IProxyServiceBinder;
import com.ford.syncV4.android.service.IProxyServiceEvent;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.TTSChunkFactory;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.AddSubMenu;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.OnAudioPassThru;
import com.ford.syncV4.proxy.rpc.OnKeyboardInput;
import com.ford.syncV4.proxy.rpc.OnTouchEvent;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThru;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.SystemRequest;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.util.logger.Logger;
import com.lamerman.FileDialog;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Vector;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class SyncProxyTester extends ActionBarActivity implements ActionBar.TabListener,
        IBluetoothDeviceManager, ConnectionListener, PutFileDialog.PutFileDialogListener,
        IProxyServiceEvent {

    @SuppressWarnings("unused")
    private static final String LOG_TAG = SyncProxyTester.class.getSimpleName();

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
    public final static int REQUEST_CHOOSE_XML_TEST = 51;
    private static SyncProxyTester sActivityInstance;
    private static byte[] _ESN;
    /**
     * Autoincrementing id for new softbuttons.
     */
    private static int autoIncSoftButtonId = 5500;
    private ModuleTest mTesterMain;
    /**
     * Latest SyncSubMenu, required to add the submenu from the adapter when a
     * successful AddSubMenuResponse comes.
     */
    private SyncSubMenu mLatestAddSubmenu = null;
    private Pair<Integer, Integer> mLatestAddCommand = null;

    private ArrayAdapter<VehicleDataType> mVehicleDataType = null;
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
    public static final int REQUEST_LIST_SOFTBUTTONS = 43;
    // Request id for ChoiceListActivity
    public static final int REQUEST_LIST_CHOICES = 45;
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
    private final static String REGISTER_APP_INTERFACE_DIALOG_TAG = "RegisterAppInterfaceDialogTag";

    public static final String ButtonSubscriptions = "ButtonSubscriptions";
    public static final String VehicleDataSubscriptions = "VehicleDataSubscriptions";

    private SyncReceiver mSyncReceiver;
    private BluetoothDeviceManager mBluetoothDeviceManager;
    private Session rpcSession = new Session();

    // Get Bound Proxy Service from MainApp
    // make it public to access from Fragments
    public ProxyService mBoundProxyService;

    // Probably this is obsolete approch as there is Services Pool inside SDK
    private ExecutorService mStreamCommandsExecutorService;

    public static SyncProxyTester getInstance() {
        return sActivityInstance;
    }

    public static void setESN(byte[] ESN) {
        _ESN = ESN;
    }

    public static byte[] getESN() {
        return _ESN;
    }

    public static int getNewSoftButtonId() {
        return autoIncSoftButtonId++;
    }

    public void runInUIThread(Runnable runnable) {
        mUIHandler.post(runnable);
    }
    
    /**
     * The {@link android.support.v4.view.PagerAdapter} that will provide
     * fragments for each of the sections. We use a
     * {@link android.support.v4.app.FragmentPagerAdapter} derivative, which will keep every
     * loaded fragment in memory. If this becomes too memory intensive, it
     * may be best to switch to a
     * {@link android.support.v4.app.FragmentStatePagerAdapter}.
     */
    private SectionsPagerAdapter mSectionsPagerAdapter;
    private ArrayAdapter<String> adapter;
    private ActionBar mActionBar;

    /**
     * The {@link android.support.v4.view.ViewPager} that will host the section contents.
     */
    private ViewPager mViewPager;
    private ImageView mImageView;

    /**
     * Action Tab section - start
     */

    @Override
    public void onTabSelected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {
        // When the given tab is selected, switch to the corresponding page in
        // the ViewPager.
        mViewPager.setCurrentItem(tab.getPosition());

        PlaceholderFragment fragment = getCurrentActiveFragment();
        if (fragment == null) {
            Logger.w("Current active fragment is NULL");
            return;
        }
        Logger.d("Current active fragment hash:" + fragment.hashCode());
        mBoundProxyService.setActiveAppId(fragment.getAppId());
    }

    @Override
    public void onTabUnselected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {

    }

    @Override
    public void onTabReselected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {

    }

    /**
     * Action Tab section - end
     */

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Logger.d(LOG_TAG + " On Create");

        sActivityInstance = this;

        setContentView(R.layout.activity_main);

        mImageView = (ImageView) findViewById(R.id.logo_view);

        // Set up the action bar.
        mActionBar = getSupportActionBar();
        mActionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);

        // Create the adapter that will return a fragment for each of the three
        // primary sections of the activity.
        mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

        // Set up the ViewPager with the sections adapter.
        mViewPager = (ViewPager) findViewById(R.id.pager);
        mViewPager.setAdapter(mSectionsPagerAdapter);

        // When swiping between different sections, select the corresponding
        // tab. We can also use ActionBar.Tab#select() to do this if we have
        // a reference to the Tab.
        mViewPager.setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener() {
            @Override
            public void onPageSelected(int position) {
                mActionBar.setSelectedNavigationItem(position);
            }
        });

        addListeners();
        mStreamCommandsExecutorService = Executors.newFixedThreadPool(3);
    }

    /**
     *
     * @param syncAppId Id of the SyncProxyTester application instance
     */
    public void onSetUpDialogResult(final String syncAppId) {

        final PlaceholderFragment fragment = getCurrentActiveFragment();
        if (fragment == null) {
            Logger.e("Current active Fragment is NULL");
            return;
        }

        Logger.d("Current active Fragment '" + syncAppId + "'");

        fragment.setAppId(syncAppId);

        mActionBar.getSelectedTab().setText(syncAppId);

        setUpReceiver();
        showProtocolPropertiesInTitle();
        if (mBoundProxyService != null) {
            mBoundProxyService.setActiveAppId(syncAppId);
            initProxyService(fragment.getLogAdapter());
            try {
                mBoundProxyService.syncProxyOpenSession(syncAppId);
            } catch (SyncException e) {
                Logger.e("SyncProxy Open Session error", e);
            }
        } else {
            MainApp.getInstance().bindProxyToMainApp(new IProxyServiceBinder() {
                @Override
                public void onServiceBindComplete() {
                    Logger.i("Service Bind Complete");
                    getProxyService();
                    initProxyService(fragment.getLogAdapter());

                    mBoundProxyService.setActiveAppId(syncAppId);
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
            registerReceiver(mSyncReceiver, intentFilter);
        }
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

        // TODO : Iterate over all AppId
        /*if (mBoundProxyService.hasServiceInServicesPool(ServiceType.Audio_Service)) {
            //stopAudioService();
        }
        if (mBoundProxyService.hasServiceInServicesPool(ServiceType.Mobile_Nav)) {
            //stopMobileNavService();
        }*/

        if (mBoundProxyService != null) {
            mBoundProxyService.destroyService();
        }
    }

    @Override
    public void onProxyClosed() {
        // TODO : Set callback to Fragment
        //resetAdapters();
    }

    @Override
    public void onPutFileSelected(String fileName) {
        if (getCurrentActiveFragment().getPutFileAdapter() != null) {
            getCurrentActiveFragment().getPutFileAdapter().add(fileName);
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

        // TODO : Implement callback to Fragment

        /*mLogAdapter.logMessage("Service '" + serviceType + "' ended");

        if (mServicesCounter == null) {
            Logger.w("Service End -> Services counter is NULL");
            executeDestroyService();
            return;
        }
        int remainServicesNumber = mServicesCounter.decrementAndGet();
        mLogAdapter.logMessage("Services number remained:" + remainServicesNumber);
        if (remainServicesNumber == 1) {
            executeDestroyService();
        }*/
    }

    @Override
    public void onServiceStart(ServiceType serviceType, byte sessionId, String appId) {

        final PlaceholderFragment fragment = getFragmentByAppId(appId);
        if (fragment == null) {
            Logger.w("OnServiceStart, Fragment with AppId'" + appId + "' is null");
            return;
        }

        if (mBoundProxyService == null) {
            //mLogAdapter.logMessage(SyncProxyTester.class.getSimpleName() + " '" + serviceType +
            //        "' service can not " + "start with NULL Proxy Service", Log.WARN);
            Logger.w(SyncProxyTester.class.getSimpleName() + " '" + serviceType +
                            "' service can not " + "start with NULL Proxy Service");
            return;
        }

        Logger.d(LOG_TAG + " Fragment '" + fragment + "' associated with " +
                "sessionId:" + sessionId + " appId:" + appId);

        if (fragment == getCurrentActiveFragment()) {
            mBoundProxyService.setActiveAppId(appId);
        }

        fragment.getLogAdapter().logMessage("Service '" + serviceType + "' started, " +
                "protocol version: " + mBoundProxyService.syncProxyGetWiProVersion(), true);

        if (serviceType == ServiceType.Audio_Service) {
            MainApp.getInstance().runInUIThread(new Runnable() {
                @Override
                public void run() {
                    OutputStream outputStream = mBoundProxyService
                            .syncProxyStartAudioDataTransfer(fragment.getSessionId());
                    if (outputStream == null) {
                        return;
                    }
                    fragment.setAudioServiceStateOn(outputStream);
                }
            });
        } else if (serviceType == ServiceType.Mobile_Nav) {
            MainApp.getInstance().runInUIThread(new Runnable() {
                @Override
                public void run() {
                    OutputStream outputStream = mBoundProxyService
                            .syncProxyStartH264(fragment.getSessionId());
                    if (outputStream == null) {
                        return;
                    }
                    fragment.setMobileNaviStateOn(outputStream);
                }
            });
        } else if (serviceType == ServiceType.RPC) {
            // Assign Session Id to the associated Fragment (by AppId)
            fragment.setSessionId(sessionId);
        }



        /*if (serviceType == ServiceType.Audio_Service) {
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

        mServicesCounter.incrementAndGet();*/
    }

    @Override
    public void onAckReceived(int frameReceived, ServiceType serviceType) {
        // TODO : Implement callback to Fragment
        //mLogAdapter.logMessage("Service '" + serviceType + "' Ack received, n:" + frameReceived);
    }

    @Override
    public void onStartServiceNackReceived(final String appId, final byte sessionId, final ServiceType serviceType) {

        // TODO : Implement callback to Fragment
        final PlaceholderFragment fragment = getFragmentByAppId(appId);
        if (fragment == null) {
            Logger.w("OnStartServiceNack, Fragment with AppId'" + appId + "' is null");
            return;
        }

        if (mBoundProxyService == null) {
            //mLogAdapter.logMessage(SyncProxyTester.class.getSimpleName() + " '" + serviceType +
            //        "' service can not " + "start with NULL Proxy Service", Log.WARN);
            Logger.w(SyncProxyTester.class.getSimpleName() + " '" + serviceType +
                    "' service can not " + "start with NULL Proxy Service");
            return;
        }

        fragment.getLogAdapter().logMessage("Start Service '" + serviceType + "' Nack received",
                true);

        MainApp.getInstance().runInUIThread(new Runnable() {
            @Override
            public void run() {
                if (serviceType == ServiceType.Mobile_Nav) {
                    fragment.setMobileNaviStateOff();
                } else if (serviceType == ServiceType.Audio_Service) {
                    fragment.setAudioServiceStateOff();
                }
            }
        });
    }

    public int getNextCorrelationIdForCurrentFragment() {
        if (getCurrentActiveFragment() == null) {
            return 0;
        }
        return getCurrentActiveFragment().getCorrelationId();
    }

    /**
     * Displays the current protocol properties in the activity's title.
     */
    private void showProtocolPropertiesInTitle() {
        final SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME, 0);
        int protocolVersion = ProtocolConstants.PROTOCOL_VERSION_MIN;
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
        setTitle(getResources().getString(R.string.tester_app_name) + " (Start protocol v:"
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

        sActivityInstance = null;
        if (mBoundProxyService != null) {
            mBoundProxyService.addLogAdapter(null);
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
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        MenuItem menuItem = menu.findItem(MenuConstants.MENU_WAKE_LOCK);
        if (menuItem != null) {
            menuItem.setChecked(!AppPreferencesManager.getDisableLockFlag());
        }
        return super.onPrepareOptionsMenu(menu);
    }

    /* Handles item selections */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        switch (id) {
            case R.id.action_add_session_instance:
                addFragment();
                return true;
            case R.id.action_app_version:
                new AlertDialog.Builder(this)
                        .setTitle("App version")
                        .setMessage("Ver:" + AppUtils.getApplicationVersion() +
                                ", Code:" + String.valueOf(AppUtils.getCodeVersionNumber()) +
                                "\n" + AppUtils.getBuildInfo() + "\n\nCHANGELOG:\n" +
                                AppUtils.getChangeLog())
                        .setNeutralButton(android.R.string.ok, null).create().show();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    public void closeSession(final String appId, final int position) throws SyncException {
        if (mBoundProxyService != null) {
            Logger.d("Session of the '" + appId + "' and pos '" + position + "' is going to be closed");
            mBoundProxyService.setCloseSessionCallback(new ICloseSession() {

                @Override
                public void onCloseSessionComplete() {
                    //finishActivity();
                    Logger.d("Session of the '" + appId + "' is closed");

                    MainApp.getInstance().runInUIThread(new Runnable() {
                        @Override
                        public void run() {
                            removeFragment(position);
                        }
                    });
                }
            });
            mBoundProxyService.syncProxyCloseSession(appId);
        }
    }

    private void addFragment() {
        mSectionsPagerAdapter.incrementCounter();

        // Create a tab with text corresponding to the page title defined by
        // the adapter. Also specify this Activity object, which implements
        // the TabListener interface, as the callback (listener) for when
        // this tab is selected.
        mActionBar.addTab(mActionBar.newTab()
                        .setText(mSectionsPagerAdapter.getPageTitle(mSectionsPagerAdapter.getCount()))
                        .setTabListener(this)
        );

        mViewPager.setCurrentItem(mSectionsPagerAdapter.getCount());

        mImageView.setVisibility(View.GONE);
        mViewPager.setVisibility(View.VISIBLE);
    }

    private void removeFragment(int position) {
        /*mSectionsPagerAdapter.decrementCounter();
        mSectionsPagerAdapter.removeFragmentFromList(position);

        *//*if (mSectionsPagerAdapter.getCount() > 0) {
            mViewPager.setCurrentItem(0);
            mActionBar.setSelectedNavigationItem(0);
        }*//*

        mActionBar.removeTabAt(position);

        if (mSectionsPagerAdapter.getCount() == 0) {
            mImageView.setVisibility(View.VISIBLE);
            mViewPager.setVisibility(View.GONE);
        }

        mViewPager.setAdapter(null);
        mViewPager.setAdapter(mSectionsPagerAdapter);*/
    }

    private void finishActivity() {
        super.finish();
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

    public int getFragmentsCount() {
        return mSectionsPagerAdapter.getCount();
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
		        	Logger.i("CLASS_NAME", "A No Paired devices with the name sync");
		        }
		        
		        if (isSYNCpaired == true) { 		        	
		        	_applinkService = new ProxyService();
		    		if (ProxyService.getInstance() == null) {
		    			Intent startIntent = new Intent(this, ProxyService.class);
		    			startService(startIntent);
		    			//bindService(startIntent, this, Context.BIND_AUTO_CREATE);
		    		} else {
		    			// need to get the instance and add myself as a listener
		    			ProxyService.getInstance().addLogAdapter(this);
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

        //loadMessageSelectCount();
    }

    /**
     * @return array adapter with a function names to perform message sending operations
     */
    public ArrayAdapter<String> getFunctionsAdapter() {
        if (adapter == null) {
            fillFunctionsAdapter();
        }
        return adapter;
    }

    private void initProxyService(LogAdapter logAdapter) {
        mBoundProxyService.addLogAdapter(logAdapter);
        mBoundProxyService.setProxyServiceEvent(this);
    }

    private void getProxyService() {
        mBoundProxyService = null;
        mBoundProxyService = MainApp.getInstance().getBoundProxyService();
    }

    /**
     * Splits the string with a comma and returns a vector of TTSChunks.
     */
    public static Vector<TTSChunk> ttsChunksFromString(String string) {
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
                getCurrentActiveFragment().getSubMenuAdapter().add(sm);
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
        return getCurrentActiveFragment().getSubMenuAdapter();
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
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.android.activity.PolicyFilesSetUpDialog}
     */
    public void onPolicyFilesSetUpDialogResult_SendUpdate(FileType fileType, RequestType requestType) {
        Logger.d("PolicyFilesSetUpDialogResult fileType:" + fileType + " requestType:" + requestType);
        if (mBoundProxyService != null) {
            mBoundProxyService.sendPolicyTableUpdate(fileType, requestType);
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

                // TODO : Implement
                //onSetUpDialogResult();
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
        getCurrentActiveFragment().getCommandAdapter().add(cmdID);
        if (null != submenuID) {
            getCurrentActiveFragment().getCommandIdToParentSubmenuMap().put(cmdID, submenuID);
        }
    }

    /**
     * Removes command ID from the adapter.
     *
     * @param cmdID ID of the command
     */
    private void removeCommandFromList(Integer cmdID) {
        getCurrentActiveFragment().getCommandAdapter().remove(cmdID);
        getCurrentActiveFragment().getCommandIdToParentSubmenuMap().remove(cmdID);
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

    /**
     * Return a clone of the {@code isVehicleDataSubscribed}
     *
     * @return a clone of the {@code isVehicleDataSubscribed}
     */
    public boolean[] cloneIsVehicleDataSubscribed() {
        return getCurrentActiveFragment().cloneIsVehicleDataSubscribed();
    }

    /**
     * @param position position in the array
     * @return
     */
    public boolean getIsVehicleDataSubscribedAt(int position) {
        return getCurrentActiveFragment().getIsVehicleDataSubscribedAt(position);
    }

    /**
     * Set a value of the {@code isVehicleDataSubscribed} array
     *
     * @param value
     */
    public void setIsVehicleDataSubscribed(boolean[] value) {
        getCurrentActiveFragment().setIsVehicleDataSubscribed(value);
    }

    /**
     * Called when a CreateChoiceSetResponse comes. If successful, add it to the
     * adapter. In any case, remove the key from the map.
     */
    public void onCreateChoiceSetResponse(boolean success) {

        // TODO : Implement callback to Fragment

        /*if (mLatestCreateChoiceSetId != CHOICESETID_UNSET) {
            if (success) {
                getCurrentActiveFragment().getChoiceSetAdapter().add(mLatestCreateChoiceSetId);
            }
            mLatestCreateChoiceSetId = CHOICESETID_UNSET;
        } else {
            Logger.w("Latest createChoiceSetId is unset");
        }*/
    }

    /**
     * Called when a DeleteChoiceSetResponse comes. If successful, remove it
     * from the adapter.
     */
    public void onDeleteChoiceSetResponse(boolean success) {

        // TODO : Implement callback to Fragment

        /*if (_latestDeleteChoiceSetId != CHOICESETID_UNSET) {
            if (success) {
                getCurrentActiveFragment().getChoiceSetAdapter().remove(_latestDeleteChoiceSetId);
            }
            _latestDeleteChoiceSetId = CHOICESETID_UNSET;
        } else {
            Logger.w("Latest deleteChoiceSetId is unset");
        }*/
    }

    /**
     * Called when a DeleteSubMenuResponse comes. If successful, remove it from
     * the adapter. We also need to delete all the commands that were added to
     * this submenu.
     */
    public void onDeleteSubMenuResponse(boolean success) {

        // TODO : Implement in the Fragment

        /*if (_latestDeleteSubmenu != null) {
            if (success) {
                getCurrentActiveFragment().getSubMenuAdapter().remove(_latestDeleteSubmenu);

                for (Iterator<Entry<Integer, Integer>> it = getCurrentActiveFragment().getCommandIdToParentSubmenuMap()
                        .entrySet().iterator(); it.hasNext(); ) {
                    Entry<Integer, Integer> entry = it.next();
                    if (entry.getValue() == _latestDeleteSubmenu.getSubMenuId()) {
                        getCurrentActiveFragment().getCommandAdapter().remove(entry.getKey());
                        it.remove();
                    }
                }
            }
            _latestDeleteSubmenu = null;
        } else {
            Logger.w("Latest deleteSubMenu is unset");
        }*/
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

        // TODO : Implement in the Fragment

        /*if (_latestDeleteCommandCmdID != null) {
            if (success) {
                removeCommandFromList(_latestDeleteCommandCmdID);
            }
            _latestDeleteCommandCmdID = null;
        } else {
            Logger.w("Latest deleteCommand is unset");
        }*/
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

        // TODO : Implement in the Fragment

        /*closeAudioPassThruStream();
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
        }*/
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

    /*private void sendCreateInteractionChoiceSet(Vector<Choice> choices) {
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
    }*/

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_LIST_SOFTBUTTONS:
                if (resultCode == RESULT_OK) {

                    // TODO : Implement callback to Fragment
                    //currentSoftButtons = (Vector<SoftButton>) IntentHelper.
                    //        getObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);


                    //if (chkIncludeSoftButtons != null) {
                    //    chkIncludeSoftButtons.setChecked(true);
                    //}
                }
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                break;

            case REQUEST_LIST_CHOICES:
                if (resultCode == RESULT_OK) {
                    Vector<Choice> choices = (Vector<Choice>) IntentHelper.
                            getObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);

                    // TODO : Implement callback to Fragment
                    //sendCreateInteractionChoiceSet(choices);
                }
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                break;

            case Const.REQUEST_FILE_OPEN:
                if (resultCode == RESULT_OK) {

                    // TODO : Implement in the Fragment

                    /*String fileName = data.getStringExtra(FileDialog.RESULT_PATH);
                    if (txtLocalFileName != null) {
                        txtLocalFileName.setText(fileName);
                    }*/
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
     * Called when the app is activated from HMI for the first time. ProxyService
     * automatically subscribes to buttons, so we reflect that in the
     * subscription list.
     */
    public void buttonsSubscribed(Vector<ButtonName> buttons) {
        if (getCurrentActiveFragment() != null) {
            getCurrentActiveFragment().buttonsSubscribed(buttons);
        }
    }

    public void startMobileNaviService(final String appId) {

        mStreamCommandsExecutorService.submit(new Runnable() {
            @Override
            public void run() {
                if (!isProxyReadyForWork()) {
                    return;
                }
                // TODO : Need to transfer AppId into SDK
                PlaceholderFragment fragment = getFragmentByAppId(appId);
                if (fragment == null) {
                    Logger.w("Start Mobile Navi service, Fragment NULL");
                    return;
                }
                fragment.getLogAdapter().logMessage("Should start Mobile Navi service", true);

                if (mBoundProxyService == null) {
                    return;
                }
                mBoundProxyService.syncProxyStartMobileNavService(fragment.getSessionId());
            }
        });
    }

    public void onMobileNaviError(String errorMsg) {
        onMobileNaviError(errorMsg, true);
    }

    public void onMobileNaviError(String errorMsg, boolean addToUI) {

        // TODO : Implement callback to Fragment
        //mLogAdapter.logMessage(errorMsg, addToUI);

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

                // TODO : Implement callback to Fragment
                //mLogAdapter.logMessage(e.getMessage(), true);
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

    public void stopMobileNavService(final String appId) {

        mStreamCommandsExecutorService.submit(new Runnable() {
            @Override
            public void run() {
                if (!isProxyReadyForWork()) {
                    return;
                }
                // TODO : Need to transfer AppId into SDK
                final PlaceholderFragment fragment = getFragmentByAppId(appId);
                if (fragment == null) {
                    Logger.w("Stop Audio Service, Fragment NULL");
                    return;
                }
                fragment.getLogAdapter().logMessage("Should stop Audio service", true);

                if (mBoundProxyService == null) {
                    return;
                }
                mBoundProxyService.syncProxyStopMobileNaviService(fragment.getSessionId());
                closeMobileNaviOutputStream();
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

    public void startAudioService(final String appId) {

        mStreamCommandsExecutorService.submit(new Runnable() {
            @Override
            public void run() {
                if (!isProxyReadyForWork()) {
                    return;
                }
                // TODO : Need to transfer AppId into SDK
                PlaceholderFragment fragment = getFragmentByAppId(appId);
                if (fragment == null) {
                    Logger.w("Start Audio Service, Fragment NULL");
                    return;
                }
                fragment.getLogAdapter().logMessage("Should start Audio service", true);

                if (mBoundProxyService == null) {
                    return;
                }
                mBoundProxyService.syncProxyStartAudioService(fragment.getSessionId());
            }
        });
    }

    public void stopAudioService(final String appId) {

        mStreamCommandsExecutorService.submit(new Runnable() {
            @Override
            public void run() {
                if (!isProxyReadyForWork()) {
                    return;
                }
                // TODO : Need to transfer AppId into SDK
                final PlaceholderFragment fragment = getFragmentByAppId(appId);
                if (fragment == null) {
                    Logger.w("Stop Audio Service, Fragment NULL");
                    return;
                }
                fragment.getLogAdapter().logMessage("Should stop Audio service", true);

                if (mBoundProxyService == null) {
                    return;
                }
                mBoundProxyService.syncProxyStopAudioService(fragment.getSessionId());
                closeAudioOutputStream();
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

    private PlaceholderFragment getFragmentAt(int index) {
        Fragment fragment = getSupportFragmentManager()
                .findFragmentByTag("android:switcher:" + R.id.pager + ":" + index);
        // based on the current position you can then cast the fragment to the correct
        // class and call the method:
        if (fragment != null) {
            return ((PlaceholderFragment) fragment);
        }
        return null;
    }

    private PlaceholderFragment getCurrentActiveFragment() {
        Fragment fragment = getSupportFragmentManager()
                .findFragmentByTag("android:switcher:" + R.id.pager + ":" +
                        mViewPager.getCurrentItem());
        // based on the current position you can then cast the fragment to the correct
        // class and call the method:
        if (fragment != null) {
            return ((PlaceholderFragment) fragment);
        }
        return null;
    }

    private PlaceholderFragment getFragmentWithoutSessionId() {
        PlaceholderFragment fragment = null;
        int pagesCount = mViewPager.getAdapter().getCount();
        for (int i = 0; i < pagesCount; i++) {
            fragment = (PlaceholderFragment) getSupportFragmentManager()
                    .findFragmentByTag("android:switcher:" + R.id.pager + ":" + i);
            if (fragment == null) {
                continue;
            }
            if (fragment.getSessionId() == 0) {
                return fragment;
            }
        }
        return fragment;
    }

    private PlaceholderFragment getFragmentByAppId(String appId) {
        PlaceholderFragment fragment = null;
        int pagesCount = mViewPager.getAdapter().getCount();
        for (int i = 0; i < pagesCount; i++) {
            fragment = (PlaceholderFragment) getSupportFragmentManager()
                    .findFragmentByTag("android:switcher:" + R.id.pager + ":" + i);
            if (fragment == null) {
                continue;
            }
            if (fragment.getAppId().equals(appId)) {
                return fragment;
            }
        }
        return fragment;
    }

    /**
     * Exit Application section
     */

    /**
     * Exit from Activity
     */
    private void exitApp() {
        Logger.i("Exit " + SyncProxyTester.class.getSimpleName() + "\n\n");
        super.finish();

        MainApp.getInstance().exitApp();
    }

    // TODO : Move this block to MainApp

    /**
     * Stops the proxy service.
     */
    public void stopProxyServiceOnExit() {
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

        // TODO : Iterate over all AppId
        /*if (mBoundProxyService.hasServiceInServicesPool(ServiceType.Audio_Service)) {
            //stopAudioService();
        }

        if (mBoundProxyService.hasServiceInServicesPool(ServiceType.Mobile_Nav)) {
            //stopMobileNavService();
        }*/

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

        // TODO : Reconsider CorId
        /*systemRequest.setCorrelationID(getCorrelationid());
        if (mBoundProxyService != null) {
            mBoundProxyService.syncProxySendRPCRequest(systemRequest);
        }*/
    }
}