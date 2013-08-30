package com.ford.syncV4.android.activity.mobilenav;

import android.test.ActivityInstrumentationTestCase2;
import android.view.SurfaceView;
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
}
