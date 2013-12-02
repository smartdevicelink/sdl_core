package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.SyncProxyALMManager.SyncCommand;
import com.ford.syncV4.proxy.rpc.AddCommandResponse;
import com.ford.syncV4.proxy.rpc.OnCommand;

public interface ISyncCommandListener {
	public void onSyncCommand(OnCommand notification, SyncCommand syncCommand, Object tag);
	
	public void onAddCommandResponse(AddCommandResponse response, SyncCommand syncCommand, Object tag);
}