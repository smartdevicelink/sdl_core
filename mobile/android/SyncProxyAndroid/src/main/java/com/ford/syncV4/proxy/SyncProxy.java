package com.ford.syncV4.proxy;

import java.util.Vector;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.transport.BTTransportConfig;
import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.test.TestConfig;
import com.ford.syncV4.util.logger.Logger;

public class SyncProxy extends SyncProxyBase<IProxyListener> {
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC. 
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC. 
	 * @throws SyncException
	 */
	public SyncProxy(IProxyListener listener) throws SyncException {
		super(	listener, 
				/*application context*/null, 
				/*enable advanced lifecycle management*/false, 
				/*app name*/ null,
				/*TTS Name*/null,
				/*ngn media screen app name*/null,
				/*vr synonyms*/null,
				/*is media app*/ null,
				/*syncMsgVersion*/null,
				/*language desired*/null,
				/*HMI Display Language Desired*/null,
				/*App Type*/null,
				/*App ID*/null,
				/*autoActivateID*/null,
				/*callbackToUIThread*/ true,
				new BTTransportConfig(), new TestConfig());
		
		Logger.i("Application constructed SyncProxy instance passing in: IProxyListener.");
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC. 
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC. 
	 * @throws SyncException
	 */
	public SyncProxy(IProxyListener listener, SyncProxyConfigurationResources syncProxyConfigurationResources) 
		throws SyncException {
		super(	listener, 
				syncProxyConfigurationResources, 
				/*enable advanced lifecycle management*/false, 
				/*app name*/ null,
				/*TTS Name*/null,
				/*ngn media screen app name*/null,
				/*vr synonyms*/null,
				/*is media app*/ null,
				/*syncMsgVersion*/null,
				/*language desired*/null,
				/*HMI Display Language Desired*/null,
				/*App Type*/null,
				/*App ID*/null,
				/*autoActivateID*/null,
				/*callbackToUIThread*/ true,
				new BTTransportConfig(), new TestConfig());
		
		Logger.i("Application constructed SyncProxy instance passing in: IProxyListener, SyncProxyConfigurationResources.");
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC.
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC. 
	 * @param callbackToUIThread - If true, all callbacks will occur on the UI thread.
	 * @throws SyncException
	 */
	public SyncProxy(IProxyListener listener, boolean callbackToUIThread) throws SyncException {
		super(	listener,  
				/*sync proxy configuration resources*/null,
				/*enable advanced lifecycle management*/false, 
				/*app name*/ null,
				/*TTS Name*/null,
				/*ngn media screen app name*/null,
				/*vr synonyms*/null,
				/*is media app*/ null,
				/*syncMsgVersion*/null,
				/*language desired*/null,
				/*HMI Display Language Desired*/null,
				/*App Type*/null,
				/*App ID*/null,
				/*autoActivateID*/null,
				callbackToUIThread,
				new BTTransportConfig(), new TestConfig());
		
		Logger.i("Application constructed SyncProxy instance passing in: IProxyListener, callBackToUIThread.");
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC.
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC.
	 * @param callbackToUIThread - If true, all callbacks will occur on the UI thread.
	 * @throws SyncException
	 */
	public SyncProxy(IProxyListener listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			boolean callbackToUIThread) throws SyncException {
		super(	listener,  
				syncProxyConfigurationResources,
				/*enable advanced lifecycle management*/false, 
				/*app name*/ null,
				/*TTS Name*/null,
				/*ngn media screen app name*/null,
				/*vr synonyms*/null,
				/*is media app*/ null,
				/*syncMsgVersion*/null,
				/*language desired*/null,
				/*HMI Display Language Desired*/null,
				/*App Type*/null,
				/*App ID*/null,
				/*autoActivateID*/null,
				callbackToUIThread,
				new BTTransportConfig(), new TestConfig());
		
		Logger.i("Application constructed SyncProxy instance passing in: IProxyListener, callBackToUIThread.");
	}
	
	/********************************************** TRANSPORT SWITCHING SUPPORT *****************************************/

	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC. 
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC.
	 * @param transportConfig Initial configuration for transport.
	 * @throws SyncException
	 */
	public SyncProxy(IProxyListener listener, BaseTransportConfig transportConfig) throws SyncException {
		super(	listener, 
				/*application context*/null, 
				/*enable advanced lifecycle management*/false, 
				/*app name*/ null,
				/*TTS Name*/null,
				/*ngn media screen app name*/null,
				/*vr synonyms*/null,
				/*is media app*/ null,
				/*syncMsgVersion*/null,
				/*language desired*/null,
				/*HMI Display Language Desired*/null,
				/*App Type*/null,
				/*App ID*/null,
				/*autoActivateID*/null,
				/*callbackToUIThread*/ true,
				transportConfig, new TestConfig());
		
		Logger.i("Application constructed SyncProxy instance passing in: IProxyListener.");
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC. 
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC. 
	 * @param transportConfig Initial configuration for transport.
	 * @throws SyncException
	 */
	public SyncProxy(IProxyListener listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
					BaseTransportConfig transportConfig) 
		throws SyncException {
		super(	listener, 
				syncProxyConfigurationResources, 
				/*enable advanced lifecycle management*/false, 
				/*app name*/ null,
				/*TTS Name*/null,
				/*ngn media screen app name*/null,
				/*vr synonyms*/null,
				/*is media app*/ null,
				/*syncMsgVersion*/null,
				/*language desired*/null,
				/*HMI Display Language Desired*/null,
				/*App Type*/null,
				/*App ID*/null,
				/*autoActivateID*/null,
				/*callbackToUIThread*/ true,
				transportConfig, new TestConfig());
		
		Logger.i("Application constructed SyncProxy instance passing in: IProxyListener, SyncProxyConfigurationResources.");
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC.
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC. 
	 * @param callbackToUIThread - If true, all callbacks will occur on the UI thread.
	 * @param transportConfig Initial configuration for transport.
	 * @throws SyncException
	 */
	public SyncProxy(IProxyListener listener, boolean callbackToUIThread, BaseTransportConfig transportConfig) throws SyncException {
		super(	listener,  
				/*sync proxy configuration resources*/null,
				/*enable advanced lifecycle management*/false, 
				/*app name*/ null,
				/*TTS Name*/null,
				/*ngn media screen app name*/null,
				/*vr synonyms*/null,
				/*is media app*/ null,
				/*syncMsgVersion*/null,
				/*language desired*/null,
				/*HMI Display Language Desired*/null,
				/*App Type*/null,
				/*App ID*/null,
				/*autoActivateID*/null,
				callbackToUIThread,
				transportConfig, new TestConfig());
		
		Logger.i("Application constructed SyncProxy instance passing in: IProxyListener, callBackToUIThread.");
	}
	
	/**
	 * Constructor for the SyncProxy object, the proxy for communicating between the App and SYNC.
	 * 
	 * @param listener - Reference to the object in the App listening to callbacks from SYNC.
	 * @param callbackToUIThread - If true, all callbacks will occur on the UI thread.
	 * @param transportConfig Initial configuration for transport.
	 * @throws SyncException
	 */
	public SyncProxy(IProxyListener listener, SyncProxyConfigurationResources syncProxyConfigurationResources, 
			boolean callbackToUIThread, BaseTransportConfig transportConfig) throws SyncException {
		super(	listener,  
				syncProxyConfigurationResources,
				/*enable advanced lifecycle management*/false, 
				/*app name*/ null,
				/*TTS Name*/null,
				/*ngn media screen app name*/null,
				/*vr synonyms*/null,
				/*is media app*/ null,
				/*syncMsgVersion*/null,
				/*language desired*/null,
				/*HMI Display Language Desired*/null,
				/*App Type*/null,
				/*App ID*/null,
				/*autoActivateID*/null,
				callbackToUIThread,
				transportConfig, new TestConfig());
		
		Logger.i("Application constructed SyncProxy instance passing in: IProxyListener, " +
                "callBackToUIThread.");
	}
		
	/******************** Public Helper Methods *************************/

	/**
	 *  Sends a RegisterAppInterface RPCRequest to SYNC. Responses are captured through callback
     *  on IProxyListener.
	 *  
	 *  @param syncMsgVersion
	 *  @param appName
	 *  @param ngnMediaScreenAppName
	 *  @param vrSynonyms
	 *  @param isMediaApp
	 *  @param languageDesired
	 *  @param correlationID
	 *  
	 *  @throws SyncException
	 */
	public void registerAppInterface(
			SyncMsgVersion syncMsgVersion, String appName, String ngnMediaScreenAppName,
			Vector<Object> vrSynonyms, Boolean isMediaApp, Language languageDesired,
            Integer correlationID) throws SyncException {
		
		// Test if proxy has been disposed
		if (mIsProxyDisposed) {
			throw new SyncException("This SyncProxy object has been disposed, it is no long " +
                    "capable of sending requests.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}
		
		registerAppInterfacePrivate(
				syncMsgVersion, 
				appName,
				null,
				ngnMediaScreenAppName,
				vrSynonyms,
				isMediaApp, 
				languageDesired,
				null,
				null,
				null,
				correlationID,
                getHashId(appName), getDeviceInfo());
	}
	
	/**
	 * Sends a RegisterAppInterface RPCRequest to SYNC. Responses are captured through callback
     * on IProxyListener.
	 * 
	 * @param appName
	 * @param isMediaApp
	 * @throws SyncException
	 */
	public void registerAppInterface(String appName, Boolean isMediaApp,
                                     Integer correlationID) throws SyncException {
		
		registerAppInterface(
				/*syncMsgVersion*/null, 
				appName,
				/*ngnMediaScreenAppName*/null,
				/*vrSynonyms*/null,
				isMediaApp, 
				/*languageDesired*/null,
				correlationID);
	}
	
	/**
	 * Sends a RegisterAppInterface RPCRequest to SYNC. Responses are captured through callback
     * on IProxyListener.
	 * 
	 * @param appName
	 * @throws SyncException
	 */
	public void registerAppInterface(String appName, Integer correlationId) throws SyncException {
		registerAppInterface(appName, false, correlationId);
	}
	
	/**
	 * Sends an UnregisterAppInterface RPCRequest to SYNC. Responses are captured through callback
     * on IProxyListener.
	 * 
	 * @param correlationID
	 * @throws SyncException
	 */
	/*public void unregisterAppInterface(Integer correlationID) throws SyncException {
		// Test if proxy has been disposed
		if (mIsProxyDisposed) {
			throw new SyncException("This SyncProxy object has been disposed, it is no long " +
                    "capable of executing methods.", SyncExceptionCause.SYNC_PROXY_DISPOSED);
		}		
				
		unregisterAppInterfacePrivate(correlationID);
	}*/
	
	/**
	 * Returns is isConnected state of the SYNC transport.
	 * 
	 * @return Boolean isConnected
	 */
	public boolean getIsConnected() {
		return super.getIsConnected();
	}
}