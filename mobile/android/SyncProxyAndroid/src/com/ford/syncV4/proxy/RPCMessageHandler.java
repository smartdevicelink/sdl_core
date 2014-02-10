package com.ford.syncV4.proxy;

import android.os.Handler;
import android.util.Log;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.interfaces.IProxyListenerBase;
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
import com.ford.syncV4.proxy.rpc.OnKeyboardInput;
import com.ford.syncV4.proxy.rpc.OnLanguageChange;
import com.ford.syncV4.proxy.rpc.OnPermissionsChange;
import com.ford.syncV4.proxy.rpc.OnSyncPData;
import com.ford.syncV4.proxy.rpc.OnTBTClientState;
import com.ford.syncV4.proxy.rpc.OnTouchEvent;
import com.ford.syncV4.proxy.rpc.OnVehicleData;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThruResponse;
import com.ford.syncV4.proxy.rpc.PerformInteractionResponse;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.ReadDIDResponse;
import com.ford.syncV4.proxy.rpc.RegisterAppInterfaceResponse;
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
import com.ford.syncV4.proxy.rpc.enums.HMILevel;
import com.ford.syncV4.proxy.rpc.enums.SyncConnectionState;
import com.ford.syncV4.proxy.rpc.enums.SyncDisconnectedReason;
import com.ford.syncV4.trace.SyncTrace;
import com.ford.syncV4.trace.enums.InterfaceActivityDirection;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;


/**
 * Created by Andrew Batutin on 2/10/14.
 */
public class RPCMessageHandler implements IRPCMessageHandler {


    private SyncProxyBase syncProxyBase;

    public RPCMessageHandler(SyncProxyBase syncProxyBase) {
        this.syncProxyBase = syncProxyBase;
    }

    @Override
    public void handleRPCMessage(Hashtable hash) {
        handleRPCMessageInt(hash);
    }

