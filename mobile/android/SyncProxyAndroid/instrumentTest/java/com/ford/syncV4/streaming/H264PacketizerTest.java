package com.ford.syncV4.streaming;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.ProtocolMessage;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.Collections;
import java.util.Map;
import java.util.Random;
import java.util.TreeMap;

/**
 * Created by Andrew Batutin on 9/30/13.
 */

public class H264PacketizerTest extends AndroidTestCase {

    private H264Packetizer sut;
    private PipedInputStream inputStream;
    private PipedOutputStream outputStream;
    private ByteBuffer buffer;
    private byte[] sampleData;

    private static byte[] combine(byte[] a, byte[] b) {
        int length = a.length + b.length;
        byte[] result = new byte[length];
        System.arraycopy(a, 0, result, 0, a.length);
        System.arraycopy(b, 0, result, a.length, b.length);
        return result;
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        inputStream = new PipedInputStream();
        outputStream = new PipedOutputStream();
        inputStream.connect(outputStream);
        sut = new H264Packetizer(null, inputStream, (byte) 0);
        sampleData = generateRandomBytes(MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
        buffer = ByteBuffer.allocate(MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
    }

    private byte[] generateRandomBytes(int size) {
        Random random = new Random(47);
        byte[] data = new byte[size];
        random.nextBytes(data);
        return data;
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
        outputStream.write(sampleData);
        byte[] data = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE", Arrays.equals(sampleData, data));
    }

    public void testFixDataSizeFrameWasCreatedAfterTwoInputs() throws Exception {
        byte[] data1 = generateRandomBytes(500);
        outputStream.write(data1);
        byte[] data2 = generateRandomBytes(500);
        outputStream.write(data2);
        byte[] combined = combine(data1, data2);
        byte[] frame = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, Arrays.equals(combined, frame));
    }

    public void testFixDataSizeFrameWasCreatedAfterShortInput() throws Exception {
        byte[] data1 = generateRandomBytes(500);
        outputStream.write(data1);
        outputStream.close();
        byte[] frame = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to 500; frame.length = " + frame.length, Arrays.equals(data1, frame));
    }

    public void testFixDataSizeFrameWasCreatedAfterInputMoreThanMOBILE_NAVI_DATA_SIZE() throws Exception {
        byte[] data1 = generateRandomBytes(500);
        byte[] data2 = generateRandomBytes(10);
        outputStream.write(data1);
        outputStream.write(data1);
        outputStream.write(data2);
        outputStream.close();
        byte[] combined = combine(data1, data1);
        byte[] frame = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, Arrays.equals(combined, frame));
    }

