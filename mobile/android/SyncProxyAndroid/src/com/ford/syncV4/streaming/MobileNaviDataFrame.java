package com.ford.syncV4.streaming;

import java.util.Arrays;

/**
 * Created by Andrew Batutin on 9/30/13.
 */
public class MobileNaviDataFrame {

    public final static int MOBILE_NAVI_DATA_SIZE = 1000;
    private byte[] data;
    private MobileNaviDataFrameType type;

    public MobileNaviDataFrame(byte[] data) {
        checkPreconditions(data);
        this.type = MobileNaviDataFrameType.DATA_FRAME_TYPE;
        this.data = data;
    }

    public static MobileNaviDataFrame createEndOfSessionFrame() {
        MobileNaviDataFrame frame = new MobileNaviDataFrame(new byte[]{-1});
        frame.type = MobileNaviDataFrameType.END_OS_SESSION_TYPE;
        return frame;
    }

    public byte[] getData() {
        return data;
    }

    private void checkPreconditions(byte[] data) {
        if (data == null) {
            throw new IllegalArgumentException("data length must be !=  null");
        }
        if (data.length > MOBILE_NAVI_DATA_SIZE) {
            throw new IllegalArgumentException("data length must be > MOBILE_NAVI_DATA_SIZE");
        }
    }

    @Override
    public String toString() {
        String result = super.toString() + "; data = " + Arrays.toString(data);
        return result;
    }

    public MobileNaviDataFrameType getType() {
        return type;
    }

    public void setType(MobileNaviDataFrameType type) {
        this.type = type;
    }
}
