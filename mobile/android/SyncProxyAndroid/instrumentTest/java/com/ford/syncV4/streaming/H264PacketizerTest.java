package com.ford.syncV4.streaming;

import android.test.AndroidTestCase;

import java.io.PipedInputStream;
import java.io.PipedOutputStream;

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
        byte[] data = sut.createFramePayload();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE", data.length == H264Packetizer.MOBILE_NAVI_DATA_SIZE);
    }

    public void testFixDataSizeFrameWasCreatedAfterTwoInputs() throws Exception {
        outputStream.write(new byte[500]);
        outputStream.write(new byte[500]);
        byte[] frame = sut.createFramePayload();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, frame.length == H264Packetizer.MOBILE_NAVI_DATA_SIZE);
    }
}
