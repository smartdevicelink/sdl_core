package com.ford.syncV4.streaming;

import android.util.Log;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.SessionType;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.Arrays;

public class H264Packetizer extends AbstractPacketizer implements Runnable {

    public final static String TAG = "H264Packetizer";
    public static final int EOS = -1;
    private Thread t = null;

    public H264Packetizer(IStreamListener streamListener, InputStream is, byte rpcSessionID) throws IOException {
        super(streamListener, is, rpcSessionID);
    }

    public synchronized void start() throws IOException {
        if (t == null) {
            t = new Thread(this);
            t.start();
        }
    }

    public synchronized void stop() {
        try {
            if (is != null) {
                is.close();
            }
        } catch (IOException ignore) {
        }
        if (t != null) {
            t.interrupt();
            t = null;
        }
    }

    public void run() {
        int length = 0;

        try {
            while (!Thread.interrupted()) {
                ByteArrayOutputStream bb = new ByteArrayOutputStream();

                do {
                    if (is != null) {
                        try {
                            length = is.read();
                        } catch (NullPointerException e) {
                            Log.e("SyncProxyTester", e.toString());
                        }

                        if (length != EOS) {
                            bb.write(length);
                        }
                    }

                } while (length != EOS && bb.size() < 1000);

                if (length != EOS) {
                    bb.flush();
                    buffer = bb.toByteArray();
                    ProtocolMessage pm = new ProtocolMessage();
                    pm.setSessionID(_rpcSessionID);
                    pm.setSessionType(SessionType.Mobile_Nav);
                    pm.setFunctionID(0);
                    pm.setCorrID(0);
                    pm.setData(buffer, buffer.length);
                    _streamListener.sendH264(pm);
                }
            }
        } catch (IOException e) {
            Log.e("SyncProxyTester", e.toString());
        }
    }

    byte[] createFramePayload() throws IOException, IllegalArgumentException {
        if (is == null) {
            throw new IllegalArgumentException("Input stream is null");
        }
        byte[] data = new byte[MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE];
        int length = is.read(data);
        byte[] result = Arrays.copyOf(data, length);
        return result;
    }

    byte[] readFrameData() throws IOException {
        ByteBuffer buffer = ByteBuffer.allocate(MobileNaviDataFrame.MOBILE_NAVI_DATA_SIZE);
        do {
            byte[] data = createFramePayload();
            buffer.put(data, 0, (data.length > buffer.remaining()) ? buffer.remaining() : data.length);
        } while (buffer.remaining() > 0);
        byte[] data = buffer.array();
        return data;
    }


}
