package com.ford.syncV4.android.tests.activity;

import android.test.ActivityInstrumentationTestCase2;
import android.test.UiThreadTest;
import android.widget.Button;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;

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

    public void testSendVideoDataButtonInflated() {
        Button button = (Button) sut.findViewById(R.id.btnSendVideoData);
        assertNotNull("Button should be inflated", button);
    }

    public void testSendVideoDataButtonEnabled() {
        Button button = (Button) sut.findViewById(R.id.btnSendVideoData);
        assertTrue("btnSendVideoData should be enabled", button.isEnabled());
    }

    @UiThreadTest
    public void testSendVideoDataOnClickListenerSet() throws Exception {
        Button startSessionButton = (Button) sut.findViewById(R.id.btnSendVideoData);
        assertTrue("On click listener should be set", startSessionButton.performClick());
    }
}
