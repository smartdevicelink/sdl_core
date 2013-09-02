package com.ford.syncV4.android.activity.mobilenav;

import android.test.AndroidTestCase;

/**
 * Created by Andrew Batutin on 9/2/13.
 */
public class MockVideoDataSourceTest extends AndroidTestCase {

    MockVideoDataSource sut;

    public MockVideoDataSourceTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        sut = new MockVideoDataSource();
    }

    public void testSutImplementsRunnable() throws Exception {
        try {
            Runnable r = sut;
            r.run();
            assertNotNull("should implement runnable", r);
        }catch (ClassCastException e){
            assertNull("Should not get here", e);
        }
    }
}
