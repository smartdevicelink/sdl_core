package com.ford.syncV4.proxy;

import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.AddSubMenu;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteCommand;
import com.ford.syncV4.proxy.rpc.DeleteFile;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteSubMenu;
import com.ford.syncV4.proxy.rpc.DeviceInfo;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.proxy.rpc.ListFiles;
import com.ford.syncV4.proxy.rpc.MenuParams;
import com.ford.syncV4.proxy.rpc.PerformInteraction;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimer;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.Speak;
import com.ford.syncV4.proxy.rpc.StartTime;
import com.ford.syncV4.proxy.rpc.SubscribeButton;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.SyncPData;
import com.ford.syncV4.proxy.rpc.SystemRequest;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterface;
import com.ford.syncV4.proxy.rpc.UnsubscribeButton;
import com.ford.syncV4.proxy.rpc.enums.AppHMIType;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;


/**
 * This class provides the functionality to create messages of
 * {@link com.ford.syncV4.proxy.RPCMessage} type
 */

import java.util.Vector;


public class RPCRequestFactory {

    private static final String LOG_TAG = RPCRequestFactory.class.getSimpleName();

    /**
     * Since version 3.0O revision P
     */
	private static final int NGN_MEDIA_SCREEN_APP_NAME_MAX_LENGTH = 100;
	private static final int APP_NAME_MAX_LENGTH = 100;
	private static final int VR_SYNONYM_MAX_LENGTH = 40;
	private static final int VR_SYNONYM_MIX_SIZE = 1;
	private static final int VR_SYNONYM_MAX_SIZE = 100;

    private static final int SYNC_MSG_MAJOR_VERSION = 1;
    private static final int SYNC_MSG_MINOR_VERSION = 0;

	public static EncodedSyncPData buildEncodedSyncPData(
			Vector<String> data, Integer correlationID) {
		
		if(data == null) return null;
		
		EncodedSyncPData msg = new EncodedSyncPData();
		msg.setCorrelationId(correlationID);
		msg.setData(data);
		return msg;
	}
	
	public static SyncPData buildSyncPData(
			byte[] data, Integer correlationID) {
		
		if(data == null) return null;
		
		SyncPData msg = new SyncPData();
		msg.setCorrelationId(correlationID);
		msg.setSyncPData(data);
		return msg;
	}

    /**
     * AddCommand section
     */

    /**
     * Build <b>AddCommand</b> empty object
     *
     * @return created empty <b>AddCommand</b> object
     */
    public static AddCommand buildAddCommand() {
        return new AddCommand();
    }

	public static AddCommand buildAddCommand(Integer commandID,
			String menuText, Integer parentID, Integer position,
			Vector<String> vrCommands, Integer correlationID) {
		AddCommand addCommand = new AddCommand();
		addCommand.setCorrelationId(correlationID);
		addCommand.setCmdID(commandID);
		addCommand.setVrCommands(vrCommands);
		
		if (menuText != null || parentID != null || position != null) {
			MenuParams menuParams = new MenuParams();
			menuParams.setMenuName(menuText);
			menuParams.setPosition(position);
			menuParams.setParentID(parentID);
			addCommand.setMenuParams(menuParams);
		}
		
		return addCommand;
	}
	
	public static AddCommand buildAddCommand(Integer commandID,
			String menuText, Vector<String> vrCommands, Integer correlationID) {
		AddCommand addCommand = buildAddCommand(commandID, menuText, null, null,
				vrCommands, correlationID);
		return addCommand;
	}
	
	public static AddCommand buildAddCommand(Integer commandID,
			Vector<String> vrCommands, Integer correlationID) {
		AddCommand addCommand = new AddCommand();
		addCommand.setCorrelationId(correlationID);
		addCommand.setCmdID(commandID);
		addCommand.setVrCommands(vrCommands);

		return addCommand;
	}

    /**
     * AddSubMenu section
     */

    /**
     * Build <b>AddSubMenu</b> empty object
     *
     * @return created empty <b>AddSubMenu</b> object
     */
    public static AddSubMenu buildAddSubMenu() {
        return new AddSubMenu();
    }

	public static AddSubMenu buildAddSubMenu(Integer menuID, String menuName,
			Integer correlationID) {
		AddSubMenu addSubMenu = buildAddSubMenu(menuID, menuName, null, correlationID);
		return addSubMenu;
	}

