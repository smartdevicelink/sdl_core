package com.ford.syncV4.android.activity;

import android.test.ActivityInstrumentationTestCase2;
import android.test.UiThreadTest;
import android.view.View;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.mobilenav.DataReaderListener;
import com.ford.syncV4.android.activity.mobilenav.StaticFileReader;

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

    @UiThreadTest
    public void testStaticFileReader() throws Exception {
        StaticFileReader staticFileReader = new StaticFileReader(getActivity(), new DataReaderListener() {
            @Override
            public void onStartReading() {

            }

            @Override
            public void onDataReceived(final byte[] data) {
                getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        assertNotNull(data);
                    }
                });

            }

            @Override
            public void onCancelReading() {

            }

            @Override
            public void onEndReading() {

            }
        });
        staticFileReader.execute(R.raw.test_video);
    }

}
