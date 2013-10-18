package com.ford.sdlreverse.rater;

import java.util.Timer;
import java.util.TimerTask;

import com.ford.sdlreverse.jsoncontroller.JSONRateController;
import com.ford.sdlreverse.util.Const;
import com.ford.sdlreverse.util.MessageConst;
import com.ford.sdlreverse.util.NetUtils;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

public class AppRater {
	private Context mContext;
	private JSONRateController mRateController;

	private final int PING_TIMEOUT = 5000;// in ms
	private final int REDIRECT_MSG_DELAY = 3000;// in ms

	public AppRater(Context context) {
		mContext = context;
	}

	public void showRateDialog(Context context, JSONRateController controller) {

	}

	public int loadGooglePlay(JSONRateController controller) {
		mRateController = controller;
		int res = 0; // redirection succeed
		if (NetUtils.isOnline(mContext)) {
			String packageName = mContext.getApplicationContext()
					.getPackageName();
			String url = "https://play.google.com"; //
			if (NetUtils.ping(url, PING_TIMEOUT)) {
				url = "market://details?id=" + packageName;
				redirectOnGooglePlay(url);
			} else {
				res = Const.RATE_GOOGLEPLAY_ERR_CODE;
			}
		} else {
			res = Const.RATE_NO_NETWORK_ERR_CODE;
		}
		return res;
	}

	private void redirectOnGooglePlay(final String url) {
		// start activity with google play
		Uri marketUri = Uri.parse(url);
		mContext.startActivity(new Intent(Intent.ACTION_VIEW, marketUri));
	}
}
