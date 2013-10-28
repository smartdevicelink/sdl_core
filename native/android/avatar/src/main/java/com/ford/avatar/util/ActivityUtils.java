package com.ford.avatar.util;

import android.app.Activity;
import android.view.WindowManager;

/**
 * Title: ActivityUtils.java<br>
 * Description: Utility class for regular operations with
 * activity<br>
 *   
 * @author Eugene Sagan
 */
public class ActivityUtils {

	private static boolean mAppIsForeground = true;
	
	/**
	 * Method setActivityProperties.
	 * @param activity Activity
	 * @param layoutResource int
	 */
	public static void setActivityProperties(Activity activity, int layoutResource) {
		setActivityProperties(activity);
		activity.setContentView(layoutResource);

	}
	
	/**
	 * Method setActivityProperties.
	 * @param activity Activity
	 */
	public static void setActivityProperties(Activity activity) {
		activity.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

	}
	
	public static boolean getAppIsForeground(){
		return mAppIsForeground;
	}
	
	public static void setAppIsForeground(boolean value){
		mAppIsForeground = value;
	}
	
}
