package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.SyncProxyALMManager.SyncChoiceSet;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSetResponse;

public interface ISyncCreateInteractionChoiceSetResponseListener {	
	public void onCreateInteractionChoiceSetResponse(CreateInteractionChoiceSetResponse response, 
			SyncChoiceSet syncChoiceSet, Object tag);
}