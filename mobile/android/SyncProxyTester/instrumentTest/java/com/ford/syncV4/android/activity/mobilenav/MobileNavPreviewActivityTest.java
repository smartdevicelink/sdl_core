package com.ford.syncV4.android.activity.mobilenav;

import android.test.ActivityInstrumentationTestCase2;
import android.test.UiThreadTest;
import android.widget.Button;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;


/**
 * Created by Andrew Batutin on 8/30/13.
 */
public class MobileNavPreviewActivityTest extends ActivityInstrumentationTestCase2<MobileNavPreviewActivity> {

    private MobileNavPreviewActivity sut;

    public MobileNavPreviewActivityTest() {
        super(MobileNavPreviewActivity.class);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        sut = this.getActivity();
    }

    public void testVideoStreamingCheckBoxInflated() throws Exception {
        CheckBox view = (CheckBox) sut.findViewById(R.id.videoStreamingCheckBox);
        assertNotNull("videoStreamingCheckBox should be inflated", view);
    }

    public void testMobileNavSessionCheckBoxInflated() throws Exception {
        CheckBox view = (CheckBox) sut.findViewById(R.id.mobileNavCheckBox);
        assertNotNull("mobileNavCheckBox should be inflated", view);
    }

    @UiThreadTest
    public void testVideoSteamingCheckBoxOnClickListenerSet() throws Exception {
        Button button = (Button) sut.findViewById(R.id.videoStreamingCheckBox);
        assertTrue("On click listener should be set", button.performClick());
    }

    @UiThreadTest
    public void testMobileNavSessionCheckBoxOnClickListenerSet() throws Exception {
        Button button = (Button) sut.findViewById(R.id.mobileNavCheckBox);
        assertTrue("On click listener should be set", button.performClick());
    }

    public void testVideoCheckBoxInitialStateDisabled() throws Exception {
        assertEquals("video checkbox should be DISABLED", CheckBoxStateValue.DISABLED, sut.getCheckBoxState().getState());
    }

    public void testMockVideoDataSourceCreatedOnClick() throws Exception {
        assertTrue("state should be off",sut.getCheckBoxState().getState().equals(CheckBoxStateValue.OFF));
        sut.onVideoStreamingCheckBoxAction(null);
        assertTrue("state should be disabled",sut.getCheckBoxState().getState().equals(CheckBoxStateValue.DISABLED));
        assertNotNull("videoDataSource should be created",sut.getVideoDataSource());
    }
}
