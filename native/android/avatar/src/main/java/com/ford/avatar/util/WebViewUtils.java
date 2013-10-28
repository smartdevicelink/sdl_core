package com.ford.avatar.util;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.content.Context;
import android.media.AudioManager;
import android.os.Build;
import android.provider.Settings;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnLongClickListener;
import android.view.View.OnTouchListener;
import android.webkit.WebSettings;
import android.webkit.WebView;

import com.ford.avatar.activity.AvatarActivity;
import com.ford.avatar.jssupport.AvatarWebViewClient;
import com.ford.avatar.jssupport.JavaScriptFacade;

/**
 * @author vsaenko
 * 
 */
public class WebViewUtils {

	private static Boolean isPushed = false;
	private final static String TAG = WebViewUtils.class.getName();
	private static long lastTouchTime = -1; // double tap blocker

	/**
	 * @param activity
	 * @param wv
	 */
	public static void initWebView(final AvatarActivity activity) {
		// Fix white line
		// http://stackoverflow.com/questions/3859502/why-isnt-my-webview-filling-up-the-entire-width-of-my-screen/5623215#5623215
		WebView wv = activity.getWebView();
		wv.setScrollBarStyle(View.SCROLLBARS_INSIDE_OVERLAY);
		wv.setWebViewClient(new AvatarWebViewClient());
		wv.addJavascriptInterface(new JavaScriptFacade(activity), "Android");
		wv.setBackgroundColor(0x00000000);
		wv.setVerticalScrollBarEnabled(false);
		wv.setHorizontalScrollBarEnabled(false);
		wv.setPadding(0, 0, 0, 0);

		final WebSettings settings = wv.getSettings();
		settings.setBuiltInZoomControls(true);
		settings.setAllowFileAccess(true);
		settings.setCacheMode(WebSettings.LOAD_CACHE_ELSE_NETWORK);
		settings.setJavaScriptEnabled(true);
		settings.setPluginsEnabled(true);
		settings.setSupportZoom(false);
		settings.setDomStorageEnabled(true);
		

		// http://community.htc.com/na/htc-forums/android/f/91/p/2332/10916.aspx
		// To avoid WebView pan and zoom problem.
		// add the following section code before load any URL or data.
		// ===================== Begin =====================

		final String incradible = "ADR6300";
		if (Build.MODEL.equalsIgnoreCase(incradible)) {
			try {

				final Method method = wv.getClass().getMethod(
						"setIsCacheDrawBitmap", boolean.class);
				if (method != null) {
					method.invoke(wv, false);
					wv.getSettings().setLayoutAlgorithm(
							WebSettings.LayoutAlgorithm.NORMAL);
				}
			} catch (NoSuchMethodException e) {
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				e.printStackTrace();
			}

		}

		// ===================== End =====================

		wv.setKeepScreenOn(true);

		if (android.os.Build.VERSION.SDK_INT > 13) {

			final Method[] declaredMethods = wv.getClass().getMethods();

			for (Method method : declaredMethods) {
				if (method.getName().equalsIgnoreCase("setLayerType")) {
					method.setAccessible(true);
					try {
						method.invoke(wv, 0x1, null);
					} catch (IllegalArgumentException e) {
						e.printStackTrace();
					} catch (IllegalAccessException e) {
						e.printStackTrace();
					} catch (InvocationTargetException e) {
						e.printStackTrace();
					}
				}
			}

		}

		//do not set touch listener for android 4 and higher
		String androidVersion = activity.getBEController().getOSVersion();
		if (androidVersion.substring(0, 1).compareTo("3") <= 0)
			setOnTouchListener(activity, BLOCKING_ACTION_MOVE_MODE);

		wv.setOnLongClickListener(new OnLongClickListener() {
			// disable selection text
			public boolean onLongClick(View v) {
				return true;
			}
		});
		
	}
	
	
	public static final int BLOCKING_ACTION_MOVE_MODE = 0;
	public static final int NONBLOCKING_ACTION_MOVE_MODE = 1;
	
	public static void setOnTouchListener(final AvatarActivity activity, final int mode) {
		WebView wv = activity.getWebView();
		wv.setOnTouchListener(new OnTouchListener() {

			public boolean onTouch(View v, MotionEvent event) {
				// onClick does not work
				if ((event.getAction() == MotionEvent.ACTION_UP) && (isPushed)) {
					playClickSound();
					isPushed = false;
					return false;
				} else if (event.getAction() == MotionEvent.ACTION_DOWN) {
					long thisTime = System.currentTimeMillis();
					if (thisTime - lastTouchTime < Const.KEY_PRESS_DELAY) {
						lastTouchTime = -1;
						event.setAction(MotionEvent.ACTION_CANCEL);
					} else {
						lastTouchTime = thisTime;
					}
					isPushed = true;
					return false;
				} else if (event.getAction() == MotionEvent.ACTION_MOVE) {
					//Log.d(TAG, "onTouch MotionEvent=ACTION_MOVE");
					if (mode == BLOCKING_ACTION_MOVE_MODE)
						return true;
					else
						return false;
				} else {
					//Log.e(TAG, "onTouch MotionEvent=" + event.getAction());
					//DO NOTHING FOR MULTITOUCH ACTION
				}
				return true;//Allow only one touch events
			}

			private void playClickSound() {
				if (Settings.System.getInt(activity.getContentResolver(),
						Settings.System.SOUND_EFFECTS_ENABLED, 0) != 0) {
					final AudioManager am = (AudioManager) activity
							.getSystemService(Context.AUDIO_SERVICE);
					am.playSoundEffect(AudioManager.FX_KEY_CLICK);
				}
			}
		});
	}
}
