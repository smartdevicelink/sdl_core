package com.ford.syncV4.android.activity.mobilenav;

import android.content.res.Resources;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;

/**
 * Created by Andrew Batutin on 8/30/13.
 */
public class VideoCheckBoxState {


    private final CheckBox item;
    private VideoStreamingCheckBoxState state;

    public VideoCheckBoxState(CheckBox item) {
        this.item = item;
        setStateOff();
    }

    public VideoStreamingCheckBoxState getState() {
        return state;
    }

    public CheckBox getItem() {
        return item;
    }

    public void setStateDisabled() {
        item.setEnabled(false);
        state = VideoStreamingCheckBoxState.DISABLED;
    }

    public void setStateOff() {
        item.setText("");
        item.setHint(getResources().getString(R.string.video_streaming_hint));
        item.setEnabled(true);
        state = VideoStreamingCheckBoxState.OFF;
    }

    public void setStateOn() {
        item.setText(getResources().getString(R.string.video_check_box_on));
        item.setEnabled(true);
        state = VideoStreamingCheckBoxState.ON;
    }

    private Resources getResources() {
        return item.getContext().getResources();
    }
}
