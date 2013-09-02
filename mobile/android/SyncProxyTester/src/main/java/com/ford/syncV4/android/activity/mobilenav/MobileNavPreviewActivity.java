package com.ford.syncV4.android.activity.mobilenav;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.View;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;

public class MobileNavPreviewActivity extends Activity implements SurfaceHolder.Callback {

    private VideoCheckBoxState checkBoxState;

    public VideoCheckBoxState getCheckBoxState() {
        return checkBoxState;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mobile_nav_preview);
        initiateVideoCheckBox();
    }

    private void initiateVideoCheckBox() {
        checkBoxState = new VideoCheckBoxState((CheckBox) findViewById(R.id.videoStreamingCheckBox));
        checkBoxState.setStateDisabled();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.mobile_nav_preview, menu);
        return true;
    }

    public void onVideoStreamingCheckBoxAction(View checkBox) {
        changeCheckBoxState((CheckBox) checkBox);
    }

    private void changeCheckBoxState(CheckBox checkBox) {
        checkBoxState.setStateDisabled();
    }

    public void onMobileNaviCheckBoxAction(View v) {
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        checkBoxState.setStateOff();
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i2, int i3) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }
}
