/*
 * 
 */
package com.ford.avatar.jssupport;

import com.ford.avatar.util.Const;

import android.util.Log;
import android.webkit.WebView;
import android.webkit.WebViewClient;

/**
 * @author vsaenko
 * 
 * @version $Revision: 1.0 $
 */
public class AvatarWebViewClient extends WebViewClient {

	public AvatarWebViewClient() {
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.webkit.WebViewClient#onLoadResource(android.webkit.WebView,
	 * java.lang.String)
	 */
	/**
	 * Method onLoadResource.
	 * 
	 * @param view
	 *            WebView
	 * @param url
	 *            String
	 */
	@Override
	public void onLoadResource(WebView view, String url) {
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.webkit.WebViewClient#onReceivedError(android.webkit.WebView,
	 * int, java.lang.String, java.lang.String)
	 */
	/**
	 * Method onReceivedError.
	 * 
	 * @param view
	 *            WebView
	 * @param errorCode
	 *            int
	 * @param description
	 *            String
	 * @param failingUrl
	 *            String
	 */
	@Override
	public void onReceivedError(WebView view, int errorCode,
			String description, String failingUrl) {
		if (Log.isLoggable(Const.APP_TAG, Log.DEBUG)) {
			Log.d(Const.APP_TAG, "onReceivedError" + failingUrl);
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * android.webkit.WebViewClient#shouldOverrideUrlLoading(android.webkit.
	 * WebView, java.lang.String)
	 */
	/**
	 * Method shouldOverrideUrlLoading.
	 * 
	 * @param view
	 *            WebView
	 * @param url
	 *            String
	 * @return boolean
	 */
	@Override
	public boolean shouldOverrideUrlLoading(WebView view, String url) {
		if (url.startsWith("http://")) {
			view.loadUrl(url);
		}
		return true;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.webkit.WebViewClient#onPageFinished(android.webkit.WebView,
	 * java.lang.String)
	 */
	/**
	 * Method onPageFinished.
	 * 
	 * @param view
	 *            WebView
	 * @param url
	 *            String
	 */
	@Override
	public void onPageFinished(WebView view, String url) {
	}
}
