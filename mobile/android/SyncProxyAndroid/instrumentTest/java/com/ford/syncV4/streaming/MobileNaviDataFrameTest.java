package com.ford.syncV4.streaming;

import android.test.AndroidTestCase;

import java.util.Arrays;

/**
 * Created by Andrew Batutin on 9/30/13.
 */
public class MobileNaviDataFrameTest extends AndroidTestCase {

    MobileNaviDataFrame sut;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        sut = new MobileNaviDataFrame(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
    }

    public void testSutWasCreated() throws Exception {
        assertNotNull(sut);
    }

    public void testSutThrowsExptWithToBigDataSizeToCreate() throws Exception {
        try {
            MobileNaviDataFrame frame = new MobileNaviDataFrame(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE + 10]);
            assertNull("should not get here", frame);
        } catch (IllegalArgumentException e) {
            assertNotNull("frame should not be created with size more than MOBILE_NAVI_DATA_SIZE", e.getMessage());
        }
    }

    public void testSutThrowsExpWithNullDataToCreate() throws Exception {
        try {
            MobileNaviDataFrame frame = new MobileNaviDataFrame(null);
            assertNull("should not get here", frame);
        } catch (IllegalArgumentException e) {
            assertNotNull("frame should not be created with null data", e.getMessage());
        }
    }

    public void testMobileNaviEndServiceFrameWasCreated() throws Exception {
        MobileNaviDataFrame endFrame = MobileNaviDataFrame.createEndOfServiceFrame();
        assertNotNull(endFrame);
    }

    public void testMobileNaviEndServiceFrameDataIsByteArrayWithNegOne() throws Exception {
        byte[] endData = new byte[]{-1};
        MobileNaviDataFrame frame = MobileNaviDataFrame.createEndOfServiceFrame();
        assertTrue("end of currentSession array should have only single -1 value in it", Arrays.equals(endData, frame.getData()));
    }

    public void testMobileNaviEndServiceWasCreatedWithEndSessionType() throws Exception {
        MobileNaviDataFrame frame = MobileNaviDataFrame.createEndOfServiceFrame();
        MobileNaviDataFrameType type = frame.getType();
        assertEquals("frame should have END_OS_SESSION_TYPE", type, MobileNaviDataFrameType.END_OS_SESSION_TYPE);
    }

    public void testMobileNaviDataFrameWasCreatedWithDATA_FRAME_TYPE() throws Exception {
        sut = new MobileNaviDataFrame(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertEquals("Frmae should has DATA_FRAME_TYPE", sut.getType(), MobileNaviDataFrameType.DATA_FRAME_TYPE);
    }
}
