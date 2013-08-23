package com.ford.syncV4.android.constants;

import java.util.Vector;

import android.util.Log;

import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.TTSChunkFactory;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.AddSubMenu;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.AlertManeuver;
import com.ford.syncV4.proxy.rpc.ChangeRegistration;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteCommand;
import com.ford.syncV4.proxy.rpc.DeleteFile;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteSubMenu;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.proxy.rpc.EndAudioPassThru;
import com.ford.syncV4.proxy.rpc.GetDTCs;
import com.ford.syncV4.proxy.rpc.GetVehicleData;
import com.ford.syncV4.proxy.rpc.ListFiles;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThru;
import com.ford.syncV4.proxy.rpc.PerformInteraction;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.ReadDID;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.ResetGlobalProperties;
import com.ford.syncV4.proxy.rpc.ScrollableMessage;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.SetDisplayLayout;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimer;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.ShowConstantTBT;
import com.ford.syncV4.proxy.rpc.Slider;
import com.ford.syncV4.proxy.rpc.Speak;
import com.ford.syncV4.proxy.rpc.StartTime;
import com.ford.syncV4.proxy.rpc.SubscribeButton;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterface;
import com.ford.syncV4.proxy.rpc.UnsubscribeButton;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.UpdateTurnList;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;
	
public class TestRPC {
	Object rpc;
	
	public TestRPC(String functionName) {
		if (functionName.equalsIgnoreCase(Names.RegisterAppInterface)) {
			rpc = new RegisterAppInterface();
		} else if (functionName.equalsIgnoreCase(Names.UnregisterAppInterface)) {
			rpc = new UnregisterAppInterface();
		} else if (functionName.equalsIgnoreCase(Names.SetGlobalProperties)) {
			rpc = new SetGlobalProperties();
		} else if (functionName.equalsIgnoreCase(Names.ResetGlobalProperties)) {
			rpc = new ResetGlobalProperties();
		} else if (functionName.equalsIgnoreCase(Names.AddCommand)) {
			rpc = new AddCommand();
		} else if (functionName.equalsIgnoreCase(Names.DeleteCommand)) {
			rpc = new DeleteCommand();
		} else if (functionName.equalsIgnoreCase(Names.AddSubMenu)) {
			rpc = new AddSubMenu();
		} else if (functionName.equalsIgnoreCase(Names.DeleteSubMenu)) {
			rpc = new DeleteSubMenu();
		} else if (functionName.equalsIgnoreCase(Names.CreateInteractionChoiceSet)) {
			rpc = new CreateInteractionChoiceSet();
		} else if (functionName.equalsIgnoreCase(Names.PerformInteraction)) {
			rpc = new PerformInteraction();
		} else if (functionName.equalsIgnoreCase(Names.DeleteInteractionChoiceSet)) {
			rpc = new DeleteInteractionChoiceSet();
		} else if (functionName.equalsIgnoreCase(Names.Alert)) {
			rpc = new Alert();
		} else if (functionName.equalsIgnoreCase(Names.Show)) {
			rpc = new Show();
		} else if (functionName.equalsIgnoreCase(Names.Speak)) {
			rpc = new Speak();
		} else if (functionName.equalsIgnoreCase(Names.SetMediaClockTimer)) {
			rpc = new SetMediaClockTimer();
		} else if (functionName.equalsIgnoreCase(Names.EncodedSyncPData)) {
			rpc = new EncodedSyncPData();
		} else if (functionName.equalsIgnoreCase(Names.PerformAudioPassThru)) {
			rpc = new PerformAudioPassThru();
		} else if (functionName.equalsIgnoreCase(Names.EndAudioPassThru)) {
			rpc = new EndAudioPassThru();
		} else if (functionName.equalsIgnoreCase(Names.SubscribeButton)) {
			rpc = new SubscribeButton();
		} else if (functionName.equalsIgnoreCase(Names.UnsubscribeButton)) {
			rpc = new UnsubscribeButton();
		} else if (functionName.equalsIgnoreCase(Names.SubscribeVehicleData)) {
			rpc = new SubscribeVehicleData();
	    } else if (functionName.equalsIgnoreCase(Names.UnsubscribeVehicleData)) {
			rpc = new UnsubscribeVehicleData();
	    } else if (functionName.equalsIgnoreCase(Names.GetVehicleData)) {
			rpc = new GetVehicleData();
	    } else if (functionName.equalsIgnoreCase(Names.ReadDID)) {
			rpc = new ReadDID();
	    } else if (functionName.equalsIgnoreCase(Names.GetDTCs)) {
			rpc = new GetDTCs();
	    } else if (functionName.equalsIgnoreCase(Names.ScrollableMessage)) {
			rpc = new ScrollableMessage();
	    } else if (functionName.equalsIgnoreCase(Names.Slider)) {
			rpc = new Slider();
	    } else if (functionName.equalsIgnoreCase(Names.ShowConstantTBT)) {
			rpc = new ShowConstantTBT();
	    } else if (functionName.equalsIgnoreCase(Names.AlertManeuver)) {
			rpc = new AlertManeuver();
	    } else if (functionName.equalsIgnoreCase(Names.UpdateTurnList)) {
			rpc = new UpdateTurnList();
	    } else if (functionName.equalsIgnoreCase(Names.ChangeRegistration)) {
			rpc = new ChangeRegistration();
	    } else if (functionName.equalsIgnoreCase(Names.PutFile)) {
			rpc = new PutFile();
	    } else if (functionName.equalsIgnoreCase(Names.DeleteFile)) {
			rpc = new DeleteFile();
	    } else if (functionName.equalsIgnoreCase(Names.ListFiles)) {
			rpc = new ListFiles();
	    } else if (functionName.equalsIgnoreCase(Names.SetAppIcon)) {
			rpc = new SetAppIcon();
	    } else if (functionName.equalsIgnoreCase(Names.SetDisplayLayout)) {
			rpc = new SetDisplayLayout();
		} else if (functionName.equalsIgnoreCase("ClearMediaClockTimer")) {
			rpc = new Show();
			((Show) rpc).setMainField1(null);
			((Show) rpc).setMainField2(null);
			((Show) rpc).setStatusBar(null);
			((Show) rpc).setMediaClock("     ");
			((Show) rpc).setMediaTrack(null);
			((Show) rpc).setAlignment(null);
		} else if (functionName.equalsIgnoreCase("PauseMediaClockTimer")) {
			rpc = new SetMediaClockTimer();
			StartTime startTime = new StartTime();
			startTime.setHours(0);
			startTime.setMinutes(0);
			startTime.setSeconds(0);
			((SetMediaClockTimer) rpc).setStartTime(startTime);
			((SetMediaClockTimer) rpc).setUpdateMode(UpdateMode.PAUSE);
		} else if (functionName.equalsIgnoreCase("ResumeMediaClockTimer")) {
			rpc = new SetMediaClockTimer();
			StartTime startTime = new StartTime();
			startTime.setHours(0);
			startTime.setMinutes(0);
			startTime.setSeconds(0);
			((SetMediaClockTimer) rpc).setStartTime(startTime);
			((SetMediaClockTimer) rpc).setUpdateMode(UpdateMode.RESUME);
		}
	}
	
