package com.ford.syncV4.android.constants;

import java.util.ArrayList;

import com.ford.syncV4.proxy.constants.Names;

public class AcceptedRPC {
	ArrayList<String> acceptedRPC = new ArrayList<String>();
	
	public AcceptedRPC() {
		acceptedRPC.add(Names.RegisterAppInterface);
		acceptedRPC.add(Names.UnregisterAppInterface);
		acceptedRPC.add(Names.SetGlobalProperties);
		acceptedRPC.add(Names.ResetGlobalProperties);
		acceptedRPC.add(Names.AddCommand);
		acceptedRPC.add(Names.DeleteCommand);
		acceptedRPC.add(Names.AddSubMenu);
		acceptedRPC.add(Names.DeleteSubMenu);
		acceptedRPC.add(Names.CreateInteractionChoiceSet);
		acceptedRPC.add(Names.PerformInteraction);
		acceptedRPC.add(Names.DeleteInteractionChoiceSet);
		acceptedRPC.add(Names.Alert);
		acceptedRPC.add(Names.Show);
		acceptedRPC.add(Names.Speak);
		acceptedRPC.add(Names.SetMediaClockTimer);
		acceptedRPC.add(Names.EncodedSyncPData);
	    acceptedRPC.add(Names.DialNumber);
	    acceptedRPC.add(Names.PerformAudioPassThru);
	    acceptedRPC.add(Names.EndAudioPassThru);
		acceptedRPC.add(Names.SubscribeButton);
		acceptedRPC.add(Names.UnsubscribeButton);
	    acceptedRPC.add(Names.SubscribeVehicleData);
	    acceptedRPC.add(Names.UnsubscribeVehicleData);
	    acceptedRPC.add(Names.GetVehicleData);
	    acceptedRPC.add(Names.ReadDID);
	    acceptedRPC.add(Names.GetDTCs);
	    acceptedRPC.add(Names.ScrollableMessage);
	    acceptedRPC.add(Names.Slider);
	    acceptedRPC.add(Names.ShowConstantTBT);
	    acceptedRPC.add(Names.AlertManeuver);
	    acceptedRPC.add(Names.UpdateTurnList);
	    acceptedRPC.add(Names.ChangeRegistration);
	    acceptedRPC.add(Names.PutFile);
	    acceptedRPC.add(Names.DeleteFile);
	    acceptedRPC.add(Names.ListFiles);
	    acceptedRPC.add(Names.SetAppIcon);
	    acceptedRPC.add(Names.SetDisplayLayout);
		
		acceptedRPC.add("ClearMediaClockTimer");
		acceptedRPC.add("PauseMediaClockTimer");
		acceptedRPC.add("ResumeMediaClockTimer");
	}
	
	public String getFunctionName(int i) {
		return acceptedRPC.get(i);
	}
	
	public int getFunctionID(String functionName) {
		return acceptedRPC.indexOf(functionName);
	}
	
	public boolean isAcceptedRPC(String rpc) {
		return acceptedRPC.contains(rpc);
	}
}