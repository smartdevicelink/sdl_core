package com.ford.syncV4.android.activity.mobilenav;

import android.widget.CheckBox;

import com.ford.syncV4.android.R;

/**
 * Created by Andrew Batutin on 8/30/13.
 */
public class VideoCheckBoxState extends CheckBoxState {

    public VideoCheckBoxState(CheckBox item) {
        super(item);
        hintString = getResources().getString(R.string.video_streaming_hint);
        textString = getResources().getString(R.string.video_check_box_on);
        setStateOff();
    }
}
