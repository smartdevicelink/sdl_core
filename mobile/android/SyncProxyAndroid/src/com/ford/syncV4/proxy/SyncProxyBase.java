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
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.proxy.callbacks.InternalProxyMessage;
import com.ford.syncV4.proxy.callbacks.OnError;
import com.ford.syncV4.proxy.callbacks.OnProxyClosed;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.interfaces.IProxyListenerBase;
import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.AddCommandResponse;
import com.ford.syncV4.proxy.rpc.AddSubMenu;
import com.ford.syncV4.proxy.rpc.AddSubMenuResponse;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.AlertManeuverResponse;
import com.ford.syncV4.proxy.rpc.AlertResponse;
import com.ford.syncV4.proxy.rpc.ButtonCapabilities;
import com.ford.syncV4.proxy.rpc.ChangeRegistrationResponse;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSetResponse;
import com.ford.syncV4.proxy.rpc.DeleteCommand;
import com.ford.syncV4.proxy.rpc.DeleteCommandResponse;
import com.ford.syncV4.proxy.rpc.DeleteFileResponse;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSetResponse;
import com.ford.syncV4.proxy.rpc.DeleteSubMenu;
import com.ford.syncV4.proxy.rpc.DeleteSubMenuResponse;
import com.ford.syncV4.proxy.rpc.DisplayCapabilities;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.proxy.rpc.EncodedSyncPDataResponse;
import com.ford.syncV4.proxy.rpc.EndAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.GenericResponse;
import com.ford.syncV4.proxy.rpc.GetDTCsResponse;
import com.ford.syncV4.proxy.rpc.GetVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.ListFilesResponse;
import com.ford.syncV4.proxy.rpc.OnAppInterfaceUnregistered;
import com.ford.syncV4.proxy.rpc.OnAudioPassThru;
import com.ford.syncV4.proxy.rpc.OnButtonEvent;
import com.ford.syncV4.proxy.rpc.OnButtonPress;
import com.ford.syncV4.proxy.rpc.OnCommand;
import com.ford.syncV4.proxy.rpc.OnDriverDistraction;
import com.ford.syncV4.proxy.rpc.OnEncodedSyncPData;
import com.ford.syncV4.proxy.rpc.OnHMIStatus;
import com.ford.syncV4.proxy.rpc.OnLanguageChange;
import com.ford.syncV4.proxy.rpc.OnPermissionsChange;
import com.ford.syncV4.proxy.rpc.OnSyncPData;
import com.ford.syncV4.proxy.rpc.OnTBTClientState;
import com.ford.syncV4.proxy.rpc.OnVehicleData;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.PerformInteraction;
import com.ford.syncV4.proxy.rpc.PerformInteractionResponse;
import com.ford.syncV4.proxy.rpc.PresetBankCapabilities;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.ReadDIDResponse;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.RegisterAppInterfaceResponse;
import com.ford.syncV4.proxy.rpc.ResetGlobalProperties;
import com.ford.syncV4.proxy.rpc.ResetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.ScrollableMessageResponse;
import com.ford.syncV4.proxy.rpc.SetAppIconResponse;
import com.ford.syncV4.proxy.rpc.SetDisplayLayoutResponse;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.SetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimer;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimerResponse;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.ShowConstantTBTResponse;
import com.ford.syncV4.proxy.rpc.ShowResponse;
import com.ford.syncV4.proxy.rpc.SliderResponse;
import com.ford.syncV4.proxy.rpc.SoftButtonCapabilities;
import com.ford.syncV4.proxy.rpc.Speak;
import com.ford.syncV4.proxy.rpc.SpeakResponse;
import com.ford.syncV4.proxy.rpc.SubscribeButton;
import com.ford.syncV4.proxy.rpc.SubscribeButtonResponse;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.SyncPData;
import com.ford.syncV4.proxy.rpc.SyncPDataResponse;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterface;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterfaceResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeButton;
import com.ford.syncV4.proxy.rpc.UnsubscribeButtonResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.UpdateTurnListResponse;
import com.ford.syncV4.proxy.rpc.VehicleType;
import com.ford.syncV4.proxy.rpc.enums.AppType;
import com.ford.syncV4.proxy.rpc.enums.AudioStreamingState;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;
import com.ford.syncV4.proxy.rpc.enums.HMILevel;
import com.ford.syncV4.proxy.rpc.enums.HmiZoneCapabilities;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.SyncConnectionState;
import com.ford.syncV4.proxy.rpc.enums.SyncDisconnectedReason;
import com.ford.syncV4.proxy.rpc.enums.SyncInterfaceAvailability;
import com.ford.syncV4.proxy.rpc.enums.SystemContext;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;
import com.ford.syncV4.proxy.rpc.enums.VrCapabilities;
import com.ford.syncV4.syncConnection.ISyncConnectionListener;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.trace.SyncTrace;
import com.ford.syncV4.trace.TraceDeviceInfo;
import com.ford.syncV4.trace.enums.InterfaceActivityDirection;
import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.transport.SiphonServer;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.util.Base64;
import com.ford.syncV4.util.DebugTool;

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
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.util.Hashtable;
import java.util.Vector;

public abstract class SyncProxyBase<proxyListenerType extends IProxyListenerBase> {
	// Used for calls to Android Log class.
	public static final String TAG = "SyncProxy";
	private static final String SYNC_LIB_TRACE_KEY = "42baba60-eb57-11df-98cf-0800200c9a66";

	private SyncConnection _syncConnection;
	private proxyListenerType _proxyListener = null;
	
	// Protected Correlation IDs
	private final int 	REGISTER_APP_INTERFACE_CORRELATION_ID = 65529,
						UNREGISTER_APP_INTERFACE_CORRELATION_ID = 65530,
						HEARTBEAT_CORRELATION_ID = 65531,
						POLICIES_CORRELATION_ID = 65535;
	
	// Synchronization Objects
	private static final Object CONNECTION_REFERENCE_LOCK = new Object(),
								INCOMING_MESSAGE_QUEUE_THREAD_LOCK = new Object(),
								OUTGOING_MESSAGE_QUEUE_THREAD_LOCK = new Object(),
								INTERNAL_MESSAGE_QUEUE_THREAD_LOCK = new Object(),
								APP_INTERFACE_REGISTERED_LOCK = new Object();
		
	// Heartbeat members
	private ProxyHeartBeat _proxyHeartBeat = null;
	private boolean _heartBeatEnabled = false;
	private long _interfaceIdleTimeLimit = 3000; // 3 seconds
	private long _heartbeatResponsePastDueTimeLimit = 2000; // 2 seconds

	// RPC Session ID
	private byte _rpcSessionID = 0;
	
	// Device Info for logging
	private TraceDeviceInfo _traceDeviceInterrogator = null;
		
	// Declare Queuing Threads
	private ProxyMessageDispatcher<ProtocolMessage> _incomingProxyMessageDispatcher;
	private ProxyMessageDispatcher<ProtocolMessage> _outgoingProxyMessageDispatcher;
	private ProxyMessageDispatcher<InternalProxyMessage> _internalProxyMessageDispatcher;
	
	// Flag indicating if callbacks should be called from UIThread
	private Boolean _callbackToUIThread = false;
	// UI Handler
	private Handler _mainUIHandler = null; 
	
	// SyncProxy Advanced Lifecycle Management
	protected Boolean _advancedLifecycleManagementEnabled = false;
	// Parameters passed to the constructor from the app to register an app interface
	private String _applicationName = null;
	private Vector<TTSChunk> _ttsName = null;
	private String _ngnMediaScreenAppName = null;
	private Boolean _isMediaApp = null;
	private Language _syncLanguageDesired = null;
	private Language _hmiDisplayLanguageDesired = null;
	private Vector<AppType> _appType = null;
	private String _appID = null;
	private String _autoActivateIdDesired = null;
	private SyncMsgVersion _syncMsgVersionRequest = null;
	private Vector<String> _vrSynonyms = null;
	
	// JSON RPC Marshaller
	private IJsonRPCMarshaller _jsonRPCMarshaller = new JsonRPCMarshaller();
	
	/**
	 * Contains current configuration for the transport that was selected during 
	 * construction of this object
	 */
	private BaseTransportConfig _transportConfig = null;
	// Proxy State Variables
	protected Boolean _appInterfaceRegisterd = false;
	private Boolean _haveReceivedFirstNonNoneHMILevel = false;
	protected Boolean _haveReceivedFirstFocusLevel = false;
	protected Boolean _haveReceivedFirstFocusLevelFull = false;
	protected Boolean _proxyDisposed = false;
	protected SyncConnectionState _syncConnectionState = null;
	protected SyncInterfaceAvailability _syncIntefaceAvailablity = null;
	protected HMILevel _hmiLevel = null;
	private HMILevel _priorHmiLevel = null;
	protected AudioStreamingState _audioStreamingState = null;
	private AudioStreamingState _priorAudioStreamingState = null;
	protected SystemContext _systemContext = null;
	// Variables set by RegisterAppInterfaceResponse
	protected SyncMsgVersion _syncMsgVersion = null;
	protected String _autoActivateIdReturned = null;
	protected Language _syncLanguage = null;
	protected Language _hmiDisplayLanguage = null;
	protected DisplayCapabilities _displayCapabilities = null;
	protected Vector<ButtonCapabilities> _buttonCapabilities = null;
	protected Vector<SoftButtonCapabilities> _softButtonCapabilities = null;
	protected PresetBankCapabilities _presetBankCapabilities = null;
	protected Vector<HmiZoneCapabilities> _hmiZoneCapabilities = null;
	protected Vector<SpeechCapabilities> _speechCapabilities = null;
	protected Vector<VrCapabilities> _vrCapabilities = null;
	protected VehicleType _vehicleType = null;
	protected Boolean firstTimeFull = true;
	
