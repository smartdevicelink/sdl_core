package com.ford.syncV4.android.activity.mobilenav;

import android.test.ActivityInstrumentationTestCase2;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;

/**
 * Created by Andrew Batutin on 9/20/13.
 */
public class StaticFileReaderTest extends ActivityInstrumentationTestCase2<SyncProxyTester> {

    private StaticFileReader sut;

    public StaticFileReaderTest() {
        super(SyncProxyTester.class);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        sut = new StaticFileReader(this.getActivity(), null);
    }

    public void testEOFStream() throws Exception {
        sut.readWithBufferedReader(R.raw.test_video);
    }
}