    private void handleRPCMessageInt(Hashtable hash) {
        RPCMessage rpcMsg = new RPCMessage(hash);
        String functionName = rpcMsg.getFunctionName();
        String messageType = rpcMsg.getMessageType();

        if (messageType.equals(Names.response)) {
            SyncTrace.logRPCEvent(InterfaceActivityDirection.Receive, new RPCResponse(rpcMsg), SyncProxyBase.SYNC_LIB_TRACE_KEY);

            final RPCResponse response = new RPCResponse(hash);
            final Integer responseCorrelationID = response.getCorrelationID();
            if (!syncProxyBase.handlePartialRPCResponse(response) &&
                    !syncProxyBase.handleLastInternalResponse(response)) {

                // Check to ensure response is not from an internal message (reserved correlation ID)
                if (syncProxyBase.isCorrelationIDProtected(responseCorrelationID)) {
                    // This is a response generated from an internal message, it can be trapped here
                    // The app should not receive a response for a request it did not send
                    if (responseCorrelationID ==
                            syncProxyBase.REGISTER_APP_INTERFACE_CORRELATION_ID &&
                            syncProxyBase.getAdvancedLifecycleManagementEnabled() &&
                            functionName.equals(Names.RegisterAppInterface)) {
                        final RegisterAppInterfaceResponse msg =
                                new RegisterAppInterfaceResponse(hash);
                        if (msg.getSuccess()) {
                            syncProxyBase.setAppInterfaceRegisterd(true);
                        }

                        //_autoActivateIdReturned = msg.getAutoActivateID();
                        /*Place holder for legacy support*/
                        syncProxyBase.setAutoActivateIdReturned("8675309");
                        syncProxyBase.setButtonCapabilities(msg.getButtonCapabilities());
                        syncProxyBase.setDisplayCapabilities(msg.getDisplayCapabilities());
                        syncProxyBase.setSoftButtonCapabilities(msg.getSoftButtonCapabilities());
                        syncProxyBase.setPresetBankCapabilities(msg.getPresetBankCapabilities());
                        syncProxyBase.setHmiZoneCapabilities(msg.getHmiZoneCapabilities());
                        syncProxyBase.setSpeechCapabilities(msg.getSpeechCapabilities());
                        syncProxyBase.setSyncLanguage(msg.getLanguage());
                        syncProxyBase.setHmiDisplayLanguage(msg.getHmiDisplayLanguage());
                        syncProxyBase.setSyncMsgVersion(msg.getSyncMsgVersion());
                        syncProxyBase.setVrCapabilities(msg.getVrCapabilities());
                        syncProxyBase.setVehicleType(msg.getVehicleType());
                        syncProxyBase.setSyncConnectionState(SyncConnectionState.SYNC_CONNECTED);

                        // If registerAppInterface failed, exit with OnProxyUnusable
                        if (!msg.getSuccess()) {
                            syncProxyBase.notifyProxyClosed(
                                    "Unable to register app interface. Review values passed to the SyncProxy constructor. RegisterAppInterface result code: ",
                                    new SyncException(
                                            "Unable to register app interface. Review values passed to the SyncProxy constructor. RegisterAppInterface result code: " +
                                                    msg.getResultCode(),
                                            SyncExceptionCause.SYNC_REGISTRATION_ERROR));
                        }

                        syncProxyBase.processRegisterAppInterfaceResponse(msg);
                    } else if (
                            responseCorrelationID == syncProxyBase.POLICIES_CORRELATION_ID &&
                                    functionName.equals(
                                            Names.OnEncodedSyncPData)) {
                        // OnEncodedSyncPData

                        final OnEncodedSyncPData msg =
                                new OnEncodedSyncPData(hash);

                        // If url is null, then send notification to the app, otherwise, send to URL
                        if (msg.getUrl() != null) {
                            // URL has data, attempt to post request to external server
                            Thread handleOffboardSyncTransmissionTread =
                                    new Thread() {
                                        @Override
                                        public void run() {
                                            syncProxyBase.sendEncodedSyncPDataToUrl(
                                                    msg.getUrl(), msg.getData(),
                                                    msg.getTimeout());
                                        }
                                    };

                            handleOffboardSyncTransmissionTread.start();
                        }
                    } else if ((responseCorrelationID ==
                            syncProxyBase.UNREGISTER_APP_INTERFACE_CORRELATION_ID) &&
                            functionName.equals(Names.UnregisterAppInterface)) {
                        syncProxyBase.onUnregisterAppInterfaceResponse(hash);
                    }
                    return;
                }

                if (functionName.equals(Names.RegisterAppInterface)) {
                    final RegisterAppInterfaceResponse msg =
                            new RegisterAppInterfaceResponse(hash);
                    if (msg.getSuccess()) {
                        syncProxyBase.setAppInterfaceRegisterd(true);
                    }

                    //_autoActivateIdReturned = msg.getAutoActivateID();
                /*Place holder for legacy support*/

                    syncProxyBase.setAutoActivateIdReturned("8675309");
                    syncProxyBase.setButtonCapabilities(msg.getButtonCapabilities());
                    syncProxyBase.setDisplayCapabilities(msg.getDisplayCapabilities());
                    syncProxyBase.setSoftButtonCapabilities(msg.getSoftButtonCapabilities());
                    syncProxyBase.setPresetBankCapabilities(msg.getPresetBankCapabilities());
                    syncProxyBase.setHmiZoneCapabilities(msg.getHmiZoneCapabilities());
                    syncProxyBase.setSpeechCapabilities(msg.getSpeechCapabilities());
                    syncProxyBase.setSyncLanguage(msg.getLanguage());
                    syncProxyBase.setHmiDisplayLanguage(msg.getHmiDisplayLanguage());
                    syncProxyBase.setSyncMsgVersion(msg.getSyncMsgVersion());
                    syncProxyBase.setVrCapabilities(msg.getVrCapabilities());
                    syncProxyBase.setVehicleType(msg.getVehicleType());


                    // RegisterAppInterface
                    if (syncProxyBase.getAdvancedLifecycleManagementEnabled()) {
                        syncProxyBase.setSyncConnectionState(SyncConnectionState.SYNC_CONNECTED);

                        // If registerAppInterface failed, exit with OnProxyUnusable
                        if (!msg.getSuccess()) {
                            syncProxyBase.notifyProxyClosed(
                                    "Unable to register app interface. Review values passed to the SyncProxy constructor. RegisterAppInterface result code: ",
                                    new SyncException(
                                            "Unable to register app interface. Review values passed to the SyncProxy constructor. RegisterAppInterface result code: " +
                                                    msg.getResultCode(),
                                            SyncExceptionCause.SYNC_REGISTRATION_ERROR));
                        }
                    }
                    syncProxyBase.processRegisterAppInterfaceResponse(msg);
                } else if (functionName.equals(Names.Speak)) {
                    // SpeakResponse

                    final SpeakResponse msg = new SpeakResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onSpeakResponse(msg);
                            }
                        });
                    } else {
                        getProxyListener().onSpeakResponse(msg);
                    }
                } else if (functionName.equals(Names.Alert)) {
                    // AlertResponse

                    final AlertResponse msg = new AlertResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onAlertResponse(msg);
                            }
                        });
                    } else {
                        getProxyListener().onAlertResponse(msg);
                    }
                } else if (functionName.equals(Names.Show)) {
                    // ShowResponse

                    final ShowResponse msg = new ShowResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onShowResponse(
                                        (ShowResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onShowResponse((ShowResponse) msg);
                    }
                } else if (functionName.equals(Names.AddCommand)) {
                    // AddCommand

                    final AddCommandResponse msg = new AddCommandResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onAddCommandResponse(
                                        (AddCommandResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onAddCommandResponse(
                                (AddCommandResponse) msg);
                    }
                } else if (functionName.equals(Names.DeleteCommand)) {
                    // DeleteCommandResponse

                    final DeleteCommandResponse msg =
                            new DeleteCommandResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onDeleteCommandResponse(
                                        (DeleteCommandResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onDeleteCommandResponse(
                                (DeleteCommandResponse) msg);
                    }
                } else if (functionName.equals(Names.AddSubMenu)) {
                    // AddSubMenu

                    final AddSubMenuResponse msg = new AddSubMenuResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onAddSubMenuResponse(
                                        (AddSubMenuResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onAddSubMenuResponse(
                                (AddSubMenuResponse) msg);
                    }
                } else if (functionName.equals(Names.DeleteSubMenu)) {
                    // DeleteSubMenu

                    final DeleteSubMenuResponse msg =
                            new DeleteSubMenuResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onDeleteSubMenuResponse(
                                        (DeleteSubMenuResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onDeleteSubMenuResponse(
                                (DeleteSubMenuResponse) msg);
                    }
                } else if (functionName.equals(Names.SubscribeButton)) {
                    // SubscribeButton

                    final SubscribeButtonResponse msg =
                            new SubscribeButtonResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onSubscribeButtonResponse(
                                        (SubscribeButtonResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onSubscribeButtonResponse(
                                (SubscribeButtonResponse) msg);
                    }
                } else if (functionName.equals(Names.UnsubscribeButton)) {
                    // UnsubscribeButton

                    final UnsubscribeButtonResponse msg =
                            new UnsubscribeButtonResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onUnsubscribeButtonResponse(
                                        (UnsubscribeButtonResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onUnsubscribeButtonResponse(
                                (UnsubscribeButtonResponse) msg);
                    }
                } else if (functionName.equals(Names.SetMediaClockTimer)) {
                    // SetMediaClockTimer

                    final SetMediaClockTimerResponse msg =
                            new SetMediaClockTimerResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onSetMediaClockTimerResponse(
                                        (SetMediaClockTimerResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onSetMediaClockTimerResponse(
                                (SetMediaClockTimerResponse) msg);
                    }
                } else if (functionName.equals(Names.EncodedSyncPData)) {
                    // EncodedSyncPData

                    final EncodedSyncPDataResponse msg =
                            new EncodedSyncPDataResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onEncodedSyncPDataResponse(msg);
                            }
                        });
                    } else {
                        getProxyListener().onEncodedSyncPDataResponse(msg);
                    }
                } else if (functionName.equals(Names.SyncPData)) {
                    // SyncPData

                    final SyncPDataResponse msg = new SyncPDataResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onSyncPDataResponse(msg);
                            }
                        });
                    } else {
                        getProxyListener().onSyncPDataResponse(msg);
                    }
                } else if (functionName.equals(
                        Names.CreateInteractionChoiceSet)) {
                    // CreateInteractionChoiceSet

                    final CreateInteractionChoiceSetResponse msg =
                            new CreateInteractionChoiceSetResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onCreateInteractionChoiceSetResponse(
                                        (CreateInteractionChoiceSetResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onCreateInteractionChoiceSetResponse(
                                (CreateInteractionChoiceSetResponse) msg);
                    }
                } else if (functionName.equals(
                        Names.DeleteInteractionChoiceSet)) {
                    // DeleteInteractionChoiceSet

                    final DeleteInteractionChoiceSetResponse msg =
                            new DeleteInteractionChoiceSetResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onDeleteInteractionChoiceSetResponse(
                                        (DeleteInteractionChoiceSetResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onDeleteInteractionChoiceSetResponse(
                                (DeleteInteractionChoiceSetResponse) msg);
                    }
                } else if (functionName.equals(Names.PerformInteraction)) {
                    // PerformInteraction

                    final PerformInteractionResponse msg =
                            new PerformInteractionResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onPerformInteractionResponse(
                                        (PerformInteractionResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onPerformInteractionResponse(
                                (PerformInteractionResponse) msg);
                    }
                } else if (functionName.equals(Names.SetGlobalProperties)) {
                    final SetGlobalPropertiesResponse msg =
                            new SetGlobalPropertiesResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onSetGlobalPropertiesResponse(
                                        (SetGlobalPropertiesResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onSetGlobalPropertiesResponse(
                                (SetGlobalPropertiesResponse) msg);
                    }
                } else if (functionName.equals(Names.ResetGlobalProperties)) {
                    // ResetGlobalProperties

                    final ResetGlobalPropertiesResponse msg =
                            new ResetGlobalPropertiesResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onResetGlobalPropertiesResponse(
                                        (ResetGlobalPropertiesResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onResetGlobalPropertiesResponse(
                                (ResetGlobalPropertiesResponse) msg);
                    }
                } else if (functionName.equals(Names.UnregisterAppInterface)) {
                    syncProxyBase.onUnregisterAppInterfaceResponse(hash);
                } else if (functionName.equals(Names.GenericResponse)) {
                    // GenericResponse (Usually and error)
                    final GenericResponse msg = new GenericResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onGenericResponse(
                                        (GenericResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onGenericResponse((GenericResponse) msg);
                    }
                } else if (functionName.equals(Names.Slider)) {
                    // Slider
                    final SliderResponse msg = new SliderResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onSliderResponse(
                                        (SliderResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onSliderResponse((SliderResponse) msg);
                    }
                } else if (functionName.equals(Names.PutFile)) {
                    // PutFile
                    final PutFileResponse msg = new PutFileResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onPutFileResponse(
                                        (PutFileResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onPutFileResponse((PutFileResponse) msg);
                    }
                } else if (functionName.equals(Names.DeleteFile)) {
                    // DeleteFile
                    final DeleteFileResponse msg = new DeleteFileResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onDeleteFileResponse(
                                        (DeleteFileResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onDeleteFileResponse(
                                (DeleteFileResponse) msg);
                    }
                } else if (functionName.equals(Names.ListFiles)) {
                    // ListFiles
                    final ListFilesResponse msg = new ListFilesResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onListFilesResponse(
                                        (ListFilesResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onListFilesResponse(
                                (ListFilesResponse) msg);
                    }
                } else if (functionName.equals(Names.SetAppIcon)) {
                    // SetAppIcon
                    final SetAppIconResponse msg = new SetAppIconResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onSetAppIconResponse(
                                        (SetAppIconResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onSetAppIconResponse(
                                (SetAppIconResponse) msg);
                    }
                } else if (functionName.equals(Names.ScrollableMessage)) {
                    // ScrollableMessage
                    final ScrollableMessageResponse msg =
                            new ScrollableMessageResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onScrollableMessageResponse(
                                        (ScrollableMessageResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onScrollableMessageResponse(
                                (ScrollableMessageResponse) msg);
                    }
                } else if (functionName.equals(Names.ChangeRegistration)) {
                    // ChangeLanguageRegistration
                    final ChangeRegistrationResponse msg =
                            new ChangeRegistrationResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onChangeRegistrationResponse(
                                        (ChangeRegistrationResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onChangeRegistrationResponse(
                                (ChangeRegistrationResponse) msg);
                    }
                } else if (functionName.equals(Names.SetDisplayLayout)) {
                    // SetDisplayLayout
                    final SetDisplayLayoutResponse msg =
                            new SetDisplayLayoutResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onSetDisplayLayoutResponse(
                                        (SetDisplayLayoutResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onSetDisplayLayoutResponse(
                                (SetDisplayLayoutResponse) msg);
                    }
                } else if (functionName.equals(Names.PerformAudioPassThru)) {
                    // PerformAudioPassThru
                    final PerformAudioPassThruResponse msg =
                            new PerformAudioPassThruResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onPerformAudioPassThruResponse(
                                        (PerformAudioPassThruResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onPerformAudioPassThruResponse(
                                (PerformAudioPassThruResponse) msg);
                    }
                } else if (functionName.equals(Names.EndAudioPassThru)) {
                    // EndAudioPassThru
                    final EndAudioPassThruResponse msg =
                            new EndAudioPassThruResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onEndAudioPassThruResponse(
                                        (EndAudioPassThruResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onEndAudioPassThruResponse(
                                (EndAudioPassThruResponse) msg);
                    }
                } else if (functionName.equals(Names.SubscribeVehicleData)) {
                    // SubscribeVehicleData
                    final SubscribeVehicleDataResponse msg =
                            new SubscribeVehicleDataResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onSubscribeVehicleDataResponse(
                                        (SubscribeVehicleDataResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onSubscribeVehicleDataResponse(
                                (SubscribeVehicleDataResponse) msg);
                    }
                } else if (functionName.equals(Names.UnsubscribeVehicleData)) {
                    // UnsubscribeVehicleData
                    final UnsubscribeVehicleDataResponse msg =
                            new UnsubscribeVehicleDataResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onUnsubscribeVehicleDataResponse(
                                        (UnsubscribeVehicleDataResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onUnsubscribeVehicleDataResponse(
                                (UnsubscribeVehicleDataResponse) msg);
                    }
                } else if (functionName.equals(Names.GetVehicleData)) {
                    // GetVehicleData
                    final GetVehicleDataResponse msg =
                            new GetVehicleDataResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onGetVehicleDataResponse(
                                        (GetVehicleDataResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onGetVehicleDataResponse(
                                (GetVehicleDataResponse) msg);
                    }
                } else if (functionName.equals(Names.ReadDID)) {
                    // ReadDID
                    final ReadDIDResponse msg = new ReadDIDResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onReadDIDResponse(
                                        (ReadDIDResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onReadDIDResponse((ReadDIDResponse) msg);
                    }
                } else if (functionName.equals(Names.GetDTCs)) {
                    // GetDTCs
                    final GetDTCsResponse msg = new GetDTCsResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onGetDTCsResponse(
                                        (GetDTCsResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onGetDTCsResponse((GetDTCsResponse) msg);
                    }
                } else if (functionName.equals(Names.AlertManeuver)) {
                    // AlertManeuver
                    final AlertManeuverResponse msg =
                            new AlertManeuverResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onAlertManeuverResponse(
                                        (AlertManeuverResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onAlertManeuverResponse(
                                (AlertManeuverResponse) msg);
                    }
                } else if (functionName.equals(Names.ShowConstantTBT)) {
                    // ShowConstantTBT
                    final ShowConstantTBTResponse msg =
                            new ShowConstantTBTResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onShowConstantTBTResponse(
                                        (ShowConstantTBTResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onShowConstantTBTResponse(
                                (ShowConstantTBTResponse) msg);
                    }
                } else if (functionName.equals(Names.UpdateTurnList)) {
                    // UpdateTurnList
                    final UpdateTurnListResponse msg =
                            new UpdateTurnListResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onUpdateTurnListResponse(
                                        (UpdateTurnListResponse) msg);
                            }
                        });
                    } else {
                        getProxyListener().onUpdateTurnListResponse(
                                (UpdateTurnListResponse) msg);
                    }
                } else if (functionName.equals(Names.SystemRequest)) {
                    final SystemRequestResponse msg =
                            new SystemRequestResponse(hash);
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onSystemRequestResponse(msg);
                            }
                        });
                    } else {
                        getProxyListener().onSystemRequestResponse(msg);
                    }
                } else {
                    try {
                        if (syncProxyBase.getSyncMsgVersion() != null) {
                            DebugTool.logError("Unrecognized response Message: " +
                                    functionName.toString() +
                                    "SYNC Message Version = " + syncProxyBase.getSyncMsgVersion());
                        } else {
                            DebugTool.logError("Unrecognized response Message: " +
                                    functionName.toString());
                        }
                    } catch (SyncException e) {
                        e.printStackTrace();
                    }
                } // end-if

            }
        } else if (messageType.equals(Names.notification)) {
            SyncTrace.logRPCEvent(InterfaceActivityDirection.Receive, new RPCNotification(rpcMsg), SyncProxyBase.SYNC_LIB_TRACE_KEY);
            if (functionName.equals(Names.OnHMIStatus)) {
                // OnHMIStatus

                final OnHMIStatus msg = new OnHMIStatus(hash);
                msg.setFirstRun(new Boolean(syncProxyBase.getFirstTimeFull()));
                if (msg.getHmiLevel() == HMILevel.HMI_FULL) syncProxyBase.setFirstTimeFull(false);

                if (msg.getHmiLevel() != syncProxyBase.getPriorHmiLevel() && msg.getAudioStreamingState() != syncProxyBase.getPriorAudioStreamingState()) {
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onOnHMIStatus((OnHMIStatus) msg);
                            }
                        });
                    } else {
                        getProxyListener().onOnHMIStatus((OnHMIStatus) msg);
                    }
                }
            } else if (functionName.equals(Names.OnCommand)) {
                // OnCommand

                final OnCommand msg = new OnCommand(hash);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onOnCommand((OnCommand) msg);
                        }
                    });
                } else {
                    getProxyListener().onOnCommand((OnCommand) msg);
                }
            } else if (functionName.equals(Names.OnDriverDistraction)) {
                // OnDriverDistration

                final OnDriverDistraction msg = new OnDriverDistraction(hash);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onOnDriverDistraction(msg);
                        }
                    });
                } else {
                    getProxyListener().onOnDriverDistraction(msg);
                }
            } else if (functionName.equals(Names.OnEncodedSyncPData)) {
                // OnEncodedSyncPData

                final OnEncodedSyncPData msg = new OnEncodedSyncPData(hash);

                // If url is null, then send notification to the app, otherwise, send to URL
                if (msg.getUrl() == null) {
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onOnEncodedSyncPData(msg);
                            }
                        });
                    } else {
                        getProxyListener().onOnEncodedSyncPData(msg);
                    }
                } else {
                    // URL has data, attempt to post request to external server
                    Thread handleOffboardSyncTransmissionTread = new Thread() {
                        @Override
                        public void run() {
                            syncProxyBase.sendEncodedSyncPDataToUrl(msg.getUrl(), msg.getData(), msg.getTimeout());
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
                    if (getCallbackToUIThread()) {
                        // Run in UI thread
                        getMainUIHandler().post(new Runnable() {
                            @Override
                            public void run() {
                                getProxyListener().onOnSyncPData(msg);
                            }
                        });
                    } else {
                        getProxyListener().onOnSyncPData(msg);
                    }
                } else { //url not null, send to url
                    Log.i("pt", "send syncp to url");
                    // URL has data, attempt to post request to external server
                    Thread handleOffboardSyncTransmissionTread = new Thread() {
                        @Override
                        public void run() {
                            syncProxyBase.sendSyncPDataToUrl(msg.getUrl(), msg.getSyncPData(), msg.getTimeout());
                        }
                    };

                    handleOffboardSyncTransmissionTread.start();
                }
            } else if (functionName.equals(Names.OnPermissionsChange)) {
                //OnPermissionsChange

                final OnPermissionsChange msg = new OnPermissionsChange(hash);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onOnPermissionsChange(msg);
                        }
                    });
                } else {
                    getProxyListener().onOnPermissionsChange(msg);
                }
            } else if (functionName.equals(Names.OnTBTClientState)) {
                // OnTBTClientState

                final OnTBTClientState msg = new OnTBTClientState(hash);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onOnTBTClientState(msg);
                        }
                    });
                } else {
                    getProxyListener().onOnTBTClientState(msg);
                }
            } else if (functionName.equals(Names.OnButtonPress)) {
                // OnButtonPress

                final OnButtonPress msg = new OnButtonPress(hash);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onOnButtonPress((OnButtonPress) msg);
                        }
                    });
                } else {
                    getProxyListener().onOnButtonPress((OnButtonPress) msg);
                }
            } else if (functionName.equals(Names.OnButtonEvent)) {
                // OnButtonEvent

                final OnButtonEvent msg = new OnButtonEvent(hash);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onOnButtonEvent((OnButtonEvent) msg);
                        }
                    });
                } else {
                    getProxyListener().onOnButtonEvent((OnButtonEvent) msg);
                }
            } else if (functionName.equals(Names.OnLanguageChange)) {
                // OnLanguageChange

                final OnLanguageChange msg = new OnLanguageChange(hash);
                syncProxyBase.setLastLanguageChange(msg);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onOnLanguageChange(
                                    (OnLanguageChange) msg);
                        }
                    });
                } else {
                    getProxyListener().onOnLanguageChange((OnLanguageChange) msg);
                }
            } else if (functionName.equals(Names.OnAudioPassThru)) {
                // OnAudioPassThru
                final OnAudioPassThru msg = new OnAudioPassThru(hash);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onOnAudioPassThru(
                                    (OnAudioPassThru) msg);
                        }
                    });
                } else {
                    getProxyListener().onOnAudioPassThru((OnAudioPassThru) msg);
                }
            } else if (functionName.equals(Names.OnVehicleData)) {
                // OnVehicleData
                final OnVehicleData msg = new OnVehicleData(hash);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onOnVehicleData((OnVehicleData) msg);
                        }
                    });
                } else {
                    getProxyListener().onOnVehicleData((OnVehicleData) msg);
                }
            } else if (functionName.equals(Names.OnTouchEvent)) {
                // OnTouchEvent
                final OnTouchEvent msg = new OnTouchEvent(hash);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onOnTouchEvent((OnTouchEvent) msg);
                        }
                    });
                } else {
                    getProxyListener().onOnTouchEvent((OnTouchEvent) msg);
                }
            } else if (functionName.equals(Names.OnKeyboardInput)) {
                // OnKeyboardInput
                final OnKeyboardInput msg = new OnKeyboardInput(hash);
                if (getCallbackToUIThread()) {
                    // Run in UI thread
                    getMainUIHandler().post(new Runnable() {
                        @Override
                        public void run() {
                            getProxyListener().onKeyboardInput(
                                    (OnKeyboardInput) msg);
                        }
                    });
                } else {
                    getProxyListener().onKeyboardInput((OnKeyboardInput) msg);
                }
            } else if (functionName.equals(Names.OnSystemRequest)) {
                syncProxyBase.handleOnSystemRequest(hash);
            } else if (functionName.equals(Names.OnAppInterfaceUnregistered)) {
                // OnAppInterfaceUnregistered
                syncProxyBase.setAppInterfaceRegisterd(false);
                synchronized (syncProxyBase.APP_INTERFACE_REGISTERED_LOCK) {
                    syncProxyBase.APP_INTERFACE_REGISTERED_LOCK.notify();
                }

                final OnAppInterfaceUnregistered msg = new OnAppInterfaceUnregistered(hash);

                if (syncProxyBase.getAdvancedLifecycleManagementEnabled()) {
                    if (msg.getReason() == AppInterfaceUnregisteredReason.LANGUAGE_CHANGE) {
                        if (getCallbackToUIThread()) {
                            // Run in UI thread
                            getMainUIHandler().post(new Runnable() {
                                @Override
                                public void run() {
                                    getProxyListener().onAppUnregisteredAfterLanguageChange(syncProxyBase.getLastLanguageChange());
                                }
                            });
                        } else {
                            getProxyListener().onAppUnregisteredAfterLanguageChange(syncProxyBase.getLastLanguageChange());
                        }
                    } else if (msg.getReason() != null) {
                        if (getCallbackToUIThread()) {
                            // Run in UI thread
                            getMainUIHandler().post(new Runnable() {
                                @Override
                                public void run() {
                                    getProxyListener().onAppUnregisteredReason(msg.getReason());
                                }
                            });
                        } else {
                            getProxyListener().onAppUnregisteredReason(msg.getReason());
                        }
                    } else {
                        // This requires the proxy to be cycled
                        if (syncProxyBase.getCurrentTransportType() == TransportType.BLUETOOTH) {
                            syncProxyBase.cycleProxy(SyncDisconnectedReason.convertAppInterfaceUnregisteredReason(msg.getReason()));
                        } else {
                            Log.e(this.getClass().getName(), "HandleRPCMessage. No cycle required if transport is TCP");
                        }
                        syncProxyBase.notifyOnAppInterfaceUnregistered(msg);
                    }
                } else {
                    syncProxyBase.notifyOnAppInterfaceUnregistered(msg);
                }
            } else {

                try {
                    if (syncProxyBase.getSyncMsgVersion() != null) {
                        DebugTool.logInfo("Unrecognized notification Message: " + functionName.toString() +
                                " connected to SYNC using message version: " + syncProxyBase.getSyncMsgVersion().getMajorVersion() + "." + syncProxyBase.getSyncMsgVersion().getMinorVersion());
                    } else {
                        DebugTool.logInfo("Unrecognized notification Message: " + functionName.toString());
                    }
                } catch (SyncException e) {
                    e.printStackTrace();
                }
            } // end-if
        } // end-if notification

        SyncTrace.logProxyEvent("Proxy received RPC Message: " + functionName, SyncProxyBase.SYNC_LIB_TRACE_KEY);
    }

    private Handler getMainUIHandler() {
        return syncProxyBase.getMainUIHandler();
    }

    private IProxyListenerBase getProxyListener() {
        return syncProxyBase.getProxyListener();
    }

    private Boolean getCallbackToUIThread() {
        return syncProxyBase.getCallbackToUIThread();
    }

}
