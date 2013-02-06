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
		functionIds[1] = Names.RegisterAppInterface;
		functionIds[2] = Names.UnregisterAppInterface;
		functionIds[3] = Names.SetGlobalProperties;
		functionIds[4] = Names.ResetGlobalProperties;
		functionIds[5] = Names.AddCommand;
		functionIds[6] = Names.DeleteCommand;
		functionIds[7] = Names.AddSubMenu;
		functionIds[8] = Names.DeleteSubMenu;
		functionIds[9] = Names.CreateInteractionChoiceSet;
		functionIds[10] = Names.PerformInteraction;
		functionIds[11] = Names.DeleteInteractionChoiceSet;
		functionIds[12] = Names.Alert;
		functionIds[13] = Names.Show;
		functionIds[14] = Names.Speak;
		functionIds[15] = Names.SetMediaClockTimer;
		functionIds[16] = Names.EncodedSyncPData;
		functionIds[17] = Names.DialNumber;
		functionIds[18] = Names.PerformAudioPassThru;
		functionIds[19] = Names.EndAudioPassThru;
		functionIds[20] = Names.SubscribeButton;
		functionIds[21] = Names.UnsubscribeButton;
		functionIds[22] = Names.SubscribeVehicleData;
		functionIds[23] = Names.UnsubscribeVehicleData;
		functionIds[24] = Names.GetVehicleData;
		functionIds[25] = Names.ReadDID;
		functionIds[26] = Names.GetDTCs;
		functionIds[27] = Names.ScrollableMessage;
		functionIds[28] = Names.Slider;
		functionIds[29] = Names.ShowConstantTBT;
		functionIds[30] = Names.AlertManeuver;
		functionIds[31] = Names.UpdateTurnList;
		functionIds[32] = Names.ChangeRegistration;
		functionIds[33] = Names.GenericResponse;
		functionIds[34] = Names.PutFile;
		functionIds[35] = Names.DeleteFile;
		functionIds[36] = Names.ListFiles;
		functionIds[37] = Names.SetAppIcon;
		functionIds[38] = Names.SetDisplayLayout;
		functionIds[39] = Names.OnHMIStatus;
		functionIds[40] = Names.OnAppInterfaceUnregistered;
		functionIds[41] = Names.OnButtonEvent;
		functionIds[42] = Names.OnButtonPress;
		functionIds[43] = Names.OnVehicleData;
		functionIds[44] = Names.OnCommand;
		functionIds[45] = Names.OnEncodedSyncPData;
		functionIds[46] = Names.OnTBTClientState;
		functionIds[47] = Names.OnDriverDistraction;
		functionIds[48] = Names.OnPermissionsChange;
		functionIds[49] = Names.OnAudioPassThru;
		functionIds[50] = Names.OnLanguageChange;	
		
		functionID = new ArrayList<String>(Arrays.asList(functionIds));
	}
	
	static public int getFunctionID(String functionName) {
		if (functionID == null) {
			initFunctionIds();
		}
		return functionID.indexOf(functionName);
	}
}