package com.ford.syncV4.proxy;

import java.util.Hashtable;
import java.util.Iterator;
import java.util.Vector;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.proxy.interfaces.ISyncALMLifeCycleListener;
import com.ford.syncV4.proxy.interfaces.ISyncAddSubMenuResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncAlertResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncCreateInteractionChoiceSetResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncDeleteCommandResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncDeleteInteractionChoiceSetResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncDeleteSubMenuResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncDriverDistractionListener;
import com.ford.syncV4.proxy.interfaces.ISyncEncodedSyncPDataListener;
import com.ford.syncV4.proxy.interfaces.ISyncEncodedSyncPDataResponseListener;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.interfaces.ISyncChoiceListener;
import com.ford.syncV4.proxy.interfaces.ISyncPerformInteractionResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncResetGlobalPropertiesListener;
import com.ford.syncV4.proxy.interfaces.ISyncSetGlobalPropertiesResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncSetMediaClockTimerResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncShowResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncSpeakResponseListener;
import com.ford.syncV4.proxy.interfaces.ISyncButtonListener;
import com.ford.syncV4.proxy.interfaces.ISyncCommandListener;
import com.ford.syncV4.proxy.interfaces.ISyncTBTClientStateListener;
import com.ford.syncV4.proxy.interfaces.ISyncUnsubscribeButtonResponseListener;
import com.ford.syncV4.proxy.rpc.AddCommandResponse;
import com.ford.syncV4.proxy.rpc.AddSubMenuResponse;
import com.ford.syncV4.proxy.rpc.AlertResponse;
import com.ford.syncV4.proxy.rpc.ChangeRegistrationResponse;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSetResponse;
import com.ford.syncV4.proxy.rpc.DeleteCommandResponse;
import com.ford.syncV4.proxy.rpc.DeleteFileResponse;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSetResponse;
import com.ford.syncV4.proxy.rpc.DeleteSubMenuResponse;
import com.ford.syncV4.proxy.rpc.EncodedSyncPDataResponse;
import com.ford.syncV4.proxy.rpc.GenericResponse;
import com.ford.syncV4.proxy.rpc.GetDTCs;
import com.ford.syncV4.proxy.rpc.GetVehicleData;
import com.ford.syncV4.proxy.rpc.EndAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.OnAudioPassThru;
import com.ford.syncV4.proxy.rpc.ListFilesResponse;
import com.ford.syncV4.proxy.rpc.OnButtonEvent;
import com.ford.syncV4.proxy.rpc.OnButtonPress;
import com.ford.syncV4.proxy.rpc.OnCommand;
import com.ford.syncV4.proxy.rpc.OnDriverDistraction;
import com.ford.syncV4.proxy.rpc.OnEncodedSyncPData;
import com.ford.syncV4.proxy.rpc.OnHMIStatus;
import com.ford.syncV4.proxy.rpc.OnLanguageChange;
import com.ford.syncV4.proxy.rpc.OnPermissionsChange;
import com.ford.syncV4.proxy.rpc.OnSyncChoiceChosen;
import com.ford.syncV4.proxy.rpc.OnTBTClientState;
import com.ford.syncV4.proxy.rpc.OnVehicleData;
import com.ford.syncV4.proxy.rpc.PerformInteractionResponse;
import com.ford.syncV4.proxy.rpc.ReadDID;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.ResetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.ScrollableMessageResponse;
import com.ford.syncV4.proxy.rpc.SetAppIconResponse;
import com.ford.syncV4.proxy.rpc.SetDisplayLayoutResponse;
import com.ford.syncV4.proxy.rpc.SetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimerResponse;
import com.ford.syncV4.proxy.rpc.ShowResponse;
import com.ford.syncV4.proxy.rpc.SliderResponse;
import com.ford.syncV4.proxy.rpc.SpeakResponse;
import com.ford.syncV4.proxy.rpc.SubscribeButtonResponse;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.UnsubscribeButtonResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;
import com.ford.syncV4.util.DebugTool;

public class SyncProxyALMManager {
	
	public class SyncSubMenu {
		private Integer _menuID = null;
		private Integer _position = null;
		private String _menuName = null;
		
		// Constructor
		SyncSubMenu(Integer menuID, Integer position, String menuName) {
			_menuID = menuID;
			_position = position;
			_menuName = menuName;
		}
		
		// Restrict no-arg constructor
		private SyncSubMenu() {}
		
		// Public Getters
		public Integer getMenuID() {
			return _menuID;
		}
		
		public String getMenuName() {
			return _menuName;
		}
		
		public String toString() {
			return _menuName;
		}
	}
	
	public class SyncCommand {
		private Integer _commandID = null;
		private SyncSubMenu _parentSubMenu = null;
		private Integer _position = null;
		private String _menuName = null;
		private Vector<String> _vrCommands = null;
		
		// Constructor
		SyncCommand(Integer commandID, SyncSubMenu parentSubMenu, Integer position, String menuName, Vector<String> vrCommands) {
			_commandID = commandID;
			_parentSubMenu = parentSubMenu;
			_position = position;
			_menuName = menuName;
			_vrCommands = vrCommands;
		}
		
		// Restrict no-arg constructor
		private SyncCommand() {}
		
		// Public Getters
		public Integer getCommandID() {
			return _commandID;
		}
		
		public SyncSubMenu getParentSubMenu() {
			return _parentSubMenu;
		}
		
		public String getMenuName() {
			return _menuName;
		}
		
		public Vector<String> getVrCommands() {
			return _vrCommands;
		}
		
		public String toString() {
			return _menuName;
		}
	}
	
	public class SyncChoice {
		
		private Choice _choice = null;
		
		// Constructor
		SyncChoice(Choice choice) {
			_choice = choice;
		}
		
		public Choice getChoice() {
			return _choice;
		}
		
		public Integer getChoiceID() {
			return _choice.getChoiceID();
		}
		
		public String getMenuName() {
			return _choice.getMenuName();
		}
		
		public Vector<String> getVrCommands() {
			return _choice.getVrCommands();
		}
		
		public String toString() {
			return _choice.getMenuName();
		}
	}
	
	public class SyncChoiceSet {
		private Integer _choiceSetID = null;
		private Vector<SyncChoice> _choiceSet = null;
		
		// Constructor
		SyncChoiceSet(Integer choiceSetID, Vector<SyncChoice> choiceSet) {
			_choiceSetID = choiceSetID;
			_choiceSet = choiceSet;
		}
		
		public Integer getChoiceSetID() {
			return _choiceSetID;
		}
		
		public Vector<SyncChoice> getChoiceSet() {
			return _choiceSet;
		}
	}
	
	private class SpeakTextParser implements ISyncSpeakResponseListener {

		private int MAX_SUBSTRING_LENGTH = 500;
		
		private Object _mainTag;
		private ISyncSpeakResponseListener _mainSpeakListener;
		private BlockingQueue<String> _speakQueue;
		
		public SpeakTextParser(String ttsText, Object tag, ISyncSpeakResponseListener listener) throws SyncException {
			if (ttsText == null) {
				throw new SyncException("ttsText must not be null", SyncExceptionCause.INVALID_ARGUMENT);
			}
			
			// Set the main tag
			_mainTag = tag;			
			// Set the main listener
			_mainSpeakListener = listener;
			
			// TODO What size is needed? Add one for remained and another for chars cut short based on
				// separating by word and not by a character limit
			_speakQueue = new ArrayBlockingQueue<String>((ttsText.length()/MAX_SUBSTRING_LENGTH)+2);
			
			setTextToSpeak(ttsText);			
		}
		
		private void setTextToSpeak(String text) throws SyncException {
			String[] wordArray = text.split(" ");
			StringBuilder sb = new StringBuilder();
			
			for (int i=0; i<wordArray.length; i++) {
				if ((sb.length()+wordArray[i].length()+1)<MAX_SUBSTRING_LENGTH) {
					sb.append(wordArray[i]);
					sb.append(" ");
				} else {
					_speakQueue.offer(sb.toString());
					sb = new StringBuilder();
					
					// Is the "word" greater than MAX_SUBSTRING_LENGTH characters
					if ((wordArray[i].length()+1) > MAX_SUBSTRING_LENGTH) {
						throw new SyncException("No word for speaking can be " + MAX_SUBSTRING_LENGTH + " characters of length or greater.", SyncExceptionCause.INVALID_ARGUMENT);
					} 

					sb.append(wordArray[i]);
					sb.append(" ");
				}
			}
			
			// Add remaining content of the string builder
			if (sb.length() > 0) {
				_speakQueue.offer(sb.toString());
			}
		}
		
		public void speakFromQueue() {
			if (_speakQueue == null || _speakQueue.size() < 1) {
				return;
			}

			try {
				String textToSpeak = _speakQueue.poll();
				
				speak(textToSpeak, _mainTag, this);
			} catch (SyncException e) {
				DebugTool.logError("smartSpeak failed to speak!", e);
				return;
			}
		}
		
		@Override
		public void onSpeakResponse(SpeakResponse response, Object tag) {
			if (!response.getSuccess()) {
				// There was an error, notify main listener and return
				if (_mainSpeakListener != null) {
					_mainSpeakListener.onSpeakResponse(response, tag);
				}
				
				return;
			} else {			
				if (_speakQueue.size() > 0) {
					speakFromQueue();
				} else {
					// All strings have been spoken, notify main listener and return
					if (_mainSpeakListener != null) {
						_mainSpeakListener.onSpeakResponse(response, tag);
					}
				}
			}
		}
	}
	
