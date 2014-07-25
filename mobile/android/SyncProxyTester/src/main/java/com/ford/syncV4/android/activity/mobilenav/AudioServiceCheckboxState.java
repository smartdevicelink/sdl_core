package com.ford.syncV4.android.activity.mobilenav;

import android.content.Context;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;

/**
 * Created by Andrew Batutin on 1/24/14
 */
public class AudioServiceCheckboxState extends CheckBoxState {

    public AudioServiceCheckboxState(CheckBox item, Context context) {
        super(item, context);
        textStringOff = getResources().getString(R.string.service_check_view_off);
        textStringOn = getResources().getString(R.string.service_check_view_on);
        setStateOff();
    }
}
