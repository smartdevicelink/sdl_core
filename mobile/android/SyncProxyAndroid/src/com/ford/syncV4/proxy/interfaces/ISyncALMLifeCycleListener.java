package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.ChangeRegistrationResponse;
import com.ford.syncV4.proxy.rpc.DeleteFileResponse;
import com.ford.syncV4.proxy.rpc.EndAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.AlertManeuverResponse;
import com.ford.syncV4.proxy.rpc.GenericResponse;
import com.ford.syncV4.proxy.rpc.GetDTCsResponse;
import com.ford.syncV4.proxy.rpc.GetVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.OnAudioPassThru;
import com.ford.syncV4.proxy.rpc.OnHMIStatus;
import com.ford.syncV4.proxy.rpc.OnPermissionsChange;
import com.ford.syncV4.proxy.rpc.OnVehicleData;
import com.ford.syncV4.proxy.rpc.ReadDIDResponse;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.ListFilesResponse;
import com.ford.syncV4.proxy.rpc.OnLanguageChange;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.ScrollableMessageResponse;
import com.ford.syncV4.proxy.rpc.SetAppIconResponse;
import com.ford.syncV4.proxy.rpc.SetDisplayLayoutResponse;
import com.ford.syncV4.proxy.rpc.SliderResponse;
import com.ford.syncV4.proxy.rpc.ShowConstantTBTResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.UpdateTurnListResponse;

public interface ISyncALMLifeCycleListener {

	/**
	 * onOnHMIStatus being called indicates that there has been an HMI Level change,
	 * system context change or audio streaming state change.
	 * 
	 * @param notification - Contains information about the HMI Level,
	 * system context and audio streaming state.
	 */
	public void onOnHMIStatus(OnHMIStatus notification);

	/**
	 * onProxyClosed has different functionality for the different models.
	 * In the non-ALM model this indicates that the proxy has experienced an unrecoverable error.
	 * A new proxy object must be initiated to reestablish connection with SYNC.
	 * In the ALM model this indicates that the app is no longer registered with SYNC
	 * All resources on SYNC (addCommands and ChoiceSets) have been deleted and will have to be
	 * recreated upon the next onReadyForInitialization() call-back. 
	 * 
	 * @param info - Includes information about the reason the proxy has been closed.
	 * @param e - The exception that occurred. 
	 */
	public void onProxyClosed(String info, Exception e);
	
	/**
	 * onError() being called indicates that the proxy has experienced an error.
	 * 
	 * @param info - Any info present about the error that occurred.
	 * @param e - Any exception thrown by the error.
	 */
	public void onError(String info, Exception e);
	
	/**
	 * onGenericResponse() being called indicates that the SYNC could not determine the
	 * type of request it is responding to. This is usually result of an unknown RPC Request
	 * being sent.
	 * 
	 * @param response - Includes detailed information about the response.
	 */
	public void onGenericResponse(GenericResponse response);

	/**
	 * onOnPermissionsChange being called indicates that your app permissions have 
	 * changed due to a policy table change. This can mean your app has received additional
	 * permissions OR lost permissions.
	 * 
	 * @param notification - Contains information about the changed permissions.
	 */
	public void onOnPermissionsChange(OnPermissionsChange notification);

	public void onSubscribeVehicleDataResponse(SubscribeVehicleDataResponse response);

	public void onUnsubscribeVehicleDataResponse(UnsubscribeVehicleDataResponse response);

	public void onGetVehicleDataResponse(GetVehicleDataResponse response);

	public void onReadDIDResponse(ReadDIDResponse response);

	public void onGetDTCsResponse(GetDTCsResponse response);

	public void onOnVehicleData(OnVehicleData notification);

	public void onPutFileResponse(PutFileResponse response);
	
	public void onPerformAudioPassThruResponse(PerformAudioPassThruResponse response);

	public void onEndAudioPassThruResponse(EndAudioPassThruResponse response);

	public void onOnAudioPassThru(OnAudioPassThru notification);
	
	public void onDeleteFileResponse(DeleteFileResponse response);

	public void onListFilesResponse(ListFilesResponse response);
	
	public void onSetAppIconResponse(SetAppIconResponse response);

	public void onScrollableMessageResponse(ScrollableMessageResponse response);
	
	public void onChangeRegistrationResponse(ChangeRegistrationResponse response);

	public void onSetDisplayLayoutResponse(SetDisplayLayoutResponse response);

	public void onOnLanguageChange(OnLanguageChange notification);
	
	public void onSliderResponse(SliderResponse response);

	public void onAlertManeuverResponse(AlertManeuverResponse response);

	public void onShowConstantTBTResponse(ShowConstantTBTResponse response);

	public void onUpdateTurnListResponse(UpdateTurnListResponse response);

	/**
	 * **Deprecated** - onSyncConntected() being called indicates that the proxy, for the first time in it's active 
	 * life-cycle, has entered a state in which the application may create SYNC related resources (addCommands, ChoiceSets). 
	 */
	//public void onSyncConntected(RegisterAppInterfaceResponse response);
	
	/**
	 * **Deprecated** - onSyncDisconnected() being called indicates that the app is no longer registered with SYNC
	 * All resources on SYNC (addCommands and ChoiceSets) have been deleted and will have to be
	 * recreated upon the next onReadyForInitialization() call-back. 
	 */
	//public void onSyncDisconnected(SyncDisconnectedReason syncDisconnectedReason);
	
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
	 * **Deprecated** - onProxyUnusable() being called indicates that the proxy has experienced an unrecoverable error.
	 * A new proxy object must be initiated to reestablish connection with SYNC.
	 * 
	 * @param info - Any info present about the error that occurred.
	 * @param e - Any exception thrown by the error.
	 */
	//public void onProxyUnusable(String info, Exception e);
	
	/**
	 * **Deprecated** - ALM HMI states converted back to HMI Levels
	 * 
	 * HMI Full = onSyncInFocus(Boolean isFirstSyncInFocus);
	 * HMI Limited = onSyncInFocusLimited();
	 * HMI Background = onSyncLostFocus();
	 */
}
