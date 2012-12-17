package com.ford.syncV4.protocol.enums;

import java.util.ArrayList;
import java.util.Arrays;

import com.ford.syncV4.proxy.constants.Names;

public class FunctionID {
	static ArrayList<String> functionID = null;
	
	public FunctionID() {
	}
	
	static public String getFunctionName(int i) {
		if (functionID == null) {
			initFunctionIds();
		}
		return functionID.get(i);
	}
	
	static public void initFunctionIds() {
		String [] functionIds = new String[60];
		functionIds[5] = Names.RegisterAppInterface;
		functionIds[6] = Names.UnregisterAppInterface;
		functionIds[7] = Names.SetGlobalProperties;
		functionIds[8] = Names.ResetGlobalProperties;
		functionIds[9] = Names.AddCommand;
		functionIds[10] = Names.DeleteCommand;
		functionIds[11] = Names.AddSubMenu;
		functionIds[12] = Names.DeleteSubMenu;
		functionIds[13] = Names.CreateInteractionChoiceSet;
		functionIds[14] = Names.PerformInteraction;
		functionIds[15] = Names.DeleteInteractionChoiceSet;
		functionIds[16] = Names.Alert;
		functionIds[17] = Names.Show;
		functionIds[18] = Names.Speak;
		functionIds[19] = Names.SetMediaClockTimer;
		functionIds[20] = Names.EncodedSyncPData;
		functionIds[21] = Names.DialNumber;
		functionIds[22] = Names.PerformAudioPassThru;
		functionIds[23] = Names.EndAudioPassThru;
		functionIds[24] = Names.SubscribeButton;
		functionIds[25] = Names.UnsubscribeButton;
		functionIds[26] = Names.SubscribeVehicleData;
		functionIds[27] = Names.UnsubscribeVehicleData;
		functionIds[28] = Names.GetVehicleData;
		functionIds[29] = Names.ReadDID;
		functionIds[30] = Names.GetDTCs;
		functionIds[31] = Names.ScrollableMessage;
		functionIds[32] = Names.Slider;
		functionIds[33] = Names.ShowConstantTBT;
		functionIds[34] = Names.AlertManeuver;
		functionIds[35] = Names.UpdateTurnList;
		functionIds[36] = Names.ChangeRegistration;
		functionIds[37] = Names.GenericResponse;
		functionIds[38] = Names.PutFile;
		functionIds[39] = Names.DeleteFile;
		functionIds[40] = Names.ListFiles;
		functionIds[41] = Names.SetAppIcon;
		functionIds[42] = Names.SetDisplayLayout;
		functionIds[43] = Names.OnHMIStatus;
		functionIds[44] = Names.OnAppInterfaceUnregistered;
		functionIds[45] = Names.OnButtonEvent;
		functionIds[46] = Names.OnButtonPress;
		functionIds[47] = Names.OnVehicleData;
		functionIds[48] = Names.OnCommand;
		functionIds[49] = Names.OnEncodedSyncPData;
		functionIds[50] = Names.OnTBTClientState;
		functionIds[51] = Names.OnDriverDistraction;
		functionIds[52] = Names.OnPermissionsChange;
		functionIds[53] = Names.OnAudioPassThru;
		functionIds[54] = Names.OnLanguageChange;	
		
		functionID = new ArrayList<String>(Arrays.asList(functionIds));
	}
	
	static public int getFunctionID(String functionName) {
		if (functionID == null) {
			initFunctionIds();
		}
		return functionID.indexOf(functionName);
	}
}