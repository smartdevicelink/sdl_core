package com.ford.syncV4.util;


public interface IConsole {
	void logInfo(String msg);
	void logError(String msg);
	void logError(String msg, Throwable ex);
	void logRPCSend(String rpcMsg);
	void logRPCReceive(String rpcMsg);
}