	protected byte _wiproVersion = 1;
	
	// Interface broker
	private SyncInterfaceBroker _interfaceBroker = null;
	
	// Private Class to Interface with SyncConnection
	private class SyncInterfaceBroker implements ISyncConnectionListener {
		
		@Override
		public void onTransportDisconnected(String info) {
			// proxyOnTransportDisconnect is called to alert the proxy that a requested
			// disconnect has completed
			
			if (_advancedLifecycleManagementEnabled) {
				// If ALM, nothing is required to be done here
			} else {
				// If original model, notify app the proxy is closed so it will delete and reinstanciate 
				notifyProxyClosed(info, new SyncException("Transport disconnected.", SyncExceptionCause.SYNC_UNAVAILALBE));
			}
		}

		@Override
		public void onTransportError(String info, Exception e) {
			DebugTool.logError("Transport failure: " + info, e);
			
			if (_advancedLifecycleManagementEnabled) {			
				// Cycle the proxy
				cycleProxy(SyncDisconnectedReason.TRANSPORT_ERROR);
			} else {
				notifyProxyClosed(info, e);
			}
		}

		@Override
		public void onProtocolMessageReceived(ProtocolMessage msg) {
			if (_proxyHeartBeat != null && _heartBeatEnabled) {
				_proxyHeartBeat.recordMostRecentIncomingProtocolInterfaceActivity();
			} // end-if
			
			try {if (msg.getData().length > 0) queueIncomingMessage(msg);}
			catch (Exception e) {}
			try {if (msg.getBulkData().length > 0) queueIncomingMessage(msg);}
			catch (Exception e) {}
		}

		@Override
		public void onProtocolSessionStarted(SessionType sessionType,
				byte sessionID, byte version, String correlationID) {
			if (_wiproVersion == 1) {
				if (version == 2) setWiProVersion(version);
			}
			if (sessionType.eq(SessionType.RPC)) {			
				startRPCProtocolSession(sessionID, correlationID);
			} else if (_wiproVersion == 2) {
				//If version 2 then don't need to specify a Session Type
				startRPCProtocolSession(sessionID, correlationID);
			}  else {
				// Handle other protocol session types here
			}
		}

		@Override
		public void onProtocolSessionEnded(SessionType sessionType,
				byte sessionID, String correlationID) {
			// How to handle protocol session ended?
				// How should protocol session management occur? 
		}

		@Override
		public void onProtocolError(String info, Exception e) {
			passErrorToProxyListener(info, e);
		}
	}
	
	private class ProxyHeartBeat {
		
		private final String THREAD_NAME = "SyncHeartbeat";
		private final long POLLING_INTERVAL = 1000; // 1 second
		
		private boolean _isHalted = false;
		
		private Thread _heartbeatThread = null;
		private boolean _heartbeatActive = false;
		private int _pendingRequestCount = 0;
		private long _interfaceMostRecentActivityTimestamp_ms = 0;
		private long _mostRecentHeartbeatSentTimestamp_ms = 0;
		private boolean _heartbeatRequestInProgress = false;		
		
		public ProxyHeartBeat() {
			// Set initial value of heart beat members
			_heartbeatRequestInProgress = false;
			_heartbeatActive = false;
			_interfaceMostRecentActivityTimestamp_ms = 0;
			_mostRecentHeartbeatSentTimestamp_ms = 0;
			_pendingRequestCount = 0;
			
			_heartbeatThread = new Thread(new Runnable() {
				@Override
				public void run() {
					try {
						while (!_isHalted) {
							heartbeatProcessing();
						}
					} catch (InterruptedException e) {
						SyncTrace.logProxyEvent("Heartbeat thread interupted.", SYNC_LIB_TRACE_KEY);
					}
				}
			});
			_heartbeatThread.setName(THREAD_NAME);
			_heartbeatThread.setDaemon(true);
			_heartbeatActive = true;
		}
		
		// Method to start the heart beat
		public void startHeartBeat() {
			_heartbeatThread.start();
		}
		
		// Set heart beat active
		public void setHeartBeatActive(boolean isActive) {
			_heartbeatActive = isActive;
		}
		
		// Manage most-recent activity timestamp for pseudo-heartbeat
		// TODO Remove on next release if proven unnecessary 
		@Deprecated
		private	synchronized void changePendingRequestCount(boolean incrementCount) {		
			if (incrementCount) {
				_pendingRequestCount++;
			} else {
				_pendingRequestCount--;
			}
		}
		
		private synchronized void recordMostRecentHeartbeatSentTimestamp() {
			_mostRecentHeartbeatSentTimestamp_ms = System.currentTimeMillis();
		}
		
		private synchronized void recordMostRecentIncomingProtocolInterfaceActivity() {		
			_interfaceMostRecentActivityTimestamp_ms = System.currentTimeMillis();
			if (_heartbeatRequestInProgress) {
				cancelHeartbeatRequest();
			}
		}
		
		private synchronized boolean heartbeatRequired() {		
			long idleTime = System.currentTimeMillis() - _interfaceMostRecentActivityTimestamp_ms;

			return (idleTime >= _interfaceIdleTimeLimit);
		}

		private synchronized boolean heartbeatResponsePastDue() {		
			long heartbeatElapsedTime = System.currentTimeMillis() - _mostRecentHeartbeatSentTimestamp_ms;

			return (heartbeatElapsedTime >= _heartbeatResponsePastDueTimeLimit);
		}
		
		private void cancelHeartbeatRequest() {
			_heartbeatRequestInProgress = false;
		}

		private void issueHeartbeatRequest() {		
			SetGlobalProperties req = new SetGlobalProperties();
			req.setCorrelationID(HEARTBEAT_CORRELATION_ID);

			try {
				sendRPCRequestPrivate(req);
				_heartbeatRequestInProgress = true;
			} catch (Exception ex) {
				// If sending the heartbeat request fails, cancel the heartbeat monitoring
				DebugTool.logError("Failure issuing heartbeat request: " + ex.toString(), ex);
				halt();
				_heartBeatEnabled = false;
			} // end-catch
		} // end-method
		
		public void receivedHeartBeatResponse() {
			_heartbeatRequestInProgress = false;
		}
		
		private void heartbeatProcessing() throws InterruptedException {		
			// This function polls to manage an RPC-level heartbeat to more quickly detect
			// a lost connection to SYNC.

			Thread.sleep(POLLING_INTERVAL);

			if (_heartbeatRequestInProgress) {
				if (heartbeatResponsePastDue()) {
					DebugTool.logError("HEARTBEAT PAST DUE (i.e. not received within " +  _heartbeatResponsePastDueTimeLimit + "ms)");
					if (_advancedLifecycleManagementEnabled) {
						// Heartbeat past due and connection lost, interface no longer valid
						_appInterfaceRegisterd = false;
						synchronized(APP_INTERFACE_REGISTERED_LOCK) {
							APP_INTERFACE_REGISTERED_LOCK.notify();
						}
						
						// Cycle the proxy with HEARTBEAT_PAST_DUE as the disconnect reason
						cycleProxy(SyncDisconnectedReason.HEARTBEAT_PAST_DUE);
					} else {
						notifyProxyClosed("HeartbeatPastDue", new SyncException("Heartbeat past due.", SyncExceptionCause.HEARTBEAT_PAST_DUE));
					}
					return;
				} // end-if
			} else {
				// There's not a heartbeat request in-flight at the moment ...
				//
				// If heartbeat is deactivated (for whatever reason), then just
				// iterate loop.
				if (!_heartbeatActive) {
					return; // Do not need to request a heartbeat
				} // end-if

				//if (heartbeatRequired()) && m_pendingRequestCount == 0) {
				if (heartbeatRequired()) {
					issueHeartbeatRequest();
				} // end-if
			} // end-if
		} // end-method
		
		private void halt() {		
			_isHalted = true;
			
			if (_heartbeatThread != null) {
				try	{
					_heartbeatThread.interrupt();
				} catch (Exception ex) {
					DebugTool.logError("Failure interrupting heartbeat thread: " + ex.toString(), ex);
				}
				_heartbeatThread = null;
			} // end-if
		}
	}

