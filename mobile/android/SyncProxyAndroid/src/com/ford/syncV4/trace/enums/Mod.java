package com.ford.syncV4.trace.enums;

public enum Mod {
	  tran,
	  proto,
	  mar,
	  rpc,
	  app,
	  proxy;

	public static Mod valueForString(String value) {
		return valueOf(value);
	}
};