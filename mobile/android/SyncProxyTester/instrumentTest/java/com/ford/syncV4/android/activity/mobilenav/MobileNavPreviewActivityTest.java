package com.ford.syncV4.android.activity.mobilenav;

import android.test.ActivityInstrumentationTestCase2;
import android.test.UiThreadTest;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
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

    public void testSurfaceViewInflated() throws Exception {
        SurfaceView view = (SurfaceView) sut.findViewById(R.id.videoSurfaceView);
        assertNotNull("SurfaceView should be inflated", view);
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

    public void testSutImplementsSurfaceHolderCallback() throws Exception {
        try {
            SurfaceHolder.Callback callback = sut;
        }catch (ClassCastException e){
            assertNull("Should not get here", e);
        }
    }

    public void testVideoCheckBoxInitialStateDisabled() throws Exception {
        assertEquals("video checkbox should be DISABLED", VideoStreamingCheckBoxState.DISABLED, sut.checkBoxState.getState());
    }

    @UiThreadTest
    public void testOnSurfaceCreatedVideoCheckBoxStateIsOff() throws Exception {
        sut.surfaceCreated(null);
        assertEquals("video checkbox should be OFF", VideoStreamingCheckBoxState.OFF, sut.checkBoxState.getState());
    }

    @UiThreadTest
    public void testOnVideoCheckBoxCheckedStateIsDisabled() throws Exception {
        sut.surfaceCreated(null);
        CheckBox checkBox = (CheckBox) sut.findViewById(R.id.videoStreamingCheckBox);
        sut.onVideoStreamingCheckBoxAction(checkBox);
        assertEquals("video checkbox should be DISABLED", VideoStreamingCheckBoxState.DISABLED, sut.checkBoxState.getState());
    }
}
