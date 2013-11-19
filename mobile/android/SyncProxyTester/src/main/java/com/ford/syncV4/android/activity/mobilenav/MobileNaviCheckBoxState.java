package com.ford.syncV4.android.activity.mobilenav;

import android.content.Context;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;

/**
 * Created by Andrew Batutin on 8/30/13.
 */
public class MobileNaviCheckBoxState extends CheckBoxState {

    public MobileNaviCheckBoxState(CheckBox item, Context context) {
        super(item, context);
        hintString = getResources().getString(R.string.mobile_navi_hint);
        textString = getResources().getString(R.string.mobile_navi_check_box_on);
        setStateOff();
    }
}
