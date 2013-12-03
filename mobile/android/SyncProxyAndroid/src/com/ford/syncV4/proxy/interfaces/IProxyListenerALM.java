package com.ford.syncV4.proxy.interfaces;

public interface IProxyListenerALM extends IProxyListenerBase {
	// Adds Advanced Life-cycle Management call-backs to the IProxyListenerAbstract interface
	
	/**
	 * **MOVED TO IProxyListenerBase** - onOnHMIStatus() being called indicates that the proxy has entered a state in which the 
	 * application may create SYNC related resources (addCommands, ChoiceSets). 
	 */
	//public void onOnHMIStatus(OnHMIStatus notification);
	
	/**
	 * **MOVED TO IProxyListenerBase** - onProxyClosed() being called indicates that the app is no longer registered with SYNC
	 * All resources on SYNC (addCommands and ChoiceSets) have been deleted and will have to be
	 * recreated upon the next onReadyForInitialization() call-back. 
	 */
	//public void onProxyClosed(String info, Exception e);
	
	/**
	 * **MOVED TO IProxyListenerBase** - onError() being called indicates that the proxy has experienced an unrecoverable error.
	 * A new proxy object must be initiated to reestablish connection with SYNC.
	 * 
	 * @param info - Any info present about the error that occurred.
	 * @param e - Any exception thrown by the error.
	 */
	//public void onError(String info, Exception e);
	
	/**
	 * **Deprecated** - onSyncInterfaceAvailable() being called indicates that the proxy now has access to SYNC's HMI. 
	 * Monitor the onFocusChange call-back to determine which level of HMI is available to the proxy.
	 * 
	 * @param isFirstAvailability - Indicates this is the first onSyncInterfaceAvailable in this lifecycle.
	 */
	// HMI (Background, Limited, Full) from Unavailable  = onSyncInterfaceAvailable(Boolean isFirstAvailability);

	/**
	 * **Deprecated** - onSyncInterfaceUnavailable() being called indicates that the proxy does NOT have access to SYNC's HIM.
	 */
	// HMI None onSyncInterfaceUnavailable();
	
	/**
	 * **Deprecated** - ALM HMI states converted back to HMI Levels
	 * 
	 * HMI Full = onSyncInFocus(Boolean isFirstSyncInFocus);
	 * HMI Limited = onSyncInFocusLimited();
	 * HMI Background = onSyncLostFocus();
	 */
}
