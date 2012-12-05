package com.ford.syncV4.proxy;

import java.util.Vector;

import com.ford.syncV4.proxy.rpc.*;
import com.ford.syncV4.proxy.rpc.enums.*;

public class RPCRequestFactory {

	public static final int NGN_MEDIA_SCREEN_APP_NAME_MAX_LENGTH = 5;
	public static final int SYNC_MSG_MAJOR_VERSION = 1;
	public static final int SYNC_MSG_MINOR_VERSION = 0;

	public static EncodedSyncPData buildEncodedSyncPData(
			Vector<String> data, Integer correlationID) {
		
		if(data == null) return null;
		
		EncodedSyncPData msg = new EncodedSyncPData();
		msg.setCorrelationID(correlationID);
		msg.setData(data);
		return msg;
	}
	
	public static AddCommand buildAddCommand(Integer commandID,
			String menuText, Integer parentID, Integer position,
			Vector<String> vrCommands, Integer correlationID) {
		AddCommand msg = new AddCommand();
		msg.setCorrelationID(correlationID);
		msg.setCmdID(commandID);
		msg.setVrCommands(vrCommands);
		
		if(menuText != null || parentID != null || position != null) {
			MenuParams menuParams = new MenuParams();
			menuParams.setMenuName(menuText);
			menuParams.setPosition(position);
			menuParams.setParentID(parentID);
			msg.setMenuParams(menuParams);
		}
		
		return msg;
	}
	
	public static AddCommand buildAddCommand(Integer commandID,
			String menuText, Vector<String> vrCommands, Integer correlationID) {
		AddCommand msg = buildAddCommand(commandID, menuText, null, null,
				vrCommands, correlationID);
		return msg;
	}
	
	public static AddCommand buildAddCommand(Integer commandID,
			Vector<String> vrCommands, Integer correlationID) {
		AddCommand msg = new AddCommand();
		msg.setCorrelationID(correlationID);
		msg.setCmdID(commandID);
		msg.setVrCommands(vrCommands);

		return msg;
	}

	public static AddSubMenu buildAddSubMenu(Integer menuID, String menuName,
			Integer correlationID) {
		AddSubMenu msg = buildAddSubMenu(menuID, menuName, null, correlationID);
		return msg;
	}

	public static AddSubMenu buildAddSubMenu(Integer menuID, String menuName,
			Integer position, Integer correlationID) {
		AddSubMenu msg = new AddSubMenu();
		msg.setCorrelationID(correlationID);
		msg.setMenuName(menuName);
		msg.setMenuID(menuID);
		msg.setPosition(position);

		return msg;
	}
	
	public static Alert buildAlert(String ttsText, Boolean playTone,
			Integer correlationID) {
		Vector<TTSChunk> chunks = TTSChunkFactory
				.createSimpleTTSChunks(ttsText);
		Alert msg = buildAlert(chunks, null, null, playTone, null,
				correlationID);
		return msg;
	}
	
	public static Alert buildAlert(String alertText1, String alertText2,
			Integer duration, Integer correlationID) {
		Alert msg = buildAlert((Vector<TTSChunk>) null, alertText1, alertText2,
				null, duration, correlationID);
		return msg;
	}
	
	public static Alert buildAlert(String ttsText, String alertText1,
			String alertText2, Boolean playTone, Integer duration,
			Integer correlationID) {
		Vector<TTSChunk> chunks = TTSChunkFactory
				.createSimpleTTSChunks(ttsText);
		Alert msg = buildAlert(chunks, alertText1, alertText2, playTone,
				duration, correlationID);
		return msg;
	}
	
	public static Alert buildAlert(Vector<TTSChunk> chunks, Boolean playTone,
			Integer correlationID) {
		Alert msg = buildAlert(chunks, null, null, playTone, null,
				correlationID);
		return msg;
	}
	
