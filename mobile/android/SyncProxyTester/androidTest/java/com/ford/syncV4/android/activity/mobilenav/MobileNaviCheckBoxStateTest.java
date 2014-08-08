package com.ford.syncV4.android.activity.mobilenav;

import android.test.ActivityInstrumentationTestCase2;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;

/**
 * Created by Andrew Batutin on 8/30/13
 * Co-author: Yuriy Chernyshov
 */
public class MobileNaviCheckBoxStateTest extends ActivityInstrumentationTestCase2<SyncProxyTester> {

    private MobileNaviCheckBoxState mCheckBoxState;
    private String mTextOn;
    private String mTextOff;

    public MobileNaviCheckBoxStateTest() {
        super(SyncProxyTester.class);
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        mTextOn = getInstrumentation().getTargetContext().getString(R.string.service_check_view_on);
        mTextOff = getInstrumentation().getTargetContext().getString(R.string.service_check_view_off);
        mCheckBoxState = new MobileNaviCheckBoxState(new CheckBox(getActivity()), getActivity());
    }

    public void testInitialState() throws Exception {
        assertEquals("should have initial text",
                mTextOn, mCheckBoxState.getItem().getText());
        assertEquals("should have video checkbox text", mTextOn,
                mCheckBoxState.getItem().getText());
        assertTrue("should be enabled", mCheckBoxState.getItem().isEnabled());
        assertEquals("should be at OFF state", mCheckBoxState.getState(), CheckBoxStateValue.OFF);
    }

    public void testStateDisabled() throws Exception {
        mCheckBoxState.setStateDisabled();
        assertEquals("check box should be disabled", false, mCheckBoxState.getItem().isEnabled());
    }

    public void testStateOff() throws Exception {
        mCheckBoxState.setStateOff();
        assertEquals("should have initial text", mTextOn, mCheckBoxState.getItem().getText());
        assertEquals("should have video checkbox text", mTextOn,
                mCheckBoxState.getItem().getText());
        assertTrue("should be enabled", mCheckBoxState.getItem().isEnabled());
        assertEquals("should be at OFF state", mCheckBoxState.getState(), CheckBoxStateValue.OFF);
    }

    public void testStateOn() throws Exception {
        mCheckBoxState.setStateOn();
        assertEquals("should have video streaming text", mTextOff,
                mCheckBoxState.getItem().getText());
        assertTrue("should be enabled", mCheckBoxState.getItem().isEnabled());
        assertEquals("should be at ON state", mCheckBoxState.getState(), CheckBoxStateValue.ON);
    }

    public void testTransitionFormOffToDisabled() throws Exception {
        mCheckBoxState.setStateOff();
        mCheckBoxState.setStateDisabled();
        assertEquals("check box should be disabled", false, mCheckBoxState.getItem().isEnabled());
        assertEquals("should be at DISABLED state", mCheckBoxState.getState(),
                CheckBoxStateValue.DISABLED);
    }

    public void testTransitionFromDisabledToOn() throws Exception {
        mCheckBoxState.setStateOff();
        mCheckBoxState.setStateDisabled();
        mCheckBoxState.setStateOn();
        assertEquals("should have video streaming text",
                mTextOff, mCheckBoxState.getItem().getText());
        assertTrue("should be enabled", mCheckBoxState.getItem().isEnabled());
        assertEquals("should be at ON state", mCheckBoxState.getState(), CheckBoxStateValue.ON);
    }

    public void testTransitionFromOnToDisabled() throws Exception {
        mCheckBoxState.setStateOff();
        mCheckBoxState.setStateDisabled();
        mCheckBoxState.setStateOn();
        mCheckBoxState.setStateDisabled();
        assertEquals("check box should be disabled", false, mCheckBoxState.getItem().isEnabled());
        assertEquals("should have video streaming text",
                getActivity().getString(R.string.service_check_view_off),
                mCheckBoxState.getItem().getText());
        assertEquals("should be at DISABLED state", mCheckBoxState.getState(),
                CheckBoxStateValue.DISABLED);
    }

    public void testTransitionFromDisabledToOff() throws Exception {
        mCheckBoxState.setStateOff();
        mCheckBoxState.setStateDisabled();
        mCheckBoxState.setStateOn();
        mCheckBoxState.setStateDisabled();
        mCheckBoxState.setStateOff();
        assertEquals("should have initial text", mTextOn, mCheckBoxState.getItem().getText());
        assertEquals("should have video checkbox text", mTextOn,
                mCheckBoxState.getItem().getText());
        assertTrue("should be enabled", mCheckBoxState.getItem().isEnabled());
        assertEquals("should be at OFF state", mCheckBoxState.getState(), CheckBoxStateValue.OFF);
    }
}