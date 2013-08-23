package com.ford.syncV4.android.constants;

import java.util.Vector;

import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;
	
public class TestObj {
	public Boolean playTone = null;
	public Integer choiceID = null;
	public Vector<Choice> choiceSet = null;
	public Integer commandID = null;
	public Integer corrID = null;
	public Integer duration = null;
	public Integer hours = null;
	public Integer interactionChoiceSetID = null;
	public Vector<Integer> interactionChoiceSetIDList = null;
	public Integer menuID = null;
	public Integer minutes = null;
	public Integer parentID = null;
	public Integer position = null;
	public Integer seconds = null;
	public Integer timeout = null;
	public String alertText1 = null;
	public String alertText2 = null;
	public String choiceMenuName = null;
	public String displayText = null;
	public String helpPrompt = null;
	public Vector<TTSChunk> helpChunks = null;
	public String initPrompt = null;
	public Vector<TTSChunk> initChunks = null;
	public String mainText1 = null;
	public String mainText2 = null;
	public String mediaClock = null;
	public String mediaTrack = null;
	public String menuName = null;
	public String menuText = null;
	public String RPC = null;
	public String statusBar = null;
	public String timeoutPrompt = null;
	public Vector<TTSChunk> timeoutChunks = null;
	public String ttsText = null;
	public Vector<TTSChunk> ttsChunks = null;
	public Vector<GlobalProperty> properties = null;
	public Vector<String> data = null;
	public Vector<String> choiceVrCommands = null;
	public Vector<String> vrCommands = null;
	public ButtonName buttonName = null;
	public InteractionMode interactionMode = null;
	public TextAlignment alignment = null;
	public UpdateMode updateMode = null;
}