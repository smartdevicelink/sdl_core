package com.ford.syncV4.android.activity.mobilenav;

import android.content.Context;
import android.os.Environment;
import android.test.ActivityInstrumentationTestCase2;
import android.util.Log;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.streaming.H264Packetizer;
import com.ford.syncV4.streaming.IStreamListener;
import com.ford.syncV4.streaming.MobileNaviDataFrame;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.nio.ByteBuffer;
import java.util.Map;
import java.util.TreeMap;

/**
 * Created by Andrew Batutin on 10/1/13.
 */
public class H264PacketizerIntergationTest extends ActivityInstrumentationTestCase2<SyncProxyTester> implements IStreamListener {

    private PipedInputStream inputStream;
    private PipedOutputStream outputStream;
    private H264Packetizer sut;
    private ByteBuffer buffer;
    private byte[] sampleData;
    private FileOutputStream fos;

    public H264PacketizerIntergationTest() {
        super(SyncProxyTester.class);
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        inputStream = new PipedInputStream();
        outputStream = new PipedOutputStream();
        inputStream.connect(outputStream);
        sut = new H264Packetizer(this, inputStream, (byte) 0);
    }

    public void testStressTest() throws Exception {

        //final ByteBuffer sampleDataBuffer = ByteBuffer.allocate(359534);
        //ByteBuffer realDataBuffer = ByteBuffer.allocate(359534);
        initVideoDumpStream();
        Thread t = new Thread() {
            int currentSize = 0;

            @Override
            public void run() {
                super.run();
                // Open the input stream
                Context context = getActivity();
                InputStream is = context.getResources().openRawResource(R.raw.faq_welcome_orientation);
                byte[] buffer = new byte[1000];
                int length;
                try {
                    while ((length = is.read(buffer)) != -1) {
                        outputStream.write(buffer);
                    }
                    is.close();
                } catch (IOException e) {
                    Log.e("SyncProxyTester", e.toString());
                }
            }
        };

        t.start();
        buffer = ByteBuffer.allocate(MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
        sampleData = new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE];
        byte[] frame = null;
        int currentSize = 0;
        try {

            while ((frame = sut.readFrameData(buffer, sampleData)).length >= 1) {

                ProtocolMessage message = sut.createProtocolMessage(frame);

                writeToSdCard(frame);
                currentSize += frame.length;

            }

        } catch (IOException e) {
            Log.e("SyncProxyTester", e.toString());
        }
        t.interrupt();

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

    private void initVideoDumpStream() {
        String filename = "ford_video.mp4";
        File file = new File(Environment.getExternalStorageDirectory(), filename);

        try {
            fos = new FileOutputStream(file);
        } catch (FileNotFoundException e) {
            e.getMessage();
        }
    }

    private void writeToSdCard(byte[] data) {
        try {
            if (fos != null) {
                fos.write(data);
                fos.flush();
            }
        } catch (IOException e) {
            Log.w("SyncProxyTester", e.toString());
        }


    }

    @Override
    public void sendH264(ProtocolMessage pm) {

    }
}
