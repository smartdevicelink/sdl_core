package com.ford.syncV4.proxy.callbacks;

public class InternalProxyMessage {
	private String _functionName;
	
	public InternalProxyMessage(String functionName) {
		//this(functionName, null, null);
		this._functionName = functionName;
	}
	
	public String getFunctionName() {
		return _functionName;
	}
}