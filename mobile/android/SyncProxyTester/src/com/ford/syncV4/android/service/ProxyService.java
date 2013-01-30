package com.ford.syncV4.android.service;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Vector;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.media.MediaPlayer;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;
import android.util.Pair;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.android.adapters.logAdapter;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.module.ModuleTest;
import com.ford.syncV4.android.policies.PoliciesTest;
import com.ford.syncV4.android.receivers.SyncReceiver;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.AddCommandResponse;
import com.ford.syncV4.proxy.rpc.AddSubMenuResponse;
import com.ford.syncV4.proxy.rpc.AlertManeuverResponse;
import com.ford.syncV4.proxy.rpc.AlertResponse;
import com.ford.syncV4.proxy.rpc.ChangeRegistrationResponse;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSetResponse;
import com.ford.syncV4.proxy.rpc.DeleteCommandResponse;
import com.ford.syncV4.proxy.rpc.DeleteFileResponse;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSetResponse;
import com.ford.syncV4.proxy.rpc.DeleteSubMenuResponse;
import com.ford.syncV4.proxy.rpc.DialNumberResponse;
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
import com.ford.syncV4.proxy.rpc.OnLanguageChange;
import com.ford.syncV4.proxy.rpc.OnPermissionsChange;
import com.ford.syncV4.proxy.rpc.OnTBTClientState;
import com.ford.syncV4.proxy.rpc.OnVehicleData;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.PerformInteractionResponse;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.ReadDIDResponse;
import com.ford.syncV4.proxy.rpc.ResetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.ScrollableMessageResponse;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.SetAppIconResponse;
import com.ford.syncV4.proxy.rpc.SetDisplayLayoutResponse;
import com.ford.syncV4.proxy.rpc.SetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimerResponse;
import com.ford.syncV4.proxy.rpc.ShowConstantTBTResponse;
import com.ford.syncV4.proxy.rpc.ShowResponse;
import com.ford.syncV4.proxy.rpc.SliderResponse;
import com.ford.syncV4.proxy.rpc.SpeakResponse;
import com.ford.syncV4.proxy.rpc.SubscribeButtonResponse;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeButtonResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.UpdateTurnListResponse;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.transport.TCPTransportConfig;

public class ProxyService extends Service implements IProxyListenerALM {	
	static final String TAG = "SyncProxyTester";
	private Integer autoIncCorrId = 1;
	
	private static final String ICON_SYNC_FILENAME = "icon.png";

	private static SyncProxyTester _mainInstance;	
	private static ProxyService _instance;
	private static SyncProxyALM _syncProxy;
	private static logAdapter _msgAdapter;
	private ModuleTest _testerMain;
	private BluetoothAdapter mBtAdapter;
	private MediaPlayer embeddedAudioPlayer;
	private Boolean playingAudio = false;
	protected SyncReceiver mediaButtonReceiver;
	
	private boolean firstHMIStatusChange = true;
	
	private static boolean waitingForResponse = false;
	
	public void onCreate() {
		super.onCreate();
		
		IntentFilter mediaIntentFilter = new IntentFilter();
		mediaIntentFilter.addAction(Intent.ACTION_MEDIA_BUTTON);
		
		mediaButtonReceiver = new SyncReceiver();
		registerReceiver(mediaButtonReceiver, mediaIntentFilter);
		
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage("ProxyService.onCreate()", Log.INFO);
		else Log.i(TAG, "ProxyService.onCreate()");
		
		_instance = this;
	}
	
	public void showLockMain() {
		if(SyncProxyTester.getInstance() == null) {
			Intent i = new Intent(this, SyncProxyTester.class);
			i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			startActivity(i);
		}		
	}
	
	public int onStartCommand(Intent intent, int flags, int startId) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage("ProxyService.onStartCommand()", Log.INFO);
		else Log.i(TAG, "ProxyService.onStartCommand()");
		
		startProxyIfNetworkConnected();
		
        setCurrentActivity(SyncProxyTester.getInstance());
			
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

	private void startProxyIfNetworkConnected() {
		final SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME,
				MODE_PRIVATE);
		final int transportType = prefs.getInt(
				Const.Transport.PREFS_KEY_TRANSPORT_TYPE,
				Const.Transport.PREFS_DEFAULT_TRANSPORT_TYPE);

