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

    public void testStartVideoSessionButtonInflated() {
        Button startSessionButton = (Button) sut.findViewById(R.id.btnStartVideoSession);
        assertNotNull("Button should be inflated", startSessionButton);
    }

    public void testStartVideoSessionButtonEnabled() {
        Button startSessionButton = (Button) sut.findViewById(R.id.btnStartVideoSession);
        assertTrue("startSessionButton should be enabled", startSessionButton.isEnabled());
    }

    @UiThreadTest
    public void testStartVideoSessionOnClickListenerSet() throws Exception {
        Button startSessionButton = (Button) sut.findViewById(R.id.btnStartVideoSession);
        assertTrue("On click listener should be set", startSessionButton.performClick());
    }

    public void testStopVideoSessionButtonInflated() {
        Button stopSessionButton = (Button) sut.findViewById(R.id.btnStopVideoSession);
        assertNotNull("Button should be inflated", stopSessionButton);
    }

    public void testStopVideoSessionButtonEnabled() {
        Button stopSessionButton = (Button) sut.findViewById(R.id.btnStopVideoSession);
        assertFalse("startSessionButton should be disabled", stopSessionButton.isEnabled());
    }

    @UiThreadTest
    public void testStopVideoSessionOnClickListenerSet() throws Exception {
        Button stopSessionButton = (Button) sut.findViewById(R.id.btnStopVideoSession);
        assertTrue("On click listener should be set", stopSessionButton.performClick());
    }
}
