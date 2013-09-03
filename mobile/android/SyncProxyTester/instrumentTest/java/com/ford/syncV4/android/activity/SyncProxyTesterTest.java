package com.ford.syncV4.android.activity;

import android.test.ActivityInstrumentationTestCase2;
import android.view.View;

import com.ford.syncV4.android.R;

/**
 * Created by Andrew Batutin on 7/30/13.
 */
public class SyncProxyTesterTest extends ActivityInstrumentationTestCase2<SyncProxyTester> {

    private SyncProxyTester sut;

    public SyncProxyTesterTest() {
        super(SyncProxyTester.class);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        sut = this.getActivity();
    }

    // Preconditions

    public void testShowVideoPreviewButtonInflated() {
        View view = sut.findViewById(R.id.videoFragment);
        assertNotNull("Button should be inflated", view);
    }
}