		if (transportType == Const.Transport.KEY_BLUETOOTH) {
			Log.d(TAG, "ProxyService. onStartCommand(). Transport = Bluetooth.");
			mBtAdapter = BluetoothAdapter.getDefaultAdapter();
			if (mBtAdapter != null) {
				if (mBtAdapter.isEnabled()) {
					startProxy();
				}
			}
		} else {
			Log.d(TAG, "ProxyService. onStartCommand(). Transport = WiFi.");
			if (hasWiFiConnection() == true) {
				Log.d(TAG, "ProxyService. onStartCommand(). WiFi enabled.");
				startProxy();
			} else {
				Log.w(TAG,
						"ProxyService. onStartCommand(). WiFi is not enabled.");
			}
		}
	}

	public void startProxy() {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage("ProxyService.startProxy()", true);
		else Log.i(TAG, "ProxyService.startProxy()");
		
		if (_syncProxy == null) {
			try {
				SharedPreferences settings = getSharedPreferences(
						Const.PREFS_NAME, 0);
				boolean isMediaApp = settings.getBoolean(
						Const.PREFS_KEY_ISMEDIAAPP,
						Const.PREFS_DEFAULT_ISMEDIAAPP);
				int versionNumber = settings.getInt(
						Const.PREFS_KEY_PROTOCOLVERSION,
						Const.PREFS_DEFAULT_PROTOCOLVERSION);
				String appName = settings.getString(Const.PREFS_KEY_APPNAME,
						Const.PREFS_DEFAULT_APPNAME);
				Language lang = Language.valueOf(settings.getString(
						Const.PREFS_KEY_LANG, Const.PREFS_DEFAULT_LANG));
				Language hmiLang = Language.valueOf(settings.getString(
						Const.PREFS_KEY_HMILANG, Const.PREFS_DEFAULT_HMILANG));
				Log.i(TAG, "Using protocol version " + versionNumber);
				int transportType = settings.getInt(
						Const.Transport.PREFS_KEY_TRANSPORT_TYPE,
						Const.Transport.PREFS_DEFAULT_TRANSPORT_TYPE);
				String ipAddress = settings.getString(
						Const.Transport.PREFS_KEY_TRANSPORT_IP,
						Const.Transport.PREFS_DEFAULT_TRANSPORT_IP);
				int tcpPort = settings.getInt(
						Const.Transport.PREFS_KEY_TRANSPORT_PORT,
						Const.Transport.PREFS_DEFAULT_TRANSPORT_PORT);
				boolean autoReconnect = settings
						.getBoolean(
								Const.Transport.PREFS_KEY_TRANSPORT_RECONNECT,
								Const.Transport.PREFS_DEFAULT_TRANSPORT_RECONNECT_DEFAULT);

				if (transportType == Const.Transport.KEY_BLUETOOTH) {
					_syncProxy = new SyncProxyALM(this,
							/*sync proxy configuration resources*/null,
							/*enable advanced lifecycle management true,*/
							appName,
							/*ngn media app*/null,
							/*vr synonyms*/null,
							/*is media app*/isMediaApp,
							/*syncMsgVersion*/null,
							/*language desired*/lang,
							/*HMI Display Language Desired*/hmiLang,
							/*App ID*/"8675309",
							/*autoActivateID*/null,
							/*callbackToUIThread*/ false,
							/*preRegister*/ false,
							versionNumber);
				} else {
					_syncProxy = new SyncProxyALM(this,
							/*sync proxy configuration resources*/null,
							/*enable advanced lifecycle management true,*/
							appName,
							/*ngn media app*/null,
							/*vr synonyms*/null,
							/*is media app*/isMediaApp,
							/*syncMsgVersion*/null,
							/*language desired*/lang,
							/*HMI Display Language Desired*/hmiLang,
							/*App ID*/"8675308",
							/*autoActivateID*/null,
							/*callbackToUIThre1ad*/ false,
							/*preRegister*/ false,
							versionNumber,
							new TCPTransportConfig(tcpPort, ipAddress, autoReconnect));
				}
			} catch (SyncException e) {
				e.printStackTrace();
				//error creating proxy, returned proxy = null
				if (_syncProxy == null){
					stopSelf();
				}
			}
		}
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage("ProxyService.startProxy() returning", Log.INFO);
		else Log.i(TAG, "ProxyService.startProxy() returning");
	}
	
	public void onDestroy() {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage("ProxyService.onDestroy()", Log.INFO);
		else Log.i(TAG, "ProxyService.onDestroy()");
		
		disposeSyncProxy();
		_instance = null;
		if (embeddedAudioPlayer != null) embeddedAudioPlayer.release();		
		unregisterReceiver(mediaButtonReceiver);	
		super.onDestroy();
	}
	
	public void disposeSyncProxy() {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage("ProxyService.disposeSyncProxy()", Log.INFO);
		else Log.i(TAG, "ProxyService.disposeSyncProxy()");
		
		if (_syncProxy != null) {
			try {
				_syncProxy.dispose();
			} catch (SyncException e) {
				e.printStackTrace();
			}
			_syncProxy = null;
		}
	}
	
	private void initialize() {
		playingAudio = true;
		playAnnoyingRepetitiveAudio();

		try {
			_syncProxy.show("Sync Proxy", "Tester", null, null, null, null, nextCorrID());
		} catch (SyncException e) {
			if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
			if (_msgAdapter != null) _msgAdapter.logMessage("Error sending show", Log.ERROR, e, true);
			else Log.e(TAG, "Error sending show", e);
		}

		try { 
			_syncProxy.subscribeButton(ButtonName.OK, nextCorrID());
			_syncProxy.subscribeButton(ButtonName.SEEKLEFT, nextCorrID());
			_syncProxy.subscribeButton(ButtonName.SEEKRIGHT, nextCorrID());
			_syncProxy.subscribeButton(ButtonName.TUNEUP, nextCorrID());
			_syncProxy.subscribeButton(ButtonName.TUNEDOWN, nextCorrID());
			Vector<ButtonName> buttons = new Vector<ButtonName>(Arrays.asList(new ButtonName[] {
					ButtonName.OK, ButtonName.SEEKLEFT, ButtonName.SEEKRIGHT, ButtonName.TUNEUP,
					ButtonName.TUNEDOWN }));
			SyncProxyTester.getInstance().buttonsSubscribed(buttons);
		} catch (SyncException e) {
			if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
			if (_msgAdapter != null) _msgAdapter.logMessage("Error subscribing to buttons", Log.ERROR, e, true);
			else Log.e(TAG, "Error subscribing to buttons", e);
		}

		
		try {
			_syncProxy.addCommand(100, "XML Test", new Vector<String>(Arrays.asList(new String[] {"XML Test", "XML"})), nextCorrID());
			_syncProxy.addCommand(101, "Policies Test", new Vector<String>(Arrays.asList(new String[] {"Policies Test", "Policies"})), nextCorrID());
		} catch (SyncException e) {
			if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
			if (_msgAdapter != null) _msgAdapter.logMessage("Error adding AddCommands", Log.ERROR, e, true);
			else Log.e(TAG, "Error adding AddCommands", e);
		}
	}
	
	public void playPauseAnnoyingRepetitiveAudio() {
		if (embeddedAudioPlayer != null && embeddedAudioPlayer.isPlaying()) {
			playingAudio = false;
			pauseAnnoyingRepetitiveAudio();
		} else {
			playingAudio = true;
			playAnnoyingRepetitiveAudio();
		}
	}

	private void playAnnoyingRepetitiveAudio() {
		if (embeddedAudioPlayer == null) {
			embeddedAudioPlayer = MediaPlayer.create(this, R.raw.arco);
			embeddedAudioPlayer.setLooping(true);
		}
		embeddedAudioPlayer.start();
		
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage("Playing audio", true);
		else Log.i(TAG, "Playing audio");
	}
	
	public void pauseAnnoyingRepetitiveAudio() {
		if (embeddedAudioPlayer != null && embeddedAudioPlayer.isPlaying()) {
			embeddedAudioPlayer.pause();
			
			if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
			if (_msgAdapter != null) _msgAdapter.logMessage("Paused audio", true);
			else Log.i(TAG, "Paused audio");
		}
	}
	
	public static SyncProxyALM getProxyInstance() {
		return _syncProxy;
	}

	public static ProxyService getInstance() {
		return _instance;
	}
	
	public SyncProxyTester getCurrentActivity() {
		return _mainInstance;
	}

	public static void waiting(boolean waiting) {
		waitingForResponse = waiting;
	}

	public void setCurrentActivity(SyncProxyTester currentActivity) {
		if (this._mainInstance != null) {
			this._mainInstance.finish();
			this._mainInstance = null;
		}
		
		this._mainInstance = currentActivity;
		// update the _msgAdapter
		_msgAdapter = SyncProxyTester.getMessageAdapter();
	}
	
	protected int nextCorrID() {
		autoIncCorrId++;
		return autoIncCorrId;
	}

	@Override
	public void onOnHMIStatus(OnHMIStatus notification) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);

		switch(notification.getSystemContext()) {
			case SYSCTXT_MAIN:
				break;
			case SYSCTXT_VRSESSION:
				break;
			case SYSCTXT_MENU:
				break;
			default:
				return;
		}
		
		switch(notification.getAudioStreamingState()) {
			case AUDIBLE:
				if (playingAudio) playAnnoyingRepetitiveAudio();
				break;
			case NOT_AUDIBLE:
				pauseAnnoyingRepetitiveAudio();
				break;
			default:
				return;
		}
		
		boolean hmiChange = false;
		switch(notification.getHmiLevel()) {
			case HMI_FULL:
				if (notification.getFirstRun()) {
					showLockMain();
					_testerMain = new ModuleTest();
					_testerMain = ModuleTest.getModuleTestInstance();
					initialize();
				}
				else {
					try {
						if (!waitingForResponse && _testerMain.getThreadContext() != null) {
							_syncProxy.show("Sync Proxy", "Tester Ready", null, null, null, null, nextCorrID());
						}
					} catch (SyncException e) {
						if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
						if (_msgAdapter != null) _msgAdapter.logMessage("Error sending show", Log.ERROR, e, true);
						else Log.e(TAG, "Error sending show", e);
					}
				}
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
		
		if (hmiChange && firstHMIStatusChange) {
			firstHMIStatusChange = false;
			
			InputStream is = null;
			try {
				is = getResources().openRawResource(R.drawable.fiesta);
				ByteArrayOutputStream os = new ByteArrayOutputStream(is.available());
				final int buffersize = 4096;
				final byte[] buffer = new byte[buffersize];
				int available = 0;
				while ((available = is.read(buffer)) >= 0) {
					os.write(buffer, 0, available);
				}
				
				PutFile putFile = new PutFile();
				putFile.setFileType(FileType.GRAPHIC_PNG);
				putFile.setSyncFileName(ICON_SYNC_FILENAME);
				putFile.setCorrelationID(nextCorrID());
				putFile.setBulkData(os.toByteArray());
				getProxyInstance().sendRPCRequest(putFile);
				
				SetAppIcon setAppIcon = new SetAppIcon();
				setAppIcon.setSyncFileName(ICON_SYNC_FILENAME);
				setAppIcon.setCorrelationID(nextCorrID());
				getProxyInstance().sendRPCRequest(setAppIcon);
			} catch (IOException e) {
				Log.w(TAG, "Can't read icon file", e);
			} catch (SyncException e) {
				Log.w(TAG, "Failed to set app icon", e);
			} finally {
				if (is != null) {
					try {
						is.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}
	}
	
	@Override
	public void onOnCommand(OnCommand notification) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);
		
		switch(notification.getCmdID())
		{
			case 100: //XML Test
				_testerMain.restart();
				break;
			case 101: //Policies Test
				PoliciesTest.runPoliciesTest();
				break;
			default:
				break;
		}
	}

	@Override
	public void onProxyClosed(String info, Exception e) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage("onProxyClosed: " + info, Log.ERROR, e);
		else Log.e(TAG, "onProxyClosed: " + info, e);
		
		boolean wasConnected = !firstHMIStatusChange;
		firstHMIStatusChange = true;
		
		if (wasConnected) {
			final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
			if (mainActivity != null) {
				mainActivity.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						mainActivity.onProxyClosed();
					}
				});
			} else {
				Log.w(TAG, "mainActivity not found");
			}
		}
		
		if(((SyncException) e).getSyncExceptionCause() != SyncExceptionCause.SYNC_PROXY_CYCLED
				&& ((SyncException) e).getSyncExceptionCause() != SyncExceptionCause.BLUETOOTH_DISABLED) {
			reset();
		}
	}
	
	public void reset(){
	   try {
		   _syncProxy.resetProxy();
		} catch (SyncException e1) {
			e1.printStackTrace();
			//something goes wrong, & the proxy returns as null, stop the service.
			//do not want a running service with a null proxy
			if (_syncProxy == null){
				stopSelf();
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
	public void onError(String info, Exception e) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
 		if (_msgAdapter != null) {
 			_msgAdapter.logMessage("******onProxyError******", Log.ERROR);
 			_msgAdapter.logMessage("ERROR: " + info, Log.ERROR, e);
		} else {
			Log.e(TAG, "******onProxyError******");
			Log.e(TAG, "ERROR: " + info, e);
 		}
	}
	
	/*********************************
	** SYNC AppLink Base Callback's **
	*********************************/
	@Override
	public void onAddSubMenuResponse(AddSubMenuResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onCreateInteractionChoiceSetResponse(CreateInteractionChoiceSetResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
		final boolean success = response.getSuccess();
		mainActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mainActivity.onCreateChoiceSetResponse(success);
			}
		});
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onDeleteCommandResponse(DeleteCommandResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onDeleteInteractionChoiceSetResponse(DeleteInteractionChoiceSetResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onDeleteSubMenuResponse(DeleteSubMenuResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onEncodedSyncPDataResponse(EncodedSyncPDataResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onResetGlobalPropertiesResponse(ResetGlobalPropertiesResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onSetMediaClockTimerResponse(SetMediaClockTimerResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onSpeakResponse(SpeakResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onSubscribeButtonResponse(SubscribeButtonResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onUnsubscribeButtonResponse(UnsubscribeButtonResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onOnDriverDistraction(OnDriverDistraction notification) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);
	}
	@Override
	public void onGenericResponse(GenericResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}

	/*********************************
	** SYNC AppLink Soft Button Image Callback's **
	*********************************/
	@Override
	public void onPutFileResponse(PutFileResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onDeleteFileResponse(DeleteFileResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onListFilesResponse(ListFilesResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onSetAppIconResponse(SetAppIconResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onOnButtonEvent(OnButtonEvent notification) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);
	}
	@Override
	public void onOnButtonPress(OnButtonPress notification) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);
		
		switch(notification.getButtonName())
		{
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
	
	/*********************************
	** SYNC AppLink Updated Callback's **
	*********************************/
	@Override
	public void onAddCommandResponse(AddCommandResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onAlertResponse(AlertResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}	
	@Override
	public void onPerformInteractionResponse(PerformInteractionResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onSetGlobalPropertiesResponse(SetGlobalPropertiesResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onShowResponse(ShowResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}

	/*********************************
	** SYNC AppLink New Callback's **
	*********************************/
	@Override
	public void onSliderResponse(SliderResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onScrollableMessageResponse(ScrollableMessageResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onChangeRegistrationResponse(ChangeRegistrationResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onSetDisplayLayoutResponse(SetDisplayLayoutResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onOnLanguageChange(OnLanguageChange notification) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);
	}

	/*********************************
	** SYNC AppLink Audio Pass Thru Callback's **
	*********************************/
	@Override
	public void onPerformAudioPassThruResponse(PerformAudioPassThruResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
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
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
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
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);
		
		final SyncProxyTester mainActivity = SyncProxyTester.getInstance();
		final byte[] aptData = notification.getAPTData();
		mainActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mainActivity.onAudioPassThru(aptData);
			}
		});
	}

	/*********************************
	** SYNC AppLink Vehicle Data Callback's **
	*********************************/
	@Override
	public void onSubscribeVehicleDataResponse(SubscribeVehicleDataResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onUnsubscribeVehicleDataResponse(UnsubscribeVehicleDataResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onGetVehicleDataResponse(GetVehicleDataResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onReadDIDResponse(ReadDIDResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onGetDTCsResponse(GetDTCsResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onOnVehicleData(OnVehicleData notification) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);
	}
	
	/*********************************
	** SYNC AppLink TBT Callback's **
	*********************************/
	@Override
	public void onShowConstantTBTResponse(ShowConstantTBTResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onAlertManeuverResponse(AlertManeuverResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onUpdateTurnListResponse(UpdateTurnListResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	@Override
	public void onOnTBTClientState(OnTBTClientState notification) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);
	}

	/*********************************
	** SYNC AppLink Policies Callback's **
	*********************************/
	@Override
	public void onOnPermissionsChange(OnPermissionsChange notification) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);
	}
	@Override
	public void onOnEncodedSyncPData(OnEncodedSyncPData notification) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(notification, true);
		else Log.i(TAG, "" + notification);
	}

	@Override
	public void onDialNumberResponse(DialNumberResponse response) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage(response, true);
		else Log.i(TAG, "" + response);
		
		if (waitingForResponse && _testerMain.getThreadContext() != null) {
			ModuleTest.responses.add(new Pair<Integer, Result>(response.getCorrelationID(), response.getResultCode()));
			synchronized (_testerMain.getThreadContext()) { _testerMain.getThreadContext().notify();};
		}
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		if (_msgAdapter == null) _msgAdapter = SyncProxyTester.getMessageAdapter();
		if (_msgAdapter != null) _msgAdapter.logMessage("Service on Bind");
		else Log.i(TAG, "Service on Bind");
		return new Binder();
	}
}
