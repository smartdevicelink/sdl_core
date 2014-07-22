package com.ford.syncV4.android.activity.mobilenav;

import android.content.Context;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;

/**
 * Created with Android Studio.
 * User: Yuriy Chernyshov
 * Date: 3/26/14
 * Time: 12:02 AM
 */
public class RPCServiceCheckboxState extends CheckBoxState {

    public RPCServiceCheckboxState(CheckBox item, Context context) {
        super(item, context);
        //hintString = getResources().getString(R.string.service_check_view_on);
        textStringOff = getResources().getString(R.string.secure_service_check_view_off);
        textStringOn = getResources().getString(R.string.secure_service_send_rpc);
        setStateOff();
    }
}