	public static Alert buildAlert(Vector<TTSChunk> ttsChunks,
			String alertText1, String alertText2, Boolean playTone,
			Integer duration, Integer correlationID) {
		Alert msg = new Alert();
		msg.setCorrelationID(correlationID);
		msg.setAlertText1(alertText1);
		msg.setAlertText2(alertText2);
		msg.setDuration(duration);
		msg.setPlayTone(playTone);
		msg.setTtsChunks(ttsChunks);

		return msg;
	}
	
	public static CreateInteractionChoiceSet buildCreateInteractionChoiceSet(
			Vector<Choice> choiceSet, Integer interactionChoiceSetID,
			Integer correlationID) {
		CreateInteractionChoiceSet msg = new CreateInteractionChoiceSet();
		msg.setChoiceSet(choiceSet);
		msg.setInteractionChoiceSetID(interactionChoiceSetID);
		msg.setCorrelationID(correlationID);
		return msg;
	}
	
	public static DeleteCommand buildDeleteCommand(Integer commandID,
			Integer correlationID) {
		DeleteCommand msg = new DeleteCommand();
		msg.setCmdID(commandID);
		msg.setCorrelationID(correlationID);
		return msg;
	}
	
	public static DeleteInteractionChoiceSet buildDeleteInteractionChoiceSet(
			Integer interactionChoiceSetID, Integer correlationID) {
		DeleteInteractionChoiceSet msg = new DeleteInteractionChoiceSet();
		msg.setInteractionChoiceSetID(interactionChoiceSetID);
		msg.setCorrelationID(correlationID);

		return msg;
	}
	
	public static DeleteSubMenu buildDeleteSubMenu(Integer menuID,
			Integer correlationID) {
		DeleteSubMenu msg = new DeleteSubMenu();
		msg.setCorrelationID(correlationID);
		msg.setMenuID(menuID);

		return msg;
	}

	public static PerformInteraction buildPerformInteraction(
			Vector<TTSChunk> initChunks, String displayText,
			Vector<Integer> interactionChoiceSetIDList,
			Vector<TTSChunk> helpChunks, Vector<TTSChunk> timeoutChunks,
			InteractionMode interactionMode, Integer timeout,
			Integer correlationID) {
		PerformInteraction msg = new PerformInteraction();
		msg.setInitialPrompt(initChunks);
		msg.setInitialText(displayText);
		msg.setInteractionChoiceSetIDList(interactionChoiceSetIDList);
		msg.setInteractionMode(interactionMode);
		msg.setTimeout(timeout);
		msg.setHelpPrompt(helpChunks);
		msg.setTimeoutPrompt(timeoutChunks);
		msg.setCorrelationID(correlationID);
		
		return msg;
	}

	public static PerformInteraction buildPerformInteraction(
			String initPrompt, 	String displayText, 
			Vector<Integer> interactionChoiceSetIDList,
			String helpPrompt, String timeoutPrompt,
			InteractionMode interactionMode, Integer timeout,
			Integer correlationID) {
		Vector<TTSChunk> initChunks = TTSChunkFactory
				.createSimpleTTSChunks(initPrompt);
		Vector<TTSChunk> helpChunks = TTSChunkFactory
				.createSimpleTTSChunks(helpPrompt);
		Vector<TTSChunk> timeoutChunks = TTSChunkFactory
				.createSimpleTTSChunks(timeoutPrompt);
		return buildPerformInteraction(initChunks,
				displayText, interactionChoiceSetIDList, helpChunks,
				timeoutChunks, interactionMode, timeout, correlationID);
	}
	
	public static PerformInteraction buildPerformInteraction(
			String initPrompt, 	String displayText, 
			Integer interactionChoiceSetID,
			String helpPrompt, String timeoutPrompt,
			InteractionMode interactionMode, Integer timeout,
			Integer correlationID) {
		Vector<Integer> interactionChoiceSetIDs = new Vector<Integer>();
			interactionChoiceSetIDs.add(interactionChoiceSetID);
		
		return buildPerformInteraction(
				initPrompt, displayText, interactionChoiceSetIDs, 
				helpPrompt, timeoutPrompt, interactionMode, 
				timeout, correlationID);
	}
	
