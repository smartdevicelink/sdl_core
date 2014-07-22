package com.ford.syncV4.android.activity.mobilenav;

import android.content.Context;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;

/**
 * Created by Andrew Batutin on 8/30/13.
 */
public class VideoCheckBoxState extends CheckBoxState {

    public VideoCheckBoxState(CheckBox item, Context context) {
        super(item, context);
        //hintString = getResources().getString(R.string.service_check_view_on);
        textStringOff = getResources().getString(R.string.service_check_view_off);
        textStringOn = getResources().getString(R.string.service_check_view_on);
        setStateOff();
    }
}
