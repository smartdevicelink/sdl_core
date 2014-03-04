package com.ford.syncV4.proxy.callbacks;

import com.ford.syncV4.proxy.constants.Names;

public class OnError extends InternalProxyMessage {

	private String _info;
	private Throwable _e;
	
	public OnError() {
		super(Names.OnProxyError);
	}

	public OnError(String info, Throwable e) {
		super(Names.OnProxyError);
		this._info = info;
		this._e = e;
	}
	
	public String getInfo() {
		return _info;
	}
	
	public Throwable getThrowable() {
		return _e;
	}
}