	public static PerformInteraction buildPerformInteraction(String initPrompt,
			String displayText, Integer interactionChoiceSetID,
			Integer correlationID) {

		return buildPerformInteraction(initPrompt, displayText, 
				interactionChoiceSetID, null, null,
				InteractionMode.BOTH, null, correlationID);
	}
	
	@Deprecated
	public static PerformInteraction buildPerformInteraction(
			Vector<TTSChunk> initChunks, String displayText,
			Vector<Integer> interactionChoiceSetIDList,
			Vector<TTSChunk> helpChunks, InteractionMode interactionMode,
			Integer timeout, Integer correlationID) {
		PerformInteraction msg = new PerformInteraction();
		msg.setInitialPrompt(initChunks);
		msg.setInitialText(displayText);
		msg.setInteractionChoiceSetIDList(interactionChoiceSetIDList);
		msg.setInteractionMode(interactionMode);
		msg.setTimeout(timeout);
		msg.setHelpPrompt(helpChunks);
		msg.setCorrelationID(correlationID);
		return msg;
	}
	
	@Deprecated
	public static PerformInteraction buildPerformInteraction(String initPrompt,
			String displayText, Vector<Integer> interactionChoiceSetIDList,
			String helpPrompt, InteractionMode interactionMode,
			Integer timeout, Integer correlationID) {
		Vector<TTSChunk> initChunks = TTSChunkFactory
				.createSimpleTTSChunks(initPrompt);
		Vector<TTSChunk> helpChunks = TTSChunkFactory
				.createSimpleTTSChunks(helpPrompt);
		PerformInteraction msg = buildPerformInteraction(initChunks,
				displayText, interactionChoiceSetIDList, helpChunks,
				interactionMode, timeout, correlationID);
		return msg;
	}
	
	public static RegisterAppInterface buildRegisterAppInterface(String appName) {
		return buildRegisterAppInterface(appName, false, "");
	}
	
	public static RegisterAppInterface buildRegisterAppInterface(
			String appName, Boolean isMediaApp, String autoActivateID) {
		
		return buildRegisterAppInterface(null, appName, null, null, isMediaApp, 
				null, autoActivateID, null); 
	}
	
	public static RegisterAppInterface buildRegisterAppInterface(
			SyncMsgVersion syncMsgVersion, String appName, String ngnMediaScreenAppName,
			Vector<String> vrSynonyms, Boolean isMediaApp, Language languageDesired, 
			String autoActivateID, Integer correlationID) {
		RegisterAppInterface msg = new RegisterAppInterface();
		
		if (correlationID == null) {
			correlationID = 1;
		}
		msg.setCorrelationID(correlationID);
		
		if (syncMsgVersion == null) {
			syncMsgVersion = new SyncMsgVersion();
			syncMsgVersion.setMajorVersion(new Integer(SYNC_MSG_MAJOR_VERSION));
			syncMsgVersion.setMinorVersion(new Integer(SYNC_MSG_MINOR_VERSION));
		} 
		msg.setSyncMsgVersion(syncMsgVersion);
		
		msg.setAppName(appName);
		
		msg.setAutoActivateID(autoActivateID);
		
		if (ngnMediaScreenAppName == null) {
			ngnMediaScreenAppName = appName;
		}
		
		if (ngnMediaScreenAppName.length() > NGN_MEDIA_SCREEN_APP_NAME_MAX_LENGTH) {
			ngnMediaScreenAppName = ngnMediaScreenAppName.substring(0,
					NGN_MEDIA_SCREEN_APP_NAME_MAX_LENGTH);
		}
		msg.setNgnMediaScreenAppName(ngnMediaScreenAppName);
		
		if (vrSynonyms == null) {
			vrSynonyms = new Vector<String>();
			vrSynonyms.add(appName);
		}
		msg.setVrSynonyms(vrSynonyms);
		
		msg.setIsMediaApplication(isMediaApp);
		
		if (languageDesired == null) {
			languageDesired = Language.EN_US;
		}
		msg.setLanguageDesired(languageDesired);

		return msg;
	}
	
