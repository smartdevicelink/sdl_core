package com.ford.syncV4.streaming;

import android.test.AndroidTestCase;

import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.Arrays;

/**
 * Created by Andrew Batutin on 9/30/13.
 */

public class H264PacketizerTest extends AndroidTestCase {

    private H264Packetizer sut;
    private PipedInputStream inputStream;
    private PipedOutputStream outputStream;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        inputStream = new PipedInputStream();
        outputStream = new PipedOutputStream();
        inputStream.connect(outputStream);
        sut = new H264Packetizer(null, inputStream, (byte) 0);
    }

    @Override
    public void tearDown() throws Exception {
        super.tearDown();
        inputStream.close();
        outputStream.close();
    }

    public void testPacketizerWasCreated() throws Exception {
        assertNotNull(sut);
    }

    public void testFixDataSizeFrameWasCreated() throws Exception {
        outputStream.write(new byte[1000]);
        byte[] data = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE", data.length == MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
    }

    public void testFixDataSizeFrameWasCreatedAfterTwoInputs() throws Exception {
        outputStream.write(new byte[500]);
        outputStream.write(new byte[500]);
        byte[] frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, frame.length == MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
    }

    public void testFixDataSizeFrameWasCreatedAfterShortInput() throws Exception {
        outputStream.write(new byte[500]);
        byte[] frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to 500; frame.length = " + frame.length, frame.length == 500);
    }

    public void testFixDataSizeFrameWasCreatedAfterInputMoreThanMOBILE_NAVI_DATA_SIZE() throws Exception {
        outputStream.write(new byte[500]);
        outputStream.write(new byte[500]);
        outputStream.write(new byte[10]);
        byte[] frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, frame.length == MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
    }

    public void test2FixDataSizeFramesWereCreatedAfter1000_1010DataInput() throws Exception {
        outputStream.write(new byte[500]);
        outputStream.write(new byte[500]);
        byte[] frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, frame.length == MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
        outputStream.write(new byte[500]);
        outputStream.write(new byte[500]);
        outputStream.write(new byte[10]);
        frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, frame.length == MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
        frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to 10; frame.length = " + frame.length, frame.length == 10);
    }

    public void test3FixDataSizeFramesWereCreatedAfter1000_1010_1010DataInput() throws Exception {
        outputStream.write(new byte[500]);
        outputStream.write(new byte[500]);
        byte[] frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, frame.length == MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
        outputStream.write(new byte[500]);
        outputStream.write(new byte[500]);
        outputStream.write(new byte[10]);
        frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, frame.length == MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
        outputStream.write(new byte[500]);
        outputStream.write(new byte[500]);
        outputStream.write(new byte[10]);
        frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, frame.length == MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
        frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]).getData();
        assertTrue("data size should be equal to 20; frame.length = " + frame.length, frame.length == 20);
    }

    public void testEndOfSessionFrameCreatedAfterEOS() throws Exception {
        outputStream.write(new byte[1000]);
        sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        outputStream.close();
        MobileNaviDataFrame frame = sut.createFramePayload(new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("Should get {-1} array after EOS", Arrays.equals(new byte[]{-1}, frame.getData()));
    }

    public void testCreateFrameWithNullDataParameterThrowsExp() throws Exception {
        try {
            outputStream.write(new byte[1000]);
            MobileNaviDataFrame frame = sut.createFramePayload(null);
            assertNull("should not get here", frame);
        } catch (IllegalArgumentException e) {
            assertNotNull("can't create frame with null data parameter", e.getMessage());
        }
    }

    public void testCreateFrameWithNullInputStreamThrowsExp() throws Exception {
        try {
            H264Packetizer packetizer = new H264Packetizer(null, null, (byte) 0);
            MobileNaviDataFrame frame = packetizer.createFramePayload(new byte[10]);
            assertNull("should not get here", frame);
        } catch (IllegalArgumentException e) {
            assertNotNull("can't create frame with null input stream", e.getMessage());
        }
    }
}