	public RPCMessage getRPC() {
		return (RPCMessage) rpc;
	}
	
	public void setField(String field, String value) {
		if (field.equalsIgnoreCase(Names.playTone)) {
			if (value.equalsIgnoreCase("true")) ((RPCMessage) rpc).setParameters(field, true);
			else if (value.equalsIgnoreCase("false")) ((RPCMessage) rpc).setParameters(field, false);
		} else if (field.equalsIgnoreCase(Names.correlationID)) {
			try {((RPCRequest) rpc).setCorrelationID(Integer.parseInt(value));} 
			catch (Exception e) {Log.e("parser", "Unable to parse Integer");}
		} else if (field.equalsIgnoreCase(Names.choiceID)
				|| field.equalsIgnoreCase(Names.cmdID)
				|| field.equalsIgnoreCase(Names.correlationID)
				|| field.equalsIgnoreCase(Names.duration)
				|| field.equalsIgnoreCase(Names.menuID)
				|| field.equalsIgnoreCase(Names.parentID)
				|| field.equalsIgnoreCase(Names.position)
				|| field.equalsIgnoreCase(Names.timeout)) {
			try {((RPCMessage) rpc).setParameters(field, Integer.parseInt(value));} 
			catch (Exception e) {Log.e("parser", "Unable to parse Integer");}
		} else if (field.equalsIgnoreCase(Names.hours)
				|| field.equalsIgnoreCase(Names.minutes)
				|| field.equalsIgnoreCase(Names.seconds)) {
			StartTime startTime = (StartTime) ((RPCMessage) rpc).getParameters(Names.startTime);
			if (startTime == null) startTime = new StartTime();
			try {
			if (field.equalsIgnoreCase(Names.hours)) startTime.setHours(Integer.parseInt(value));
			if (field.equalsIgnoreCase(Names.minutes)) startTime.setMinutes(Integer.parseInt(value));
			if (field.equalsIgnoreCase(Names.seconds)) startTime.setSeconds(Integer.parseInt(value));
			} catch (Exception e) {Log.e("parser", "Unable to parse Integer");}
		} else if (field.equalsIgnoreCase(Names.interactionChoiceSetID)
				|| field.equalsIgnoreCase(Names.interactionChoiceSetIDList)) {
			try {
			if (((RPCMessage) rpc).getFunctionName().equalsIgnoreCase(Names.CreateInteractionChoiceSet)
					|| ((RPCMessage) rpc).getFunctionName().equalsIgnoreCase(Names.DeleteInteractionChoiceSet)) {
				try {((RPCMessage) rpc).setParameters(field, Integer.parseInt(value));} 
				catch (Exception e) {Log.e("parser", "Unable to parse Integer");}
			} else if (((RPCMessage) rpc).getFunctionName().equalsIgnoreCase(Names.PerformInteraction)) {
				Vector<Integer> interactionChoiceSetIDs = new Vector<Integer>();
				interactionChoiceSetIDs.add(Integer.parseInt(value));
				((RPCMessage) rpc).setParameters(Names.interactionChoiceSetIDList, interactionChoiceSetIDs);
			}} catch (Exception e) {Log.e("parser", "Unable to parse Integer");}
		} else if (field.equalsIgnoreCase(Names.alertText1)
				|| field.equalsIgnoreCase(Names.alertText2)
				|| field.equalsIgnoreCase(Names.initialText)
				|| field.equalsIgnoreCase(Names.mainField1)
				|| field.equalsIgnoreCase(Names.mainField2)
				|| field.equalsIgnoreCase(Names.mediaClock)
				|| field.equalsIgnoreCase(Names.mediaTrack)
				|| field.equalsIgnoreCase(Names.menuName)
				|| field.equalsIgnoreCase(Names.statusBar)
				|| field.equalsIgnoreCase(Names.data)) {
			((RPCMessage) rpc).setParameters(field ,value);
		} else if (field.equalsIgnoreCase(Names.helpPrompt)
				|| field.equalsIgnoreCase(Names.initialPrompt)
				|| field.equalsIgnoreCase(Names.timeoutPrompt)
				|| field.equalsIgnoreCase(Names.ttsChunks)) {
			Vector<TTSChunk> ttsChunks = TTSChunkFactory.createSimpleTTSChunks(value);
			((RPCMessage) rpc).setParameters(field, ttsChunks);
		} else if (field.equalsIgnoreCase(Names.properties)) {
			Vector<GlobalProperty> globalProperties = new Vector<GlobalProperty>();
			globalProperties.add(GlobalProperty.valueForString(value));
			((RPCMessage) rpc).setParameters(field, globalProperties);
		} else if (field.equalsIgnoreCase(Names.vrCommands)) {
			Vector<String> vrCommands = new Vector<String>();
			vrCommands.add(new String(value));
			((RPCMessage) rpc).setParameters(field, vrCommands);
		} else if (field.equalsIgnoreCase(Names.buttonName)) {
			((RPCMessage) rpc).setParameters(field, ButtonName.valueForString(value));
		} else if (field.equalsIgnoreCase(Names.interactionMode)) {
			((RPCMessage) rpc).setParameters(field, InteractionMode.valueForString(value));
		} else if (field.equalsIgnoreCase(Names.alignment)) {
			((RPCMessage) rpc).setParameters(field, TextAlignment.valueForString(value));
		} else if (field.equalsIgnoreCase(Names.updateMode)) {
			((RPCMessage) rpc).setParameters(field, UpdateMode.valueForString(value));
		} else {
			((RPCMessage) rpc).setParameters(field, value);
		}
	}
	
	public void setChoiceSet(Vector<Choice> choiceSet) {
		((RPCMessage) rpc).setParameters(Names.choiceSet, choiceSet);
	}
	
	public void setStartTime(StartTime startTime) {
		((RPCMessage) rpc).setParameters(Names.startTime, startTime);
	}
}