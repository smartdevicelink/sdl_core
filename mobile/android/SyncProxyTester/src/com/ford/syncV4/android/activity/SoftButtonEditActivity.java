package com.ford.syncV4.android.activity;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.rpc.SoftButton;

public class SoftButtonEditActivity extends Activity {
	private final static String LOG_TAG = SoftButtonEditActivity.class
			.getSimpleName();

	private SoftButton softButton = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.softbutton);
		
		softButton = (SoftButton) IntentHelper.getObjectForKey(Const.INTENTHELPER_KEY_SOFTBUTTON);
		Log.w(LOG_TAG, "sb = " + softButton);
	}

}