	// ArrayLists of SyncCommands, SyncChoices
	private Hashtable<Integer, SyncCommand> _syncCommandsByCommandID = new Hashtable<Integer, SyncCommand>();
	private Hashtable<Integer, SyncCommand> _syncCommandsByCorrelationID = new Hashtable<Integer, SyncCommand>();
	private Hashtable<Integer, SyncSubMenu> _syncSubMenusByCorrelationID = new Hashtable<Integer, SyncSubMenu>();
	private Hashtable<Integer, SyncChoice> _syncChoicesByChoiceID = new Hashtable<Integer, SyncChoice>();
	private Hashtable<Integer, Vector<SyncChoice>> _syncChoicesBySyncChoiceSetID = new Hashtable<Integer, Vector<SyncChoice>>();
	private Hashtable<Integer, SyncChoiceSet> _syncChoiceSetByCorrelationID = new Hashtable<Integer, SyncChoiceSet>();
	
	// Generic Object Tags
	private Hashtable<Integer, Object> _genericTagsByCorrelationID = new Hashtable<Integer, Object>();
	private Hashtable<Integer, Object> _genericTagsByCommandID = new Hashtable<Integer, Object>();
	private Hashtable<Integer, Object> _genericTagsByChoiceSetID = new Hashtable<Integer, Object>();
	private Hashtable<Integer, Object> _genericTagsByChoiceID = new Hashtable<Integer, Object>();

	// Containers for the Interfaces to the Application
	private ISyncALMLifeCycleListener _lifecycleListener = null;
	private Vector<ISyncDriverDistractionListener> _driverDistractionListeners = new Vector<ISyncDriverDistractionListener>();
	private Vector<ISyncEncodedSyncPDataListener> _encodedSyncPDataListeners = new Vector<ISyncEncodedSyncPDataListener>();
	private Vector<ISyncTBTClientStateListener> _tbtClientStateListeners = new Vector<ISyncTBTClientStateListener>();
	private Hashtable<ButtonName, ISyncButtonListener> _buttonListeners = new Hashtable<ButtonName, ISyncButtonListener>();
	private Hashtable<Integer, ISyncButtonListener> _buttonResponseListeners = new Hashtable<Integer, ISyncButtonListener>();
	private Hashtable<Integer, ISyncCommandListener> _commandListeners = new Hashtable<Integer, ISyncCommandListener>();
	private Hashtable<Integer, ISyncCommandListener> _commandResponseListeners = new Hashtable<Integer, ISyncCommandListener>();
	private Hashtable<Integer, ISyncEncodedSyncPDataResponseListener> _encodedSyncPDataResponseListeners = new Hashtable<Integer, ISyncEncodedSyncPDataResponseListener>();
	private Hashtable<Integer, ISyncAddSubMenuResponseListener> _addSubMenuResponseListeners = new Hashtable<Integer,ISyncAddSubMenuResponseListener>();
	private Hashtable<Integer, ISyncAlertResponseListener> _alertResponseListeners = new Hashtable<Integer, ISyncAlertResponseListener>();
	private Hashtable<Integer, ISyncCreateInteractionChoiceSetResponseListener> _createInteractionChoiceSetResponseListeners = new Hashtable<Integer, ISyncCreateInteractionChoiceSetResponseListener>();
	private Hashtable<Integer, ISyncChoiceListener> _interactionChoiceListeners = new Hashtable<Integer, ISyncChoiceListener>();
	private Hashtable<Integer, ISyncDeleteCommandResponseListener> _createDeleteCommandResponseListeners = new Hashtable<Integer, ISyncDeleteCommandResponseListener>();
	private Hashtable<Integer, ISyncDeleteInteractionChoiceSetResponseListener> _deleteInteractionchoiceSetResponseListeners = new Hashtable<Integer, ISyncDeleteInteractionChoiceSetResponseListener>();
	private Hashtable<Integer, ISyncDeleteSubMenuResponseListener> _deleteSubMenuResponseListeners = new Hashtable<Integer, ISyncDeleteSubMenuResponseListener>();
	private Hashtable<Integer, ISyncPerformInteractionResponseListener> _performInteractionResponseListeners = new Hashtable<Integer, ISyncPerformInteractionResponseListener>();
	private Hashtable<Integer, ISyncSetGlobalPropertiesResponseListener> _setGlobalPropertiesResponseListeners = new Hashtable<Integer, ISyncSetGlobalPropertiesResponseListener>();
	private Hashtable<Integer, ISyncResetGlobalPropertiesListener> _resetGlobalPropertiesResponseListeners = new Hashtable<Integer, ISyncResetGlobalPropertiesListener>();
	private Hashtable<Integer, ISyncSetMediaClockTimerResponseListener> _setMediaClockTimerResponseListeners = new Hashtable<Integer, ISyncSetMediaClockTimerResponseListener>();
	private Hashtable<Integer, ISyncShowResponseListener> _showResponseListeners = new Hashtable<Integer, ISyncShowResponseListener>();
	private Hashtable<Integer, ISyncSpeakResponseListener> _speakResponseListeners = new Hashtable<Integer, ISyncSpeakResponseListener>();
	private Hashtable<Integer, ISyncUnsubscribeButtonResponseListener> _unsubscribeButtonResponseListeners = new Hashtable<Integer, ISyncUnsubscribeButtonResponseListener>();
	
	// SyncProxy Object
	private SyncProxyALM syncProxy = null;
	
	// ALM Interface Broker
	private ALMInterfaceBroker _almInterfaceBroker = null;
	
	// Command/Menu ID
	private Integer _commandMenuID = 0;
	
	// Choice ID
	private Integer _interactionChoiceID = 0;
	
	// ChoiceSet ID
	private Integer _interactionChoiceSetID = 0;
	
	// Correlation ID
	private Integer _correlationID = 0;
	
	
	/************************** Start Constructors **********************************/
	/**
	 * Constructor for the SyncProxyALMManager object, the manager for communicating between the App and SYNC
	 * 
	 * Takes advantage of the advanced lifecycle management.
	 * @param appName - Name of the application displayed on SYNC. 
	 * @param isMediaApp - Indicates if the app is a media application.
	 */
	public SyncProxyALMManager(ISyncALMLifeCycleListener lifeCycleListener, String appName, Boolean isMediaApp, 
			Language languageDesired, Language hmiDisplayLanguageDesired, String appID) throws SyncException {
		
		this(lifeCycleListener);
		
		syncProxy = new SyncProxyALM(_almInterfaceBroker, appName, isMediaApp, 
				languageDesired, hmiDisplayLanguageDesired, appID);
	}
	
