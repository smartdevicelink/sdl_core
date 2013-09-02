package com.ford.syncV4.android.activity.mobilenav;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;

public class MobileNavPreviewActivity extends Activity implements VideoDataListener {

    private static final String logTag = "MobileNavPreviewActivity";
    private CheckBoxState checkBoxState;
    private MockVideoDataSource videoDataSource;

    public MockVideoDataSource getVideoDataSource() {
        return videoDataSource;
    }

    public VideoCheckBoxState getCheckBoxState() {
        return (VideoCheckBoxState) checkBoxState;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i(logTag, "activity created");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mobile_nav_preview);
        initiateVideoCheckBox();
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.i(logTag, "activity paused");
        if (videoDataSource != null){
            checkBoxState.setStateDisabled();
            videoDataSource.stop();
        }
    }

    private void initiateVideoCheckBox() {
        checkBoxState = new VideoCheckBoxState((CheckBox) findViewById(R.id.videoStreamingCheckBox));
        checkBoxState.setStateOff();
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
        if (checkBoxState.getState().equals(CheckBoxStateValue.OFF)) {
            videoDataSource = new MockVideoDataSource(this);
            checkBoxState.setStateDisabled();
            videoDataSource.start();
        } else if (checkBoxState.getState().equals(CheckBoxStateValue.ON)) {
            checkBoxState.setStateDisabled();
            videoDataSource.stop();
        }
    }

    public void onMobileNaviCheckBoxAction(View v) {
    }

    @Override
    public void onStreamingStart() {
        checkBoxState.setStateOn();
        Log.i(logTag, "video streaming started");

    }

    @Override
    public void videoFrameReady(byte[] videoFrame) {
        Log.d(logTag, "video frame received" + videoFrame.toString());
    }

    @Override
    public void onStreamStop() {
        checkBoxState.setStateOff();
        Log.i(logTag, "video streaming stopped");

    }
}
