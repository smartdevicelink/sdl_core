package com.ford.syncV4.android.activity.mobilenav;

import android.test.InstrumentationTestCase;

import java.util.Arrays;

import static org.mockito.Mockito.mock;

/**
 * Created by Andrew Batutin on 9/2/13.
 */
public class MockVideoDataSourceTest extends InstrumentationTestCase {

    MockVideoDataSource sut;

    public MockVideoDataSourceTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext().getCacheDir().getPath());
        sut = new MockVideoDataSource(mock(VideoDataListener.class));
    }

    public void testSutImplementsRunnable() throws Exception {
        try {
            Runnable r = sut;
            assertNotNull("should implement runnable", r);
        } catch (ClassCastException e) {
            assertNull("Should not get here", e);
        }
    }

    public void testSutWasCreatedWithListenerAndStream() throws Exception {
        assertNotNull("should have a listener after init", sut.getDataListener());
    }

    public void testSutCreationWithNullListenerShouldThroughException() throws Exception {
        try {
            MockVideoDataSource source = new MockVideoDataSource(null);
            assertNull("should not get here", source);
        } catch (IllegalArgumentException e) {
            assertNotNull("should through exception", e);
        }
    }

    public void testTreadIsNullAfterCreation() throws Exception {
        assertNotNull("thread should not be null", sut.getThread());
    }

    public void testSutShouldStartThreadOnStart() throws Exception {
        sut.start();
        assertNotNull("thread should be not null", sut.getThread());
    }




    public void testThreadStartedOnlyOnce() throws Exception {
        try {
            sut.start();
            sut.start();
        } catch (IllegalThreadStateException e) {
            assertNull("should not get here", e);
        }
    }

    public void testVideoDataListenerOnStartCallbackIsCalledOnStart() throws Exception {
        final boolean[] callFlag = {false};
        MockVideoDataSource dataSource = new MockVideoDataSource(new VideoDataListener() {
            @Override
            public void onStreamingStart() {
                callFlag[0] = true;
                assertEquals("should get here", true, true);
            }

            @Override
            public void videoFrameReady(byte[] videoFrame) {
                callFlag[0] = false;
                assertEquals("should not get here", true, false);
            }

            @Override
            public void onStreamStop() {
                callFlag[0] = false;
                assertEquals("should not get here", true, false);
            }
        });
        dataSource.start();
        assertEquals("callback should be called", true, callFlag[0]);
    }

    public void testMockDataCreation() throws Exception {
        byte[] data = sut.createMockData();
        assertTrue("arrays should be same", Arrays.equals(data, new byte[100]));
    }

    public void testOutputDataDispatchedToListener() throws Exception {
        final boolean[] callFlag = {false};
        MockVideoDataSource dataSource = new MockVideoDataSource(new VideoDataListener() {
            @Override
            public void onStreamingStart() {
            }

            @Override
            public void videoFrameReady(final byte[] videoFrame) {
                callFlag[0] = true;
                assertTrue("arrays should be same", Arrays.equals(videoFrame, new byte[100]));
            }

            @Override
            public void onStreamStop() {

            }
        });
        dataSource.dispatchDataToListener();
        assertEquals("callback should be called", true, callFlag[0]);
    }

    public void testStopCallBackIsCalledOnStop() throws Exception {
        final boolean[] callFlag = {false};
        MockVideoDataSource dataSource = new MockVideoDataSource(new VideoDataListener() {
            @Override
            public void onStreamingStart() {
                callFlag[0] = false;
                assertEquals("should not get here", true, false);
            }

            @Override
            public void videoFrameReady(byte[] videoFrame) {
                callFlag[0] = false;
                assertEquals("should not get here", true, false);
            }

            @Override
            public void onStreamStop() {
                callFlag[0] = true;
                assertEquals("should get here", true, true);
            }
        });
        dataSource.stop();
        assertEquals("callback should be called", true, callFlag[0]);
    }
}