	/**
	 * Constructor.
	 * 
	 * @param listener Type of listener for this proxy base.
	 * @param syncProxyConfigurationResources Configuration resources for this proxy.
	 * @param enableAdvancedLifecycleManagement Flag that ALM should be enabled or not.
	 * @param appName Client application name.
	 * @param ttsName TTS name.
	 * @param ngnMediaScreenAppName Media Screen Application name.
	 * @param vrSynonyms List of synonyms.
	 * @param isMediaApp Flag that indicates that client application if media application or not.
	 * @param syncMsgVersion Version of Sync Message.
	 * @param languageDesired Desired language.
	 * @param hmiDisplayLanguageDesired Desired language for HMI. 
	 * @param appType Type of application.
	 * @param appID Application identifier.
	 * @param autoActivateID Auto activation identifier.
	 * @param callbackToUIThread Flag that indicates that this proxy should send callback to UI thread or not.
	 * @param transportConfig Configuration of transport to be used by underlying connection.
	 * @throws SyncException
	 */
	protected SyncProxyBase(proxyListenerType listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			boolean enableAdvancedLifecycleManagement, String appName, Vector<TTSChunk> ttsName, 
			String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion, 
			Language languageDesired, Language hmiDisplayLanguageDesired, Vector<AppType> appType, String appID, 
			String autoActivateID, boolean callbackToUIThread, BaseTransportConfig transportConfig) 
			throws SyncException {
		
		_interfaceBroker = new SyncInterfaceBroker();
		
		_callbackToUIThread = callbackToUIThread;
		
		if (_callbackToUIThread) {
			_mainUIHandler = new Handler(Looper.getMainLooper());
		}
		
		// Set variables for Advanced Lifecycle Management
		_advancedLifecycleManagementEnabled = enableAdvancedLifecycleManagement;
		_applicationName = appName;
		_ttsName = ttsName;
		_ngnMediaScreenAppName = ngnMediaScreenAppName;
		_isMediaApp = isMediaApp;
		_syncMsgVersionRequest = syncMsgVersion;
		_vrSynonyms = vrSynonyms; 
		_syncLanguageDesired = languageDesired;
		_hmiDisplayLanguageDesired = hmiDisplayLanguageDesired;
		_appType = appType;
		_appID = appID;
		_autoActivateIdDesired = autoActivateID;
		_transportConfig = transportConfig;
		
		// Test conditions to invalidate the proxy
		if (listener == null) {
			throw new IllegalArgumentException("IProxyListener listener must be provided to instantiate SyncProxy object.");
		}
		if (_advancedLifecycleManagementEnabled) {
			if (_applicationName == null ) {
				throw new IllegalArgumentException("To use SyncProxyALM, an application name, appName, must be provided");
			}
			if (_applicationName.length() < 1 || _applicationName.length() > 100) {
				throw new IllegalArgumentException("A provided application name, appName, must be between 1 and 100 characters in length.");
			}
			if (_isMediaApp == null) {
				throw new IllegalArgumentException("isMediaApp must not be null when using SyncProxyALM.");
			}
		}
		
		_proxyListener = listener;
		
		// Get information from syncProxyConfigurationResources
		TelephonyManager telephonyManager = null;
		if (syncProxyConfigurationResources != null) {
			telephonyManager = syncProxyConfigurationResources.getTelephonyManager();
		} 
		
		// Use the telephonyManager to get and log phone info
		if (telephonyManager != null) {
			// Following is not quite thread-safe (because m_traceLogger could test null twice),
			// so we need to fix this, but vulnerability (i.e. two instances of listener) is
			// likely harmless.
			if (_traceDeviceInterrogator == null) {
				_traceDeviceInterrogator = new TraceDeviceInfo(syncProxyConfigurationResources.getTelephonyManager());
			} // end-if
		} // end-if
		
		// Setup Internal ProxyMessage Dispatcher
		synchronized(INTERNAL_MESSAGE_QUEUE_THREAD_LOCK) {
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
							dispatchInternalMessage((InternalProxyMessage)message);
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
		
		// Setup Incoming ProxyMessage Dispatcher
		synchronized(INCOMING_MESSAGE_QUEUE_THREAD_LOCK) {
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
							dispatchIncomingMessage((ProtocolMessage)message);
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
		
		// Setup Outgoing ProxyMessage Dispatcher
		synchronized(OUTGOING_MESSAGE_QUEUE_THREAD_LOCK) {
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
							dispatchOutgoingMessage((ProtocolMessage)message);
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
		
		// Trace that ctor has fired
		SyncTrace.logProxyEvent("SyncProxy Created, instanceID=" + this.toString(), SYNC_LIB_TRACE_KEY);
	}
	
	/**
	 * Constructor.
	 * 
	 * @param listener Type of listener for this proxy base.
	 * @param syncProxyConfigurationResources Configuration resources for this proxy.
	 * @param enableAdvancedLifecycleManagement Flag that ALM should be enabled or not.
	 * @param appName Client application name.
	 * @param ttsName TTS name.
	 * @param ngnMediaScreenAppName Media Screen Application name.
	 * @param vrSynonyms List of synonyms.
	 * @param isMediaApp Flag that indicates that client application if media application or not.
	 * @param syncMsgVersion Version of Sync Message.
	 * @param languageDesired Desired language.
	 * @param hmiDisplayLanguageDesired Desired language for HMI. 
	 * @param appType Type of application.
	 * @param appID Application identifier.
	 * @param autoActivateID Auto activation identifier.
	 * @param callbackToUIThread Flag that indicates that this proxy should send callback to UI thread or not.
	 * @param preRegister Flag that indicates that this proxy should be pre-registerd or not.
	 * @param version Version of Sync protocol to be used by the underlying connection.
	 * @param transportConfig Configuration of transport to be used by underlying connection.
	 * @throws SyncException
	 */	
	protected SyncProxyBase(proxyListenerType listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			boolean enableAdvancedLifecycleManagement, String appName, Vector<TTSChunk> ttsName, 
			String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion, 
			Language languageDesired, Language hmiDisplayLanguageDesired, Vector<AppType> appType, String appID, 
			String autoActivateID, boolean callbackToUIThread, boolean preRegister, int version, 
			BaseTransportConfig transportConfig) 
			throws SyncException {
		
		setWiProVersion((byte)version);
		if (preRegister) _appInterfaceRegisterd = preRegister;
		
		_interfaceBroker = new SyncInterfaceBroker();
		
		_callbackToUIThread = callbackToUIThread;
		
		if (_callbackToUIThread) {
			_mainUIHandler = new Handler(Looper.getMainLooper());
		}
		
		// Set variables for Advanced Lifecycle Management
		_advancedLifecycleManagementEnabled = enableAdvancedLifecycleManagement;
		_applicationName = appName;
		_ttsName = ttsName;
		_ngnMediaScreenAppName = ngnMediaScreenAppName;
		_isMediaApp = isMediaApp;
		_syncMsgVersionRequest = syncMsgVersion;
		_vrSynonyms = vrSynonyms; 
		_syncLanguageDesired = languageDesired;
		_hmiDisplayLanguageDesired = hmiDisplayLanguageDesired;
		_appType = appType;
		_appID = appID;
		_autoActivateIdDesired = autoActivateID;
		_transportConfig = transportConfig;
		
		// Test conditions to invalidate the proxy
		if (listener == null) {
			throw new IllegalArgumentException("IProxyListener listener must be provided to instantiate SyncProxy object.");
		}
		if (_advancedLifecycleManagementEnabled) {
			if (_applicationName == null ) {
				throw new IllegalArgumentException("To use SyncProxyALM, an application name, appName, must be provided");
			}
			if (_applicationName.length() < 1 || _applicationName.length() > 100) {
				throw new IllegalArgumentException("A provided application name, appName, must be between 1 and 100 characters in length.");
			}
			if (_isMediaApp == null) {
				throw new IllegalArgumentException("isMediaApp must not be null when using SyncProxyALM.");
			}
		}
		
		_proxyListener = listener;
		
		// Get information from syncProxyConfigurationResources
		TelephonyManager telephonyManager = null;
		if (syncProxyConfigurationResources != null) {
			telephonyManager = syncProxyConfigurationResources.getTelephonyManager();
		} 
		
		// Use the telephonyManager to get and log phone info
		if (telephonyManager != null) {
			// Following is not quite thread-safe (because m_traceLogger could test null twice),
			// so we need to fix this, but vulnerability (i.e. two instances of listener) is
			// likely harmless.
			if (_traceDeviceInterrogator == null) {
				_traceDeviceInterrogator = new TraceDeviceInfo(syncProxyConfigurationResources.getTelephonyManager());
			} // end-if
		} // end-if
		
		// Setup Internal ProxyMessage Dispatcher
		synchronized(INTERNAL_MESSAGE_QUEUE_THREAD_LOCK) {
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
							dispatchInternalMessage((InternalProxyMessage)message);
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
		
		// Setup Incoming ProxyMessage Dispatcher
		synchronized(INCOMING_MESSAGE_QUEUE_THREAD_LOCK) {
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
							dispatchIncomingMessage((ProtocolMessage)message);
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
		
		// Setup Outgoing ProxyMessage Dispatcher
		synchronized(OUTGOING_MESSAGE_QUEUE_THREAD_LOCK) {
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
							dispatchOutgoingMessage((ProtocolMessage)message);
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
		
		// Trace that ctor has fired
		SyncTrace.logProxyEvent("SyncProxy Created, instanceID=" + this.toString(), SYNC_LIB_TRACE_KEY);
	}

	public void sendEncodedSyncPDataToUrl(String urlString, Vector<String> encodedSyncPData, Integer timeout) {
		try {
			final int CONNECTION_TIMEOUT = timeout*1000; // in ms
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
				Log.i("sendEncodedSyncPDataToUrl","Response from server returned null: ");
				return;
			}

			Vector<String> encodedSyncPDataReceived = new Vector<String>();			
			if (response.getStatusLine().getStatusCode() == 200) {
				
				// Convert the response to JSON
				JSONObject jsonResponse = new JSONObject(EntityUtils.toString(response.getEntity(), "UTF-8"));
				
				if (jsonResponse.get("data") instanceof JSONArray) {
					JSONArray jsonArray = jsonResponse.getJSONArray("data");
					for (int i=0; i<jsonArray.length(); i++) {
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
			final int CONNECTION_TIMEOUT = timeout*1000; // in ms
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
				Log.i("sendEncodedSyncPDataToUrl","Response from server returned null: ");
				return;
			}

			Vector<String> encodedSyncPDataReceived = new Vector<String>();			
			if (response.getStatusLine().getStatusCode() == 200) {
				// Convert the response to JSON
				JSONObject jsonResponse = new JSONObject(EntityUtils.toString(response.getEntity(), "UTF-8"));
				
				if (jsonResponse.get("data") instanceof JSONArray) {
					JSONArray jsonArray = jsonResponse.getJSONArray("data");
					for (int i=0; i<jsonArray.length(); i++) {
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
	private boolean isCorrelationIDProtected(Integer correlationID) {
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
		return _syncConnection.getIsConnected();
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
	
	
	// Function to initialize new proxy connection
	private void initializeProxy() throws SyncException {		
		// Reset all of the flags and state variables
		_haveReceivedFirstNonNoneHMILevel = false;
		_haveReceivedFirstFocusLevel = false;
		_haveReceivedFirstFocusLevelFull = false;
		_syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;
		
		//TODO: Set Heart Beat Active!
		//_proxyHeartBeat = new ProxyHeartBeat();
		//_heartBeatEnabled = true;
		
		// Setup SyncConnection
		synchronized(CONNECTION_REFERENCE_LOCK) {
			if (_syncConnection != null) {
				_syncConnection.closeConnection(_rpcSessionID);
				_syncConnection = null;
			}
			_syncConnection = new SyncConnection(_interfaceBroker, _transportConfig);
			WiProProtocol protocol = (WiProProtocol)_syncConnection.getWiProProtocol();
			protocol.setVersion(_wiproVersion);
		}
		
		synchronized(CONNECTION_REFERENCE_LOCK) {
			if (_syncConnection != null) {
				_syncConnection.startTransport();
			}
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

	@Deprecated
	public void close() throws SyncException {
		dispose();
	}
	
	private void cleanProxy(SyncDisconnectedReason disconnectedReason) throws SyncException {
		try {
			
			// ALM Specific Cleanup
			if (_advancedLifecycleManagementEnabled) {
				_syncConnectionState = SyncConnectionState.SYNC_DISCONNECTED;
				
				firstTimeFull = true;
			
				// Should we wait for the interface to be unregistered?
				Boolean waitForInterfaceUnregistered = false;
				// Unregister app interface
				synchronized(CONNECTION_REFERENCE_LOCK) {
					if (_appInterfaceRegisterd == true && _syncConnection != null && _syncConnection.getIsConnected()) {
						waitForInterfaceUnregistered = true;
						unregisterAppInterfacePrivate(UNREGISTER_APP_INTERFACE_CORRELATION_ID);
					}
				}
				
				// Wait for the app interface to be unregistered
				if (waitForInterfaceUnregistered) {
					synchronized(APP_INTERFACE_REGISTERED_LOCK) {
						try {
							APP_INTERFACE_REGISTERED_LOCK.wait(1000);
						} catch (InterruptedException e) {
							// Do nothing
						}
					}
				}
			}
			
			// Clean up SYNC Connection
			synchronized(CONNECTION_REFERENCE_LOCK) {
				if (_syncConnection != null) {
					_syncConnection.closeConnection(_rpcSessionID);
					_syncConnection = null;
				}
			}
		
			// Clean up Heartbeat Thread
			if (_proxyHeartBeat != null) {
				_proxyHeartBeat.halt();
			}		
		} catch (SyncException e) {
			throw e;
		} finally {
			SyncTrace.logProxyEvent("SyncProxy cleaned.", SYNC_LIB_TRACE_KEY);
		}
	}
	
	/**
	 * Terminates the App's Interface Registration, closes the transport connection, ends the protocol session, and frees any resources used by the proxy.
	 */
	public void dispose() throws SyncException
	{		
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		_proxyDisposed = true;
		
		SyncTrace.logProxyEvent("Application called dispose() method.", SYNC_LIB_TRACE_KEY);
		
		try{
			// Clean the proxy
			cleanProxy(SyncDisconnectedReason.APPLICATION_REQUESTED_DISCONNECT);
		
			// Close IncomingProxyMessageDispatcher thread
			synchronized(INCOMING_MESSAGE_QUEUE_THREAD_LOCK) {
				if (_incomingProxyMessageDispatcher != null) {
					_incomingProxyMessageDispatcher.dispose();
					_incomingProxyMessageDispatcher = null;
				}
			}
			
			// Close OutgoingProxyMessageDispatcher thread
			synchronized(OUTGOING_MESSAGE_QUEUE_THREAD_LOCK) {
				if (_outgoingProxyMessageDispatcher != null) {
					_outgoingProxyMessageDispatcher.dispose();
					_outgoingProxyMessageDispatcher = null;
				}
			}
			
			// Close InternalProxyMessageDispatcher thread
			synchronized(INTERNAL_MESSAGE_QUEUE_THREAD_LOCK) {
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
		try{
			cleanProxy(disconnectedReason);
			initializeProxy();	
			notifyProxyClosed("Sync Proxy Cycled", new SyncException("Sync Proxy Cycled", SyncExceptionCause.SYNC_PROXY_CYCLED));
		} catch (SyncException e) {
			switch(e.getSyncExceptionCause()) {
			case BLUETOOTH_DISABLED:
				notifyProxyClosed("Bluetooth is disabled. Bluetooth must be enabled to connect to SYNC. Reattempt a connection once Bluetooth is enabled.", 
						new SyncException("Bluetooth is disabled. Bluetooth must be enabled to connect to SYNC. Reattempt a connection once Bluetooth is enabled.", SyncExceptionCause.BLUETOOTH_DISABLED));
				break;
			case BLUETOOTH_ADAPTER_NULL:
				notifyProxyClosed("Cannot locate a Bluetooth adapater. A SYNC connection is impossible on this device until a Bluetooth adapter is added.", 
						new SyncException("Cannot locate a Bluetooth adapater. A SYNC connection is impossible on this device until a Bluetooth adapter is added.", SyncExceptionCause.HEARTBEAT_PAST_DUE));
				break;
			default :
				notifyProxyClosed("Cycling the proxy failed.", e);
				break;
			}
		} catch (Exception e) { 
			notifyProxyClosed("Cycling the proxy failed.", e);
		}
	}

	
	
	/************* Functions used by the Message Dispatching Queues ****************/
	private void dispatchIncomingMessage(ProtocolMessage message) {
		try{
			// Dispatching logic
			if (message.getSessionType().equals(SessionType.RPC)) {
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
						if (message.getRPCType() == 0x00) {
							hash.put(Names.request, hashTemp);
						} else if (message.getRPCType() == 0x01) {
							hash.put(Names.response, hashTemp);
						} else if (message.getRPCType() == 0x02) {
							hash.put(Names.notification, hashTemp);
						}
						if (message.getBulkData() != null) hash.put(Names.bulkData, message.getBulkData());
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
	
	public byte getWiProVersion() {
		return this._wiproVersion;
	}
	
	private void setWiProVersion(byte version) {
        Log.i(TAG, "Setting WiPro version from " + (int)this._wiproVersion + " to " + (int)version);
        Log.i(TAG, "setter called from: " + Log.getStackTraceString(new Exception()));
		this._wiproVersion = version;
	}

	private void handleErrorsFromIncomingMessageDispatcher(String info, Exception e) {
		passErrorToProxyListener(info, e);
	}
	
	private void dispatchOutgoingMessage(ProtocolMessage message) {
		synchronized(CONNECTION_REFERENCE_LOCK) {
			if (_syncConnection != null) {
				_syncConnection.sendMessage(message);
			}
		}		
		SyncTrace.logProxyEvent("SyncProxy sending Protocol Message: " + message.toString(), SYNC_LIB_TRACE_KEY);
	}
	
	private void handleErrorsFromOutgoingMessageDispatcher(String info, Exception e) {
		passErrorToProxyListener(info, e);
	}
	
	void dispatchInternalMessage(final InternalProxyMessage message) {
		try{
			if (message.getFunctionName().equals(Names.OnProxyError)) {
				final OnError msg = (OnError)message;			
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
							((IProxyListener)_proxyListener).onProxyOpened();
						}
					});
				} else {
					((IProxyListener)_proxyListener).onProxyOpened();
				}
			} else if (message.getFunctionName().equals(Names.OnProxyClosed)) {
				final OnProxyClosed msg = (OnProxyClosed)message;
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
		} catch(final Exception e) {
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
		_proxyListener.onError("Proxy callback dispatcher is down. Proxy instance is invalid.", e);
	}
	/************* END Functions used by the Message Dispatching Queues ****************/
	
	// Private sendPRCRequest method. All RPCRequests are funneled through this method after
		// error checking. 
	private void sendRPCRequestPrivate(RPCRequest request) throws SyncException {
		try {
			SyncTrace.logRPCEvent(InterfaceActivityDirection.Transmit, request, SYNC_LIB_TRACE_KEY);
	
			byte[] msgBytes = _jsonRPCMarshaller.marshall(request, _wiproVersion);
            Log.d(TAG, "Version: " + _wiproVersion + " | msg: " + new String(msgBytes));

			ProtocolMessage pm = new ProtocolMessage();
			pm.setData(msgBytes);
			pm.setSessionID(_rpcSessionID);
			pm.setMessageType(MessageType.RPC);
			pm.setSessionType(SessionType.RPC);
			pm.setFunctionID(FunctionID.getFunctionID(request.getFunctionName()));
			pm.setCorrID(request.getCorrelationID());
			if (request.getBulkData() != null) 
				pm.setBulkData(request.getBulkData());
			
			// Queue this outgoing message
			synchronized(OUTGOING_MESSAGE_QUEUE_THREAD_LOCK) {
				if (_outgoingProxyMessageDispatcher != null) {
						_outgoingProxyMessageDispatcher.queueMessage(pm);
				}
			}
		} catch (OutOfMemoryError e) {
			SyncTrace.logProxyEvent("OutOfMemory exception while sending request " + request.getFunctionName(), SYNC_LIB_TRACE_KEY);
			throw new SyncException("OutOfMemory exception while sending request " + request.getFunctionName(), e, SyncExceptionCause.INVALID_ARGUMENT);
		}

		// Record most recent heart beat activity 
		if (_proxyHeartBeat != null && _heartBeatEnabled) {
			if (request.getCorrelationID() != null && request.getCorrelationID() == HEARTBEAT_CORRELATION_ID) {
				_proxyHeartBeat.recordMostRecentHeartbeatSentTimestamp();
			}

			_proxyHeartBeat.changePendingRequestCount(true);
		} // end-if
	}
	
	private void handleRPCMessage(Hashtable hash) {
		RPCMessage rpcMsg = new RPCMessage(hash);
		String functionName = rpcMsg.getFunctionName();
		String messageType = rpcMsg.getMessageType();
		
		if (messageType.equals(Names.response)) {			
			SyncTrace.logRPCEvent(InterfaceActivityDirection.Receive, new RPCResponse(rpcMsg), SYNC_LIB_TRACE_KEY);

			if (_proxyHeartBeat != null && _heartBeatEnabled) {
				_proxyHeartBeat.changePendingRequestCount(false);
			} // end-if

			// Check to ensure response is not from an internal message (reserved correlation ID)
			if (isCorrelationIDProtected((new RPCResponse(hash)).getCorrelationID())) {
				// This is a response generated from an internal message, it can be trapped here
				// The app should not receive a response for a request it did not send
				if ((new RPCResponse(hash)).getCorrelationID() == REGISTER_APP_INTERFACE_CORRELATION_ID 
						&& _advancedLifecycleManagementEnabled 
						&& functionName.equals(Names.RegisterAppInterface)) {
					final RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(hash);
					if (msg.getSuccess()) {
						_appInterfaceRegisterd = true;
					}
					
					//_autoActivateIdReturned = msg.getAutoActivateID();
					/*Place holder for legacy support*/ _autoActivateIdReturned = "8675309";
					_buttonCapabilities = msg.getButtonCapabilities();
					_displayCapabilities = msg.getDisplayCapabilities();
					_softButtonCapabilities = msg.getSoftButtonCapabilities();
					_presetBankCapabilities = msg.getPresetBankCapabilities();
					_hmiZoneCapabilities = msg.getHmiZoneCapabilities();
					_speechCapabilities = msg.getSpeechCapabilities();
					_syncLanguage = msg.getLanguage();
					_hmiDisplayLanguage = msg.getHmiDisplayLanguage();
					_syncMsgVersion = msg.getSyncMsgVersion();
					_vrCapabilities = msg.getVrCapabilities();
					_vehicleType = msg.getVehicleType();
					
					// Send onSyncConnected message in ALM
					_syncConnectionState = SyncConnectionState.SYNC_CONNECTED;
					
					// If registerAppInterface failed, exit with OnProxyUnusable
					if (!msg.getSuccess()) {
						notifyProxyClosed("Unable to register app interface. Review values passed to the SyncProxy constructor. RegisterAppInterface result code: ", 
								new SyncException("Unable to register app interface. Review values passed to the SyncProxy constructor. RegisterAppInterface result code: " + msg.getResultCode(), SyncExceptionCause.SYNC_REGISTRATION_ERROR));
					}
					
					if (_callbackToUIThread) {
						// Run in UI thread
						_mainUIHandler.post(new Runnable() {
							@Override
							public void run() {
								if (_proxyListener instanceof IProxyListener) {
									((IProxyListener)_proxyListener).onRegisterAppInterfaceResponse(msg);
								} else if (_proxyListener instanceof IProxyListenerALMTesting) {
									((IProxyListenerALMTesting)_proxyListener).onRegisterAppInterfaceResponse(msg);
								}
							}
						});
					} else {
						if (_proxyListener instanceof IProxyListener) {
							((IProxyListener)_proxyListener).onRegisterAppInterfaceResponse(msg);
						} else if (_proxyListener instanceof IProxyListenerALMTesting) {
							((IProxyListenerALMTesting)_proxyListener).onRegisterAppInterfaceResponse(msg);
						}
					}
				} else if ((new RPCResponse(hash)).getCorrelationID() == POLICIES_CORRELATION_ID 
						&& functionName.equals(Names.OnEncodedSyncPData)) {
					// OnEncodedSyncPData
					
					final OnEncodedSyncPData msg = new OnEncodedSyncPData(hash);
					
					// If url is null, then send notification to the app, otherwise, send to URL
					if (msg.getUrl() != null) {
						// URL has data, attempt to post request to external server
						Thread handleOffboardSyncTransmissionTread = new Thread() {
							@Override
							public void run() {
								sendEncodedSyncPDataToUrl(msg.getUrl(), msg.getData(), msg.getTimeout());
							}
						};
						
						handleOffboardSyncTransmissionTread.start();
					}
				}
				return;
			}
			
			if (functionName.equals(Names.RegisterAppInterface)) {
				final RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(hash);
				if (msg.getSuccess()) {
					_appInterfaceRegisterd = true;
				}

				//_autoActivateIdReturned = msg.getAutoActivateID();
				/*Place holder for legacy support*/ _autoActivateIdReturned = "8675309";
				_buttonCapabilities = msg.getButtonCapabilities();
				_displayCapabilities = msg.getDisplayCapabilities();
				_softButtonCapabilities = msg.getSoftButtonCapabilities();
				_presetBankCapabilities = msg.getPresetBankCapabilities();
				_hmiZoneCapabilities = msg.getHmiZoneCapabilities();
				_speechCapabilities = msg.getSpeechCapabilities();
				_syncLanguage = msg.getLanguage();
				_hmiDisplayLanguage = msg.getHmiDisplayLanguage();
				_syncMsgVersion = msg.getSyncMsgVersion();
				_vrCapabilities = msg.getVrCapabilities();
				_vehicleType = msg.getVehicleType();
				
				// RegisterAppInterface
				if (_advancedLifecycleManagementEnabled) {
					
					// Send onSyncConnected message in ALM
					_syncConnectionState = SyncConnectionState.SYNC_CONNECTED;
					
					// If registerAppInterface failed, exit with OnProxyUnusable
					if (!msg.getSuccess()) {
						notifyProxyClosed("Unable to register app interface. Review values passed to the SyncProxy constructor. RegisterAppInterface result code: ", 
								new SyncException("Unable to register app interface. Review values passed to the SyncProxy constructor. RegisterAppInterface result code: " + msg.getResultCode(), SyncExceptionCause.SYNC_REGISTRATION_ERROR));
					}
				}
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							if (_proxyListener instanceof IProxyListener) {
								((IProxyListener)_proxyListener).onRegisterAppInterfaceResponse(msg);
							} else if (_proxyListener instanceof IProxyListenerALMTesting) {
								((IProxyListenerALMTesting)_proxyListener).onRegisterAppInterfaceResponse(msg);
							}
						}
					});
				} else {
					if (_proxyListener instanceof IProxyListener) {
						((IProxyListener)_proxyListener).onRegisterAppInterfaceResponse(msg);
					} else if (_proxyListener instanceof IProxyListenerALMTesting) {
						((IProxyListenerALMTesting)_proxyListener).onRegisterAppInterfaceResponse(msg);
					}
				}
			} else if (functionName.equals(Names.Speak)) {
				// SpeakResponse
				
				final SpeakResponse msg = new SpeakResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onSpeakResponse(msg);
						}
					});
				} else {
					_proxyListener.onSpeakResponse(msg);						
				}
			} else if (functionName.equals(Names.Alert)) {
				// AlertResponse
				
				final AlertResponse msg = new AlertResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onAlertResponse(msg);
						}
					});
				} else {
					_proxyListener.onAlertResponse(msg);						
				}
			} else if (functionName.equals(Names.Show)) {
				// ShowResponse
				
				final ShowResponse msg = new ShowResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onShowResponse((ShowResponse)msg);
						}
					});
				} else {
					_proxyListener.onShowResponse((ShowResponse)msg);						
				}
			} else if (functionName.equals(Names.AddCommand)) {
				// AddCommand
				
				final AddCommandResponse msg = new AddCommandResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onAddCommandResponse((AddCommandResponse)msg);
						}
					});
				} else {
					_proxyListener.onAddCommandResponse((AddCommandResponse)msg);					
				}
			} else if (functionName.equals(Names.DeleteCommand)) {
				// DeleteCommandResponse
				
				final DeleteCommandResponse msg = new DeleteCommandResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onDeleteCommandResponse((DeleteCommandResponse)msg);
						}
					});
				} else {
					_proxyListener.onDeleteCommandResponse((DeleteCommandResponse)msg);					
				}
			} else if (functionName.equals(Names.AddSubMenu)) {
				// AddSubMenu
				
				final AddSubMenuResponse msg = new AddSubMenuResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onAddSubMenuResponse((AddSubMenuResponse)msg);
						}
					});
				} else {
					_proxyListener.onAddSubMenuResponse((AddSubMenuResponse)msg);					
				}
			} else if (functionName.equals(Names.DeleteSubMenu)) {
				// DeleteSubMenu
				
				final DeleteSubMenuResponse msg = new DeleteSubMenuResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onDeleteSubMenuResponse((DeleteSubMenuResponse)msg);
						}
					});
				} else {
					_proxyListener.onDeleteSubMenuResponse((DeleteSubMenuResponse)msg);					
				}
			} else if (functionName.equals(Names.SubscribeButton)) {
				// SubscribeButton
				
				final SubscribeButtonResponse msg = new SubscribeButtonResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onSubscribeButtonResponse((SubscribeButtonResponse)msg);
						}
					});
				} else {
					_proxyListener.onSubscribeButtonResponse((SubscribeButtonResponse)msg);				
				}
			} else if (functionName.equals(Names.UnsubscribeButton)) {
				// UnsubscribeButton
				
				final UnsubscribeButtonResponse msg = new UnsubscribeButtonResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onUnsubscribeButtonResponse((UnsubscribeButtonResponse)msg);
						}
					});
				} else {
					_proxyListener.onUnsubscribeButtonResponse((UnsubscribeButtonResponse)msg);			
				}
			} else if (functionName.equals(Names.SetMediaClockTimer)) {
				// SetMediaClockTimer
				
				final SetMediaClockTimerResponse msg = new SetMediaClockTimerResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onSetMediaClockTimerResponse((SetMediaClockTimerResponse)msg);
						}
					});
				} else {
					_proxyListener.onSetMediaClockTimerResponse((SetMediaClockTimerResponse)msg);		
				}
			} else if (functionName.equals(Names.EncodedSyncPData)) {
				// EncodedSyncPData
				
				final EncodedSyncPDataResponse msg = new EncodedSyncPDataResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onEncodedSyncPDataResponse(msg); 
						}
					});
				} else {
					_proxyListener.onEncodedSyncPDataResponse(msg); 		
				}
			} else if (functionName.equals(Names.SyncPData)) {
				// SyncPData
				
				final SyncPDataResponse msg = new SyncPDataResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onSyncPDataResponse(msg); 
						}
					});
				} else {
					_proxyListener.onSyncPDataResponse(msg); 		
				}
			} else if (functionName.equals(Names.CreateInteractionChoiceSet)) {
				// CreateInteractionChoiceSet
				
				final CreateInteractionChoiceSetResponse msg = new CreateInteractionChoiceSetResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onCreateInteractionChoiceSetResponse((CreateInteractionChoiceSetResponse)msg);
						}
					});
				} else {
					_proxyListener.onCreateInteractionChoiceSetResponse((CreateInteractionChoiceSetResponse)msg);		
				}
			} else if (functionName.equals(Names.DeleteInteractionChoiceSet)) {
				// DeleteInteractionChoiceSet
				
				final DeleteInteractionChoiceSetResponse msg = new DeleteInteractionChoiceSetResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onDeleteInteractionChoiceSetResponse((DeleteInteractionChoiceSetResponse)msg);
						}
					});
				} else {
					_proxyListener.onDeleteInteractionChoiceSetResponse((DeleteInteractionChoiceSetResponse)msg);		
				}
			} else if (functionName.equals(Names.PerformInteraction)) {
				// PerformInteraction
				
				final PerformInteractionResponse msg = new PerformInteractionResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onPerformInteractionResponse((PerformInteractionResponse)msg);
						}
					});
				} else {
					_proxyListener.onPerformInteractionResponse((PerformInteractionResponse)msg);		
				}
			} else if (functionName.equals(Names.SetGlobalProperties)) {
				// SetGlobalPropertiesResponse (can also be Heartbeat)
				
				final SetGlobalPropertiesResponse msg = new SetGlobalPropertiesResponse(hash);
				
				if (msg.getCorrelationID() != null && HEARTBEAT_CORRELATION_ID == msg.getCorrelationID()) {
					if (_proxyHeartBeat != null) {
						_proxyHeartBeat.receivedHeartBeatResponse();
					}
				} else {
					if (_callbackToUIThread) {
						// Run in UI thread
						_mainUIHandler.post(new Runnable() {
							@Override
							public void run() {
								_proxyListener.onSetGlobalPropertiesResponse((SetGlobalPropertiesResponse)msg);
							}
						});
					} else {
						_proxyListener.onSetGlobalPropertiesResponse((SetGlobalPropertiesResponse)msg);		
					}
				} // end-if
			} else if (functionName.equals(Names.ResetGlobalProperties)) {
				// ResetGlobalProperties				
				
				final ResetGlobalPropertiesResponse msg = new ResetGlobalPropertiesResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onResetGlobalPropertiesResponse((ResetGlobalPropertiesResponse)msg);
						}
					});
				} else {
					_proxyListener.onResetGlobalPropertiesResponse((ResetGlobalPropertiesResponse)msg);		
				}
			} else if (functionName.equals(Names.UnregisterAppInterface)) {
				// UnregisterAppInterface
				
				_appInterfaceRegisterd = false;
				synchronized(APP_INTERFACE_REGISTERED_LOCK) {
					APP_INTERFACE_REGISTERED_LOCK.notify();
				}
				
				final UnregisterAppInterfaceResponse msg = new UnregisterAppInterfaceResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							if (_proxyListener instanceof IProxyListener) {
								((IProxyListener)_proxyListener).onUnregisterAppInterfaceResponse(msg);
							} else if (_proxyListener instanceof IProxyListenerALMTesting) {
								((IProxyListenerALMTesting)_proxyListener).onUnregisterAppInterfaceResponse(msg);
							}
						}
					});
				} else {
					if (_proxyListener instanceof IProxyListener) {
						((IProxyListener)_proxyListener).onUnregisterAppInterfaceResponse(msg);
					} else if (_proxyListener instanceof IProxyListenerALMTesting) {
						((IProxyListenerALMTesting)_proxyListener).onUnregisterAppInterfaceResponse(msg);
					}
				}
				
				notifyProxyClosed("UnregisterAppInterfaceResponse", null);
			} else if (functionName.equals(Names.GenericResponse)) {
				// GenericResponse (Usually and error)
				final GenericResponse msg = new GenericResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onGenericResponse((GenericResponse)msg);
						}
					});
				} else {
					_proxyListener.onGenericResponse((GenericResponse)msg);	
				}
			} else if (functionName.equals(Names.Slider)) {
                // Slider
                final SliderResponse msg = new SliderResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onSliderResponse((SliderResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onSliderResponse((SliderResponse)msg);   
                }
            } else if (functionName.equals(Names.PutFile)) {
                // PutFile
                final PutFileResponse msg = new PutFileResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onPutFileResponse((PutFileResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onPutFileResponse((PutFileResponse)msg);
                }
            } else if (functionName.equals(Names.DeleteFile)) {
                // DeleteFile
                final DeleteFileResponse msg = new DeleteFileResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onDeleteFileResponse((DeleteFileResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onDeleteFileResponse((DeleteFileResponse)msg);   
                }
            } else if (functionName.equals(Names.ListFiles)) {
                // ListFiles
                final ListFilesResponse msg = new ListFilesResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onListFilesResponse((ListFilesResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onListFilesResponse((ListFilesResponse)msg);     
                }
            } else if (functionName.equals(Names.SetAppIcon)) {
                // SetAppIcon
                final SetAppIconResponse msg = new SetAppIconResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onSetAppIconResponse((SetAppIconResponse)msg);
                        }
                    });
                } else {
                        _proxyListener.onSetAppIconResponse((SetAppIconResponse)msg);   
                }
            } else if (functionName.equals(Names.ScrollableMessage)) {
                // ScrollableMessage
                final ScrollableMessageResponse msg = new ScrollableMessageResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onScrollableMessageResponse((ScrollableMessageResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onScrollableMessageResponse((ScrollableMessageResponse)msg);     
                }
            } else if (functionName.equals(Names.ChangeRegistration)) {
                // ChangeLanguageRegistration
                final ChangeRegistrationResponse msg = new ChangeRegistrationResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onChangeRegistrationResponse((ChangeRegistrationResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onChangeRegistrationResponse((ChangeRegistrationResponse)msg);   
                }
            } else if (functionName.equals(Names.SetDisplayLayout)) {
                // SetDisplayLayout
                final SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onSetDisplayLayoutResponse((SetDisplayLayoutResponse)msg);
                        }
                    });
                } else {
                        _proxyListener.onSetDisplayLayoutResponse((SetDisplayLayoutResponse)msg);
                }
            } else if (functionName.equals(Names.PerformAudioPassThru)) {
                // PerformAudioPassThru
                final PerformAudioPassThruResponse msg = new PerformAudioPassThruResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onPerformAudioPassThruResponse((PerformAudioPassThruResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onPerformAudioPassThruResponse((PerformAudioPassThruResponse)msg);       
                }
            } else if (functionName.equals(Names.EndAudioPassThru)) {
                // EndAudioPassThru
                final EndAudioPassThruResponse msg = new EndAudioPassThruResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onEndAudioPassThruResponse((EndAudioPassThruResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onEndAudioPassThruResponse((EndAudioPassThruResponse)msg);
                }
            } else if (functionName.equals(Names.SubscribeVehicleData)) {
                // SubscribeVehicleData
                final SubscribeVehicleDataResponse msg = new SubscribeVehicleDataResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onSubscribeVehicleDataResponse((SubscribeVehicleDataResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onSubscribeVehicleDataResponse((SubscribeVehicleDataResponse)msg);       
                }
            } else if (functionName.equals(Names.UnsubscribeVehicleData)) {
                // UnsubscribeVehicleData
                final UnsubscribeVehicleDataResponse msg = new UnsubscribeVehicleDataResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onUnsubscribeVehicleDataResponse((UnsubscribeVehicleDataResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onUnsubscribeVehicleDataResponse((UnsubscribeVehicleDataResponse)msg);   
                }
            } else if (functionName.equals(Names.GetVehicleData)) {
                // GetVehicleData
                final GetVehicleDataResponse msg = new GetVehicleDataResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onGetVehicleDataResponse((GetVehicleDataResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onGetVehicleDataResponse((GetVehicleDataResponse)msg);   
                }
            } else if (functionName.equals(Names.ReadDID)) {
                // ReadDID
                final ReadDIDResponse msg = new ReadDIDResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onReadDIDResponse((ReadDIDResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onReadDIDResponse((ReadDIDResponse)msg); 
                }
            } else if (functionName.equals(Names.GetDTCs)) {
                // GetDTCs
                final GetDTCsResponse msg = new GetDTCsResponse(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onGetDTCsResponse((GetDTCsResponse)msg);
                        }
                    });
                } else {
                    _proxyListener.onGetDTCsResponse((GetDTCsResponse)msg); 
                }
            } else if (functionName.equals(Names.AlertManeuver)) {
				// AlertManeuver
				final AlertManeuverResponse msg = new AlertManeuverResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onAlertManeuverResponse((AlertManeuverResponse)msg);
						}
					});
				} else {
					_proxyListener.onAlertManeuverResponse((AlertManeuverResponse)msg);	
				}
			} else if (functionName.equals(Names.ShowConstantTBT)) {
				// ShowConstantTBT
				final ShowConstantTBTResponse msg = new ShowConstantTBTResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onShowConstantTBTResponse((ShowConstantTBTResponse)msg);
						}
					});
				} else {
					_proxyListener.onShowConstantTBTResponse((ShowConstantTBTResponse)msg);	
				}
			} else if (functionName.equals(Names.UpdateTurnList)) {
				// UpdateTurnList
				final UpdateTurnListResponse msg = new UpdateTurnListResponse(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onUpdateTurnListResponse((UpdateTurnListResponse)msg);
						}
					});
				} else {
					_proxyListener.onUpdateTurnListResponse((UpdateTurnListResponse)msg);	
				}
			} else {
				if (_syncMsgVersion != null) {
					DebugTool.logError("Unrecognized response Message: " + functionName.toString() + 
							"SYNC Message Version = " + _syncMsgVersion);
				} else {
					DebugTool.logError("Unrecognized response Message: " + functionName.toString());
				}
			} // end-if
		} else if (messageType.equals(Names.notification)) {
			SyncTrace.logRPCEvent(InterfaceActivityDirection.Receive, new RPCNotification(rpcMsg), SYNC_LIB_TRACE_KEY);
			if (functionName.equals(Names.OnHMIStatus)) {
				// OnHMIStatus
				
				final OnHMIStatus msg = new OnHMIStatus(hash);
				msg.setFirstRun(new Boolean(firstTimeFull));
				if (msg.getHmiLevel() == HMILevel.HMI_FULL) firstTimeFull = false;
				
				if (msg.getHmiLevel() != _priorHmiLevel && msg.getAudioStreamingState() != _priorAudioStreamingState) {
					if (_callbackToUIThread) {
						// Run in UI thread
						_mainUIHandler.post(new Runnable() {
							@Override
							public void run() {
								_proxyListener.onOnHMIStatus((OnHMIStatus)msg);
							}
						});
					} else {
						_proxyListener.onOnHMIStatus((OnHMIStatus)msg);
					}
				}
				
				// Take action dependent on first non-NONE HMI Message
				if (_haveReceivedFirstNonNoneHMILevel) {
					if (_proxyHeartBeat != null && _heartBeatEnabled) {
						if (msg.getHmiLevel() == HMILevel.HMI_NONE) {
							_proxyHeartBeat.setHeartBeatActive(false);
						} else {
							// This will usually be a redundant set of the boolean, but
							// should be harmless.
							_proxyHeartBeat.setHeartBeatActive(true);
						} // end-if
					} // end-if
				} else {
					if (msg.getHmiLevel() == HMILevel.HMI_NONE) {
						if (_proxyHeartBeat != null && _heartBeatEnabled) {
							// This particular case will, I think, ALWAYS be redundant
							// (and will only happen once per life of proxy instance), but
							// setting to false will ALWAYS be correct logic anyway.
							_proxyHeartBeat.setHeartBeatActive(false);
						} // end-if
					} else {
						// This is the first non-None HMI Level
						_haveReceivedFirstNonNoneHMILevel = true;
						
						if (_proxyHeartBeat != null && _heartBeatEnabled) {
							_proxyHeartBeat.startHeartBeat();
						} // end-if
					} // end-if
				}
			} else if (functionName.equals(Names.OnCommand)) {
				// OnCommand
				
				final OnCommand msg = new OnCommand(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onOnCommand((OnCommand)msg);
						}
					});
				} else {
					_proxyListener.onOnCommand((OnCommand)msg);
				}
			} else if (functionName.equals(Names.OnDriverDistraction)) {
				// OnDriverDistration
				
				final OnDriverDistraction msg = new OnDriverDistraction(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onOnDriverDistraction(msg);
						}
					});
				} else {
					_proxyListener.onOnDriverDistraction(msg);
				}
			} else if (functionName.equals(Names.OnEncodedSyncPData)) {
				// OnEncodedSyncPData
				
				final OnEncodedSyncPData msg = new OnEncodedSyncPData(hash);
				
				// If url is null, then send notification to the app, otherwise, send to URL
				if (msg.getUrl() == null) {		
					if (_callbackToUIThread) {
						// Run in UI thread
						_mainUIHandler.post(new Runnable() {
							@Override
							public void run() {
								_proxyListener.onOnEncodedSyncPData(msg);
							}
						});
					} else {
						_proxyListener.onOnEncodedSyncPData(msg);
					}
				} else {
					// URL has data, attempt to post request to external server
					Thread handleOffboardSyncTransmissionTread = new Thread() {
						@Override
						public void run() {
							sendEncodedSyncPDataToUrl(msg.getUrl(), msg.getData(), msg.getTimeout());
						}
					};

					handleOffboardSyncTransmissionTread.start();
				}
			} else if (functionName.equals(Names.OnSyncPData)) {
				// OnSyncPData
				Log.i("pt", "functionName.equals(Names.OnEncodedSyncPData)");
				final OnSyncPData msg = new OnSyncPData(hash);

				// If url is null, then send notification to the app, otherwise, send to URL
				if (msg.getUrl() == null) {		
					Log.i("pt", "send syncp to app");
					if (_callbackToUIThread) {
						// Run in UI thread
						_mainUIHandler.post(new Runnable() {
							@Override
							public void run() {
								_proxyListener.onOnSyncPData(msg);
							}
						});
					} else {
						_proxyListener.onOnSyncPData(msg);
					}
				} else { //url not null, send to url
					Log.i("pt", "send syncp to url");
					// URL has data, attempt to post request to external server
					Thread handleOffboardSyncTransmissionTread = new Thread() {
						@Override
						public void run() {
							sendSyncPDataToUrl(msg.getUrl(), msg.getSyncPData(), msg.getTimeout());
						}
					};
					
					handleOffboardSyncTransmissionTread.start();
				}
			} else if (functionName.equals(Names.OnPermissionsChange)) {
				//OnPermissionsChange
				
				final OnPermissionsChange msg = new OnPermissionsChange(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onOnPermissionsChange(msg);
						}
					});
				} else {
					_proxyListener.onOnPermissionsChange(msg);
				}
			} else if (functionName.equals(Names.OnTBTClientState)) {
				// OnTBTClientState
				
				final OnTBTClientState msg = new OnTBTClientState(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onOnTBTClientState(msg);
						}
					});
				} else {
					_proxyListener.onOnTBTClientState(msg);
				}
			} else if (functionName.equals(Names.OnButtonPress)) {
				// OnButtonPress
				
				final OnButtonPress msg = new OnButtonPress(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onOnButtonPress((OnButtonPress)msg);
						}
					});
				} else {
					_proxyListener.onOnButtonPress((OnButtonPress)msg);
				}
			} else if (functionName.equals(Names.OnButtonEvent)) {
				// OnButtonEvent
				
				final OnButtonEvent msg = new OnButtonEvent(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onOnButtonEvent((OnButtonEvent)msg);
						}
					});
				} else {
					_proxyListener.onOnButtonEvent((OnButtonEvent)msg);
				}
			} else if (functionName.equals(Names.OnLanguageChange)) {
				// OnLanguageChange
				
				final OnLanguageChange msg = new OnLanguageChange(hash);
				if (_callbackToUIThread) {
					// Run in UI thread
					_mainUIHandler.post(new Runnable() {
						@Override
						public void run() {
							_proxyListener.onOnLanguageChange((OnLanguageChange)msg);
						}
					});
				} else {
					_proxyListener.onOnLanguageChange((OnLanguageChange)msg);
				}
			} else if (functionName.equals(Names.OnAudioPassThru)) {
                // OnAudioPassThru
                final OnAudioPassThru msg = new OnAudioPassThru(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onOnAudioPassThru((OnAudioPassThru)msg);
                        }
                    });
                } else {
                    _proxyListener.onOnAudioPassThru((OnAudioPassThru)msg);
                }
			} else if (functionName.equals(Names.OnVehicleData)) {
                // OnVehicleData
                final OnVehicleData msg = new OnVehicleData(hash);
                if (_callbackToUIThread) {
                    // Run in UI thread
                    _mainUIHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            _proxyListener.onOnVehicleData((OnVehicleData)msg);
                        }
                    });
                } else {
                    _proxyListener.onOnVehicleData((OnVehicleData)msg);
                }
			} else if (functionName.equals(Names.OnAppInterfaceUnregistered)) {
				// OnAppInterfaceUnregistered
				
				_appInterfaceRegisterd = false;
				synchronized(APP_INTERFACE_REGISTERED_LOCK) {
					APP_INTERFACE_REGISTERED_LOCK.notify();
				}
				
				final OnAppInterfaceUnregistered msg = new OnAppInterfaceUnregistered(hash);
								
				if (_advancedLifecycleManagementEnabled) {
					// This requires the proxy to be cycled
                    if (this.getCurrentTransportType() == TransportType.BLUETOOTH) {
                        cycleProxy(SyncDisconnectedReason.convertAppInterfaceUnregisteredReason(msg.getReason()));
                    } else {
                        Log.e(this.getClass().getName(), "HandleRPCMessage. No cycle required if transport is TCP");
                    }
                } else {
					if (_callbackToUIThread) {
						// Run in UI thread
						_mainUIHandler.post(new Runnable() {
							@Override
							public void run() {
								((IProxyListener)_proxyListener).onOnAppInterfaceUnregistered(msg);
							}
						});
					} else {
						((IProxyListener)_proxyListener).onOnAppInterfaceUnregistered(msg);
					}
					
					notifyProxyClosed("OnAppInterfaceUnregistered", null);
				}
			} else {
				if (_syncMsgVersion != null) {
					DebugTool.logInfo("Unrecognized notification Message: " + functionName.toString() + 
							" connected to SYNC using message version: " + _syncMsgVersion.getMajorVersion() + "." + _syncMsgVersion.getMinorVersion());
				} else {
					DebugTool.logInfo("Unrecognized notification Message: " + functionName.toString());
				}
			} // end-if
		} // end-if notification
		
		SyncTrace.logProxyEvent("Proxy received RPC Message: " + functionName, SYNC_LIB_TRACE_KEY);
	}
	
	/**
	 * Takes an RPCRequest and sends it to SYNC.  Responses are captured through callback on IProxyListener.  
	 * 
	 * @param request
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
			
		// Test if SyncConnection is null
		synchronized(CONNECTION_REFERENCE_LOCK) {
			if (_syncConnection == null || !_syncConnection.getIsConnected()) {
				SyncTrace.logProxyEvent("Application attempted to send and RPCRequest without a connected transport.", SYNC_LIB_TRACE_KEY);
				throw new SyncException("There is no valid connection to SYNC. sendRPCRequest cannot be called until SYNC has been connected.", SyncExceptionCause.SYNC_UNAVAILALBE);
			}
		}
		
		// Test for illegal correlation ID
		if (isCorrelationIDProtected(request.getCorrelationID())) {
			
			SyncTrace.logProxyEvent("Application attempted to use the reserved correlation ID, " + request.getCorrelationID(), SYNC_LIB_TRACE_KEY);
			throw new SyncException("Invalid correlation ID. The correlation ID, " + request.getCorrelationID()
					+ " , is a reserved correlation ID.", SyncExceptionCause.RESERVED_CORRELATION_ID);
		}
		
		// Throw exception if RPCRequest is sent when SYNC is unavailable 
		if (!_appInterfaceRegisterd && request.getFunctionName() != Names.RegisterAppInterface) {
			if (!allowExtraTesting()) {
				SyncTrace.logProxyEvent("Application attempted to send an RPCRequest (non-registerAppInterface), before the interface was registerd.", SYNC_LIB_TRACE_KEY);
				throw new SyncException("SYNC is currently unavailable. RPC Requests cannot be sent.", SyncExceptionCause.SYNC_UNAVAILALBE);
			}
		}
				
		if (_advancedLifecycleManagementEnabled) {
			if (		   request.getFunctionName() == Names.RegisterAppInterface
					|| request.getFunctionName() == Names.UnregisterAppInterface) {
				if (!allowExtraTesting()) {
					SyncTrace.logProxyEvent("Application attempted to send a RegisterAppInterface or UnregisterAppInterface while using ALM.", SYNC_LIB_TRACE_KEY);
					throw new SyncException("The RPCRequest, " + request.getFunctionName() + 
							", is unnallowed using the Advanced Lifecycle Management Model.", SyncExceptionCause.INCORRECT_LIFECYCLE_MODEL);
				}
			}
		}
		
		sendRPCRequestPrivate(request);
	} // end-method
	
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
	
	/*public void sendRPCRequest(RPCMessage request) throws SyncException {
		sendRPCRequest(request);
	}*/
	
	protected void notifyProxyClosed(final String info, final Exception e) {		
		SyncTrace.logProxyEvent("NotifyProxyClose", SYNC_LIB_TRACE_KEY);
		
		OnProxyClosed message = new OnProxyClosed(info, e);
		queueInternalMessage(message);
	}

	private void passErrorToProxyListener(final String info, final Exception e) {
				
		OnError message = new OnError(info, e);
		queueInternalMessage(message);
	}
	
	private void startRPCProtocolSession(byte sessionID, String correlationID) {
		_rpcSessionID = sessionID;
		
		// Set Proxy Lifecyclek Available
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
						_hmiDisplayLanguageDesired,
						_appType,
						_appID,
						_autoActivateIdDesired,
						REGISTER_APP_INTERFACE_CORRELATION_ID);
				
			} catch (Exception e) {
				notifyProxyClosed("Failed to register application interface with SYNC. Check parameter values given to SyncProxy constructor.", e);
			}
		} else {
			InternalProxyMessage message = new InternalProxyMessage(Names.OnProxyOpened);
			queueInternalMessage(message);
		}
	}
	
	// Queue internal callback message
	private void queueInternalMessage(InternalProxyMessage message) {
		synchronized(INTERNAL_MESSAGE_QUEUE_THREAD_LOCK) {
			if (_internalProxyMessageDispatcher != null) {
				_internalProxyMessageDispatcher.queueMessage(message);
			}
		}
	}
	
	// Queue incoming ProtocolMessage
	private void queueIncomingMessage(ProtocolMessage message) {
		synchronized(INCOMING_MESSAGE_QUEUE_THREAD_LOCK) {
			if (_incomingProxyMessageDispatcher != null) {
				_incomingProxyMessageDispatcher.queueMessage(message);
			}
		}
	}

	/******************** Public Helper Methods *************************/
	
	/**
	 *Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 *
	 *@param commandID
	 *@param menuText
	 *@param parentID
	 *@param position
	 *@param vrCommands
	 *@param correlationID
	 *@throws SyncException
	 */
	public void addCommand(Integer commandID,
			String menuText, Integer parentID, Integer position,
			Vector<String> vrCommands, Integer correlationID) 
			throws SyncException {
		
		AddCommand msg = RPCRequestFactory.buildAddCommand(commandID, menuText, parentID, position,
			vrCommands, correlationID);
		
		sendRPCRequest(msg);
	}
	
	/**
	 *Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 *
	 *@param commandID
	 *@param menuText
	 *@param position
	 *@param vrCommands
	 *@param correlationID
	 *@throws SyncException
	 */
	public void addCommand(Integer commandID,
			String menuText, Integer position,
			Vector<String> vrCommands, Integer correlationID) 
			throws SyncException {
		
		addCommand(commandID, menuText, null, position, vrCommands, correlationID);
	}
	
	/**
	 *Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 *
	 *@param commandID
	 *@param menuText
	 *@param position
	 *@param correlationID
	 *@throws SyncException
	 */
	public void addCommand(Integer commandID,
			String menuText, Integer position,
			Integer correlationID) 
			throws SyncException {
		
		addCommand(commandID, menuText, null, position, null, correlationID);
	}
	
	/**
	 *Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 *
	 *@param commandID
	 *@param menuText
	 *@param correlationID
	 *@throws SyncException
	 */
	public void addCommand(Integer commandID,
			String menuText, Integer correlationID) 
			throws SyncException {
		
		addCommand(commandID, menuText, null, null, null, correlationID);
	}
	
	/**
	 * Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param commandID
	 * @param menuText
	 * @param vrCommands
	 * @param correlationID
	 * @throws SyncException
	 */
	public void addCommand(Integer commandID,
			String menuText, Vector<String> vrCommands, Integer correlationID) 
			throws SyncException {
		
		addCommand(commandID, menuText, null, null, vrCommands, correlationID);
	}
	
	/**
	 * Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param commandID
	 * @param vrCommands
	 * @param correlationID
	 * @throws SyncException
	 */
	public void addCommand(Integer commandID,
			Vector<String> vrCommands, Integer correlationID) 
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
	public void addSubMenu(Integer menuID, String menuName,
			Integer position, Integer correlationID) 
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
	public void addSubMenu(Integer menuID, String menuName,
			Integer correlationID) throws SyncException {
		
		addSubMenu(menuID, menuName, null, correlationID);
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
		
		alert((Vector<TTSChunk>)null, alertText1, alertText2, playTone, duration, correlationID);
	}
	
	/**
	 * Sends a CreateInteractionChoiceSet RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param choiceSet
	 * @param interactionChoiceSetID
	 * @param correlationID
	 * @throws SyncException
	 */
	public void createInteractionChoiceSet(
			Vector<Choice> choiceSet, Integer interactionChoiceSetID,
			Integer correlationID) throws SyncException {
		
		CreateInteractionChoiceSet msg = RPCRequestFactory.buildCreateInteractionChoiceSet(
				choiceSet, interactionChoiceSetID, correlationID);

		sendRPCRequest(msg);
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
	// reqisterAppInterface
	protected void registerAppInterfacePrivate(
			SyncMsgVersion syncMsgVersion, String appName, Vector<TTSChunk> ttsName,
			String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, 
			Language languageDesired, Language hmiDisplayLanguageDesired, Vector<AppType> appType,
			String appID, String autoActivateID, Integer correlationID) 
			throws SyncException {
		
		RegisterAppInterface msg = RPCRequestFactory.buildRegisterAppInterface(
				syncMsgVersion, appName, ttsName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, 
				languageDesired, hmiDisplayLanguageDesired, appType, appID, correlationID);

		sendRPCRequestPrivate(msg);
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
	 * @param choiceID -Unique ID used to identify this choice (returned in callback).
	 * @param choiceMenuName -Text name displayed for this choice.
	 * @param choiceVrCommands -Vector of vrCommands used to select this choice by voice. Must contain
	 * 			at least one non-empty element.
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
	
	/******************** END Public Helper Methods *************************/
	
	/**
	 * Gets type of transport currently used by this SyncProxy.
	 * 
	 * @return One of TransportType enumeration values.
	 * 
	 * @see TransportType
	 */
	public TransportType getCurrentTransportType() throws IllegalStateException {
		if (_syncConnection == null) {
			throw new IllegalStateException("Incorrect state of SyncProxyBase: Calling for getCurrentTransportType() while connection is not initialized");
		}
			
		return _syncConnection.getCurrentTransportType();
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
	
	public IJsonRPCMarshaller getJsonRPCMarshaller() {
		return this._jsonRPCMarshaller;
	}
	
} // end-class
