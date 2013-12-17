package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.DeleteCommandResponse;

public interface ISyncDeleteCommandResponseListener {
	public void onDeleteCommandResponse(DeleteCommandResponse response, Object tag);
}