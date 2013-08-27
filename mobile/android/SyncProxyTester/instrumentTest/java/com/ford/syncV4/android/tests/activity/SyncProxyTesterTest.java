package com.ford.syncV4.android.tests.activity;

import android.test.ActivityInstrumentationTestCase2;
import android.test.UiThreadTest;
import android.widget.Button;

import com.ford.syncV4.android.activity.SyncProxyTester;
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
        Button button = (Button) sut.findViewById(R.id.btnShowVideoPreview);
        assertNotNull("Button should be inflated", button);
    }

    public void testShowVideoPreviewButtonEnabled() {
        Button button = (Button) sut.findViewById(R.id.btnShowVideoPreview);
        assertTrue("btnShowVideoPreview should be enabled", button.isEnabled());
    }

    @UiThreadTest
    public void testShowVideoPreviewOnClickListenerSet() throws Exception {
        Button startSessionButton = (Button) sut.findViewById(R.id.btnShowVideoPreview);
        assertTrue("On click listener should be set", startSessionButton.performClick());
    }
}
