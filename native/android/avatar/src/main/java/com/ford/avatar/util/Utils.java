/*
 * 
 */
package com.ford.avatar.util;

import java.math.BigDecimal;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Handler;
import android.os.Message;

/**
 * @author vsaenko
 * @version $Revision: 1.0 $
 */
public class Utils {

	/**
	 * Method getShortDouble.
	 * 
	 * @param value
	 *            double
	 * 
	 * 
	 * 
	 * @return double
	 */
	public static double getShortDouble(final double value) {
		return BigDecimal.valueOf(value)
				.setScale(2, BigDecimal.ROUND_HALF_DOWN).doubleValue();
	}

	/**
	 * Method sendMessageToHandler.
	 * 
	 * @param what
	 *            int
	 * @param obj
	 *            Object
	 * @param mHandler
	 *            Handler
	 */
	public static void sendMessageToHandler(final int what, final Object obj,
			final Handler mHandler) {
		Message msg;
		msg = mHandler.obtainMessage();
		msg.what = what;
		msg.obj = obj;
		mHandler.sendMessage(msg);
	}

	/**
	 * Method sendMessageToHandler.
	 * 
	 * @param what
	 *            int
	 * @param obj
	 *            Object
	 * @param mHandler
	 *            Handler
	 * @param delay
	 *            int
	 */
	public static void sendMessageToHandler(final int what, final Object obj,
			final Handler mHandler, final int delay) {
		Message msg;
		msg = mHandler.obtainMessage();
		msg.what = what;
		msg.obj = obj;
		mHandler.sendMessageDelayed(msg, delay);
	}

	public static int getAppVersionCode(final Context ctx){
		PackageInfo pinfo;
		try {
			pinfo = ctx.getPackageManager().getPackageInfo(ctx.getPackageName(), 0);
			return pinfo.versionCode;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return 1;
	}
	
}