	public static AddSubMenu buildAddSubMenu(Integer menuID, String menuName,
			Integer position, Integer correlationID) {
		AddSubMenu addSubMenu = new AddSubMenu();
		addSubMenu.setCorrelationId(correlationID);
		addSubMenu.setMenuName(menuName);
		addSubMenu.setMenuID(menuID);
		addSubMenu.setPosition(position);

		return addSubMenu;
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
		msg.setCorrelationId(correlationID);
		msg.setAlertText1(alertText1);
		msg.setAlertText2(alertText2);
		msg.setDuration(duration);
		msg.setPlayTone(playTone);
		msg.setTtsChunks(ttsChunks);

		return msg;
	}

    /**
     * CreateInteractionChoiceSet section
     */

    /**
     * Build empty <b>CreateInteractionChoiceSet</b> request object
     *
     * @return an empty <b>CreateInteractionChoiceSet</b> request object
     */
    public static CreateInteractionChoiceSet buildCreateInteractionChoiceSet() {
        return new CreateInteractionChoiceSet();
    }

    /**
     * Build <b>CreateInteractionChoiceSet</b> request
     *
     * @param choiceSet Vector of {@link com.ford.syncV4.proxy.rpc.Choice} objects
     * @param interactionChoiceSetID Id of the interaction choice set
     * @param correlationID Correlation Id of the request
     * @return instance of the <b>CreateInteractionChoiceSet</b> request
     */
	public static CreateInteractionChoiceSet buildCreateInteractionChoiceSet(
			Vector<Choice> choiceSet, Integer interactionChoiceSetID,
			Integer correlationID) {
		CreateInteractionChoiceSet createInteractionChoiceSet = new CreateInteractionChoiceSet();
		createInteractionChoiceSet.setChoiceSet(choiceSet);
		createInteractionChoiceSet.setInteractionChoiceSetID(interactionChoiceSetID);
		createInteractionChoiceSet.setCorrelationId(correlationID);
		return createInteractionChoiceSet;
	}
	
	public static DeleteCommand buildDeleteCommand(Integer commandID,
			Integer correlationID) {
		DeleteCommand msg = new DeleteCommand();
		msg.setCmdID(commandID);
		msg.setCorrelationId(correlationID);
		return msg;
	}
	
	public static DeleteFile buildDeleteFile(String syncFileName,
			Integer correlationID) {
		DeleteFile deleteFile = new DeleteFile();
		deleteFile.setCorrelationId(correlationID);
		deleteFile.setSyncFileName(syncFileName);
		return deleteFile;
	}
	
	public static DeleteInteractionChoiceSet buildDeleteInteractionChoiceSet(
			Integer interactionChoiceSetID, Integer correlationID) {
		DeleteInteractionChoiceSet msg = new DeleteInteractionChoiceSet();
		msg.setInteractionChoiceSetID(interactionChoiceSetID);
		msg.setCorrelationId(correlationID);

		return msg;
	}
	
	public static DeleteSubMenu buildDeleteSubMenu(Integer menuID,
			Integer correlationID) {
		DeleteSubMenu msg = new DeleteSubMenu();
		msg.setCorrelationId(correlationID);
		msg.setMenuID(menuID);

		return msg;
	}
	
