package com.ford.syncV4.proxy.callbacks;

import com.ford.syncV4.proxy.constants.Names;

public class OnError extends InternalProxyMessage {

	private String _info;
	private Exception _e;
	
	public OnError() {
		super(Names.OnProxyError);
	}

	public OnError(String info, Exception e) {
		super(Names.OnProxyError);
		this._info = info;
		this._e = e;
	}
	
	public String getInfo() {
		return _info;
	}
	
	public Exception getException() {
		return _e;
	}
}