    public void test2FixDataSizeFramesWereCreatedAfter1000_1010DataInput() throws Exception {
        byte[] data1 = generateRandomBytes(500);
        byte[] data2 = generateRandomBytes(10);
        outputStream.write(data1);
        outputStream.write(data1);
        byte[] combined = combine(data1, data1);
        byte[] frame = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, Arrays.equals(combined, frame));
        outputStream.write(data1);
        outputStream.write(data1);
        outputStream.write(data2);
        combined = combine(data1, data1);
        frame = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, Arrays.equals(combined, frame));
        outputStream.close();
        frame = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to 10; frame.length = " + frame.length, Arrays.equals(data2, frame));
    }

    public void test3FixDataSizeFramesWereCreatedAfter1000_1010_1010DataInput() throws Exception {
        byte[] data1 = generateRandomBytes(500);
        byte[] data2 = generateRandomBytes(10);
        byte[] combined = combine(data1, data1);
        outputStream.write(data1);
        outputStream.write(data1);
        byte[] frame = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, Arrays.equals(combined, frame));
        outputStream.write(data1);
        outputStream.write(data1);
        outputStream.write(data2);
        frame = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, Arrays.equals(combined, frame));
        outputStream.write(data1);
        outputStream.write(data1);
        outputStream.write(data2);
        frame = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to MOBILE_NAVI_DATA_SIZE; frame.length = " + frame.length, frame.length == MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
        outputStream.close();
        combined = combine(Arrays.copyOfRange(data1, data1.length - 10, data1.length), data2);
        frame = sut.readFrameData(buffer, new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE]);
        assertTrue("data size should be equal to 20; frame.length = " + frame.length, Arrays.equals(combined, frame));
    }

    public void testCreateFrameWithNullDataParameterThrowsExp() throws Exception {
        try {
            outputStream.write(new byte[1000]);
            byte[] frame = sut.readFrameData(buffer, null);
            assertNull("should not get here", frame);
        } catch (IllegalArgumentException e) {
            assertNotNull("can't create frame with null data parameter", e.getMessage());
        }
    }

    public void testCreateFrameWithNullInputStreamThrowsExp() throws Exception {
        try {
            H264Packetizer packetizer = new H264Packetizer(null, null, (byte) 0);
            byte[] frame = packetizer.readFrameData(buffer, new byte[10]);
            assertNull("should not get here", frame);
        } catch (IllegalArgumentException e) {
            assertNotNull("can't create frame with null input stream", e.getMessage());
        }
    }

    public void testRead_1_FrameDataFromOuptutStreamShouldReturn_1_Frame() throws Exception {
        outputStream.write(sampleData);
        byte[] frame = sut.readFrameData(buffer, sampleData);
        assertTrue("Arrays written to output stream and get from packetizer should be same", Arrays.equals(sampleData, frame));
    }

    public void testRead_half_FrameDataFromOuptutStreamShouldReturn_half_Frame() throws Exception {
        sampleData = generateRandomBytes(MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE / 2);
        buffer = ByteBuffer.allocate(MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE / 2);
        outputStream.write(sampleData);
        byte[] frame = sut.readFrameData(buffer, sampleData);
        assertTrue("Arrays written to output stream and get from packetizer should be same", Arrays.equals(sampleData, frame));
    }

    public void testProtocolFrameHeaderWasCreatedFormInputStream() throws Exception {
        final byte[] data2 = generateRandomBytes(10);
        final boolean[] isTestValid = {false};
        outputStream.write(sampleData);
        outputStream.write(data2);
        final H264Packetizer packetizer = new H264Packetizer(new IStreamListener() {
            private int count = 0;

            @Override
            public void sendH264(ProtocolMessage pm) {
                if (count == 0 && pm != null && Arrays.equals(pm.getData(), sampleData)) {
                    isTestValid[0] = true;
                    count++;
                } else if (count == 1 && pm != null && Arrays.equals(pm.getData(), data2)) {
                    isTestValid[0] = true;
                    count++;
                } else {
                    isTestValid[0] = false;
                }
            }
        }, inputStream, (byte) 0);
        outputStream.close();
        packetizer.doDataReading();
        assertTrue("ProtocolMessage should be created", isTestValid[0]);
    }

    public void testStressTest() throws Exception {
        final int maxSize = 1024 * 10;
        int currentSize = 0;
        final ByteBuffer sampleDataBuffer = ByteBuffer.allocate(1024 * 10 + 1000);
        ByteBuffer realDataBuffer = ByteBuffer.allocate(1024 * 10 + 1000);

        Thread t = new Thread() {
            int currentSize = 0;
            @Override
            public void run() {
                super.run();
                Random random = new Random(47);
                do {
                    byte[] data = new byte[random.nextInt(1000)];
                    random.nextBytes(data);
                    sampleDataBuffer.put(data);
                    try {
                        outputStream.write(data);
                        outputStream.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    currentSize += data.length;
                    try {
                        synchronized (this) {
                            Thread.currentThread().wait(100);
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                } while (currentSize < 1024 * 10);
                try {
                    outputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        };

        t.start();
        buffer = ByteBuffer.allocate(MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
        sampleData = new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE];
        while (currentSize < 1024 * 10) {
            try {
                byte[] frame = sut.readFrameData(buffer, sampleData);
                realDataBuffer.put(frame);
                currentSize += frame.length;
            } catch (IOException e) {
                e.getMessage();
            }
        }
        t.interrupt();
        byte[] sample = sampleDataBuffer.array();
        byte[] real = realDataBuffer.array();
        Map<Integer, Byte> map = compareArrays(sample, real);
        if (map.size() > 0) {
            Integer minI = Collections.min(map.keySet());
        }
        assertTrue("Sample and real data should be the same", Arrays.equals(sample, real));
    }

    private Map<Integer, Byte> compareArrays(byte[] one, byte[] two) {
        Map<Integer, Byte> map = new TreeMap<Integer, Byte>();
        for (int i = 0; i < one.length; i++) {
            if (one[i] != two[i]) {
                map.put(i, one[i]);
            }
        }
        return map;
    }

}