	/**
	 * Constructor for the SyncProxyALMManager object, the manager for communicating between the App and SYNC.
	 * 
	 * Takes advantage of the advanced lifecycle management. 
	 *  
	 * @param appName - Name of the application displayed on SYNC. 
	 * @param ngnMediaScreenAppName - Name of the application displayed on SYNC for Navigation equipped 
	 * vehicles. Limited to five characters. 
	 * @param vrSynonyms - A vector of strings, all of which can be used as voice commands to 
	 * @param isMediaApp - Indicates if the app is a media application.
	 * @param syncMsgVersion - Indicates the version of SYNC AppLink Messages desired. Must be less than
	 * or equal to the version of SYNC AppLink running on the vehicle. 
	 * @param languageDesired - Indicates the language desired for the SYNC interface.
	 * @param autoActivateID - ID used to re-register previously registered application.
	 * @throws SyncException
	 */
	public SyncProxyALMManager(ISyncALMLifeCycleListener lifeCycleListener, String appName, String ngnMediaScreenAppName, 
			Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion, 
			Language languageDesired, Language hmiDisplayLanguageDesired, String appID, 
			String autoActivateID) throws SyncException{
		
		this(lifeCycleListener);
		
		syncProxy = new SyncProxyALM(_almInterfaceBroker, appName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, 
				languageDesired, hmiDisplayLanguageDesired, appID, autoActivateID);
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC.
	 * 
	 * Takes advantage of the advanced lifecycle management. 
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC. 
	 * @param applicationContext - Context of the application. Used to access application specific resources.
	 * @param appName - Name of the application displayed on SYNC. 
	 * @param ngnMediaScreenAppName - Name of the application displayed on SYNC for Navigation equipped 
	 * vehicles. Limited to five characters. 
	 * @param vrSynonyms - A vector of strings, all of which can be used as voice commands to 
	 * @param isMediaApp - Indicates if the app is a media application.
	 * @param syncMsgVersion - Indicates the version of SYNC AppLink Messages desired. Must be less than
	 * or equal to the version of SYNC AppLink running on the vehicle. 
	 * @param languageDesired - Indicates the language desired for the SYNC interface.
	 * @param autoActivateID - ID used to re-register previously registered application.
	 * @throws SyncException
	 */
	public SyncProxyALMManager(ISyncALMLifeCycleListener lifeCycleListener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			String appName, String ngnMediaScreenAppName, Vector<String> vrSynonyms, 
			Boolean isMediaApp, SyncMsgVersion syncMsgVersion, Language languageDesired, 
			Language hmiDisplayLanguageDesired, String appID, String autoActivateID) throws SyncException {
		
		this(lifeCycleListener);
		
		syncProxy = new SyncProxyALM(_almInterfaceBroker, syncProxyConfigurationResources, 
				appName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, 
				languageDesired, hmiDisplayLanguageDesired, appID, autoActivateID);
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC.
	 * 
	 * Takes advantage of the advanced lifecycle management. 
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC. 
	 * @param appName - Name of the application displayed on SYNC. 
	 * @param ngnMediaScreenAppName - Name of the application displayed on SYNC for Navigation equipped 
	 * vehicles. Limited to five characters. 
	 * @param vrSynonyms - A vector of strings, all of which can be used as voice commands to 
	 * @param isMediaApp - Indicates if the app is a media application.
	 * @param syncMsgVersion - Indicates the version of SYNC AppLink Messages desired. Must be less than
	 * or equal to the version of SYNC AppLink running on the vehicle. 
	 * @param languageDesired - Indicates the language desired for the SYNC interface.
	 * @param autoActivateID - ID used to re-register previously registered application.
	 * @param callbackToUIThread - If true, all callbacks will occur on the UI thread.
	 * @throws SyncException
	 */
	public SyncProxyALMManager(ISyncALMLifeCycleListener lifeCycleListener, String appName, String ngnMediaScreenAppName, 
			Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion, 
			Language languageDesired, Language hmiDisplayLanguageDesired, String appID, 
			String autoActivateID, boolean callbackToUIThread) throws SyncException {
		
		this(lifeCycleListener);
		
		syncProxy = new SyncProxyALM(_almInterfaceBroker, appName, ngnMediaScreenAppName, 
				vrSynonyms, isMediaApp, syncMsgVersion, 
				languageDesired, hmiDisplayLanguageDesired, appID, autoActivateID, callbackToUIThread);
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC.
	 * 
	 * Takes advantage of the advanced lifecycle management. 
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC. 
	 * @param applicationContext - Context of the application. Used to access application specific resources.
	 * @param appName - Name of the application displayed on SYNC. 
	 * @param ngnMediaScreenAppName - Name of the application displayed on SYNC for Navigation equipped 
	 * vehicles. Limited to five characters. 
	 * @param vrSynonyms - A vector of strings, all of which can be used as voice commands to 
	 * @param isMediaApp - Indicates if the app is a media application.
	 * @param syncMsgVersion - Indicates the version of SYNC AppLink Messages desired. Must be less than
	 * or equal to the version of SYNC AppLink running on the vehicle. 
	 * @param languageDesired - Indicates the language desired for the SYNC interface.
	 * @param autoActivateID - ID used to re-register previously registered application.
	 * @param callbackToUIThread - If true, all callbacks will occur on the UI thread.
	 * @throws SyncException
	 */
	public SyncProxyALMManager(ISyncALMLifeCycleListener lifeCycleListener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			String appName, String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, 
			SyncMsgVersion syncMsgVersion, Language languageDesired, 
			Language hmiDisplayLanguageDesired, String appID, String autoActivateID, 
			boolean callbackToUIThread) throws SyncException {
		
		this(lifeCycleListener);
		
		syncProxy = new SyncProxyALM(_almInterfaceBroker, syncProxyConfigurationResources, appName, ngnMediaScreenAppName, 
				vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, hmiDisplayLanguageDesired, appID, autoActivateID, 
				callbackToUIThread);		
	}
	
	// Private constructor called by all public constructors
	private SyncProxyALMManager(ISyncALMLifeCycleListener lifeCycleListener) throws SyncException {
		
		// Ensure lifeCycleListener is not null
		if (lifeCycleListener == null) {
			throw new SyncException("IProxyALMLifeCycleListener cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		_almInterfaceBroker = new ALMInterfaceBroker();
		
		_lifecycleListener = lifeCycleListener;
	}
	/************************** End Constructors **********************************/
	
	
	// Function to return next correlation ID
	private synchronized Integer getNextAlmCorrelationId() {
		// Note: This method is synchronized to ensure no two calls ever return the same value
		return _correlationID++;
	}
	
	// Function to return next commandMenuID
	private synchronized Integer getNextAlmCommandMenuId() {
		// Note: This method is synchronized to ensure no two calls ever return the same value
		return _commandMenuID++;
	}
	
	// Function to return next interactionChoiceID
	private synchronized Integer getNextInteractionChoiceID() {
		// Note: This method is synchronized to ensure no two calls ever return the same value
		return _interactionChoiceID++;
	}
	
	// Function to return next interactionChoiceSetID
	private synchronized Integer getNextInteractionChoiceSetID() {
		// Note: This method is synchronized to ensure no two calls ever return the same value
		return _interactionChoiceSetID++;
	}
	
	/************************** Siphon Methods *************************************/
	// Public method to enable the siphon transport
	public static void enableSiphonDebug() {
		SyncProxyBase.enableSiphonDebug();
	}
	
	// Public method to disable the Siphon Trace Server
	public static void disableSiphonDebug() {
		SyncProxyBase.disableSiphonDebug();
	}
	/************************** End Siphon Methods *************************************/
	
	/*********************** Expose Public Proxy Methods *******************************/
	public void dispose() throws SyncException {
		syncProxy.dispose();
	}
	
	public void resetProxy() throws SyncException {
		syncProxy.resetProxy();
	}
	/********************* End Expose Public Proxy Methods *****************************/
	
	
	
	// DriverDistraction Listeners
	private void addIProxyDriverDistractionListener(ISyncDriverDistractionListener newListener) {
		_driverDistractionListeners.add(newListener);
	}	
	private void removeIProxyDriverDistractionListener(ISyncDriverDistractionListener removeListener) {
		_driverDistractionListeners.remove(removeListener);
	}
	
	// EncodedSyncPData Listeners
	private void addIProxyEncodedSyncPDataListener(ISyncEncodedSyncPDataListener newListener) {
		// Add a listener for new Encoded Sync PData
		_encodedSyncPDataListeners.add(newListener);
	}
	private void removeIProxyEncodedSyncPDataListener(ISyncEncodedSyncPDataListener removeListener) {
		// Add a listener for new Encoded Sync PData
		_encodedSyncPDataListeners.remove(removeListener);
	}
	
	// TBTClientState Listeners
	private void addIProxyTBTClientStateListener(ISyncTBTClientStateListener newListener) {
		_tbtClientStateListeners.add(newListener);
	}
	private void removeIProxyTBTClientStateListener(ISyncTBTClientStateListener removeListener) {
		_tbtClientStateListeners.remove(removeListener);
	}
	
	// Button Listeners
	private void addIProxySyncButtonListener(ISyncButtonListener newListener, Integer correlationID, ButtonName button) {
		// Add button event/press listener
		_buttonListeners.put(button, newListener);
		
		// Add addButton response listener
		_buttonResponseListeners.put(correlationID, newListener);
	}
	private void removeAddIProxySyncButtonListener(ButtonName button) {
		_buttonListeners.remove(button);
	}
	private void removeAddIProxySyncButtonResponseListener(Integer correlationID) {
		_buttonResponseListeners.remove(correlationID);
	}
	
	// Command Listeners
	private void addIProxySyncCommandListener(ISyncCommandListener newListener, 
			Integer correltionID, Integer commandID) {
		// Add listener for command selections
		_commandListeners.put(commandID, newListener);
		
		// Add listener for addCommand response
		_commandResponseListeners.put(correltionID, newListener);
	}	
	private void removeIProxySyncCommandListener(Integer commandID) {
		_commandListeners.remove(commandID);
	}
	private void removeAddCommandResponseListener(Integer correlationID) {
		_commandResponseListeners.remove(correlationID);
	}
	
	// SyncCommands
	private void addSyncCommand(SyncCommand newSyncCommand, Integer correlationID) {
		_syncCommandsByCommandID.put(newSyncCommand.getCommandID(), newSyncCommand);
		_syncCommandsByCorrelationID.put(correlationID, newSyncCommand);
	}
	private void removeSyncCommand(SyncCommand syncCommandToRemove) {
		_syncCommandsByCommandID.remove(syncCommandToRemove.getCommandID());
	}
	private void removeSyncCommandByCorrelationID(Integer correlationID) {
		_syncCommandsByCorrelationID.remove(correlationID);
	}
	
	// EncodedSyncPDataResponse Listeners
	private void addIProxyEncodedSyncPDataResponseListener(ISyncEncodedSyncPDataResponseListener newListener, Integer correlationID) {
		// Add a listener for the response
		_encodedSyncPDataResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyEncodedSyncPDataResponseListener(Integer correlationID) {
		_encodedSyncPDataResponseListeners.remove(correlationID);
	}
	
	// AddSubMenuResponse Listeners
	private void addIProxyAddSubMenuResponseListener(ISyncAddSubMenuResponseListener newListener, Integer correlationID) {
		_addSubMenuResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyAddSubMenuResponseListener(Integer correlationID) {
		_addSubMenuResponseListeners.remove(correlationID);
	}
	private void addSyncSubMenuByCorrelationID(SyncSubMenu syncSubMenu, Integer correlationID) {
		_syncSubMenusByCorrelationID.put(correlationID, syncSubMenu);		
	}
	private void removeSyncSubMenuByCorrelationID(Integer correlationID) {
		_syncSubMenusByCorrelationID.remove(correlationID);
	}
	
	// AlertResponse Listeners
	private void addIProxyAlertResponseListener(ISyncAlertResponseListener newListener, Integer correlationID) {
		_alertResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyAlertResponseListener(Integer correlationID) {
		_alertResponseListeners.remove(correlationID);
	}
	
	// CreateInteractionChoiceSetResponse Listeners
	private void addIProxyCreateInteractionChoiceSetResponseListener(ISyncCreateInteractionChoiceSetResponseListener newListener, Integer correlationID) {
		_createInteractionChoiceSetResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyCreateInteractionChoiceSetResponse(Integer correlationID) {
		_createInteractionChoiceSetResponseListeners.remove(correlationID);
	}
	private void associateSyncChoicesWithInteractionChoiceSet(Vector<SyncChoice> syncChoiceSet, Integer interactionChoiceSetID) {
		_syncChoicesBySyncChoiceSetID.put(interactionChoiceSetID, syncChoiceSet);
	}
	private void addSyncChoiceSetByCorrelationID(SyncChoiceSet syncChoiceSet, Integer correlationID) {
		_syncChoiceSetByCorrelationID.put(correlationID, syncChoiceSet);
	}
	private void removeSyncChoiceSetByCorrelationID(Integer correlationID) {
		_syncChoiceSetByCorrelationID.remove(correlationID);
	}
	
	// DeleteCommandResponse Listeners
	private void addIProxyDeleteCommandResponse(ISyncDeleteCommandResponseListener newListener, Integer correlationID) {
		_createDeleteCommandResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyDeleteCommandResponse(Integer correlationID) {
		_createDeleteCommandResponseListeners.remove(correlationID);
	}
	
	// DeleteInteractionChoiceSetResponse Listeners
	private void addIProxyDeleteInteractionChoiceSetResponseListener(ISyncDeleteInteractionChoiceSetResponseListener newListener, Integer correlationID) {
		_deleteInteractionchoiceSetResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyDeleteInteractionChoiceSetResponseListener(Integer correlationID) {
		_deleteInteractionchoiceSetResponseListeners.remove(correlationID);
	}
	private void disassociateSyncChoicesWithInteractionChoiceSet(Integer interactionChoiceSetID) {
		_syncChoicesBySyncChoiceSetID.remove(interactionChoiceSetID);
	}
	
	// DeleteSubMenuRespose Listeners
	private void addIProxyDeleteSubMenuResponseListener(ISyncDeleteSubMenuResponseListener newListener, Integer correlationID) {
		_deleteSubMenuResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyDeleteSubMenuResponseListener(Integer correlationID) {
		_deleteSubMenuResponseListeners.remove(correlationID);
	}
	
	// PerformInteractionResponse Listener
	private void addIProxyPerformInteractionResponseListener(ISyncPerformInteractionResponseListener newListener, Integer correlationID) {
		_performInteractionResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyPerformInteractionResponseListener(Integer correlationID) {
		_performInteractionResponseListeners.remove(correlationID);
	}
	
	// SetGlobalPropertiesResponse Listener
	private void addIProxySetGlobalPropertiesResponseListener(ISyncSetGlobalPropertiesResponseListener newListener, Integer correlationID) {
		_setGlobalPropertiesResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxySetGlobalPropertiesResponseListener(Integer correlationID) {
		_setGlobalPropertiesResponseListeners.remove(correlationID);
	}
	
	// ResetGlobalPropertiesResponse Listener
	private void addIProxyResetGlobalPropertiesListener(ISyncResetGlobalPropertiesListener newListener, Integer correlationID) {
		_resetGlobalPropertiesResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyResetGlobalPropertiesResponse(Integer correlationID) {
		_resetGlobalPropertiesResponseListeners.remove(correlationID);
	}
	
	// SetMediaClockTimerResponse Listeners
	private void addIProxySetMediaClockTimerResponseListener(ISyncSetMediaClockTimerResponseListener newListener, Integer correlationID) {
		_setMediaClockTimerResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxySetMediaClockTimerResponseListener(Integer correlationID) {
		_setMediaClockTimerResponseListeners.remove(correlationID);
	}
	
	// ShowResponse Listeners
	private void addIProxyShowResponseListener(ISyncShowResponseListener newListener, Integer correlationID) {
		_showResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyShowResponseListener(Integer correlationID) {
		_showResponseListeners.remove(correlationID);
	}
	
	// SpeakResponse Listeners
	private void addIProxySpeakResponseListener(ISyncSpeakResponseListener newListener, Integer correlationID) {
		_speakResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxySpeakResponseListener(Integer correlationID) {
		_speakResponseListeners.remove(correlationID);
	}
	
	// UnsubscribeButtonResponse Listeners
	private void addIProxyUnsubscribeButtonResponseListener(ISyncUnsubscribeButtonResponseListener newListener, Integer correlationID) {
		_unsubscribeButtonResponseListeners.put(correlationID, newListener);
	}
	private void removeIProxyUnsubscribeButtonResponseListener(Integer correlationID) {
		_unsubscribeButtonResponseListeners.remove(correlationID);
	}
	
	//****** IProxySyncInteractionChoiceListener
	private void addIProxySyncInteractionChoiceListener(ISyncChoiceListener newListener, 
			Integer choiceID) {
		// Add listener for choice selections
		_interactionChoiceListeners.put(choiceID, newListener);
	}
	private void deleteIProxySyncInteractionChoiceListener(Integer choiceID) {
		// TODO This is never called. Is there a time when we can delete a choice listener. I don't thinks so.
		
		// Delete listener for choice selections
		_interactionChoiceListeners.remove(choiceID);
	}
	
	// SyncChoices
	private void addSyncChoice(SyncChoice newSyncChoice) {
		_syncChoicesByChoiceID.put(newSyncChoice.getChoiceID(), newSyncChoice);
	}
	private void removeSyncChoice(SyncChoice syncChoiceToRemove) {
		// TODO Currently, we do not delete SYNC Choices as they have no direct counterpart on SYNC.
		_syncChoicesByChoiceID.remove(syncChoiceToRemove.getChoice());
	}
	
	// Generic Object Tags By CorrelationID
	private void addGenericObjectTagByCorrelationID(Object tag, Integer correlationID) {
		_genericTagsByCorrelationID.put(correlationID, tag);
	}
	private void removeGenericObjectTagByCorrelationID(Integer correlationID) {
		_genericTagsByCorrelationID.remove(correlationID);
	}
	
	// Generic Object Tags By CommandID
	private void addGenericObjectTagByCommandID(Object tag, Integer commandID) {
		_genericTagsByCommandID.put(commandID, tag);
	}
	private void removeGenericObjectTagByCommandID(Integer commandID) {
		_genericTagsByCommandID.remove(commandID);
	}
	
	// Generic Object Tags By ChoiceSetID
	private void addGenericObjectTagByChoiceSetID(Object tag, Integer choiceSetID) {
		_genericTagsByChoiceSetID.put(choiceSetID, tag);
	}
	private void removeGenericObjectTagByChoiceSetID(Integer choiceSetID) {
		_genericTagsByChoiceSetID.remove(choiceSetID);
	}
	
	// Generic Object Tags By ChoiceID
	private void addGenericObjectTagByChoiceID(Object tag, Integer choiceID) {
		_genericTagsByChoiceID.put(choiceID, tag);
	}
	private void removeGenericObjectTagByChoiceID(Integer choiceID) {
		// TODO: Is this needed? It's never called. I don't think so.
		_genericTagsByChoiceID.remove(choiceID);
	}
	
	/***************** Public RPC Helper Methods ***************/
	// Note: These are mandatory to use the native interface construct
	/**
	 *Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 *
	 *@param commandID
	 *@param menuText
	 *@param parentID
	 *@param position
	 *@param vrCommands
	 *@param _correlationID
	 *@throws SyncException
	 */
	public SyncCommand addSyncCommand(String menuText, SyncSubMenu parentSubMenu, Integer position,
			Vector<String> vrCommands, Object tag, ISyncCommandListener listener) 
			throws SyncException {
		
		// Listener required for addCommand
		if (listener == null) {
			throw new SyncException("IProxySyncCommandListener cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		Integer correlationID = getNextAlmCorrelationId();
		Integer commandID = getNextAlmCommandMenuId();
		
		// Create an SyncCommand Object to return
		SyncCommand thisSyncCommand = new SyncCommand(commandID, parentSubMenu, position, menuText, vrCommands);
		
		// Add listener
		addIProxySyncCommandListener(listener, correlationID, commandID);
		
		// Add SyncCommand
		addSyncCommand(thisSyncCommand, correlationID);
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
			addGenericObjectTagByCommandID(tag, commandID);
		}
		
		// Call base addCommand
		if (parentSubMenu != null) {
			syncProxy.addCommand(commandID, menuText, parentSubMenu.getMenuID(), position, vrCommands, correlationID);
		} else {
			syncProxy.addCommand(commandID, menuText, null, position, vrCommands, correlationID);
		}
		return thisSyncCommand;
	}
	
	/**
	 * Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param commandID
	 * @param menuText
	 * @param vrCommands
	 * @param _correlationID
	 * @return 
	 * @throws SyncException
	 */
	public SyncCommand addSyncCommand(String menuText, Vector<String> vrCommands, Object tag, ISyncCommandListener listener) 
			throws SyncException {
		
		return addSyncCommand(menuText, null, null, vrCommands, tag, listener);
	}
	
	/**
	 * Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param commandID
	 * @param vrCommands
	 * @param _correlationID
	 * @return 
	 * @throws SyncException
	 */
	public SyncCommand addSyncCommand(Vector<String> vrCommands, Object tag, ISyncCommandListener listener) 
			throws SyncException {
		
		return addSyncCommand(null, null, null, vrCommands, tag, listener);
	}
	
	/**
	 *Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 *
	 *@param commandID
	 *@param menuText
	 *@param position
	 *@param _correlationID
	 * @return 
	 *@throws SyncException
	 */
	public SyncCommand addSyncCommand(String menuText, Integer position, Object tag,
			ISyncCommandListener listener) 
			throws SyncException {
		
		return addSyncCommand(menuText, null, position, null, tag, listener);
	}
	
	/**
	 *Sends an AddCommand RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 *
	 *@param commandID
	 *@param menuText
	 *@param _correlationID
	 * @return 
	 *@throws SyncException
	 */
	public SyncCommand addSyncCommand(String menuText, Object tag, ISyncCommandListener listener) 
			throws SyncException {
		
		return addSyncCommand(menuText, null, null, null, tag, listener);
	}
	
	/**
	 * Sends an AddSyncSubMenu RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param menuID
	 * @param menuName
	 * @param position
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void addSyncSubMenu(String menuName, Integer position, Object tag,
			ISyncAddSubMenuResponseListener listener) 
			throws SyncException {
		
		// Require a listener
		if (listener == null) {
			throw new SyncException("ISyncAddSubMenuResponseListener cannot be null.", 
					SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		Integer correlationID = getNextAlmCorrelationId();
		Integer menuID = getNextAlmCommandMenuId();
				
		SyncSubMenu thisSubMenu = new SyncSubMenu(menuID, position, menuName);
		
		addIProxyAddSubMenuResponseListener(listener, correlationID);
		addSyncSubMenuByCorrelationID(thisSubMenu, correlationID);
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}

		// Base addSubMenu
		syncProxy.addSubMenu(menuID, menuName, position, correlationID);
	}
	
	/**
	 * Sends an AddSyncSubMenu RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param menuID
	 * @param menuName
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void addSyncSubMenu(String menuName, Object tag, ISyncAddSubMenuResponseListener listener) 
			throws SyncException {
		
		addSyncSubMenu(menuName, null, tag, listener);
	}
	
	/**
	 * Sends an EncodedData RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param data
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void encodedSyncPData(Vector<String> data, Object tag, 
			ISyncEncodedSyncPDataResponseListener listener) 
			throws SyncException {
		
		Integer correlationID = getNextAlmCorrelationId();

		if (listener != null) {
			// Add listener
			addIProxyEncodedSyncPDataResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base encodedSyncPData
		syncProxy.encodedSyncPData(data, correlationID);
	}
	
	/**
	 * Sends an Alert RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param ttsText
	 * @param alertText1
	 * @param alertText2
	 * @param playTone
	 * @param duration
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void alert(String ttsText, String alertText1,
			String alertText2, Boolean playTone, Integer duration,
			Object tag, ISyncAlertResponseListener listener) throws SyncException {

		Integer correlationID = getNextAlmCorrelationId();

		// Add listener if listener is given
		if (listener != null) {
			addIProxyAlertResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base alert
		syncProxy.alert(ttsText, alertText1, alertText2, playTone, duration, correlationID);
	}
	
	/**
	 * Sends an Alert RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param ttsChunks
	 * @param alertText1
	 * @param alertText2
	 * @param playTone
	 * @param duration
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void alert(Vector<TTSChunk> ttsChunks,
			String alertText1, String alertText2, Boolean playTone,
			Integer duration,  Object tag, ISyncAlertResponseListener listener) 
			throws SyncException {
		
		Integer correlationID = getNextAlmCorrelationId();

		// Add listener if listener is given
		if (listener != null) {
			addIProxyAlertResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		syncProxy.alert(ttsChunks, alertText1, alertText2, playTone, duration, correlationID);
	}
	
	/**
	 * Sends an Alert RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param ttsText
	 * @param playTone
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void alert(String ttsText, Boolean playTone, Object tag,
			ISyncAlertResponseListener listener) throws SyncException {
		
		alert(ttsText, null, null, playTone, null, tag, listener);
	}
	
	/**
	 * Sends an Alert RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param alertText1
	 * @param alertText2
	 * @param duration
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void alert(String alertText1, String alertText2, Boolean playTone,
			Integer duration, Object tag, ISyncAlertResponseListener listener) 
			throws SyncException {
		
		alert((Vector<TTSChunk>)null, alertText1, alertText2, playTone, duration, tag, listener);
	}
	
	/**
	 * Sends an Alert RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param chunks
	 * @param playTone
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void alert(Vector<TTSChunk> chunks, Boolean playTone, Object tag,
			ISyncAlertResponseListener listener) throws SyncException {
		
		alert(chunks, null, null, playTone, null, tag, listener);
	}
	
	/**
	 * Sends a CreateInteractionChoiceSet RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param syncChoiceSet
	 * @param _interactionChoiceSetID
	 * @param _correlationID
	 * @throws SyncException
	 */
	public SyncChoiceSet createInteractionChoiceSet(Vector<SyncChoice> syncChoiceSet, Object tag, 
			ISyncCreateInteractionChoiceSetResponseListener listener) 
			throws SyncException {
		
		// Require a SyncChoiceSet
		if (syncChoiceSet == null) {
			throw new SyncException("syncChoiceSet cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		// Require a listener
		if (listener == null) {
			throw new SyncException("ISyncCreateInteractionChoiceSetResponseListener cannot be null.", 
					SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		Integer correlationID = getNextAlmCorrelationId();
		Integer interactionChoiceSetID = getNextInteractionChoiceSetID();
		
		// Add listener
		addIProxyCreateInteractionChoiceSetResponseListener(listener, correlationID);
		
		SyncChoiceSet syncInteractionChoiceSet = new SyncChoiceSet(interactionChoiceSetID, syncChoiceSet);
		
		// Convert the Vector<SyncChoice> to a Vector<Choice>
		Vector<Choice> choiceSet = new Vector<Choice>();
		Iterator<SyncChoice> itr = syncChoiceSet.iterator();
		while (itr.hasNext()) {
			SyncChoice tempChoice = itr.next();
			
			if (tempChoice != null) {
				choiceSet.add(tempChoice.getChoice());
			}
		}
		
		// Add vector of choices for deletion purposes, because each choice 
		// can only be used with one interaction choice set
		associateSyncChoicesWithInteractionChoiceSet(syncChoiceSet, interactionChoiceSetID);
		
		// Associate this choice set with its correlationID
		addSyncChoiceSetByCorrelationID(syncInteractionChoiceSet, correlationID);
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
			addGenericObjectTagByChoiceSetID(tag, interactionChoiceSetID);
		}
		
		// Base createInteracationChoiceSet
		syncProxy.createInteractionChoiceSet(choiceSet, interactionChoiceSetID, correlationID);
		return syncInteractionChoiceSet;
	}
	
	/**
	 * Sends a DeleteInteractionChoiceSet RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param _interactionChoiceSetID
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void deleteSyncChoiceSet(SyncChoiceSet syncChoiceSetToDelete, 
			Object tag, ISyncDeleteInteractionChoiceSetResponseListener listener) 
			throws SyncException {
		
		// Ensure syncChoiceSetToDelete is not null
		if (syncChoiceSetToDelete == null) {
			throw new SyncException("SyncChoiceSet cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		Integer correlationID = getNextAlmCorrelationId();

		// Add listener if listener is given
		if (listener != null) {
			addIProxyDeleteInteractionChoiceSetResponseListener(listener, correlationID);
		}
		
		// Delete list of choice set choices
		disassociateSyncChoicesWithInteractionChoiceSet(syncChoiceSetToDelete.getChoiceSetID());
		// Delete any object tags associated with the choice set
		removeGenericObjectTagByChoiceSetID(syncChoiceSetToDelete.getChoiceSetID());
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base deleteInteracationChoiceSet
		syncProxy.deleteInteractionChoiceSet(syncChoiceSetToDelete.getChoiceSetID(), correlationID);
	}
	

	
	/**
	 * Deletes a SyncCommand from the Ford SYNC System. 
	 * 
	 * @param syncCommandToDelete - The Choice SyncCommand to delete.
	 * @param listener - Interface to return the response to the deleteCommand request.
	 * @throws SyncException
	 */
	public void deleteSyncCommand(SyncCommand syncCommandToDelete, Object tag,
			ISyncDeleteCommandResponseListener listener) throws SyncException{
		
		// Do not allow syncCommandToDelete to be null
		if (syncCommandToDelete == null) {
			throw new SyncException("SyncCommand cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Remove command selector listener
		removeIProxySyncCommandListener(syncCommandToDelete.getCommandID());
		
		// Remove the Tag for this Command
		removeGenericObjectTagByCommandID(syncCommandToDelete.getCommandID());

		// Add listener if listener is given
		if (listener != null) {
			addIProxyDeleteCommandResponse(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base deleteCommand (from SYNC)
		syncProxy.deleteCommand(syncCommandToDelete.getCommandID(), correlationID);
		
		// Remove SyncCommand from list of SyncCommands
		removeSyncCommand(syncCommandToDelete);
	}
	
	/**
	 * Sends a DeleteSubMenu RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param menuID
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void deleteSyncSubMenu(SyncSubMenu syncSubMenuToDelete, Object tag,
			ISyncDeleteSubMenuResponseListener listener) throws SyncException {
		
		if (syncSubMenuToDelete == null) {
			throw new SyncException("SyncSubMenu must not be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		Integer correlationID = getNextAlmCorrelationId();

		// Add listener if listener is given
		if (listener != null) {
			addIProxyDeleteSubMenuResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base deleteSubMenu
		syncProxy.deleteSubMenu(syncSubMenuToDelete.getMenuID(), correlationID);
	}
	
	/**
	 * Sends a PerformInteraction RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param initPrompt
	 * @param displayText
	 * @param syncChoiceSet
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void performInteraction(String initPrompt, String displayText, 
			SyncChoiceSet syncChoiceSet, Object tag,
			ISyncPerformInteractionResponseListener listener) 
			throws SyncException {
		
		if (syncChoiceSet == null) {
			throw new SyncException("SyncChoiceSet cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxyPerformInteractionResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base performInteraction
		syncProxy.performInteraction(initPrompt, displayText, syncChoiceSet.getChoiceSetID(), correlationID);
	}
	
	/**
	 * Sends a PerformInteraction RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param initPrompt
	 * @param displayText
	 * @param syncChoiceSet
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void performInteraction(String initPrompt,
			String displayText, SyncChoiceSet syncChoiceSet,
			String helpPrompt, String timeoutPrompt, InteractionMode interactionMode,
			Integer timeout, Object tag, 
			ISyncPerformInteractionResponseListener listener) 
			throws SyncException {
		
		if (syncChoiceSet == null) {
			throw new SyncException("SyncChoiceSet must not be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxyPerformInteractionResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base performInteraction
		syncProxy.performInteraction(initPrompt, displayText, syncChoiceSet.getChoiceSetID(), helpPrompt, timeoutPrompt, interactionMode, timeout, correlationID);
	}
	
	/**
	 * Sends a PerformInteraction RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param initPrompt
	 * @param displayText
	 * @param syncChoiceSetList
	 * @param helpPrompt
	 * @param timeoutPrompt
	 * @param interactionMode
	 * @param timeout
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void performInteraction(String initPrompt,
			String displayText, Vector<SyncChoiceSet> syncChoiceSetList,
			String helpPrompt, String timeoutPrompt,
			InteractionMode interactionMode, Integer timeout, Object tag,
			ISyncPerformInteractionResponseListener listener) 
			throws SyncException {
		
		if (syncChoiceSetList == null) {
			throw new SyncException("syncChoiceSetList must not be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxyPerformInteractionResponseListener(listener, correlationID);
		}
		
		// Convert the vector of InteractionChoiceSets to a vecto of their IDs
		Vector<Integer> interactionChoiceSetIDList = new Vector<Integer>();
		Iterator<SyncChoiceSet> itr = syncChoiceSetList.iterator();
		while (itr.hasNext()) {
			SyncChoiceSet thisChoiceSet = itr.next();
			
			if (thisChoiceSet != null) {
				interactionChoiceSetIDList.add(thisChoiceSet.getChoiceSetID());
			}
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}

		// Base performInteraction
		syncProxy.performInteraction(initPrompt, displayText, interactionChoiceSetIDList, helpPrompt, 
				timeoutPrompt, interactionMode, timeout, correlationID);
	}
	
	/**
	 * Sends a PerformInteraction RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param initChunks
	 * @param displayText
	 * @param syncChoiceSetList
	 * @param helpChunks
	 * @param timeoutChunks
	 * @param interactionMode
	 * @param timeout
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void performInteraction(
			Vector<TTSChunk> initChunks, String displayText,
			Vector<SyncChoiceSet> syncChoiceSetList,
			Vector<TTSChunk> helpChunks, Vector<TTSChunk> timeoutChunks,
			InteractionMode interactionMode, Integer timeout, Object tag,
			ISyncPerformInteractionResponseListener listener) 
			throws SyncException {
		
		if (syncChoiceSetList == null) {
			throw new SyncException("syncChoiceSetList must not be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxyPerformInteractionResponseListener(listener, correlationID);
		}
		
		// Convert the vector of InteractionChoiceSets to a vecto of their IDs
		Vector<Integer> interactionChoiceSetIDList = new Vector<Integer>();
		Iterator<SyncChoiceSet> itr = syncChoiceSetList.iterator();
		while (itr.hasNext()) {
			SyncChoiceSet thisChoiceSet = itr.next();
			
			if (thisChoiceSet != null) {
				interactionChoiceSetIDList.add(itr.next().getChoiceSetID());
			}
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base performInteracation
		syncProxy.performInteraction(initChunks, displayText, interactionChoiceSetIDList, 
				helpChunks, timeoutChunks, interactionMode, timeout, correlationID);
	}
	
	/**
	 * Sends a SetGlobalProperties RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param helpPrompt
	 * @param timeoutPrompt
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void setGlobalProperties(String helpPrompt, String timeoutPrompt, 
			Object tag, ISyncSetGlobalPropertiesResponseListener listener) 
		throws SyncException {
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxySetGlobalPropertiesResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base setGlobalProperties
		syncProxy.setGlobalProperties(helpPrompt, timeoutPrompt, correlationID);
	}
	
	/**
	 * Sends a SetGlobalProperties RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param helpChunks
	 * @param timeoutChunks
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void setGlobalProperties(
			Vector<TTSChunk> helpChunks, Vector<TTSChunk> timeoutChunks,
			Object tag, ISyncSetGlobalPropertiesResponseListener listener) 
			throws SyncException {
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxySetGlobalPropertiesResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}

		// Base setGlobalProperties
		syncProxy.setGlobalProperties(helpChunks, timeoutChunks, correlationID);
	}
	
	public void resetGlobalProperties(Vector<GlobalProperty> properties, 
			Object tag, ISyncResetGlobalPropertiesListener listener) 
			throws SyncException{
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxyResetGlobalPropertiesListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base resetGlobalProperties
		syncProxy.resetGlobalProperties(properties, correlationID);
	}
	
	/**
	 * Sends a SetMediaClockTimer RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param hours
	 * @param minutes
	 * @param seconds
	 * @param updateMode
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void setMediaClockTimer(Integer hours,
			Integer minutes, Integer seconds, UpdateMode updateMode,
			Object tag, ISyncSetMediaClockTimerResponseListener listener) 
			throws SyncException {

		Integer correlationID = getNextAlmCorrelationId();

		// Add listener if listener is given
		if (listener != null) {
			addIProxySetMediaClockTimerResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base setMediaClockTimer
		syncProxy.setMediaClockTimer(hours, minutes, seconds, updateMode, correlationID);
	}
	
	/**
	 * Pauses the media clock. Responses are captured through callback on IProxyListener.
	 * 
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void pauseMediaClockTimer(Object tag, ISyncSetMediaClockTimerResponseListener listener) 
			throws SyncException {
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxySetMediaClockTimerResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base pauseMediaClockTimer
		syncProxy.pauseMediaClockTimer(correlationID);
	}
	
	/**
	 * Resumes the media clock. Responses are captured through callback on IProxyListener.
	 * 
	 * @param listener
	 * @throws SyncException
	 */
	public void resumeMediaClockTimer(Object tag, ISyncSetMediaClockTimerResponseListener listener) 
			throws SyncException {

		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxySetMediaClockTimerResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base pauseMediaClockTimer
		syncProxy.resumeMediaClockTimer(correlationID);
	}
	
	/**
	 * Clears the media clock. Responses are captured through the ISyncShowResponseListener
	 * 
	 * @param listener
	 * @throws SyncException
	 */
	public void clearMediaClockTimer(Object tag, ISyncShowResponseListener listener) throws SyncException{
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxyShowResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		syncProxy.clearMediaClockTimer(correlationID);
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
	 * @throws SyncException
	 */
	public void show(String mainText1, String mainText2,
			String statusBar, String mediaClock, String mediaTrack,
			TextAlignment alignment, Object tag, 
			ISyncShowResponseListener listener) 
			throws SyncException {
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxyShowResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}

		// Base show
		syncProxy.show(mainText1, mainText2, statusBar, mediaClock, mediaTrack, alignment, correlationID);
	}
	
	/**
	 * Sends a Show RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param mainText1
	 * @param mainText2
	 * @param alignment
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void show(String mainText1, String mainText2,
			TextAlignment alignment, Object tag,
			ISyncShowResponseListener listener) 
			throws SyncException {
		
		show(mainText1, mainText2, null, null, null, alignment, tag, listener);
	}
	
	/**
	 * Sends a Speak RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param ttsText
	 * @throws SyncException
	 */
	public void speak(String ttsText, Object tag, 
			ISyncSpeakResponseListener listener) 
			throws SyncException {
		
		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxySpeakResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}

		// Base speak
		syncProxy.speak(ttsText, correlationID);
	}
	
	/**
	 * Sends a Speak RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param ttsChunks
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void speak(Vector<TTSChunk> ttsChunks, Object tag,
			ISyncSpeakResponseListener listener) throws SyncException {

		Integer correlationID = getNextAlmCorrelationId();
		
		// Add listener if listener is given
		if (listener != null) {
			addIProxySpeakResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base speak
		syncProxy.speak(ttsChunks, correlationID);
	}
	
	public void longSpeak(String ttsText, Object tag, ISyncSpeakResponseListener listener) throws SyncException{
		
		SpeakTextParser speakParser = new SpeakTextParser(ttsText, tag, listener);
		speakParser.speakFromQueue();
	}
	
	// TODO create a longSpeak using TTSChunks
	
	/**
	 * Sends a SubscribeButton RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param buttonName
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void subscribeButton(ButtonName buttonName, Object tag,
			ISyncButtonListener listener) throws SyncException {

		Integer correlationID = getNextAlmCorrelationId();
		
		// Listener required for addCommand
		if (listener == null) {
			throw new SyncException("IProxySyncButtonListener cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		addIProxySyncButtonListener(listener, correlationID, buttonName);
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}

		// Base subscribeButton
		syncProxy.subscribeButton(buttonName, correlationID);
	}
	
	/**
	 * Sends an UnsubscribeButton RPCRequest to SYNC. Responses are captured through callback on IProxyListener.
	 * 
	 * @param buttonName
	 * @param _correlationID
	 * @throws SyncException
	 */
	public void unsubscribeButton(ButtonName buttonName, Object tag,
			ISyncUnsubscribeButtonResponseListener listener) 
			throws SyncException {
		
		Integer correlationID = getNextAlmCorrelationId();

		removeAddIProxySyncButtonListener(buttonName);

		// Add listener if listener is given
		if (listener != null) {
			addIProxyUnsubscribeButtonResponseListener(listener, correlationID);
		}
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByCorrelationID(tag, correlationID);
		}
		
		// Base unsubscribeButton
		syncProxy.unsubscribeButton(buttonName, correlationID);
	}
	
	/**
	 * Subscribes to notifications concerning the TBTClient State
	 * 
	 * @param listener
	 * @throws SyncException 
	 */
	public void subscribeToTBTClientState(ISyncTBTClientStateListener listener) throws SyncException{
		
		if (listener == null) {
			throw new SyncException("ISyncTBTClientStateListener cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		addIProxyTBTClientStateListener(listener);
	}
	
	/**
	 * Unsubscribes from notifications concerning the TBTClient State
	 * 
	 * @param listener
	 * @throws SyncException
	 */
	public void unsubscribeFromTBTClientState(ISyncTBTClientStateListener listener) throws SyncException{
		
		if (listener == null) {
			throw new SyncException("ISyncTBTClientStateListener cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		removeIProxyTBTClientStateListener(listener);
	}
	
	/**
	 * Subscribes to notifications concerning the DriverDistraction State
	 * 
	 * @param listener
	 * @throws SyncException
	 */
	public void subscribeToDriverDistractionState(ISyncDriverDistractionListener listener) throws SyncException{
		
		if (listener == null) {
			throw new SyncException("ISyncDriverDistractionListener cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		addIProxyDriverDistractionListener(listener);
	}
	
	/**
	 * Unsubscribes from notifications concerning the DriverDistraction State
	 * 
	 * @param listener
	 * @throws SyncException
	 */
	public void unsubscribeFromDriverDistractionState(ISyncDriverDistractionListener listener) throws SyncException{
		
		if (listener == null) {
			throw new SyncException("ISyncDriverDistractionListener cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		removeIProxyDriverDistractionListener(listener);
	}
	
	/**
	 * Subscribes to notifications concerning EncodedSyncPData
	 * 
	 * @param listener
	 * @throws SyncException
	 */
	public void subscribeToEncodedSyncPData(ISyncEncodedSyncPDataListener listener) throws SyncException{
		
		if (listener == null) {
			throw new SyncException("ISyncEncodedSyncPDataListener cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		addIProxyEncodedSyncPDataListener(listener);
	}
	
	/**
	 * Unsubscribes from notifications concerning EncodedSyncPData
	 * 
	 * @param listener
	 * @throws SyncException
	 */
	public void unsubscribeFromEncodedSyncPData(ISyncEncodedSyncPDataListener listener) throws SyncException{
		
		if (listener == null) {
			throw new SyncException("ISyncEncodedSyncPDataListener cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		removeIProxyEncodedSyncPDataListener(listener);
	}
	
	/**
	 * Creates a choice to be added to a choiceset. Choice has both a voice and a visual menu component.
	 * 
	 * @param choiceMenuName -Text name displayed for this choice.
	 * @param choiceVrCommands -Vector of vrCommands used to select this choice by voice. Must contain
	 * 			at least one non-empty element.
	 * @return Choice created. 
	 * @throws SyncException 
	 */
	public SyncChoice createChoiceSetChoice(String choiceMenuName,
			Vector<String> choiceVrCommands, Object tag, 
			ISyncChoiceListener listener) throws SyncException
	{
		Integer choiceID = getNextInteractionChoiceID();
		
		// Listener required for addCommand
		if (listener == null) {
			throw new SyncException("ISyncChoiceListener cannot be null.", SyncExceptionCause.INVALID_ARGUMENT);
		}
		
		SyncChoice syncChoice = new SyncChoice(syncProxy.createChoiceSetChoice(choiceID, choiceMenuName, choiceVrCommands));
		
		addIProxySyncInteractionChoiceListener(listener, choiceID);
		
		// Add SyncChoice
		addSyncChoice(syncChoice);
		
		// Add tag, if given
		if (tag != null) {
			addGenericObjectTagByChoiceID(tag, choiceID);
		}
		
		// Base createChoiceSetChoice
		return syncChoice;
	}
	/********************* End Public Helper Methods ******************/
	
	
	
	
	
	/************************ Callbacks ************************/
	// A class which handles all IProxyListenerALM callbacks for the manager
	private class ALMInterfaceBroker implements IProxyListenerALM{
		
		@Override
		public void onAddSubMenuResponse(AddSubMenuResponse response) {
			final ISyncAddSubMenuResponseListener listener = _addSubMenuResponseListeners.get(response.getCorrelationID());
			SyncSubMenu syncSubMenuToReturn = null;
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}

			// If adding command was successful, find the command that was added
			if (response.getSuccess()) {
				syncSubMenuToReturn = _syncSubMenusByCorrelationID.get(response.getCorrelationID());
				removeSyncSubMenuByCorrelationID(response.getCorrelationID());
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onAddSubMenuResponse(response, syncSubMenuToReturn, tagToReturn);
			
			removeIProxyAddSubMenuResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onAlertResponse(AlertResponse response) {
			final ISyncAlertResponseListener listener = _alertResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onAlertResponse(response, tagToReturn);
			
			removeIProxyAlertResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onCreateInteractionChoiceSetResponse(
				CreateInteractionChoiceSetResponse response) {
			final ISyncCreateInteractionChoiceSetResponseListener listener = _createInteractionChoiceSetResponseListeners.get(response.getCorrelationID());
			SyncChoiceSet syncChoiceSetToReturn = null;
			Object tagToReturn = null;
			
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// If adding choiceset was successful, find the choiceset that was added
			if (response.getSuccess()) {
				syncChoiceSetToReturn = _syncChoiceSetByCorrelationID.get(response.getCorrelationID());
				removeSyncChoiceSetByCorrelationID(response.getCorrelationID());
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onCreateInteractionChoiceSetResponse(response, syncChoiceSetToReturn, tagToReturn);
			
			removeIProxyCreateInteractionChoiceSetResponse(response.getCorrelationID());
		}
	
		@Override
		public void onDeleteCommandResponse(DeleteCommandResponse response) {
			final ISyncDeleteCommandResponseListener listener = _createDeleteCommandResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onDeleteCommandResponse(response, tagToReturn);
			
			removeIProxyDeleteCommandResponse(response.getCorrelationID());
		}
	
		@Override
		public void onDeleteInteractionChoiceSetResponse(
				DeleteInteractionChoiceSetResponse response) {
			final ISyncDeleteInteractionChoiceSetResponseListener listener = _deleteInteractionchoiceSetResponseListeners.get(response.getCorrelationID());		
			Object tagToReturn = null;
			
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onDeleteInteractionChoiceSetResponse(response, tagToReturn);
			
			removeIProxyDeleteInteractionChoiceSetResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onDeleteSubMenuResponse(DeleteSubMenuResponse response) {
			final ISyncDeleteSubMenuResponseListener listener = _deleteSubMenuResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onDeleteSubMenuResponse(response, tagToReturn);
			
			removeIProxyDeleteSubMenuResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onOnDriverDistraction(OnDriverDistraction notification) {
			for (Iterator<ISyncDriverDistractionListener> i = _driverDistractionListeners.iterator(); i.hasNext();) {
				final ISyncDriverDistractionListener listener = i.next();
				
				listener.onOnDriverDistraction(notification);
			}
		}
	
		@Override
		public void onOnEncodedSyncPData(OnEncodedSyncPData notification) {
			for (Iterator<ISyncEncodedSyncPDataListener> i = _encodedSyncPDataListeners.iterator(); i.hasNext();) {
				final ISyncEncodedSyncPDataListener listener = i.next();
				
				listener.onOnEncodedSyncPData(notification);
			}
		}
	
		@Override
		public void onEncodedSyncPDataResponse(EncodedSyncPDataResponse response) {
			final ISyncEncodedSyncPDataResponseListener listener = _encodedSyncPDataResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onEncodedSyncPDataResponse(response, tagToReturn);
			
			removeIProxyEncodedSyncPDataResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onError(String info, Exception e) {
			_lifecycleListener.onError(info, e);
		}
		
		@Override
		public void onPerformInteractionResponse(PerformInteractionResponse response) {
			final ISyncPerformInteractionResponseListener listener = _performInteractionResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Notify Choice listener that choice was chosen
			if (response.getChoiceID() != null) {
				final ISyncChoiceListener choiceListener = _interactionChoiceListeners.get(response.getChoiceID());
				final SyncChoice syncChoiceChosen = _syncChoicesByChoiceID.get(response.getChoiceID());
			
				// Return if listener is null
				if (choiceListener != null) {
					Object optionTagToReturn = null;
					optionTagToReturn = _genericTagsByChoiceID.get(response.getChoiceID());
					
					// Only preform callback if one had been added
					OnSyncChoiceChosen choiceChosenMsg = new OnSyncChoiceChosen();
					
					if (syncChoiceChosen != null) {
						choiceChosenMsg.setSyncChoice(syncChoiceChosen);
					}
					choiceChosenMsg.setTriggerSource(response.getTriggerSource());
					
					choiceListener.onSyncChoiceChosen(choiceChosenMsg, optionTagToReturn);
				}
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onPerformInteractionResponse(response, tagToReturn);
			
			removeIProxyPerformInteractionResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onResetGlobalPropertiesResponse(
				ResetGlobalPropertiesResponse response) {
			final ISyncResetGlobalPropertiesListener listener = _resetGlobalPropertiesResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onResetGlobalPropertiesResponse(response, tagToReturn);
			
			removeIProxyResetGlobalPropertiesResponse(response.getCorrelationID());
		}
	
		@Override
		public void onSetGlobalPropertiesResponse(
				SetGlobalPropertiesResponse response) {
			final ISyncSetGlobalPropertiesResponseListener listener = _setGlobalPropertiesResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onSetGlobalPropertiesResponse(response, tagToReturn);
			
			removeIProxySetGlobalPropertiesResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onSetMediaClockTimerResponse(SetMediaClockTimerResponse response) {
			final ISyncSetMediaClockTimerResponseListener listener = _setMediaClockTimerResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onSetMediaClockTimerResponse(response, tagToReturn);
			
			removeIProxySetMediaClockTimerResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onShowResponse(ShowResponse response) {
			final ISyncShowResponseListener listener = _showResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onShowResponse(response, tagToReturn);
			
			removeIProxyShowResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onSpeakResponse(SpeakResponse response) {
			final ISyncSpeakResponseListener listener = _speakResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onSpeakResponse(response, tagToReturn);
			
			removeIProxySpeakResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onOnButtonEvent(OnButtonEvent notification) {
			final ISyncButtonListener listener = _buttonListeners.get(notification.getButtonName());
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			listener.onOnButtonEvent(notification);
		}
	
		@Override
		public void onOnButtonPress(OnButtonPress notification) {
			final ISyncButtonListener listener = _buttonListeners.get(notification.getButtonName());
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			listener.onOnButtonPress(notification);
		}
	
		@Override
		public void onSubscribeButtonResponse(SubscribeButtonResponse response) {
			final ISyncButtonListener listener = _buttonResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onSubscribeButtonResponse(response, tagToReturn);
			
			removeAddIProxySyncButtonResponseListener(response.getCorrelationID());		
		}
	
		@Override
		public void onOnCommand(OnCommand notification) {
			final ISyncCommandListener listener = _commandListeners.get(notification.getCmdID());
			final SyncCommand syncCommand = _syncCommandsByCommandID.get(notification.getCmdID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener != null) {
				// Set tag, null if none exists
				tagToReturn = _genericTagsByCommandID.get(notification.getCmdID());
				
				listener.onSyncCommand(notification, syncCommand, tagToReturn);
			}
		}
	
		@Override
		public void onAddCommandResponse(AddCommandResponse response) {
			final ISyncCommandListener listener = _commandResponseListeners.get(response.getCorrelationID());
			SyncCommand syncCommandToReturn = null;
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// If adding command was successful, find the command that was added
			if (response.getSuccess()) {
				syncCommandToReturn = _syncCommandsByCorrelationID.get(response.getCorrelationID());
				removeSyncCommandByCorrelationID(response.getCorrelationID());
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onAddCommandResponse(response, syncCommandToReturn, tagToReturn);
			
			removeAddCommandResponseListener(response.getCorrelationID());
		}
	
		@Override
		public void onOnTBTClientState(OnTBTClientState notification) {
			for (Iterator<ISyncTBTClientStateListener> i = _tbtClientStateListeners.iterator(); i.hasNext();) {
				final ISyncTBTClientStateListener listener = i.next();
				
				listener.onOnTBTClientState(notification);
			}
		}
	
		@Override
		public void onUnsubscribeButtonResponse(UnsubscribeButtonResponse response) {
			final ISyncUnsubscribeButtonResponseListener listener = _unsubscribeButtonResponseListeners.get(response.getCorrelationID());
			Object tagToReturn = null;
			
			// Return if listener is null
			if (listener == null) {
				return;
			}
			
			// Set tag, null if none exists
			tagToReturn = _genericTagsByCorrelationID.get(response.getCorrelationID());
			// Remove any tag tied to this correlationID
			removeGenericObjectTagByCorrelationID(response.getCorrelationID());
			
			listener.onUnsubscribeButtonResponse(response, tagToReturn);
			
			removeIProxyUnsubscribeButtonResponseListener(response.getCorrelationID());
		}
		
		@Override
		public void onGenericResponse(GenericResponse response) {
			_lifecycleListener.onGenericResponse(response);
		}
		
		@Override
		public void onOnPermissionsChange(OnPermissionsChange notification) {
			_lifecycleListener.onOnPermissionsChange(notification);
		}

		@Override
		public void onOnHMIStatus(OnHMIStatus notification) {
			_lifecycleListener.onOnHMIStatus(notification);
		}

		@Override
		public void onProxyClosed(String info, Exception e) {
			// Remove all manager copies of resources corresponding to copies no longer 
				// present in SYNC		
			_syncCommandsByCommandID.clear();
			_syncChoicesByChoiceID.clear();
			_syncChoicesBySyncChoiceSetID.clear();
	
			// Notify the listener that SYNC has been disconnected
			_lifecycleListener.onProxyClosed(info, e);
		}

		@Override
		public void onSubscribeVehicleDataResponse(SubscribeVehicleData response) {
			_lifecycleListener.onSubscribeVehicleDataResponse(response);
		}

		@Override
		public void onUnsubscribeVehicleDataResponse(UnsubscribeVehicleData response) {
			_lifecycleListener.onUnsubscribeVehicleDataResponse(response);
		}

		@Override
		public void onGetVehicleDataResponse(GetVehicleData response) {
			_lifecycleListener.onGetVehicleDataResponse(response);
		}

		@Override
		public void onReadDIDResponse(ReadDID response) {
			_lifecycleListener.onReadDIDResponse(response);
		}

		@Override
		public void onGetDTCsResponse(GetDTCs response) {
			_lifecycleListener.onGetDTCsResponse(response);
		}

		@Override
		public void onOnVehicleData(OnVehicleData notification) {
			_lifecycleListener.onOnVehicleData(notification);
		}
		
		@Override
		public void onPerformAudioPassThruResponse(PerformAudioPassThruResponse response) {
			_lifecycleListener.onPerformAudioPassThruResponse(response);
		}

		@Override
		public void onEndAudioPassThruResponse(EndAudioPassThruResponse response) {
			_lifecycleListener.onEndAudioPassThruResponse(response);
		}

		@Override
		public void onOnAudioPassThru(OnAudioPassThru notification) {
			_lifecycleListener.onOnAudioPassThru(notification);
		}

		@Override
		public void onPutFileResponse(PutFileResponse response) {
			_lifecycleListener.onPutFileResponse(response);
		}

		@Override
		public void onDeleteFileResponse(DeleteFileResponse response) {
			_lifecycleListener.onDeleteFileResponse(response);
		}

		@Override
		public void onListFilesResponse(ListFilesResponse response) {
			_lifecycleListener.onListFilesResponse(response);
		}

		@Override
		public void onSetAppIconResponse(SetAppIconResponse response) {
			_lifecycleListener.onSetAppIconResponse(response);
		}

		@Override
		public void onScrollableMessageResponse(ScrollableMessageResponse response) {
			_lifecycleListener.onScrollableMessageResponse(response);
		}

		@Override
		public void onChangeRegistrationResponse(ChangeRegistrationResponse response) {
			_lifecycleListener.onChangeRegistrationResponse(response);
		}

		@Override
		public void onSetDisplayLayoutResponse(SetDisplayLayoutResponse response) {
			_lifecycleListener.onSetDisplayLayoutResponse(response);
		}

		@Override
		public void onOnLanguageChange(OnLanguageChange notification) {
			_lifecycleListener.onOnLanguageChange(notification);
		}
		public void onSliderResponse(SliderResponse response) {
			_lifecycleListener.onSliderResponse(response);
		}
	}
}
