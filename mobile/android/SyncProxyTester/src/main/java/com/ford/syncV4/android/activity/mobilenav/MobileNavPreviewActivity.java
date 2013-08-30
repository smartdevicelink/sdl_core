package com.ford.syncV4.android.activity.mobilenav;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;

import com.ford.syncV4.android.R;

public class MobileNavPreviewActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mobile_nav_preview);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.mobile_nav_preview, menu);
        return true;
    }


    public void onVideoStreamingCheckBoxAction(View v){
    }

    public void onMobileNaviCheckBoxAction(View v){
    }
}