	public static ListFiles buildListFiles(Integer correlationID) {
		ListFiles listFiles = new ListFiles();
		listFiles.setCorrelationId(correlationID);
		return listFiles;
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
		msg.setCorrelationId(correlationID);
		
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
		msg.setCorrelationId(correlationID);
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

    /**
     * PutFile section
     */

    /**
     * Build empty <b>PutFile</b> object
     *
     * @return empty <b>PutFile</b> object
     */
    public static PutFile buildPutFile() {
        return new PutFile();
    }

    /**
     * Build <b>PutFile</b> object with provided parameters
     *
     * @param syncFileName   File name
     * @param fileType       File type {@link com.ford.syncV4.proxy.rpc.enums.FileType}
     * @param persistentFile Boolean value indicated whether this file is persistent or not
     * @param fileData       Raw file data
     * @param correlationId  Correlation Id of the object
     * @return <b>PutFile</b> object
     */
    public static PutFile buildPutFile(String syncFileName, FileType fileType,
                                       Boolean persistentFile, byte[] fileData,
                                       Integer correlationId) {
        PutFile putFile = buildPutFile();
        putFile.setCorrelationId(correlationId);
        putFile.setSyncFileName(syncFileName);
        putFile.setFileType(fileType);
        if (persistentFile != null) {
            putFile.setPersistentFile(persistentFile);
        }
        putFile.setBulkData(fileData);
        if (fileData != null) {
            putFile.setLength(fileData.length);
        }
        return putFile;
    }

    /**
     * Build {@link com.ford.syncV4.proxy.rpc.PutFile} object
     * @param fileName      name of the associated file
     * @param data          bytes array
     * @param correlationID correlation id of the request
     * @return {@link com.ford.syncV4.proxy.rpc.PutFile}
     */
    public static SystemRequest buildSystemRequest(String fileName, byte[] data,
                                                   Integer correlationID, RequestType requestType) {
        SystemRequest systemRequest = new SystemRequest();
        systemRequest.setFileName(fileName);
        systemRequest.setBulkData(data);
        systemRequest.setRequestType(requestType);
        systemRequest.setCorrelationId(correlationID);

        return systemRequest;
    }

    public static RegisterAppInterface buildRegisterAppInterface() {
        return new RegisterAppInterface();
    }

	public static RegisterAppInterface buildRegisterAppInterface(
			SyncMsgVersion syncMsgVersion, Object appName, Vector<TTSChunk> ttsName,
            Object ngnMediaScreenAppName, Vector<Object> vrSynonyms, Object isMediaApp,
			Language languageDesired, Language hmiDisplayLanguageDesired, Vector<AppHMIType> appHMIType,
            Object appID, Integer correlationID, String hashId, DeviceInfo deviceInfo) {
		RegisterAppInterface msg = buildRegisterAppInterface();
		
		if (correlationID == null) {
			correlationID = 1;
		}
		msg.setCorrelationId(correlationID);

        // TODO : For the TEST CASES only
		/*if (syncMsgVersion == null) {
			syncMsgVersion = new SyncMsgVersion();
			syncMsgVersion.setMajorVersion(new Integer(SYNC_MSG_MAJOR_VERSION));
			syncMsgVersion.setMinorVersion(new Integer(SYNC_MSG_MINOR_VERSION));
		}*/
		msg.setSyncMsgVersion(syncMsgVersion);

        // TODO : For the TEST CASES only
        /*if (appName.length() > APP_NAME_MAX_LENGTH) {
            appName = appName.substring(0, APP_NAME_MAX_LENGTH);
        }*/
		msg.setAppName(appName);
		
		msg.setTtsName(ttsName);

        // TODO : For the TEST CASES only
		/*if (ngnMediaScreenAppName != null) {
            if (ngnMediaScreenAppName.length() > NGN_MEDIA_SCREEN_APP_NAME_MAX_LENGTH) {
                ngnMediaScreenAppName = ngnMediaScreenAppName.substring(0,
                        NGN_MEDIA_SCREEN_APP_NAME_MAX_LENGTH);
            }

		}*/
        msg.setNgnMediaScreenAppName(ngnMediaScreenAppName);

        // TODO : For the TEST CASES only
		/*if (vrSynonyms != null && vrSynonyms.size() >= VR_SYNONYM_MIX_SIZE &&
                vrSynonyms.size() <= VR_SYNONYM_MAX_SIZE) {
			//vrSynonyms = new Vector<String>();
            msg.setVrSynonyms(vrSynonyms);
		}*/
        msg.setVrSynonyms(vrSynonyms);
		
		msg.setIsMediaApplication(isMediaApp);

		//if (languageDesired == null) {
			//languageDesired = Language.EN_US;
		//}
		msg.setLanguageDesired(languageDesired);

		msg.setHmiDisplayLanguageDesired(hmiDisplayLanguageDesired);

        // TODO : For the TEST CASES only
        /*if (appHMIType == null) {
            appHMIType = new Vector<AppHMIType>();
        }*/
		msg.setAppType(appHMIType);
		
		msg.setAppId(appID);

        // TODO : For the TEST CASES only
        /*if (hashId != null) {
            msg.setHashID(hashId);
        }*/
        msg.setHashID(hashId);

        // TODO : For the TEST CASES only
        /*if (deviceInfo != null) {
            msg.setDeviceInfo(deviceInfo);
        }*/
        msg.setDeviceInfo(deviceInfo);

		return msg;
	}

    /**
     * <b>SetAppIcon</b> section
     */

    /**
     * Build an empty {@link com.ford.syncV4.proxy.rpc.SetAppIcon} request
     *
     * @return {@link com.ford.syncV4.proxy.rpc.SetAppIcon} request object
     */
    public static SetAppIcon buildSetAppIcon() {
        return new SetAppIcon();
    }

    /**
     * Build {@link com.ford.syncV4.proxy.rpc.SetAppIcon} request
     *
     * @param syncFileName name of the file
     * @param correlationID correlation id of the request
     * @return {@link com.ford.syncV4.proxy.rpc.SetAppIcon} request object
     */
	public static SetAppIcon buildSetAppIcon(String syncFileName, Integer correlationID) {
		SetAppIcon setAppIcon = new SetAppIcon();
		setAppIcon.setCorrelationId(correlationID);
		setAppIcon.setSyncFileName(syncFileName);
		return setAppIcon;
	}

    /**
     * <b>SetGlobalProperties</b> section
     *
     */

    /**
     * Build {@link com.ford.syncV4.proxy.rpc.SetGlobalProperties} request
     *
     * @param helpPrompt
     * @param timeoutPrompt
     * @param correlationID Correlation Id of the request
     * @return {@link com.ford.syncV4.proxy.rpc.SetGlobalProperties} request object
     */
    public static SetGlobalProperties buildSetGlobalProperties(String helpPrompt,
                                                               String timeoutPrompt,
                                                               Integer correlationID) {
		return buildSetGlobalProperties(TTSChunkFactory
				.createSimpleTTSChunks(helpPrompt), TTSChunkFactory
				.createSimpleTTSChunks(timeoutPrompt), correlationID);
	}

    /**
     * Build {@link com.ford.syncV4.proxy.rpc.SetGlobalProperties} request
     *
     * @param helpChunks
     * @param timeoutChunks
     * @param correlationID
     * @return {@link com.ford.syncV4.proxy.rpc.SetGlobalProperties} request object
     */
	public static SetGlobalProperties buildSetGlobalProperties(Vector<TTSChunk> helpChunks,
                                                               Vector<TTSChunk> timeoutChunks,
                                                               Integer correlationID) {
		SetGlobalProperties req = new SetGlobalProperties();
		req.setCorrelationId(correlationID);
		req.setHelpPrompt(helpChunks);
		req.setTimeoutPrompt(timeoutChunks);

		return req;
	}

    /**
     * Build an empty {@link com.ford.syncV4.proxy.rpc.SetGlobalProperties} request
     *
     * @return {@link com.ford.syncV4.proxy.rpc.SetGlobalProperties} request object
     */
    public static SetGlobalProperties buildSetGlobalProperties() {
        return new SetGlobalProperties();
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
		msg.setCorrelationId(correlationID);

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
		msg.setCorrelationId(correlationID);
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
		msg.setCorrelationId(correlationID);

		msg.setTtsChunks(ttsChunks);

		return msg;
	}

    /**
     * <b>SubscribeButton</b> section
     *
     */

    /**
     * Create {@link com.ford.syncV4.proxy.rpc.SubscribeButton} request
     *
     * @param buttonName Name of the button
     * @param correlationID Correlation Id of the request
     * @return {@link com.ford.syncV4.proxy.rpc.SubscribeButton} request
     */
	public static SubscribeButton buildSubscribeButton(ButtonName buttonName,
                                                       Integer correlationID) {

		SubscribeButton msg = new SubscribeButton();
		msg.setCorrelationId(correlationID);
		msg.setButtonName(buttonName);

		return msg;
	}

    /**
     * Create an empty {@link com.ford.syncV4.proxy.rpc.SubscribeButton} request
     *
     * @return {@link com.ford.syncV4.proxy.rpc.SubscribeButton} request
     */
    public static SubscribeButton buildSubscribeButton() {
        return new SubscribeButton();
    }

    /**
     * <b>SubscribeVehicleData</b> section
     *
     */

    /**
     * Create an empty {@link com.ford.syncV4.proxy.rpc.SubscribeVehicleData} request
     *
     * @return {@link com.ford.syncV4.proxy.rpc.SubscribeVehicleData} request
     */
    public static SubscribeVehicleData buildSubscribeVehicleData() {
        return new SubscribeVehicleData();
    }

    /**
     * Build an empty {@link com.ford.syncV4.proxy.rpc.UnregisterAppInterface} request
     *
     * @return instance of the {@link com.ford.syncV4.proxy.rpc.UnregisterAppInterface}
     */
    public static UnregisterAppInterface buildUnregisterAppInterface() {
        return new UnregisterAppInterface();
    }

    /**
     * Build {@link com.ford.syncV4.proxy.rpc.UnregisterAppInterface} request with provided
     * Correlation Id
     *
     * @param correlationId Correlation id
     *
     * @return instance of the {@link com.ford.syncV4.proxy.rpc.UnregisterAppInterface}
     */
	public static UnregisterAppInterface buildUnregisterAppInterface(Integer correlationId) {
		UnregisterAppInterface msg = buildUnregisterAppInterface();
		msg.setCorrelationId(correlationId);
		return msg;
	}
	
	public static UnsubscribeButton buildUnsubscribeButton(ButtonName buttonName,
                                                           Integer correlationID) {

		UnsubscribeButton msg = new UnsubscribeButton();
		msg.setCorrelationId(correlationID);
		msg.setButtonName(buttonName);
		return msg;
	}
}
