package com.ford.syncV4.proxy;

import android.telephony.TelephonyManager;

public class SyncProxyConfigurationResources {
	private String _syncConfigurationFilePath;
	private TelephonyManager _telephonyManager;
	
	public SyncProxyConfigurationResources() {
		this(null, null);
	}
	
	public SyncProxyConfigurationResources(String syncConfigurationFilePath, 
			TelephonyManager telephonyManager) {
		_syncConfigurationFilePath = syncConfigurationFilePath;
		_telephonyManager = telephonyManager;
	}
	
	public void setSyncConfigurationFilePath(String syncConfigurationFilePath) {
		_syncConfigurationFilePath = syncConfigurationFilePath;
	}
	
	public String getSyncConfigurationFilePath() {
		return _syncConfigurationFilePath;
	}
	
	public void setTelephonyManager(TelephonyManager telephonyManager) {
		_telephonyManager = telephonyManager;
	}
	
	public TelephonyManager getTelephonyManager() {
		return _telephonyManager;
	}
}
