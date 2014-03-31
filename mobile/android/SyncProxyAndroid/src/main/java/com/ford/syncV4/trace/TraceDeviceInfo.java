/**
 * 
 */
package com.ford.syncV4.trace;

import java.util.Iterator;
import java.util.Set;

import com.ford.syncV4.util.DebugTool;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.telephony.TelephonyManager;

/**
 * @author vvolkman
 * 
 */
public class TraceDeviceInfo {
	// http://developer.android.com/guide/topics/data/data-storage.html

	private static TelephonyManager sTelephonyManager;
	
	// Constructor
	public TraceDeviceInfo(TelephonyManager telephonyManager) {
		sTelephonyManager = telephonyManager;
	}

	public static void setTelephonyManager(TelephonyManager telephonyManager) {
		sTelephonyManager = telephonyManager;
	}

	public static TelephonyManager getTelephonyManager() {
		return sTelephonyManager;
	}

	// package scoped
	static String getTelephonyHeader() {
		// Telephony manager can tell us a few things...
		String info = "";

		if (sTelephonyManager != null) {
			try { // getDeviceId() requires android.permission.READ_PHONE_STATE
				info = "<deviceid>" + sTelephonyManager.getDeviceId() + "</deviceid>";
			} catch (Exception e1) {
				DebugTool.logError("Failure getting telephony device ID: " + e1.toString(), e1);
			}
	
			info = "<pt>";
			switch (sTelephonyManager.getPhoneType()) {
				case TelephonyManager.PHONE_TYPE_NONE:
					info += "NONE";
					break;
				case TelephonyManager.PHONE_TYPE_GSM:
					info += "GSM";
					break;
				case TelephonyManager.PHONE_TYPE_CDMA:
					info += "CDMA";
					break;
				default:
					info += "UNKNOWN";
			} // end-switch
	
			info += "</pt>" + "<nt>";
	
			switch (sTelephonyManager.getNetworkType()) {
				case TelephonyManager.NETWORK_TYPE_UNKNOWN:
					info += "UKNOWN";
					break;
				case TelephonyManager.NETWORK_TYPE_GPRS:
					info += "GPRS";
					break;
				case TelephonyManager.NETWORK_TYPE_EDGE:
					info += "EDGE";
					break;
				case TelephonyManager.NETWORK_TYPE_UMTS:
					info += "UMTS";
					break;
				case TelephonyManager.NETWORK_TYPE_HSDPA:
					info += "HSDPA";
					break;
				case TelephonyManager.NETWORK_TYPE_HSUPA:
					info += "HSUPA";
					break;
				case TelephonyManager.NETWORK_TYPE_HSPA:
					info += "HSPA";
					break;
				case TelephonyManager.NETWORK_TYPE_CDMA:
					info += "CDMA";
					break;
				case TelephonyManager.NETWORK_TYPE_EVDO_0:
					info += "EVDO_O";
					break;
				case TelephonyManager.NETWORK_TYPE_EVDO_A:
					info += "EVDO_A";
					break;
				case TelephonyManager.NETWORK_TYPE_1xRTT:
					info += "1xRTT";
					break;
				default:
					info += "UNKNOWN";
					break;
			} // end-switch
	
			info += "</nt>";
		} // end-if
		return info;
	} // end-method

	// Package scoped
	static String getLogHeaderBluetoothPairs() {
		Set<BluetoothDevice> btDevices = BluetoothAdapter.getDefaultAdapter()
				.getBondedDevices();

		StringBuilder write = new StringBuilder("<btpairs>");
		Iterator<BluetoothDevice> iter = btDevices.iterator();
		while (iter.hasNext()) {
			write.append(SyncTrace.getBTDeviceInfo(iter.next()));
		}
		write.append("</btpairs>");

		return write.toString();
	} // end-method
} // end-class