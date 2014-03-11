package com.ford.syncV4.android.activity.secure;

import android.app.Activity;
import android.os.Bundle;

import com.ford.syncV4.android.R;
import com.ford.syncV4.protocol.secure.secureproxy.SecureProxyManager;

/**
 * Created by Andrew Batutin on 3/11/14.
 */
public class SecureTestActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.secure_activity);
        setupProxyManager();
    }

    private void setupProxyManager(){
        SecureProxyManager manager = new SecureProxyManager(this);
        manager.setupSecureConnection();
    }
}
