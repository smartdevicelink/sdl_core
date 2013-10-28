package com.ford.avatar.util;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

/**
 * Title: NetUtils.java<br>
 * Description: Network utility class<br>
 * 
 * @author Eugene Sagan
 */
public class NetUtils {

	public static boolean isOnline(Context context) {
		ConnectivityManager cm = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);

		if (cm == null) {
			return false;
		}

		NetworkInfo[] netInfos = cm.getAllNetworkInfo();
		if (netInfos == null || netInfos.length == 0) {
			return false;
		}
		for (NetworkInfo ni : netInfos){
			if (ConnectivityManager.TYPE_WIFI == ni.getType() && ni.isConnected()){
				return true;
			}
			if (ConnectivityManager.TYPE_MOBILE == ni.getType() && ni.isConnected()){
				return true;
			}
		}	
		return false;
	}
	
	public static boolean isWiFiOnline(Context context) {
		ConnectivityManager cm = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);

		if (cm == null) {
			return false;
		}

		NetworkInfo i = cm.getNetworkInfo(ConnectivityManager.TYPE_WIFI);

		return i == null ? false : i.isConnected();
	}
	
	/**
	 * Pings a HTTP URL. This effectively sends a HEAD request and returns <code>true</code> if the response code is in 
	 * the 200-399 range.
	 * @param url The HTTP URL to be pinged.
	 * @param timeout The timeout in millis for both the connection timeout and the response read timeout. Note that
	 * the total timeout is effectively two times the given timeout.
	 * @return <code>true</code> if the given HTTP URL has returned response code 200-399 on a HEAD request within the
	 * given timeout, otherwise <code>false</code>.
	 */
	public static boolean ping(String url, int timeout) {
		
		url = url.replaceFirst("https", "http"); // Otherwise an exception may be thrown on invalid SSL certificates.

	    try {
	        HttpURLConnection connection = (HttpURLConnection) new URL(url).openConnection();
	        connection.setConnectTimeout(timeout);
	        connection.setReadTimeout(timeout);
	        connection.setRequestMethod("HEAD");
	        int responseCode = connection.getResponseCode();
	        return (200 <= responseCode && responseCode <= 399);
	    } catch (IOException exception) {
	        return false;
	    }
	}
}
