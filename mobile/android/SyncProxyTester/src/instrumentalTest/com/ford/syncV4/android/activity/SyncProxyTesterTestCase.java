package instrumentalTest.com.ford.syncV4.android.activity;

import android.test.ActivityInstrumentationTestCase2;

import com.ford.syncV4.android.activity.SyncProxyTester;

/**
 * Created by admin on 7/19/13.
 */
public class SyncProxyTesterTestCase extends ActivityInstrumentationTestCase2<SyncProxyTester> {

    public SyncProxyTesterTestCase() {
        super(SyncProxyTester.class);
    }

    public void testTest() throws Exception {
        assertEquals("","d");
    }
}
