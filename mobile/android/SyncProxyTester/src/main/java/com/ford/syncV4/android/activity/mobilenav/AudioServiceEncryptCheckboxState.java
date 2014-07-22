package com.ford.syncV4.android.activity.mobilenav;

import android.content.Context;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 26.03.14
 * Time: 10:46
 */
public class AudioServiceEncryptCheckboxState extends CheckBoxState {

    public AudioServiceEncryptCheckboxState(CheckBox item, Context context) {
        super(item, context);
        textStringOff = getResources().getString(R.string.secure_service_check_view_off);
        textStringOn = getResources().getString(R.string.secure_service_send_rpc);
        setStateOff();
    }
}