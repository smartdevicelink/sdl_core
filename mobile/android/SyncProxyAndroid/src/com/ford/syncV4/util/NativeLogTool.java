package com.ford.syncV4.util;

import android.util.Log;

import com.ford.syncV4.proxy.SyncProxyBase;

public class NativeLogTool {
	
	static private boolean logToSystemEnabled = true;
	private static final int ChunkSize = 4000;
	
	private enum LogTarget {
		Info
		,Warning
		,Error
	}

	public static void setEnableState(boolean en) {
		logToSystemEnabled = en;
	} // end-method

	public static boolean isEnabled() {
		return logToSystemEnabled;
	} // end-method
	
	public static void logInfo(String message) {
		logInfo(SyncProxyBase.TAG, message);
	}
	
	public static void logInfo(String tag, String message) {
		if (logToSystemEnabled) {
			log(LogTarget.Info, tag, message);
		}
	}
	
	public static void logWarning(String message) {
		logWarning(SyncProxyBase.TAG, message);
	}
	
	public static void logWarning(String tag, String message) {
		if (logToSystemEnabled) {
			log(LogTarget.Warning, tag, message);
		}
	}
	
	public static void logError(String message) {
		logError(SyncProxyBase.TAG, message);
	}
	
	public static void logError(String tag, String message) {
		if (logToSystemEnabled) {
			log(LogTarget.Error, tag, message);
		}
	}
	
	public static void logError(String message, Throwable t) {
		logError(SyncProxyBase.TAG, message, t);
	}
	
	public static void logError(String tag, String message, Throwable t) {
		// If the call to logError is passed a throwable, write directly to the system log
		if (logToSystemEnabled) {
			Log.e(tag, message, t);
		}
	}
	
	private static void log(LogTarget ltarg, String source, String logMsg) {
		// Don't log empty messages
		if (logMsg == null || logMsg.length() == 0) {
			return;
		} // end-if

		int bytesWritten = 0;
		int substrSize = 0;
		String tag = source;
		String chunk = null;
		try {
			for (int idx=0;idx < logMsg.length();idx += substrSize) {
				substrSize = Math.min(ChunkSize, logMsg.length() - idx);
				chunk = logMsg.substring(idx, idx + substrSize);
				switch (ltarg) {
					case Info:
						bytesWritten = Log.i(tag, chunk);
						break;
					case Warning:
						bytesWritten = Log.w(tag, chunk);
						break;
					case Error:
						bytesWritten = Log.e(tag, chunk);
						break;
				} // end-switch
				if (bytesWritten < chunk.length()) {
					Log.e(SyncProxyBase.TAG, "Calling Log.e: msg length=" + chunk.length() + ", bytesWritten=" + bytesWritten);
				} // end-if
			} // end-while
		} catch (Exception ex) {
			Log.e(SyncProxyBase.TAG, "Failure writing " + ltarg.name() + " fragments to android log:" + ex.toString());
		} // end-catch
	} // end-method
}
