package com.ford.syncV4.proxy;

import java.util.Vector;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.ButtonCapabilities;
import com.ford.syncV4.proxy.rpc.DisplayCapabilities;
import com.ford.syncV4.proxy.rpc.PresetBankCapabilities;
import com.ford.syncV4.proxy.rpc.SoftButtonCapabilities;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.VehicleType;
import com.ford.syncV4.proxy.rpc.enums.AppType;
import com.ford.syncV4.proxy.rpc.enums.HmiZoneCapabilities;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.SyncDisconnectedReason;
import com.ford.syncV4.proxy.rpc.enums.VrCapabilities;
import com.ford.syncV4.trace.SyncTrace;
import com.ford.syncV4.transport.BTTransportConfig;
import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;

public class SyncProxyALM extends SyncProxyBase<IProxyListenerALM> {
	
	private static final String SYNC_LIB_TRACE_KEY = "42baba60-eb57-11df-98cf-0800200c9a66";
	private static final String SYNC_LIB_PRIVATE_TOKEN = "{DAE1A88C-6C16-4768-ACA5-6F1247EA01C2}";
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC
	 * 
	 * Takes advantage of the advanced lifecycle management.
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC. 
	 * @param appName - Name of the application displayed on SYNC. 
	 * @param isMediaApp - Indicates if the app is a media application.
	 */
	public SyncProxyALM(IProxyListenerALM listener, String appName, Boolean isMediaApp, 
			Language languageDesired, Language hmiDisplayLanguageDesired, String appID) throws SyncException {
		super(	listener, 
				/*sync proxy configuration resources*/null, 
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				/*ngn media app*/null,
				/*vr synonyms*/null,
				/*is media app*/isMediaApp,
				/*syncMsgVersion*/null,
				/*language desired*/languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/null,
				/*App ID*/appID,
				/*autoActivateID*/null,
				/*callbackToUIThread*/ false,
				new BTTransportConfig());
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using legacy constructor for BT transport) instance passing in: IProxyListener, appName, and isMediaApp.", SYNC_LIB_TRACE_KEY);
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
	 * @throws SyncException
	 */
	public SyncProxyALM(IProxyListenerALM listener, String appName, String ngnMediaScreenAppName, 
			Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion, 
			Language languageDesired, Language hmiDisplayLanguageDesired, String appID, 
			String autoActivateID) throws SyncException {
		super(	listener, 
				/*sync proxy configuration resources*/null, 
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp,
				syncMsgVersion,
				languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/null,
				/*App ID*/appID,
				autoActivateID,
				/*callbackToUIThread*/ false,
				new BTTransportConfig());
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using legacy constructor for BT transport) instance passing in: IProxyListener, appName, ngnMediaScreenAppName, " +
				"vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, and autoActivateID.", SYNC_LIB_TRACE_KEY);
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
	public SyncProxyALM(IProxyListenerALM listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			String appName, String ngnMediaScreenAppName, Vector<String> vrSynonyms, 
			Boolean isMediaApp, SyncMsgVersion syncMsgVersion, Language languageDesired, 
			Language hmiDisplayLanguageDesired, String appID, String autoActivateID) throws SyncException {
		super(	listener, 
				syncProxyConfigurationResources, 
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp,
				syncMsgVersion,
				languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/null,
				/*App ID*/appID,
				autoActivateID,
				/*callbackToUIThread*/ false,
				new BTTransportConfig());
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using legacy constructor for BT transport) instance passing in: IProxyListener, syncProxyConfigurationResources, " +
				"appName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, and autoActivateID.", SYNC_LIB_TRACE_KEY);
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
	public SyncProxyALM(IProxyListenerALM listener, String appName, String ngnMediaScreenAppName, 
			Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion, 
			Language languageDesired, Language hmiDisplayLanguageDesired, String appID, 
			String autoActivateID, boolean callbackToUIThread) throws SyncException {
		super(	listener, 
				/*sync proxy configuration resources*/null,
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp,
				syncMsgVersion,
				languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/null,
				/*App ID*/appID,
				autoActivateID,
				callbackToUIThread,
				new BTTransportConfig());
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using legacy constructor for BT transport) instance passing in: IProxyListener, " +
				"appName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, autoActivateID, " +
				"and callbackToUIThread", SYNC_LIB_TRACE_KEY);
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
	public SyncProxyALM(IProxyListenerALM listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			String appName, String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, 
			SyncMsgVersion syncMsgVersion, Language languageDesired, Language hmiDisplayLanguageDesired, 
			String appID, String autoActivateID, 
			boolean callbackToUIThread) throws SyncException {
		super(	listener, 
				syncProxyConfigurationResources,
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp,
				syncMsgVersion,
				languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/null,
				/*App ID*/appID,
				autoActivateID,
				callbackToUIThread,
				new BTTransportConfig());
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using legacy constructor for BT transport) instance passing in: IProxyListener, syncProxyConfigurationResources, " +
				"appName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, autoActivateID, " +
				"and callbackToUIThread", SYNC_LIB_TRACE_KEY);
	}
	
	public SyncProxyALM(IProxyListenerALM listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			String appName, String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, Vector<AppType> appType,
			SyncMsgVersion syncMsgVersion, Language languageDesired, Language hmiDisplayLanguageDesired, 
			String appID, String autoActivateID, boolean callbackToUIThread, boolean preRegister, int version) throws SyncException {
		super(	listener, 
				syncProxyConfigurationResources,
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp,
				syncMsgVersion,
				languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/appType,
				/*App ID*/appID,
				autoActivateID,
				callbackToUIThread,
				preRegister,
				version,
				new BTTransportConfig());
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using legacy constructor for BT transport) instance passing in: IProxyListener, syncProxyConfigurationResources, " +
				"appName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, autoActivateID, " +
				"callbackToUIThread and version", SYNC_LIB_TRACE_KEY);
	}
	
	/********************************************** TRANSPORT SWITCHING SUPPORT *****************************************/

	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC via specified transport.
	 * 
	 * Takes advantage of the advanced lifecycle management.
	 * 
	 * @param listener Reference to the object in the App listening to callbacks from SYNC. 
	 * @param appName Name of the application displayed on SYNC. 
	 * @param isMediaApp Indicates if the app is a media application.
	 * @param transportConfig Initial configuration for transport.
	 * @throws SyncException
	 */
	public SyncProxyALM(IProxyListenerALM listener, String appName, Boolean isMediaApp, 
			Language languageDesired, Language hmiDisplayLanguageDesired, String appID,
			BaseTransportConfig transportConfig) throws SyncException {
		super(	listener, 
				/*sync proxy configuration resources*/null, 
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				/*ngn media app*/null,
				/*vr synonyms*/null,
				/*is media app*/isMediaApp,
				/*syncMsgVersion*/null,
				/*language desired*/languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/null,
				/*App ID*/appID,
				/*autoActivateID*/null,
				/*callbackToUIThread*/ false,
				transportConfig);
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using new constructor with specified transport) instance passing in: IProxyListener, appName, and isMediaApp.", SYNC_LIB_TRACE_KEY);
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC via specified transport.
	 * 
	 * Takes advantage of the advanced lifecycle management. 
	 * 
	 * @param listener Reference to the object in the App listening to callbacks from SYNC. 
	 * @param appName Name of the application displayed on SYNC. 
	 * @param ngnMediaScreenAppName Name of the application displayed on SYNC for Navigation equipped 
	 * vehicles. Limited to five characters. 
	 * @param vrSynonyms A vector of strings, all of which can be used as voice commands to 
	 * @param isMediaApp Indicates if the app is a media application.
	 * @param syncMsgVersion Indicates the version of SYNC AppLink Messages desired. Must be less than
	 * or equal to the version of SYNC AppLink running on the vehicle. 
	 * @param languageDesired Indicates the language desired for the SYNC interface.
	 * @param autoActivateID ID used to re-register previously registered application.
	 * @param transportConfig Initial configuration for transport. 
	 * @throws SyncException
	 */
	public SyncProxyALM(IProxyListenerALM listener, String appName, String ngnMediaScreenAppName, 
			Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion, 
			Language languageDesired, Language hmiDisplayLanguageDesired, String appID, 
			String autoActivateID, TransportType transportType, BaseTransportConfig transportConfig) throws SyncException {
		super(	listener, 
				/*sync proxy configuration resources*/null, 
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp,
				syncMsgVersion,
				languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/null,
				/*App ID*/appID,
				autoActivateID,
				/*callbackToUIThread*/ false,
				transportConfig);
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using new constructor with specified transport) instance passing in: IProxyListener, appName, ngnMediaScreenAppName, " +
				"vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, and autoActivateID.", SYNC_LIB_TRACE_KEY);
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC via specified transport.
	 * 
	 * Takes advantage of the advanced lifecycle management. 
	 * 
	 * @param listener Reference to the object in the App listening to callbacks from SYNC. 
	 * @param applicationContext Context of the application. Used to access application specific resources.
	 * @param appName Name of the application displayed on SYNC. 
	 * @param ngnMediaScreenAppName Name of the application displayed on SYNC for Navigation equipped 
	 * vehicles. Limited to five characters. 
	 * @param vrSynonyms A vector of strings, all of which can be used as voice commands to 
	 * @param isMediaApp Indicates if the app is a media application.
	 * @param syncMsgVersion Indicates the version of SYNC AppLink Messages desired. Must be less than
	 * or equal to the version of SYNC AppLink running on the vehicle. 
	 * @param languageDesired Indicates the language desired for the SYNC interface.
	 * @param autoActivateID ID used to re-register previously registered application.
	 * @param transportConfig Initial configuration for transport. 
	 * @throws SyncException
	 */
	public SyncProxyALM(IProxyListenerALM listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			String appName, String ngnMediaScreenAppName, Vector<String> vrSynonyms, 
			Boolean isMediaApp, SyncMsgVersion syncMsgVersion, Language languageDesired, 
			Language hmiDisplayLanguageDesired, String appID, String autoActivateID,
			BaseTransportConfig transportConfig) throws SyncException {
		super(	listener, 
				syncProxyConfigurationResources, 
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp,
				syncMsgVersion,
				languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/null,
				/*App ID*/appID,
				autoActivateID,
				/*callbackToUIThread*/ false,
				transportConfig);
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using new constructor with specified transport) instance passing in: IProxyListener, syncProxyConfigurationResources, " +
				"appName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, and autoActivateID.", SYNC_LIB_TRACE_KEY);
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC via specified transport.
	 * 
	 * Takes advantage of the advanced lifecycle management. 
	 * 
	 * @param listener Reference to the object in the App listening to callbacks from SYNC. 
	 * @param appName Name of the application displayed on SYNC. 
	 * @param ngnMediaScreenAppName Name of the application displayed on SYNC for Navigation equipped 
	 * vehicles. Limited to five characters. 
	 * @param vrSynonyms A vector of strings, all of which can be used as voice commands to 
	 * @param isMediaApp Indicates if the app is a media application.
	 * @param syncMsgVersion Indicates the version of SYNC AppLink Messages desired. Must be less than
	 * or equal to the version of SYNC AppLink running on the vehicle. 
	 * @param languageDesired Indicates the language desired for the SYNC interface.
	 * @param autoActivateID ID used to re-register previously registered application.
	 * @param callbackToUIThread If true, all callbacks will occur on the UI thread.
	 * @param transportConfig Initial configuration for transport. 
	 * @throws SyncException
	 */
	public SyncProxyALM(IProxyListenerALM listener, String appName, String ngnMediaScreenAppName, 
			Vector<String> vrSynonyms, Boolean isMediaApp, SyncMsgVersion syncMsgVersion, 
			Language languageDesired, Language hmiDisplayLanguageDesired, String appID, 
			String autoActivateID, boolean callbackToUIThread, 
			BaseTransportConfig transportConfig) throws SyncException {
		super(	listener, 
				/*sync proxy configuration resources*/null,
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp,
				syncMsgVersion,
				languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/null,
				/*App ID*/appID,
				autoActivateID,
				callbackToUIThread,
				transportConfig);
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using new constructor with specified transport) instance passing in: IProxyListener, " +
				"appName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, autoActivateID, " +
				"and callbackToUIThread", SYNC_LIB_TRACE_KEY);
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC via specified transport.
	 * 
	 * Takes advantage of the advanced lifecycle management. 
	 * 
	 * @param listener Reference to the object in the App listening to callbacks from SYNC. 
	 * @param applicationContext Context of the application. Used to access application specific resources.
	 * @param appName Name of the application displayed on SYNC. 
	 * @param ngnMediaScreenAppName Name of the application displayed on SYNC for Navigation equipped 
	 * vehicles. Limited to five characters.
	 * @param vrSynonyms A vector of strings, all of which can be used as voice commands too
	 * @param isMediaApp Indicates if the app is a media application.
	 * @param syncMsgVersion Indicates the version of SYNC AppLink Messages desired. Must be less than
	 * or equal to the version of SYNC AppLink running on the vehicle. 
	 * @param languageDesired Indicates the language desired for the SYNC interface.
	 * @param autoActivateID ID used to re-register previously registered application.
	 * @param callbackToUIThread If true, all callbacks will occur on the UI thread.
	 * @param transportConfig Initial configuration for transport. 
	 * @throws SyncException
	 */
	public SyncProxyALM(IProxyListenerALM listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			String appName, String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, 
			SyncMsgVersion syncMsgVersion, Language languageDesired, Language hmiDisplayLanguageDesired, 
			String appID, String autoActivateID, 
			boolean callbackToUIThread, BaseTransportConfig transportConfig) throws SyncException {
		super(	listener, 
				syncProxyConfigurationResources,
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp,
				syncMsgVersion,
				languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/null,
				/*App ID*/appID,
				autoActivateID,
				callbackToUIThread,
				transportConfig);
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using new constructor with specified transport) instance passing in: IProxyListener, syncProxyConfigurationResources, " +
				"appName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, autoActivateID, " +
				"and callbackToUIThread", SYNC_LIB_TRACE_KEY);
	}

	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC via specified transport.
	 * 
	 * Takes advantage of the advanced lifecycle management. 
	 * 
	 * @param listener Reference to the object in the App listening to callbacks from SYNC.
	 * @param syncProxyConfigurationResources Proxy configuration resources.
	 * @param appName Name of the application displayed on SYNC.
	 * @param ngnMediaScreenAppName Name of the application displayed on SYNC for Navigation equipped 
	 * vehicles. Limited to five characters.
	 * @param vrSynonyms A vector of strings, all of which can be used as voice commands too
	 * @param isMediaApp Indicates if the app is a media application.
	 * @param syncMsgVersion Indicates the version of SYNC AppLink Messages desired. Must be less than
	 * or equal to the version of SYNC AppLink running on the vehicle.
	 * @param languageDesired Indicates the language desired for the SYNC interface.
	 * @param hmiDisplayLanguageDesired Desired language in HMI.
	 * @param appID Identifier of the client application.
	 * @param autoActivateID ID used to re-register previously registered application.
	 * @param callbackToUIThread If true, all callbacks will occur on the UI thread.
	 * @param preRegister Flag that indicates that client should be pre-registred or not
	 * @param version Desired version of SYNC protocol.
	 * @param transportConfig Initial configuration for transport. 
	 * @throws SyncException
	 */
	public SyncProxyALM(IProxyListenerALM listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			String appName, String ngnMediaScreenAppName, Vector<String> vrSynonyms, Boolean isMediaApp, Vector<AppType> appType ,
			SyncMsgVersion syncMsgVersion, Language languageDesired, Language hmiDisplayLanguageDesired, 
			String appID, String autoActivateID, boolean callbackToUIThread, boolean preRegister, int version,
			BaseTransportConfig transportConfig) throws SyncException {
		super(	listener, 
				syncProxyConfigurationResources,
				/*enable advanced lifecycle management*/true, 
				appName,
				/*TTS Name*/null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp,
				syncMsgVersion,
				languageDesired,
				/*HMI Display Language Desired*/hmiDisplayLanguageDesired,
				/*App Type*/appType,
				/*App ID*/appID,
				autoActivateID,
				callbackToUIThread,
				preRegister,
				version,
				transportConfig);
		
		SyncTrace.logProxyEvent("Application constructed SyncProxyALM (using new constructor with specified transport) instance passing in: IProxyListener, syncProxyConfigurationResources, " +
				"appName, ngnMediaScreenAppName, vrSynonyms, isMediaApp, syncMsgVersion, languageDesired, autoActivateID, " +
				"callbackToUIThread and version", SYNC_LIB_TRACE_KEY);
	}

    /***************************************** END OF TRANSPORT SWITCHING SUPPORT ***************************************/
	
	// Allow applications using ALM to reset the proxy (dispose and reinstantiate)
	/**
	 * Disconnects the application from SYNC, then recreates the transport such that
	 * the next time a SYNC unit discovers applications, this application will be
	 * available.
	 */
	public void resetProxy() throws SyncException {
		super.cycleProxy(SyncDisconnectedReason.APPLICATION_REQUESTED_DISCONNECT);
	}
	
	/********* Getters for values returned by RegisterAppInterfaceResponse **********/
	
	/**
	 * Gets buttonCapabilities set when application interface is registered.
	 * 
	 * @return buttonCapabilities
	 * @throws SyncException
	 */
	public Vector<ButtonCapabilities> getButtonCapabilities() throws SyncException{
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is unavailable. Unable to get the buttonCapabilities.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		return _buttonCapabilities;
	}
	
	/**
	 * Gets getSoftButtonCapabilities set when application interface is registered.
	 * 
	 * @return softButtonCapabilities 
	 * @throws SyncException
	 */
	public Vector<SoftButtonCapabilities> getSoftButtonCapabilities() throws SyncException {
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is not connected. Unable to get the softButtonCapabilities.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		return _softButtonCapabilities;
	}
	
	/**
	 * Gets getPresetBankCapabilities set when application interface is registered.
	 * 
	 * @return presetBankCapabilities 
	 * @throws SyncException
	 */
	public PresetBankCapabilities getPresetBankCapabilities() throws SyncException {
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is not connected. Unable to get the presetBankCapabilities.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		return _presetBankCapabilities;
	}
	
	/**
	 * Gets displayCapabilities set when application interface is registered.
	 * 
	 * @return displayCapabilities
	 * @throws SyncException
	 */
	public DisplayCapabilities getDisplayCapabilities() throws SyncException {
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is unavailable. Unable to get the displayCapabilities.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		return _displayCapabilities;
	}
	
	/**
	 * Gets hmiZoneCapabilities set when application interface is registered.
	 * 
	 * @return hmiZoneCapabilities
	 * @throws SyncException
	 */
	public Vector<HmiZoneCapabilities> getHmiZoneCapabilities() throws SyncException {
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is unavailable. Unable to get the hmiZoneCapabilities.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		return _hmiZoneCapabilities;
	}
	
	/**
	 * Gets speechCapabilities set when application interface is registered.
	 * 
	 * @return speechCapabilities
	 * @throws SyncException
	 */
	public Vector<SpeechCapabilities> getSpeechCapabilities() throws SyncException {
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is unavailable. Unable to get the speechCapabilities.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		
		return _speechCapabilities;
	}
	
	/**
	 * Gets syncLanguage set when application interface is registered.
	 * 
	 * @return syncLanguage
	 * @throws SyncException
	 */
	public Language getSyncLanguage() throws SyncException {
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is unavailable. Unable to get the syncLanguage.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		return _syncLanguage;
	}
	
	/**
	 * Gets getHmiDisplayLanguage set when application interface is registered.
	 * 
	 * @return hmiDisplayLanguage 
	 * @throws SyncException
	 */
	public Language getHmiDisplayLanguage() throws SyncException {
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is not connected. Unable to get the hmiDisplayLanguage.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		return _hmiDisplayLanguage;
	}
	
	/**
	 * Gets syncMsgVersion set when application interface is registered.
	 * 
	 * @return syncMsgVersion
	 * @throws SyncException
	 */
	public SyncMsgVersion getSyncMsgVersion() throws SyncException {
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is unavailable. Unable to get the syncMsgVersion.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		return _syncMsgVersion;
	}
	
	/**
	 * Gets vrCapabilities set when application interface is registered.
	 * 
	 * @return vrCapabilities
	 * @throws SyncException
	 */
	public Vector<VrCapabilities> getVrCapabilities() throws SyncException {
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is unavailable. Unable to get the vrCapabilities.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		return _vrCapabilities;
	}
	
	/**
	 * Gets getVehicleType set when application interface is registered.
	 * 
	 * @return vehicleType 
	 * @throws SyncException
	 */
	public VehicleType getVehicleType() throws SyncException {
		// Test if proxy has been disposed
		if (_proxyDisposed) {
			throw new SyncException("This object has been disposed, it is no long capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		// Test SYNC availability 
		if (!_appInterfaceRegisterd) {
			throw new SyncException("SYNC is not connected. Unable to get the vehicleType.", SyncExceptionCause.SYNC_UNAVAILALBE);
		}
		return _vehicleType;
	}
}
