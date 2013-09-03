package com.ford.syncV4.android.activity.mobilenav;

import android.test.ActivityInstrumentationTestCase2;
import android.test.UiThreadTest;
import android.widget.Button;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;


/**
 * Created by Andrew Batutin on 8/30/13.
 */
public class MobileNavPreviewFragmentTest extends ActivityInstrumentationTestCase2<SyncProxyTester> {

    private MobileNavPreviewFragment sut;

    public MobileNavPreviewFragmentTest() {
        super(SyncProxyTester.class);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        sut = (MobileNavPreviewFragment) this.getActivity().getSupportFragmentManager().findFragmentById(R.layout.activity_mobile_nav_preview);
    }

    public void testVideoStreamingCheckBoxInflated() throws Exception {
        CheckBox view = (CheckBox) sut.getActivity().findViewById(R.id.videoStreamingCheckBox);
        assertNotNull("videoStreamingCheckBox should be inflated", view);
    }

    public void testMobileNavSessionCheckBoxInflated() throws Exception {
        CheckBox view = (CheckBox) sut.getActivity().findViewById(R.id.mobileNavCheckBox);
        assertNotNull("mobileNavCheckBox should be inflated", view);
    }

    @UiThreadTest
    public void testVideoSteamingCheckBoxOnClickListenerSet() throws Exception {
        Button button = (Button) sut.getActivity().findViewById(R.id.videoStreamingCheckBox);
        assertTrue("On click listener should be set", button.performClick());
    }

    @UiThreadTest
    public void testMobileNavSessionCheckBoxOnClickListenerSet() throws Exception {
        Button button = (Button) sut.getActivity().findViewById(R.id.mobileNavCheckBox);
        assertTrue("On click listener should be set", button.performClick());
    }

    @UiThreadTest
    public void testVideoCheckBoxInitialStateDisabled() throws Exception {
        assertEquals("video checkbox should be OFF", CheckBoxStateValue.OFF, sut.getVideoCheckBoxState().getState());
    }

    @UiThreadTest
    public void testMockVideoDataSourceCreatedOnClick() throws Exception {
        assertTrue("state should be off",sut.getVideoCheckBoxState().getState().equals(CheckBoxStateValue.OFF));
        sut.onVideoStreamingCheckBoxAction(null);
        assertTrue("state should be on",sut.getVideoCheckBoxState().getState().equals(CheckBoxStateValue.ON));
        assertNotNull("videoDataSource should be created",sut.getVideoDataSource());
    }
}
