package com.ford.syncV4.android.activity.mobilenav;

import android.content.res.Resources;
import android.widget.CheckBox;

/**
 * Created by Andrew Batutin on 8/30/13.
 */
public class CheckBoxState {

    protected final CheckBox item;
    protected CheckBoxStateValue state;
    protected String hintString;
    protected String textString;

    public CheckBoxState(CheckBox item) {
        this.item = item;
    }

    public CheckBoxStateValue getState() {
        return state;
    }

    protected void setState(CheckBoxStateValue state) {
        this.state = state;
    }

    public CheckBox getItem() {
        return item;
    }

    protected Resources getResources() {
        return item.getContext().getResources();
    }

    public void setStateDisabled() {
        getItem().setEnabled(false);
        setState(CheckBoxStateValue.DISABLED);
    }

    public void setStateOff() {
        getItem().setText("");
        getItem().setHint(hintString);
        getItem().setEnabled(true);
        setState(CheckBoxStateValue.OFF);
    }

    public void setStateOn() {
        getItem().setText(textString);
        getItem().setEnabled(true);
        setState(CheckBoxStateValue.ON);
    }
}
