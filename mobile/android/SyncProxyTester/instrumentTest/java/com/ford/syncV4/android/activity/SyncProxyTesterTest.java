package com.ford.syncV4.android.activity;

import android.test.ActivityInstrumentationTestCase2;
import android.view.View;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.mobilenav.DataReaderListener;
import com.ford.syncV4.android.activity.mobilenav.StaticFileReader;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.streaming.H264Packetizer;
import com.ford.syncV4.streaming.IStreamListener;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

/**
 * Created by Andrew Batutin on 7/30/13.
 */
public class SyncProxyTesterTest extends ActivityInstrumentationTestCase2<SyncProxyTester> implements IStreamListener {

    private SyncProxyTester sut;
    private H264Packetizer mPacketizer;

    public SyncProxyTesterTest() {
        super(SyncProxyTester.class);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        sut = this.getActivity();
    }

    // Preconditions

    public void testShowVideoPreviewButtonInflated() {
        View view = sut.findViewById(R.id.videoFragment);
        assertNotNull("Button should be inflated", view);
    }


    public void testStaticFileReader() throws Exception {
        OutputStream os = new PipedOutputStream();
        InputStream is = new PipedInputStream((PipedOutputStream) os);
        mPacketizer = new H264Packetizer(this, is, (byte) 0);
        mPacketizer.start();

        StaticFileReader staticFileReader = new StaticFileReader(getActivity(), new DataReaderListener() {
            @Override
            public void onStartReading() {

            }

            @Override
            public void onDataReceived(final byte[] data) {
                getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        assertNotNull(data);
                    }
                });

            }

            @Override
            public void onCancelReading() {

            }

            @Override
            public void onEndReading() {

            }
        });
        staticFileReader.execute(R.raw.test_video);
    }

    @Override
    public void sendH264(ProtocolMessage pm) {

    }
}
