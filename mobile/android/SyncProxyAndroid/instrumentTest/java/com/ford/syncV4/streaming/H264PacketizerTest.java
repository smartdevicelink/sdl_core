package com.ford.syncV4.streaming;

import android.test.AndroidTestCase;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.OutputStream;

/**
 * Created by Andrew Batutin on 9/30/13.
 */
public class H264PacketizerTest extends AndroidTestCase {

    private H264Packetizer sut;
    private OutputStream outputStream;
    private ByteArrayInputStream inputStream;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        outputStream = new ByteArrayOutputStream();
        byte[] data = new byte[2000];
        inputStream = new ByteArrayInputStream(data);
        sut = new H264Packetizer(null, inputStream, (byte)0);
    }

    public void testPacketizerWasCreated() throws Exception {
        assertNotNull(sut);
    }

    public void testFixDataSizeFrameWasCreated() throws Exception {
        byte[] data = sut.createFramePayload();
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE", data.length == H264Packetizer.MOBILE_NAVI_DATA_SIZE);
    }
}
