package com.ford.syncV4.protocol.enums;

import java.util.ArrayList;

import com.ford.syncV4.proxy.constants.Names;

public class FunctionID {
	ArrayList<String> functionID = new ArrayList<String>();
	
	public FunctionID() {
		functionID.add(Names.request);
		functionID.add(Names.response);
		functionID.add(Names.notification);
		functionID.add(Names.function_name);
		functionID.add(Names.parameters);
		functionID.add(Names.RegisterAppInterface);
		functionID.add(Names.UnregisterAppInterface);
		functionID.add(Names.Alert);
		functionID.add(Names.Show);
		functionID.add(Names.Speak);
		functionID.add(Names.AddCommand);
		functionID.add(Names.DeleteCommand);
		functionID.add(Names.AddSubMenu);
		functionID.add(Names.DeleteSubMenu);
		functionID.add(Names.CreateInteractionChoiceSet);
		functionID.add(Names.DeleteInteractionChoiceSet);
		functionID.add(Names.PerformInteraction);
		functionID.add(Names.DialNumber);
		functionID.add(Names.EncodedSyncPData);
		functionID.add(Names.SubscribeButton);
		functionID.add(Names.UnsubscribeButton);
		functionID.add(Names.SubscribeVehicleData);
		functionID.add(Names.UnsubscribeVehicleData);
		functionID.add(Names.SetMediaClockTimer);
		functionID.add(Names.SetGlobalProperties);
		functionID.add(Names.GenericResponse);
		functionID.add(Names.GetDID);
		functionID.add(Names.GetDTCs);
		functionID.add(Names.GetFile);
		functionID.add(Names.PutFile);
		functionID.add(Names.EndAudioCapture);
		functionID.add(Names.GetVehicleData);
		functionID.add(Names.Slider);
		functionID.add(Names.ResetGlobalProperties);
		functionID.add(Names.PerformAudioCapture);
		functionID.add(Names.OnCommand);
		functionID.add(Names.OnDataPublished);
		functionID.add(Names.OnButtonPress);
		functionID.add(Names.OnButtonEvent);
		functionID.add(Names.OnHMIStatus);
		functionID.add(Names.OnTBTClientState);
		functionID.add(Names.OnEncodedSyncPData);
		functionID.add(Names.OnDriverDistraction);
		functionID.add(Names.OnAppInterfaceUnregistered);
		functionID.add(Names.OnProxyClosed);
		functionID.add(Names.OnProxyError);
		functionID.add(Names.OnProxyOpened);
		functionID.add(Names.OnProxyUnusable);
		functionID.add(Names.OnHMILevelChange);
		functionID.add(Names.OnSyncChoiceChosen);
		functionID.add(Names.OnPermissionsChange);
		functionID.add(Names.PermissionGroupName);
		functionID.add(Names.PermissionGroupStatus);
		functionID.add(Names.PermissionGroupItems);
	}
	
	public String getFunctionName(int i) {
		return functionID.get(i);
	}
	
	public int getFunctionID(String functionName) {
		return functionID.indexOf(functionName);
	}
}