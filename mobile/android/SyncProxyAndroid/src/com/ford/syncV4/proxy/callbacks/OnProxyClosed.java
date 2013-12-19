package com.ford.syncV4.proxy.callbacks;

import com.ford.syncV4.proxy.constants.Names;

public class OnProxyClosed extends InternalProxyMessage {
	
	private String _info;
	private Exception _e;
	
	public OnProxyClosed() {
		super(Names.OnProxyClosed);
	}
	
	public OnProxyClosed(String info, Exception e) {
		super(Names.OnProxyClosed);
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