	public static SetGlobalProperties buildSetGlobalProperties(
			String helpPrompt, String timeoutPrompt, Integer correlationID) {
		return buildSetGlobalProperties(TTSChunkFactory
				.createSimpleTTSChunks(helpPrompt), TTSChunkFactory
				.createSimpleTTSChunks(timeoutPrompt), correlationID);
	}
	
	public static SetGlobalProperties buildSetGlobalProperties(
			Vector<TTSChunk> helpChunks, Vector<TTSChunk> timeoutChunks,
			Integer correlationID) {
		SetGlobalProperties req = new SetGlobalProperties();
		req.setCorrelationID(correlationID);

		req.setHelpPrompt(helpChunks);
		req.setTimeoutPrompt(timeoutChunks);

		return req;
	}

	public static SetMediaClockTimer buildSetMediaClockTimer(Integer hours,
			Integer minutes, Integer seconds, UpdateMode updateMode,
			Integer correlationID) {

		SetMediaClockTimer msg = new SetMediaClockTimer();
		if (hours != null || minutes != null || seconds != null) {
			StartTime startTime = new StartTime();
			msg.setStartTime(startTime);
			startTime.setHours(hours);
			startTime.setMinutes(minutes);
			startTime.setSeconds(seconds);
		}

		msg.setUpdateMode(updateMode);
		msg.setCorrelationID(correlationID);

		return msg;
	}
	
	@Deprecated
	public static SetMediaClockTimer buildSetMediaClockTimer(
			UpdateMode updateMode, Integer correlationID) {
		Integer hours = null;
		Integer minutes = null;
		Integer seconds = null;

		SetMediaClockTimer msg = buildSetMediaClockTimer(hours, minutes,
				seconds, updateMode, correlationID);
		return msg;
	}

	public static Show buildShow(String mainText1, String mainText2,
			String statusBar, String mediaClock, String mediaTrack,
			TextAlignment alignment, Integer correlationID) {
		Show msg = new Show();
		msg.setCorrelationID(correlationID);
		msg.setMainField1(mainText1);
		msg.setMainField2(mainText2);
		msg.setStatusBar(statusBar);
		msg.setMediaClock(mediaClock);
		msg.setMediaTrack(mediaTrack);
		msg.setAlignment(alignment);

		return msg;
	}
	
	public static Show buildShow(String mainText1, String mainText2,
			TextAlignment alignment, Integer correlationID) {
		Show msg = buildShow(mainText1, mainText2, null, null, null, alignment,
				correlationID);
		return msg;
	}
	
	public static Speak buildSpeak(String ttsText, Integer correlationID) {
		Speak msg = buildSpeak(TTSChunkFactory.createSimpleTTSChunks(ttsText),
				correlationID);
		return msg;
	}
	
	public static Speak buildSpeak(Vector<TTSChunk> ttsChunks,
			Integer correlationID) {

		Speak msg = new Speak();
		msg.setCorrelationID(correlationID);

		msg.setTtsChunks(ttsChunks);

		return msg;
	}
	
	public static SubscribeButton buildSubscribeButton(ButtonName buttonName,
			Integer correlationID) {

		SubscribeButton msg = new SubscribeButton();
		msg.setCorrelationID(correlationID);
		msg.setButtonName(buttonName);

		return msg;
	}
	
	public static UnregisterAppInterface buildUnregisterAppInterface(
			Integer correlationID) {
		UnregisterAppInterface msg = new UnregisterAppInterface();
		msg.setCorrelationID(correlationID);

		return msg;
	}
	
	public static UnsubscribeButton buildUnsubscribeButton(
			ButtonName buttonName, Integer correlationID) {

		UnsubscribeButton msg = new UnsubscribeButton();
		msg.setCorrelationID(correlationID);
		msg.setButtonName(buttonName);

		return msg;
	}
}
