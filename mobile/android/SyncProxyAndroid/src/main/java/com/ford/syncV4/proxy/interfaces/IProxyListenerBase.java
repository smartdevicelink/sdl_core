package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.protocol.enums.ServiceType;
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
import com.ford.syncV4.proxy.rpc.OnHMIStatus;
import com.ford.syncV4.proxy.rpc.OnHashChange;
import com.ford.syncV4.proxy.rpc.OnKeyboardInput;
import com.ford.syncV4.proxy.rpc.OnLanguageChange;
import com.ford.syncV4.proxy.rpc.OnPermissionsChange;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.OnTouchEvent;
import com.ford.syncV4.proxy.rpc.OnVehicleData;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.PerformInteractionResponse;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.ReadDIDResponse;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.ResetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.ScrollableMessageResponse;
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
import com.ford.syncV4.proxy.rpc.SyncPDataResponse;
import com.ford.syncV4.proxy.rpc.SystemRequestResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeButtonResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleDataResponse;
import com.ford.syncV4.proxy.rpc.UpdateTurnListResponse;
import com.ford.syncV4.proxy.rpc.enums.AppInterfaceUnregisteredReason;

public interface IProxyListenerBase extends ISyncDriverDistractionListener,
											ISyncEncodedSyncPDataListener,
											ISyncSyncPDataListener,
											ISyncTBTClientStateListener {

	/**
	 * onOnHMIStatus being called indicates that there has been an HMI Level change,
	 * system context change or audio streaming state change.
	 * 
	 * @param notification - Contains information about the HMI Level,
	 * system context and audio streaming state.
	 */
	public void onOnHMIStatus(byte sessionId, OnHMIStatus notification);

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
	 * onProxyError() being called indicates that the SYNC Proxy experenced an error.
	 *
     * @param info - Includes information about the Exception that occurred.
     * @param e - The exception that occurred.
     */
	public void onError(String info, Throwable e);
	
	
	/**
	 * onGenericResponse() being called indicates that SYNC could not determine the
	 * type of request it is responding to. This is usually result of an unknown RPC Request
	 * being sent.
	 * 
	 * @param response - Includes detailed information about the response.
	 */
	public void onGenericResponse(GenericResponse response);
	
	/**
	 * onOnCommand() being called indicates that the user selected a command on SYNC.
	 * 
	 * @param notification - Contains information about the command chosen.
	 */
	public void onOnCommand(byte sessionId, OnCommand notification);
	
	/**
	 * onAddCommandResponse() being called indicates that SYNC has responded to
	 * a request to add a command.
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onAddCommandResponse(byte sessionId, AddCommandResponse response);
	
	/**
	 * onAddSubMenuResponse() being called indicates that SYNC has responded to
	 * a request to add a command.
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onAddSubMenuResponse(byte sessionId, AddSubMenuResponse response);
	
	/**
	 * onCreateInteractionChoiceSetResponse() being called indicates that SYNC has
	 * responded to a request to add an interactionChoiceSet.
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onCreateInteractionChoiceSetResponse(byte sessionId,
                                                     CreateInteractionChoiceSetResponse response);
	
	/**
	 * onAlertResponse being called indicates that SYNC has
	 * responded to a request to alert the user. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onAlertResponse(byte sessionId, AlertResponse response);
	
	/**
	 * onDeleteCommandResponse being called indicates that SYNC has
	 * responded to a request to delete a command. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onDeleteCommandResponse(byte sessionId, DeleteCommandResponse response);
	
	/**
	 * onDeleteCommandResponse being called indicates that SYNC has
	 * responded to a request to delete an interaction choice set. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onDeleteInteractionChoiceSetResponse(byte sessionId,
                                                     DeleteInteractionChoiceSetResponse response);
	
	/**
	 * onDeleteCommandResponse being called indicates that SYNC has
	 * responded to a request to delete a submenu. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onDeleteSubMenuResponse(byte sessionId, DeleteSubMenuResponse response);
	
	/**
	 * onEncodedSyncPDataResponse being called indicates that SYNC has
	 * responded to a request containing encodedSyncPData. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onEncodedSyncPDataResponse(byte sessionId, EncodedSyncPDataResponse response);

	public void onSyncPDataResponse(byte sessionId, SyncPDataResponse response);
	
	/**
	 * onPerformInteractionResponse being called indicates that SYNC has
	 * responded to a request to perform an interaction. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onPerformInteractionResponse(byte sessionId, PerformInteractionResponse response);
	
	/**
	 * onResetGlobalPropertiesResponse being called indicates that SYNC has
	 * responded to a request to reset global properties. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onResetGlobalPropertiesResponse(byte sessionId,
                                                ResetGlobalPropertiesResponse response);
	
	/**
	 * onSetGlobalPropertiesResponse being called indicates that SYNC has
	 * responded to a request to set global properties. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onSetGlobalPropertiesResponse(byte sessionId, SetGlobalPropertiesResponse response);
	
	/**
	 * onSetMediaClockTimerResponse being called indicates that SYNC has
	 * responded to a request to set the media clock timer. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onSetMediaClockTimerResponse(byte sessionId, SetMediaClockTimerResponse response);
	
	/**
	 * onShowResponse being called indicates that SYNC has
	 * responded to a request to display information to the user. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onShowResponse(byte sessionId, ShowResponse response);
	
	/**
	 * onSpeakResponse being called indicates that SYNC has
	 * responded to a request to speak information to the user. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onSpeakResponse(byte sessionId, SpeakResponse response);
	
	/**
	 * onButtonEvent being called indicates that a button event has occurred. 
	 * 
	 * @param notification - Contains information about the notification sent from SYNC.
	 */
	public void onOnButtonEvent(byte sessionId, OnButtonEvent notification);

	/**
	 * onButtonPress being called indicates that SYNC has a button has 
	 * been pressed by the user. 
	 * 
	 * @param notification - Contains information about the notification sent from SYNC.
	 */
	public void onOnButtonPress(byte sessionId, OnButtonPress notification);
	
	/**
	 * onSubscribeButtonResponse being called indicates that SYNC has
	 * responded to a request to subscribe to button events and button presses. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onSubscribeButtonResponse(byte sessionId, SubscribeButtonResponse response);
	
	/**
	 * onUnsubscribeButtonResponse being called indicates that SYNC has
	 * responded to a request to unsubscribe from button events and button presses. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onUnsubscribeButtonResponse(byte sessionId, UnsubscribeButtonResponse response);

	/**
	 * onOnPermissionsChange being called indicates that your app permissions have 
	 * changed due to a policy table change. This can mean your app has received additional
	 * permissions OR lost permissions.
	 * 
	 * @param notification - Contains information about the changed permissions.
	 */
	public void onOnPermissionsChange(byte sessionId, OnPermissionsChange notification);
	
	public void onSubscribeVehicleDataResponse(byte sessionId, SubscribeVehicleDataResponse response);
	
	public void onUnsubscribeVehicleDataResponse(byte sessionId, UnsubscribeVehicleDataResponse response);
	
	public void onGetVehicleDataResponse(byte sessionId, GetVehicleDataResponse response);
	
	public void onReadDIDResponse(byte sessionId, ReadDIDResponse response);
	
	public void onGetDTCsResponse(byte sessionId, GetDTCsResponse response);
	
	public void onOnVehicleData(byte sessionId, OnVehicleData notification);
	
	public void onPerformAudioPassThruResponse(byte sessionId, PerformAudioPassThruResponse response);
	
	public void onEndAudioPassThruResponse(byte sessionId, EndAudioPassThruResponse response);
	
	public void onOnAudioPassThru(byte sessionId, OnAudioPassThru notification);

	public void onPutFileResponse(byte sessionId, PutFileResponse response);
	
	public void onDeleteFileResponse(byte sessionId, DeleteFileResponse response);
	
	public void onListFilesResponse(byte sessionId, ListFilesResponse response);

	public void onSetAppIconResponse(byte sessionId, SetAppIconResponse response);
	
	public void onScrollableMessageResponse(byte sessionId, ScrollableMessageResponse response);

	public void onChangeRegistrationResponse(byte sessionId, ChangeRegistrationResponse response);

	public void onSetDisplayLayoutResponse(byte sessionId, SetDisplayLayoutResponse response);
	
	public void onOnLanguageChange(byte sessionId, OnLanguageChange notification);
	
	public void onSliderResponse(byte sessionId, SliderResponse response);

	public void onAlertManeuverResponse(byte sessionId, AlertManeuverResponse response);
	
	public void onShowConstantTBTResponse(byte sessionId, ShowConstantTBTResponse response);
	
	public void onUpdateTurnListResponse(byte sessionId, UpdateTurnListResponse response);

    public void onSystemRequestResponse(byte sessionId, SystemRequestResponse response);

    public void onMobileNaviStart(String appId, byte sessionId);

    public void onMobileNavAckReceived(String appId, byte sessionId, int frameReceivedNumber);

    public void onOnTouchEvent(byte sessionId, OnTouchEvent notification);

    public void onKeyboardInput(byte sessionId, OnKeyboardInput msg);

    public void onOnSystemRequest(byte sessionId, OnSystemRequest notification);

    public void onRegisterAppRequest(byte sessionId, RegisterAppInterface msg);

    public void onAppUnregisteredAfterLanguageChange(byte sessionId, OnLanguageChange msg);

    public void onAppUnregisteredReason(byte sessionId, AppInterfaceUnregisteredReason reason);

    public void onProtocolServiceEnded(ServiceType serviceType, byte sessionId);

    public void onProtocolServiceEndedAck(ServiceType serviceType, byte sessionId);

    /**
     * This callback is to inform SPT that session is going to be started
     */
    public void onStartSession();

    public void onSessionStarted(String appId, byte sessionId);

    public void onAudioServiceStart(String appId, byte sessionId);

    public void onStartServiceNackReceived(String appId, byte sessionId, ServiceType serviceType);

    /**
     * Notification containing an updated hashID which can be used over connection cycles
     * (i.e. loss of connection, ignition cycles, etc.).
     * Sent after initial registration and subsequently after any change in the calculated hash
     * of all persisted app data.
     *
     * @param sessionId Id of the Session for the particular Notification
     * @param onHashChange {@link com.ford.syncV4.proxy.rpc.OnHashChange} notification
     */
    public void onHashChange(byte sessionId, OnHashChange onHashChange);

    /**
     * Provide a callback to listener in case of USB problem
     * https://code.google.com/p/android/issues/detail?id=20545
     */
    public void onUSBNoSuchDeviceException();

    public void onDiagnosticMessageResponse(byte sessionId,
                                            DiagnosticMessageResponse diagnosticMessageResponse);

    /**
     * Provide a callback (in most cases for the test purposes) when
     * {@link com.ford.syncV4.proxy.rpc.PutFile} request is going to be performed
     *
     * @param putFile {@link com.ford.syncV4.proxy.rpc.PutFile}
     */
    public void onPutFileRequest(byte sessionId, PutFile putFile);
}