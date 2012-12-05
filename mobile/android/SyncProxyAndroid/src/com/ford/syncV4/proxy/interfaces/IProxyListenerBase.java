package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.AddCommandResponse;
import com.ford.syncV4.proxy.rpc.AddSubMenuResponse;
import com.ford.syncV4.proxy.rpc.AlertResponse;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSetResponse;
import com.ford.syncV4.proxy.rpc.DeleteCommandResponse;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSetResponse;
import com.ford.syncV4.proxy.rpc.DeleteSubMenuResponse;
import com.ford.syncV4.proxy.rpc.EncodedSyncPDataResponse;
import com.ford.syncV4.proxy.rpc.GenericResponse;
import com.ford.syncV4.proxy.rpc.OnButtonEvent;
import com.ford.syncV4.proxy.rpc.OnButtonPress;
import com.ford.syncV4.proxy.rpc.OnCommand;
import com.ford.syncV4.proxy.rpc.OnHMIStatus;
import com.ford.syncV4.proxy.rpc.OnPermissionsChange;
import com.ford.syncV4.proxy.rpc.PerformInteractionResponse;
import com.ford.syncV4.proxy.rpc.ResetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.SetGlobalPropertiesResponse;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimerResponse;
import com.ford.syncV4.proxy.rpc.ShowResponse;
import com.ford.syncV4.proxy.rpc.SliderResponse;
import com.ford.syncV4.proxy.rpc.SpeakResponse;
import com.ford.syncV4.proxy.rpc.SubscribeButtonResponse;
import com.ford.syncV4.proxy.rpc.UnsubscribeButtonResponse;


public interface IProxyListenerBase extends ISyncDriverDistractionListener,
											ISyncEncodedSyncPDataListener,
											ISyncTBTClientStateListener{

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
	 * onProxyError() being called indicates that the SYNC Proxy experenced an error.
	 * 
	 * @param info - Includes information about the Exception that occurred.
	 * @param e - The exception that occurred. 
	 */
	public void onError(String info, Exception e);
	
	
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
	public void onOnCommand(OnCommand notification);
	
	/**
	 * onAddCommandResponse() being called indicates that SYNC has responded to
	 * a request to add a command.
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onAddCommandResponse(AddCommandResponse response);
	
	/**
	 * onAddSubMenuResponse() being called indicates that SYNC has responded to
	 * a request to add a command.
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onAddSubMenuResponse(AddSubMenuResponse response);
	
	/**
	 * onCreateInteractionChoiceSetResponse() being called indicates that SYNC has
	 * responded to a request to add an interactionChoiceSet.
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onCreateInteractionChoiceSetResponse(CreateInteractionChoiceSetResponse response);
	
	/**
	 * onAlertResponse being called indicates that SYNC has
	 * responded to a request to alert the user. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onAlertResponse(AlertResponse response);
	
	/**
	 * onDeleteCommandResponse being called indicates that SYNC has
	 * responded to a request to delete a command. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onDeleteCommandResponse(DeleteCommandResponse response);
	
	/**
	 * onDeleteCommandResponse being called indicates that SYNC has
	 * responded to a request to delete an interaction choice set. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onDeleteInteractionChoiceSetResponse(DeleteInteractionChoiceSetResponse response);
	
	/**
	 * onDeleteCommandResponse being called indicates that SYNC has
	 * responded to a request to delete a submenu. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onDeleteSubMenuResponse(DeleteSubMenuResponse response);
	
	/**
	 * onEncodedSyncPDataResponse being called indicates that SYNC has
	 * responded to a request containing encodedSyncPData. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onEncodedSyncPDataResponse(EncodedSyncPDataResponse response);
	
	/**
	 * onPerformInteractionResponse being called indicates that SYNC has
	 * responded to a request to perform an interaction. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onPerformInteractionResponse(PerformInteractionResponse response);
	
	/**
	 * onResetGlobalPropertiesResponse being called indicates that SYNC has
	 * responded to a request to reset global properties. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onResetGlobalPropertiesResponse(ResetGlobalPropertiesResponse response);
	
	/**
	 * onSetGlobalPropertiesResponse being called indicates that SYNC has
	 * responded to a request to set global properties. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onSetGlobalPropertiesResponse(SetGlobalPropertiesResponse response);
	
	/**
	 * onSetMediaClockTimerResponse being called indicates that SYNC has
	 * responded to a request to set the media clock timer. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onSetMediaClockTimerResponse(SetMediaClockTimerResponse response);
	
	/**
	 * onShowResponse being called indicates that SYNC has
	 * responded to a request to display information to the user. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onShowResponse(ShowResponse response);
	
	/**
	 * onSpeakResponse being called indicates that SYNC has
	 * responded to a request to speak information to the user. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onSpeakResponse(SpeakResponse response);
	
	/**
	 * onButtonEvent being called indicates that a button event has occurred. 
	 * 
	 * @param response - Contains information about the notification sent from SYNC.
	 */
	public void onOnButtonEvent(OnButtonEvent notification);

	/**
	 * onButtonPress being called indicates that SYNC has a button has 
	 * been pressed by the user. 
	 * 
	 * @param response - Contains information about the notification sent from SYNC.
	 */
	public void onOnButtonPress(OnButtonPress notification);
	
	/**
	 * onSubscribeButtonResponse being called indicates that SYNC has
	 * responded to a request to subscribe to button events and button presses. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onSubscribeButtonResponse(SubscribeButtonResponse response);
	
	/**
	 * onUnsubscribeButtonResponse being called indicates that SYNC has
	 * responded to a request to unsubscribe from button events and button presses. 
	 * 
	 * @param response - Contains information about the response sent from SYNC.
	 */
	public void onUnsubscribeButtonResponse(UnsubscribeButtonResponse response);

	/**
	 * onOnPermissionsChange being called indicates that your app permissions have 
	 * changed due to a policy table change. This can mean your app has received additional
	 * permissions OR lost permissions.
	 * 
	 * @param notification - Contains information about the changed permissions.
	 */
	public void onOnPermissionsChange(OnPermissionsChange notification);

	public void onSliderResponse(SliderResponse response);
}
